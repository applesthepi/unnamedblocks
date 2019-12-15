#include "Plane.h"
#include "Global.h"

#include <iostream>

Plane::Plane(sf::Vector2u position, sf::Vector2u size, bool toolbar)
{
	m_position = new sf::Vector2u();
	m_innerPosition = new sf::Vector2u();
	m_absolutePosition = new sf::Vector2u();
	*m_position = position;

	m_size = size;
	m_toolbar = toolbar;

	m_background = sf::RectangleShape(sf::Vector2f());
	m_background.setFillColor(sf::Color(30, 30, 30));
	m_background.setPosition(position.x, position.y);
	m_background.setSize((sf::Vector2f)size);

	m_innerPositionText = sf::Text("0, 0", *Global::Font, 12);
	m_innerPositionText.setFillColor(sf::Color::White);

	m_functionAdd = new std::function<void(Stack* stack)>();
	*m_functionAdd = [&](Stack* stack)
	{
		if (m_toolbar)
		{
			delete stack;
		}
		else
		{
			AddStack(stack);
		}
	};

	m_functionRemove = new std::function<void(Stack* stack)>();
	*m_functionRemove = [&](Stack* stack)
	{
		if (m_toolbar)
		{
			for (unsigned int i = 0; i < m_stacks.size(); i++)
			{
				if (m_stacks[i] == stack)
				{
					m_stacks.erase(m_stacks.begin() + i);
					return;
				}
			}
		}
		else
		{
			for (unsigned int i = 0; i < m_stacks.size(); i++)
			{
				if (m_stacks[i] == stack)
				{
					m_stacks.erase(m_stacks.begin() + i);
					return;
				}
			}
		}
	};

	m_functionMoveTop = new std::function<void(Stack* stack)>();
	*m_functionMoveTop = [&](Stack* stack)
	{
		for (unsigned int i = 0; i < m_stacks.size(); i++)
		{
			if (m_stacks[i] == stack)
			{
				m_stacks.erase(m_stacks.begin() + i);
				m_stacks.push_back(stack);
				return;
			}
		}
	};

	m_functionAddOver = new std::function<void(Stack* stack)>();
	*m_functionAddOver = [&](Stack* stack)
	{
		((Plane*)Global::DraggingPlaneOver)->AddStack(stack);
	};

	m_draggingConnection = sf::RectangleShape(sf::Vector2f(0, 4));
	m_useDraggingConnection = false;
}

Plane::~Plane()
{
	delete m_position;
	delete m_functionAdd;
	delete m_functionRemove;
	delete m_functionAddOver;
	delete m_functionMoveTop;
	delete m_innerPosition;
	delete m_absolutePosition;

	for (unsigned int i = 0; i < m_stacks.size(); i++)
	{
		delete m_stacks[i];
	}
}

void Plane::AddStack(Stack* stack)
{
	if (stack->GetAbsolutePosition() != sf::Vector2i(0, 0))
	{
		sf::Vector2u nPlanePos = stack->GetPlanePosition() - *m_position;
		stack->SetRelitivePosition(((sf::Vector2i)nPlanePos) + stack->GetRelitivePosition());
	}
	
	stack->SetupInPlane(m_position, m_innerPosition, (void*)this, m_functionAdd, m_functionRemove, m_functionMoveTop, m_functionAddOver);
	m_stacks.push_back(stack);
}

