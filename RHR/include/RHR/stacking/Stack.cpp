#include "Stack.h"
#include "Global.h"
#include "Plane.h"
#include "handlers/runtime/RuntimeHandler.h"
#include "handlers/Logger.h"

#include <iostream>

#define IF_MARGIN 5
#define IF_WIDTH 20

Stack::Stack(sf::Vector2i relitivePosition, BlockRegistry* registry)
{
	m_setPosition = relitivePosition;
	m_functionSplit = new std::function<void(unsigned int index, sf::Vector2i mousePosition)>();
	m_functionContext = new std::function<void(unsigned int index, sf::Vector2i mousePosition)>();
	m_functionContextCallback = new std::function<void(unsigned int index)>();
	m_cutRendering = false;
	m_contextBlockIndex = 0;
	m_draggingType = DraggingType::DOWN;

	*m_functionContext = [&](unsigned int index, sf::Vector2i mousePosition)
	{
		Global::Context.Callback = m_functionContextCallback;
		Global::Context.Type = ContextType::BLOCK;
		Global::Context.Position.x = mousePosition.x + 5;
		Global::Context.Position.y = mousePosition.y + 5;
		Global::ContextData = std::to_string(index);

		if (Global::ContextActive)
			Global::ContextUpdate = true;
		else
			Global::ContextActive = true;
	};

	*m_functionContextCallback = [&, registry](unsigned int index)
	{
		Global::ContextActive = false;
		Global::Context.Type = ContextType::NONE;
		Plane* plane = (Plane*)m_planePtr;
		if (plane->IsToolbar())
			return;
		
		unsigned int blockIndex = std::stoul(Global::ContextData);

		if (index == 0)
		{
			Stack* stack = new Stack(m_setPosition + sf::Vector2i(0, blockIndex * Global::BlockHeight), registry);
			(*m_functionAdd)(stack);

			ReloadVanity();
			std::vector<StatmentIf> statments = GetVanity();
			std::vector<uint32_t> closestStatments;
			std::vector<uint32_t> closestStatmentsDistance;

			uint32_t useEnd = m_blocks.size();

			for (uint32_t i = 0; i < statments.size(); i++)
			{
				if (closestStatments.size() == 0)
				{
					if (statments[i].Location < blockIndex)
					{
						closestStatments.push_back(i);
						closestStatmentsDistance.push_back(blockIndex - statments[i].Location);
					}
				}
				else
				{
					if (statments[i].Location < blockIndex)
					{
						bool found = false;

						for (uint32_t a = 0; a < closestStatments.size(); a++)
						{
							if (blockIndex - statments[i].Location > closestStatmentsDistance[a])
							{
								closestStatments.insert(closestStatments.begin() + a, i);
								closestStatmentsDistance.insert(closestStatmentsDistance.begin() + a, blockIndex - statments[i].Location);

								found = true;
								break;
							}
						}

						if (!found)
						{
							closestStatments.push_back(i);
							closestStatmentsDistance.push_back(blockIndex - statments[i].Location);
						}
					}
				}
			}

			if (closestStatments.size() != 0)
			{
				bool redo = true;
				uint32_t statmentSelection = closestStatments.size() - 1;

				while (redo && statmentSelection >= 0)
				{
					if (closestStatments.size() == 0)
						break;

					redo = false;

					StatmentIf& statment = statments[closestStatments.back()];

					bool isInsideIfRaw = false;
					bool isInsideIfBase = false;

					if (statment.HasElseIf)
					{
						if (blockIndex < statment.LocationElseIf.front())
							isInsideIfRaw = true;
					}
					else if (statment.HasElse)
					{
						if (blockIndex < statment.LocationElse)
							isInsideIfRaw = true;
					}
					else
					{
						if (blockIndex < statment.LocationEnd)
							isInsideIfBase = true;
					}

					if (isInsideIfBase)
						useEnd = statment.LocationEnd;
					else if (isInsideIfRaw)
					{
						if (statment.HasElseIf)
							useEnd = statment.LocationElseIf.front();
						else
							useEnd = statment.LocationElse;
					}
					else
					{
						if (statment.HasElseIf)
						{
							bool found = false;

							for (uint32_t i = 0; i < statment.LocationElseIf.size(); i++)
							{
								if (blockIndex >= statment.LocationElseIf[i])
								{
									if (i + 1 == statment.LocationElseIf.size())
									{
										if (statment.HasElse)
										{
											useEnd = statment.LocationElse;
											found = true;
											break;
										}
										else
											break;
									}
									else
									{
										useEnd = statment.LocationElseIf[i + 1];
										found = true;
										break;
									}
								}
							}

							if (!found)
								redo = true;
						}
						else
						{
							if (blockIndex < statment.LocationEnd)
								useEnd = statment.LocationEnd;
							else
								redo = true;
						}
					}

					statmentSelection--;
					closestStatments.pop_back();
				}
			}

			for (unsigned int i = blockIndex; i < useEnd; i++)
			{
				Block* block = new Block(m_blocks[i]->GetUnlocalizedName(), registry);
				block->SetArgData(*(m_blocks[i]->GetUsedArgumentsRuntime().Args));
				stack->AddBlock(block);
			}

			stack->ReloadAllBlocks();
			stack->DragUp(Global::MousePosition + ((stack->GetSetPosition() + (sf::Vector2i)stack->GetPlanePosition() - ((Plane*)stack->GetPlanePointer())->GetInnerPosition()) - Global::MousePosition));
		}
		else if (index == 1)
		{
			if (blockIndex == 0)
				(*m_functionRemove)(this);
			else
			{
				Global::SkipFrame = true;

				for (unsigned int i = blockIndex; i < m_blocks.size(); i++)
					delete m_blocks[i];

				m_blocks.resize(blockIndex);
			}
		}
		else if (index == 2)
		{
			Stack* stack = new Stack(m_setPosition + sf::Vector2i(0, blockIndex * Global::BlockHeight), registry);
			(*m_functionAdd)(stack);

			Block* block = new Block(m_blocks[blockIndex]->GetUnlocalizedName(), registry);
			block->SetArgData(*(m_blocks[blockIndex]->GetUsedArgumentsRuntime().Args));
			stack->AddBlock(block);

			stack->ReloadAllBlocks();
			stack->DragUp(Global::MousePosition + ((stack->GetSetPosition() + (sf::Vector2i)stack->GetPlanePosition() - ((Plane*)stack->GetPlanePointer())->GetInnerPosition()) - Global::MousePosition));
		}
		else if (index == 3)
		{
			Global::SkipFrame = true;
			delete m_blocks[blockIndex];
			m_blocks.erase(m_blocks.begin() + blockIndex);
			ReloadAllBlocks();
		}
	};

	*m_functionSplit = [&](unsigned int index, sf::Vector2i mousePosition)
	{
		DragDown(mousePosition);

		if (index == 0)
		{
			//TODO memory leak
			m_planePosition = new sf::Vector2u(m_planePosition->x, m_planePosition->y);
		}
		else
		{
			Stack* stayStack = new Stack(m_setPosition, registry);
			(*m_functionAdd)(stayStack);

			m_setPosition.y += index * Global::BlockHeight;

			for (unsigned int i = 0; i < index; i++)
			{
				stayStack->AddBlock(m_blocks[i]);
			}

			std::vector<Block*> myBlocks = std::vector<Block*>();

			for (unsigned int i = index; i < m_blocks.size(); i++)
			{
				myBlocks.push_back(m_blocks[i]);
			}

			m_blocks.clear();

			for (unsigned int i = 0; i < myBlocks.size(); i++)
			{
				AddBlock(myBlocks[i]);
			}

			stayStack->ReloadVanity();
			ReloadVanity();
		}
	};

	m_validHighlighting = false;
}

