#include "registries/ShaderRegistry.h"
#include "Plane.h"
#include "Global.h"

#include <iostream>

Plane::Plane(sf::Vector2u position, sf::Vector2u size, bool toolbar)
{
	m_position = new sf::Vector2u();
	m_innerPosition = new sf::Vector2i();
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
		stack->SetRelitivePosition(stack->GetSetPosition() - (*m_innerPosition - ((Plane*)Global::DraggingPlaneOver)->GetInnerPosition()));
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
	//sf::Shader blankShader;
	//blankShader.loadFromFile("res/black.vs", "res/black.fs");

	//sf::RectangleShape blackRect = sf::RectangleShape((sf::Vector2f)m_size);

	sf::RenderTexture rT;
	rT.create(m_size.x, m_size.y);
	rT.clear(sf::Color(0, 0, 0, 0));
	//rT.draw(blackRect, &blackShader);

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

	sf::Shader* shader = ShaderRegistry::GetShader("dots");
	if (Global::GpuType == GpuMan::NVIDIA)
		shader->setUniform("windowSize", sf::Vector2f(200.0f, 200.0f));
	
	shader->setUniform("planeInnerPosition", sf::Glsl::Vec2(m_innerPosition->x / 2, m_innerPosition->y / -2));
	//TODO future settings
	shader->setUniform("dotMod", 30.0f);
	shader->setUniform("backgroundColor", sf::Glsl::Vec3(40 / 255.0f, 40 / 255.0f, 40 / 255.0f));
	shader->setUniform("dotColor", sf::Glsl::Vec3(70 / 255.0f, 70 / 255.0f, 70 / 255.0f));

	window->draw(m_background, shader);
	window->draw(sp);//all blocks inside the stack

	if (!m_toolbar)
	{
		if (m_useDraggingConnection)
			window->draw(m_draggingConnection);

		window->draw(m_innerPositionText);
	}
}

void Plane::RenderConnection(sf::RenderWindow* window)
{
	if (m_useDraggingConnection)
		window->draw(m_draggingConnection);
}

void Plane::FrameUpdate(sf::RenderWindow* window)
{
	if (m_dragging)
	{
		m_innerPosition->x = (m_draggingMouseStart.x - sf::Mouse::getPosition(*window).x) + (int)m_draggingStart.x;
		m_innerPosition->y = (m_draggingMouseStart.y - sf::Mouse::getPosition(*window).y) + (int)m_draggingStart.y;
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

		if (Global::DraggingPlaneOver != nullptr)
		{
			const std::vector<Stack*>* overPlaneStacks = ((Plane*)Global::DraggingPlaneOver)->GetAllStacks();

			for (unsigned int i = 0; i < overPlaneStacks->size(); i++)
			{
				if ((*overPlaneStacks)[i] == draggingStack)
					continue;

				unsigned int blockCount = (*overPlaneStacks)[i]->GetBlockCount();

				if (draggingStack->GetAbsolutePosition().x > (*overPlaneStacks)[i]->GetAbsolutePosition().x - Global::BlockConnectDistance && draggingStack->GetAbsolutePosition().x < (*overPlaneStacks)[i]->GetAbsolutePosition().x + Global::BlockConnectDistance &&
					draggingStack->GetAbsolutePosition().y >(*overPlaneStacks)[i]->GetAbsolutePosition().y - (Global::BlockHeight / 2) && draggingStack->GetAbsolutePosition().y < (*overPlaneStacks)[i]->GetAbsolutePosition().y + (Global::BlockHeight / 2) + (blockCount * Global::BlockHeight))
				{
					for (unsigned int a = 0; a < blockCount + 1; a++)
					{
						if (draggingStack->GetAbsolutePosition().x > (*overPlaneStacks)[i]->GetAbsolutePosition().x - Global::BlockConnectDistance && draggingStack->GetAbsolutePosition().x < (*overPlaneStacks)[i]->GetAbsolutePosition().x + Global::BlockConnectDistance &&
							draggingStack->GetAbsolutePosition().y >= (*overPlaneStacks)[i]->GetAbsolutePosition().y - (Global::BlockHeight / 2) + (a * Global::BlockHeight) && draggingStack->GetAbsolutePosition().y < (*overPlaneStacks)[i]->GetAbsolutePosition().y + (Global::BlockHeight / 2) + (a * Global::BlockHeight))
						{
							unsigned int blockWidth = 0;
							if (a == 0)
								blockWidth = (*overPlaneStacks)[i]->GetBlockWidth(0);
							else
								blockWidth = (*overPlaneStacks)[i]->GetBlockWidth(a - 1);

							m_draggingConnection.setSize(sf::Vector2f(blockWidth, Global::BlockBorder / 2));
							m_draggingConnection.setFillColor(sf::Color(200, 200, 200));
							m_draggingConnection.setPosition((*overPlaneStacks)[i]->GetAbsolutePosition().x, (*overPlaneStacks)[i]->GetAbsolutePosition().y + (a * Global::BlockHeight));
							m_useDraggingConnection = true;
							
							Global::DraggingStackConnected = (void*)(*overPlaneStacks)[i];
							Global::DraggingStackConnectedIndex = a;

							return;
						}
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
	*m_innerPosition = plane->GetInnerPosition();
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

sf::Vector2i Plane::GetInnerPosition()
{
	return *m_innerPosition;
}

void Plane::SetInnerPosition(sf::Vector2i position)
{
	*m_innerPosition = position;
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

bool Plane::IsToolbar()
{
	return m_toolbar;
}

const std::vector<Stack*>* Plane::GetAllStacks()
{
	return &m_stacks;
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

		if (drag && down && !m_toolbar)
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