void Plane::Render(sf::RenderWindow* window)
{
	sf::RenderTexture rT;
	rT.create(m_size.x, m_size.y);
	
	for (unsigned int i = 0; i < m_stacks.size(); i++)
	{
		m_stacks[i]->Render(&rT, window);

		if (Global::CutRenderingPlane)
		{
			Global::CutRenderingPlane = false;
			break;
		}
	}

	sf::Sprite sp = sf::Sprite(rT.getTexture());
	sp.setTextureRect(sf::IntRect(0, rT.getSize().y, rT.getSize().x, -1 * rT.getSize().y));
	sp.setPosition(sf::Vector2f(m_position->x, m_position->y));

	if (Global::Dragging && Global::DraggingPlaneOver == this)
	{
		sf::RectangleShape shape = sf::RectangleShape(sf::Vector2f(m_background.getSize().x + 2, m_background.getSize().y + 2));
		shape.setPosition(m_position->x - 1, m_position->y - 1);
		shape.setFillColor(sf::Color::Green);

		window->draw(shape);
	}

	window->draw(m_background);
	window->draw(sp);

	if (m_useDraggingConnection)
		window->draw(m_draggingConnection);

	window->draw(m_innerPositionText);
}

void Plane::FrameUpdate(sf::RenderWindow* window)
{
	if (m_dragging)
	{
		int x = (m_draggingMouseStart.x - sf::Mouse::getPosition(*window).x) + (int)m_draggingStart.x;
		int y = (m_draggingMouseStart.y - sf::Mouse::getPosition(*window).y) + (int)m_draggingStart.y;

		if (x < 0)
			m_innerPosition->x = 0;
		else
			m_innerPosition->x = x;
		
		if (y < 0)
			m_innerPosition->y = 0;
		else
			m_innerPosition->y = y;

		//std::cout << "dragging" << std::endl;
	}

	m_absolutePosition->x = m_position->x + m_innerPosition->x;
	m_absolutePosition->y = m_position->y + m_innerPosition->y;

	//std::cout << m_innerPosition->x << ", " << m_innerPosition->y << std::endl;

	//std::cout << m_dragging << std::endl;

	m_background.setPosition(m_position->x, m_position->y);
	m_background.setSize((sf::Vector2f)m_size);

	m_innerPositionText.setPosition(m_position->x, m_position->y);
	m_innerPositionText.setString(std::to_string(m_innerPosition->x) + ", " + std::to_string(m_innerPosition->y));

	m_useDraggingConnection = false;

	if (Global::Dragging && Global::DraggingPlane == this)
	{
		Global::DraggingStackConnected = nullptr;
		Global::DraggingPlaneOver = nullptr;

		Stack* draggingStack = (Stack*)Global::DraggingStack;

		for (unsigned int i = 0; i < Planes->size(); i++)
		{
			if (draggingStack->GetAbsolutePosition().x > (int32_t)((*Planes)[i]->GetPosition().x) && draggingStack->GetAbsolutePosition().x < (int32_t)((*Planes)[i]->GetPosition().x + (*Planes)[i]->GetSize().x) &&
				draggingStack->GetAbsolutePosition().y >(int32_t)((*Planes)[i]->GetPosition().y) && draggingStack->GetAbsolutePosition().y < (int32_t)((*Planes)[i]->GetPosition().y + (*Planes)[i]->GetSize().y))
			{
				Global::DraggingPlaneOver = (void*)((*Planes)[i]);

				break;
			}
		}

		for (unsigned int i = 0; i < m_stacks.size(); i++)
		{
			if (m_stacks[i] == draggingStack)
				continue;

			unsigned int blockCount = m_stacks[i]->GetBlockCount();

			if (draggingStack->GetAbsolutePosition().x > (int32_t)(m_stacks[i]->GetAbsolutePosition().x - Global::BlockConnectDistance) && draggingStack->GetAbsolutePosition().x < (int32_t)(m_stacks[i]->GetAbsolutePosition().x + Global::BlockConnectDistance) &&
				draggingStack->GetAbsolutePosition().y > (int32_t)(m_stacks[i]->GetAbsolutePosition().y - (Global::BlockHeight / 2)) && draggingStack->GetAbsolutePosition().y < (int32_t)(m_stacks[i]->GetAbsolutePosition().y + (Global::BlockHeight / 2) + (blockCount * Global::BlockHeight)))
			{
				for (unsigned int a = 0; a < blockCount + 1; a++)
				{
					if (draggingStack->GetAbsolutePosition().x > (int32_t)(m_stacks[i]->GetAbsolutePosition().x - Global::BlockConnectDistance) && draggingStack->GetAbsolutePosition().x < (int32_t)(m_stacks[i]->GetAbsolutePosition().x + Global::BlockConnectDistance) &&
						draggingStack->GetAbsolutePosition().y >= (int32_t)(m_stacks[i]->GetAbsolutePosition().y - (Global::BlockHeight / 2) + (a * Global::BlockHeight)) && draggingStack->GetAbsolutePosition().y < (int32_t)(m_stacks[i]->GetAbsolutePosition().y + (Global::BlockHeight / 2) + (a * Global::BlockHeight)))
					{
						unsigned int blockWidth = 0;
						if (a == 0)
							blockWidth = m_stacks[i]->GetBlockWidth(0);
						else
							blockWidth = m_stacks[i]->GetBlockWidth(a - 1);

						m_draggingConnection.setSize(sf::Vector2f(blockWidth, Global::BlockBorder / 2));
						m_draggingConnection.setFillColor(sf::Color(200, 200, 200));
						m_draggingConnection.setPosition(m_stacks[i]->GetAbsolutePosition().x, m_stacks[i]->GetAbsolutePosition().y + (a * Global::BlockHeight));
						m_useDraggingConnection = true;

						Global::DraggingStackConnected = (void*)m_stacks[i];
						Global::DraggingStackConnectedIndex = a;

						return;
					}
				}
			}
		}
	}

	for (int i = m_stacks.size() - 1; i >= 0; i--)
	{
		m_stacks[i]->FrameUpdate(window);
	}
}