Stack::~Stack()
{
	delete m_functionSplit;

	for (unsigned int i = 0; i < m_blocks.size(); i++)
	{
		delete m_blocks[i];
	}
}

void Stack::ImportBlocks(std::vector<Block*>* blocks)
{
	m_blocks.clear();

	for (unsigned int i = 0; i < blocks->size(); i++)
	{
		AddBlock((*blocks)[i]);
	}
}

void Stack::AddBlock(Block* block)
{
	block->SetupInStack(m_blocks.size(), &m_absolutePosition, &m_relitivePosition, m_functionSplit, m_functionContext);
	m_blocks.push_back(block);
}

void Stack::ReloadAllBlocks()
{
	for (unsigned int i = 0; i < m_blocks.size(); i++)
	{
		m_blocks[i]->SetupInStack(i, &m_absolutePosition, &m_relitivePosition, m_functionSplit, m_functionContext);
	}
}

void Stack::Render(sf::RenderTexture* render, sf::RenderWindow* window)
{
	if (render == nullptr)
	{
		for (unsigned int i = 0; i < m_blocks.size(); i++)
		{
			m_blocks[i]->Render(nullptr, window);
			if (m_cutRendering)
			{
				m_cutRendering = false;
				break;
			}
		}

		if (m_validHighlighting)
		{
			for (uint32_t i = 0; i < m_ifShapes.size(); i++)
			{
				if (m_ifShapeIdx[i] != m_highlightedShape)
					window->draw(m_ifShapes[i]);
			}

			if (m_highlightedShape != -1)
			{
				for (uint32_t i = 0; i < m_ifShapes.size(); i++)
				{
					if (m_ifShapeIdx[i] == m_highlightedShape)
						window->draw(m_ifShapes[i]);
				}
			}

			for (uint32_t i = 0; i < m_ifShapeHighlight.size(); i++)
				window->draw(m_ifShapeHighlight[i]);
		}
	}
	else
	{
		for (unsigned int i = 0; i < m_blocks.size(); i++)
		{
			m_blocks[i]->Render(render, window);
			if (m_cutRendering)
			{
				m_cutRendering = false;
				break;
			}
		}

		if (m_validHighlighting)
		{
			for (uint32_t i = 0; i < m_ifShapes.size(); i++)
			{
				if (m_ifShapeIdx[i] != m_highlightedShape)
					render->draw(m_ifShapes[i]);
			}

			if (m_highlightedShape != -1)
			{
				for (uint32_t i = 0; i < m_ifShapes.size(); i++)
				{
					if (m_ifShapeIdx[i] == m_highlightedShape)
						render->draw(m_ifShapes[i]);
				}
			}

			for (uint32_t i = 0; i < m_ifShapeHighlight.size(); i++)
				render->draw(m_ifShapeHighlight[i]);
		}
	}
}

void Stack::FrameUpdate(sf::RenderWindow* window)
{
	bool mouseDown = sf::Mouse::isButtonPressed(sf::Mouse::Left);

	if (m_dragging)
	{
		bool wantDown;
		
		if (m_draggingType == DraggingType::DOWN)
			wantDown = mouseDown;
		else if (m_draggingType == DraggingType::UP)
			wantDown = !mouseDown;

		if (wantDown)
		{
			m_relitivePosition.x = (sf::Mouse::getPosition(*window).x - m_draggingMouseBegin.x) + m_setPosition.x - m_planeInnerPosition->x;
			m_relitivePosition.y = (sf::Mouse::getPosition(*window).y - m_draggingMouseBegin.y) + m_setPosition.y - m_planeInnerPosition->y;
		}
		else
		{
			m_relitivePosition.x = (sf::Mouse::getPosition(*window).x - m_draggingMouseBegin.x) + m_setPosition.x;
			m_relitivePosition.y = (sf::Mouse::getPosition(*window).y - m_draggingMouseBegin.y) + m_setPosition.y;

			m_dragging = false;
			Global::Dragging = false;

			if (Global::DraggingStackConnected != nullptr)
			{
				std::vector<Block*> blocks;

				Stack* connectedStack = (Stack*)Global::DraggingStackConnected;
				unsigned int connectedBlockIndex = Global::DraggingStackConnectedIndex;
				/*
				std::cout << "connecting stuff" << std::endl;

				if (((Plane*)connectedStack->GetPlanePointer())->IsToolbar())
				{
					Global::CutRenderingPlane = true;
					(*m_functionRemove)(this);
					(*m_functionAddOver)(this);
					std::cout << "canceling, is toolbar" << std::endl;
					return;
				}
				*/
				for (unsigned int i = 0; i < connectedBlockIndex; i++)
				{
					blocks.push_back(connectedStack->GetBlock(i));
				}

				for (unsigned int i = 0; i < m_blocks.size(); i++)
				{
					blocks.push_back(m_blocks[i]);
				}

				for (unsigned int i = connectedBlockIndex; i < connectedStack->GetBlockCount(); i++)
				{
					blocks.push_back(connectedStack->GetBlock(i));
				}

				connectedStack->ImportBlocks(&blocks);

				Global::CutRenderingPlane = true;
				(*m_functionRemove)(this);
				return;
			}
			else
			{
				if (Global::DraggingPlaneOver == nullptr)
				{
					if (((Plane*)m_planePtr)->IsToolbar())
					{
						delete this;
						return;
					}

					(*m_functionAdd)(this);
				}
				else
				{
					(*m_functionAddOver)(this);
				}
			}

			//revert
			m_relitivePosition.x = m_setPosition.x - m_planeInnerPosition->x;
			m_relitivePosition.y = m_setPosition.y - m_planeInnerPosition->y;

			ReloadAllBlocks();
		}
	}
	else
	{
		m_relitivePosition.x = m_setPosition.x - m_planeInnerPosition->x;
		m_relitivePosition.y = m_setPosition.y - m_planeInnerPosition->y;
	}

	m_absolutePosition.x = m_relitivePosition.x + m_planePosition->x;
	m_absolutePosition.y = m_relitivePosition.y + m_planePosition->y;

	if (m_dragging)
	{
		m_highlightedShape = -1;

		for (uint32_t i = 0; i < m_ifShapeHighlight.size(); i++)
			m_ifShapeHighlight[i].setPosition(m_ifShapePositionHighlight[i] + (sf::Vector2f)m_absolutePosition);

		for (uint32_t i = 0; i < m_ifShapeHighlight.size(); i++)
		{
			if (Global::MousePosition.x > m_ifShapeHighlight[i].getPosition().x + m_planePosition->x && Global::MousePosition.x < m_ifShapeHighlight[i].getPosition().x + m_planePosition->x + Global::BlockHeight &&
				Global::MousePosition.y > m_ifShapeHighlight[i].getPosition().y + m_planePosition->y && Global::MousePosition.y < m_ifShapeHighlight[i].getPosition().y + m_planePosition->y + Global::BlockHeight)
			{
				m_ifShapeHighlight[i].setFillColor(sf::Color(60, 60, 60));
				m_highlightedShape = i;
			}
			else
				m_ifShapeHighlight[i].setFillColor(sf::Color(100, 100, 100));
		}

		for (uint32_t i = 0; i < m_ifShapes.size(); i++)
		{
			m_ifShapes[i].setPosition(m_ifShapePositions[i] + (sf::Vector2f)m_absolutePosition);

			if (m_ifShapeIdx[i] == m_highlightedShape)
				m_ifShapes[i].setFillColor(sf::Color(255, 132, 0));
			else
				m_ifShapes[i].setFillColor(sf::Color(70, 70, 70));
		}
	}
	else
	{
		m_highlightedShape = -1;

		for (uint32_t i = 0; i < m_ifShapeHighlight.size(); i++)
			m_ifShapeHighlight[i].setPosition(m_ifShapePositionHighlight[i] + (sf::Vector2f)m_relitivePosition);

		for (uint32_t i = 0; i < m_ifShapeHighlight.size(); i++)
		{
			if (Global::MousePosition.x > m_ifShapeHighlight[i].getPosition().x + m_planePosition->x && Global::MousePosition.x < m_ifShapeHighlight[i].getPosition().x + m_planePosition->x + Global::BlockHeight &&
				Global::MousePosition.y > m_ifShapeHighlight[i].getPosition().y + m_planePosition->y && Global::MousePosition.y < m_ifShapeHighlight[i].getPosition().y + m_planePosition->y + Global::BlockHeight)
			{
				m_ifShapeHighlight[i].setFillColor(sf::Color(60, 60, 60));
				m_highlightedShape = i;
			}
			else
				m_ifShapeHighlight[i].setFillColor(sf::Color(100, 100, 100));
		}

		for (uint32_t i = 0; i < m_ifShapes.size(); i++)
		{
			m_ifShapes[i].setPosition(m_ifShapePositions[i] + (sf::Vector2f)m_relitivePosition);

			if (m_ifShapeIdx[i] == m_highlightedShape)
				m_ifShapes[i].setFillColor(sf::Color(255, 132, 0));
			else
				m_ifShapes[i].setFillColor(sf::Color(70, 70, 70));
		}
	}

	if (Global::Dragging && Global::DraggingStack == this)
	{
		int16_t visualOffset = 0;

		if (m_validHighlighting)
		{
			for (unsigned int i = 0; i < m_blocks.size(); i++)
			{
				std::string un = m_blocks[i]->GetUnlocalizedName();

				if (un == "vin_execution_if_end" || un == "vin_execution_else" || un == "vin_execution_if_else")
					visualOffset--;

				m_blocks[i]->FrameUpdate(window, sf::Vector2f(visualOffset * 20, 0), true);

				if (un == "vin_execution_if" || un == "vin_execution_else" || un == "vin_execution_if_else")
					visualOffset++;
			}
		}
		else
		{
			for (unsigned int i = 0; i < m_blocks.size(); i++)
				m_blocks[i]->FrameUpdate(window, sf::Vector2f(visualOffset * 20, 0), true);
		}
	}
	else
	{
		int16_t visualOffset = 0;

		if (m_validHighlighting)
		{
			for (unsigned int i = 0; i < m_blocks.size(); i++)
			{
				std::string un = m_blocks[i]->GetUnlocalizedName();

				if (un == "vin_execution_if_end" || un == "vin_execution_else" || un == "vin_execution_if_else")
					visualOffset--;

				m_blocks[i]->FrameUpdate(window, sf::Vector2f(visualOffset * 20, 0));

				if (un == "vin_execution_if" || un == "vin_execution_else" || un == "vin_execution_if_else")
					visualOffset++;
			}
		}
		else
		{
			for (unsigned int i = 0; i < m_blocks.size(); i++)
				m_blocks[i]->FrameUpdate(window, sf::Vector2f(visualOffset * 20, 0));
		}
	}

	for (uint32_t i = 0; i < m_blocks.size(); i++)
	{
		if (m_blocks[i]->GetNext())
		{
			if (i + 1 < m_blocks.size())
				m_blocks[i + 1]->SelectFirstArgument();
			else
				Global::SelectedArgument = nullptr;

			break;
		}
	}
}