void Plane::DeleteAllBlocks()
{
	for (unsigned int i = 0; i < m_stacks.size(); i++)
	{
		m_stacks[i]->~Stack();
	}

	m_stacks.clear();
}

void Plane::CopyEverything(Plane* plane)
{
	*m_position = plane->GetPosition();
	*m_innerPosition = plane->GetInnterPosition();
	*m_absolutePosition = plane->GetAbsolutePosition();
	
	for (unsigned int i = 0; i < plane->GetStackCount(); i++)
	{
		Stack* stack = new Stack(sf::Vector2i(0, 0));
		AddStack(stack);
		stack->CopyEverything(plane->GetStack(i));
	}
}

unsigned int Plane::GetStackCount()
{
	return m_stacks.size();
}

sf::Vector2u Plane::GetSize()
{
	return m_size;
}

sf::Vector2u Plane::GetPosition()
{
	return *m_position;
}

sf::Vector2u Plane::GetInnterPosition()
{
	return *m_innerPosition;
}

sf::Vector2u Plane::GetAbsolutePosition()
{
	return *m_absolutePosition;
}

void Plane::SetPosition(sf::Vector2u position)
{
	*m_position = position;
}

void Plane::SetSize(sf::Vector2u size)
{
	m_size = size;
}

Stack* Plane::GetStack(unsigned int index)
{
	return m_stacks[index];
}

void Plane::MouseButton(bool down, sf::Vector2i position, sf::Mouse::Button button)
{
	if (position.x > (int32_t)m_position->x&& position.x < (int32_t)(m_position->x + m_size.x) &&
		position.y > (int32_t)m_position->y&& position.y < (int32_t)(m_position->y + m_size.y))
	{
		if (m_dragging && !down)
		{
			m_dragging = false;
			return;
		}

		bool drag = true;

		for (unsigned int i = 0; i < m_stacks.size(); i++)
		{
			if (m_stacks[i]->MouseButton(down, position, button))
			{
				drag = false;
				break;
			}
		}

		if (drag && down)
		{
			m_dragging = true;
			m_draggingMouseStart = position;
			m_draggingStart = *m_innerPosition;

			Global::ContextActive = false;
			Global::SelectedArgument = nullptr;
		}
	}
}

std::vector<Plane*>* Plane::Planes;