void Stack::SetupInPlane(sf::Vector2u* planePosition, sf::Vector2i* planeInnerPosition, void* planePtr, std::function<void(Stack* stack)>* functionAdd, std::function<void(Stack* stack)>* functionRemove, std::function<void(Stack* stack)>* functionMoveTop, std::function<void(Stack* stack)>* functionAddOver)
{
	m_planePosition = planePosition;
	m_planeInnerPosition = planeInnerPosition;
	m_functionAdd = functionAdd;
	m_functionRemove = functionRemove;
	m_functionMoveTop = functionMoveTop;
	m_functionAddOver = functionAddOver;
	m_planePtr = planePtr;
}

void* Stack::GetPlanePointer()
{
	return m_planePtr;
}

unsigned int Stack::GetBlockCount()
{
	return m_blocks.size();
}

sf::Vector2i Stack::GetAbsolutePosition()
{
	return m_absolutePosition;
}

sf::Vector2i Stack::GetRelitivePosition()
{
	return m_relitivePosition;
}

sf::Vector2i Stack::GetSetPosition()
{
	return m_setPosition;
}

sf::Vector2u Stack::GetPlanePosition()
{
	return *m_planePosition;
}

void Stack::SetRelitivePosition(sf::Vector2i position)
{
	m_setPosition = position;
}

unsigned int Stack::GetBlockWidth(unsigned int index)
{
	if (index >= m_blocks.size())
		return 0;
	else
		return m_blocks[index]->GetWidth();
}

Block* Stack::GetBlock(unsigned int index)
{
	return m_blocks[index];
}

void Stack::CopyEverything(Stack* stack, BlockRegistry* registry)
{
	m_absolutePosition = stack->GetAbsolutePosition();
	m_relitivePosition = stack->GetRelitivePosition();

	for (unsigned int i = 0; i < stack->GetBlockCount(); i++)
	{
		Block* block = new Block(stack->GetBlock(i)->GetUnlocalizedName(), registry);
		AddBlock(block);
		
		std::vector<std::string>* args = stack->GetBlock(i)->GetUsedArgumentSetup();
		block->SetArgData(*args);
		delete args;
	}
}

void Stack::DragDown(sf::Vector2i mousePosition)
{
	m_cutRendering = true;
	Global::DraggingStack = (void*)this;
	Global::DraggingStackConnected = nullptr;
	Global::DraggingStackConnectedIndex = 0;
	Global::DraggingPlane = m_planePtr;
	(*m_functionRemove)(this);
	Global::Dragging = true;
	m_dragging = true;
	Global::SkipFrame = true;
	m_draggingMouseBegin = mousePosition;

	m_draggingType = DraggingType::DOWN;
}

void Stack::DragUp(sf::Vector2i mousePosition)
{
	m_cutRendering = true;
	Global::DraggingStack = (void*)this;
	Global::DraggingStackConnected = nullptr;
	Global::DraggingStackConnectedIndex = 0;
	Global::DraggingPlane = m_planePtr;
	(*m_functionRemove)(this);
	Global::Dragging = true;
	m_dragging = true;
	Global::SkipFrame = true;
	m_draggingMouseBegin = mousePosition;

	m_draggingType = DraggingType::UP;
}

bool Stack::MouseButton(bool down, sf::Vector2i position, sf::Mouse::Button button)
{
	for (unsigned int i = 0; i < m_blocks.size(); i++)
	{
		if (m_blocks[i]->MouseButton(down, position, button))
			return true;
	}

	return false;
}

void Stack::ReloadVanity()
{
	m_ifShapes.clear();
	m_ifShapePositions.clear();
	m_ifShapeHighlight.clear();
	m_ifShapePositionHighlight.clear();
	m_ifShapeIdx.clear();

	std::vector<StatmentIf> statments = RuntimeHandler::ProcessIfStatments(this, true);
	m_savedVanity = statments;
	m_validHighlighting = statments.size() > 0;

	for (uint32_t i = 0; i < statments.size(); i++)
	{
		{
			m_ifShapePositions.push_back(sf::Vector2f(-IF_MARGIN + -IF_WIDTH, statments[i].Location * Global::BlockHeight + (Global::BlockHeight / 2)));

			m_ifShapeIdx.push_back(i);
			m_ifShapes.push_back(sf::RectangleShape(sf::Vector2f(IF_WIDTH, 1)));
		}

		{
			m_ifShapePositions.push_back(sf::Vector2f(-IF_MARGIN + -IF_WIDTH, statments[i].LocationEnd * Global::BlockHeight + (Global::BlockHeight / 2)));

			m_ifShapeIdx.push_back(i);
			m_ifShapes.push_back(sf::RectangleShape(sf::Vector2f(IF_WIDTH, 1)));
		}

		if (statments[i].HasElse)
		{
			m_ifShapePositions.push_back(sf::Vector2f(-IF_MARGIN + -IF_WIDTH, statments[i].LocationElse * Global::BlockHeight + (Global::BlockHeight / 2)));

			m_ifShapeIdx.push_back(i);
			m_ifShapes.push_back(sf::RectangleShape(sf::Vector2f(IF_WIDTH, 1)));
		}
		
		if (statments[i].HasElseIf)
		{
			for (uint32_t a = 0; a < statments[i].LocationElseIf.size(); a++)
			{
				m_ifShapePositions.push_back(sf::Vector2f(-IF_MARGIN + -IF_WIDTH, statments[i].LocationElseIf[a] * Global::BlockHeight + (Global::BlockHeight / 2)));

				m_ifShapeIdx.push_back(i);
				m_ifShapes.push_back(sf::RectangleShape(sf::Vector2f(IF_WIDTH, 1)));
			}
		}

		{
			m_ifShapePositions.push_back(sf::Vector2f(-IF_MARGIN + -IF_WIDTH, statments[i].Location * Global::BlockHeight + (Global::BlockHeight / 2)));

			m_ifShapeIdx.push_back(i);
			m_ifShapes.push_back(sf::RectangleShape(sf::Vector2f(1, (statments[i].LocationEnd - statments[i].Location) * Global::BlockHeight)));
		}

		{
			m_ifShapePositionHighlight.push_back(sf::Vector2f(-IF_MARGIN + -IF_WIDTH + -((int)Global::BlockHeight / 2), statments[i].Location * Global::BlockHeight + (Global::BlockHeight / 2) - (Global::BlockHeight / 2)));

			m_ifShapeHighlight.push_back(sf::RectangleShape(sf::Vector2f(Global::BlockHeight, Global::BlockHeight)));
			m_ifShapeHighlight.back().setOutlineColor(sf::Color::Black);
			m_ifShapeHighlight.back().setOutlineThickness(1);
		}
	}
}

std::vector<StatmentIf> Stack::GetVanity()
{
	return m_savedVanity;
}
