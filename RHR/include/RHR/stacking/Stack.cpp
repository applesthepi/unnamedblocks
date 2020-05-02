#include "Stack.h"
#include "RHR/Global.h"
#include "Plane.h"
#include "RHR/handlers/StatmentHandler.h"
#include <Cappuccino/block/ModBlock.h>

#include <Cappuccino/Logger.h>
#include <iostream>
#include <map>

#define IF_MARGIN 5
#define IF_WIDTH 20

Stack::Stack(sf::Vector2i relitivePosition, BlockRegistry* registry)
{
	m_blockRegistry = (void*)registry;
	m_setPosition = relitivePosition;
	m_functionSplit = new std::function<void(unsigned int index, sf::Vector2i mousePosition)>();
	m_functionContext = new std::function<void(unsigned int index, sf::Vector2i mousePosition)>();
	m_functionContextCallback = new std::function<void(unsigned int index)>();
	m_functionUpdatePreTexture = new std::function<void()>();
	m_cutRendering = false;
	m_contextBlockIndex = 0;
	m_highestWidth = 0;
	m_draggingType = DraggingType::DOWN;

	*m_functionUpdatePreTexture = [&]()
	{
		PreRender();
	};

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

	*m_functionContextCallback = [&](unsigned int index)
	{
		Global::ContextActive = false;
		Global::Context.Type = ContextType::NONE;
		Plane* plane = (Plane*)m_planePtr;
		if (plane->IsToolbar())
			return;
		
		unsigned int blockIndex = std::stoul(Global::ContextData);

		if (index == 0)
		{
			Stack* stack = new Stack(m_setPosition + sf::Vector2i(0, static_cast<int32_t>(blockIndex * Global::BlockHeight)), (BlockRegistry*)m_blockRegistry);
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
				int32_t statmentSelection = closestStatments.size() - 1;

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
				Block* block = new Block(m_blocks[i]->GetUnlocalizedName(), (BlockRegistry*)m_blockRegistry, m_functionUpdatePreTexture, m_functionSelectStack);
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
			Stack* stack = new Stack(m_setPosition + sf::Vector2i(0, static_cast<int32_t>(blockIndex * Global::BlockHeight)), (BlockRegistry*)m_blockRegistry);
			(*m_functionAdd)(stack);

			Block* block = new Block(m_blocks[blockIndex]->GetUnlocalizedName(), (BlockRegistry*)m_blockRegistry, m_functionUpdatePreTexture, m_functionSelectStack);
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
			Stack* stayStack = new Stack(m_setPosition, (BlockRegistry*)m_blockRegistry);
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
			//TODO optomize this
			stayStack->ReloadAllBlocks();
			stayStack->FrameUpdate(true, true);
			
			ReloadAllBlocks();
			FrameUpdate(true, true);
		}
	};

	m_functionSelectStack = new std::function<void()>();
	*m_functionSelectStack = [&]()
	{
		Global::SelectedStack = (void*)this;
	};

	m_validHighlighting = false;
}

Stack::~Stack()
{
	for (unsigned int i = 0; i < m_blocks.size(); i++)
		delete m_blocks[i];

	delete m_functionSplit;
	//delete m_functionContext;
	//delete m_functionContextCallback;
	//delete m_functionUpdatePreTexture;
}

void Stack::ImportBlocks(std::vector<Block*>* blocks)
{
	m_blocks.clear();

	for (unsigned int i = 0; i < blocks->size(); i++)
	{
		AddBlock((*blocks)[i]);
	}

	ReRender();
	ReloadVanity();
}

/// Add block to the stack
/// NOTE: Its recommended to call ReloadVanity and ReRender after this or several calls of this, but not required
/// You can also call ReloadAllBlocks in thier place, but thats more expensive.
void Stack::AddBlock(Block* block)
{
	if (m_blocks.size() > 0 && ((Plane*)m_planePtr)->IsToolbar())
		return;
	
	block->SetupInStack(m_blocks.size(), &m_absolutePosition, &m_relitivePosition, m_functionSplit, m_functionContext);
	block->UpdateShorts(m_functionUpdatePreTexture, m_functionSelectStack);

	m_blocks.push_back(block);
}

void Stack::ReloadAllBlocks()
{
	for (unsigned int i = 0; i < m_blocks.size(); i++)
	{
		m_blocks[i]->SetupInStack(i, &m_absolutePosition, &m_relitivePosition, m_functionSplit, m_functionContext);
		m_blocks[i]->UpdateShorts(m_functionUpdatePreTexture, m_functionSelectStack);
	}

	ReRender();
	ReloadVanity();
}

void Stack::Render(sf::RenderTexture* render, sf::RenderWindow* window)
{
	if (render == nullptr)
	{
		m_preShape.setPosition((sf::Vector2f)GetAbsolutePosition());
		window->draw(m_preShape);
		
		if (m_validHighlighting)
		{
			for (uint32_t i = 0; i < m_ifShapes.size(); i++)
			{
				if (m_ifShapeIdx[i] != (uint32_t)m_highlightedShape)
					window->draw(m_ifShapes[i]);
			}

			if (m_highlightedShape != -1)
			{
				for (uint32_t i = 0; i < m_ifShapes.size(); i++)
				{
					if (m_ifShapeIdx[i] == (uint32_t)m_highlightedShape)
						window->draw(m_ifShapes[i]);
				}
			}

			for (uint32_t i = 0; i < m_ifShapeHighlight.size(); i++)
				window->draw(m_ifShapeHighlight[i]);
		}
	}
	else
	{
		m_preShape.setPosition((sf::Vector2f)GetRelitivePosition());
		render->draw(m_preShape);

		if (m_validHighlighting)
		{
			for (uint32_t i = 0; i < m_ifShapes.size(); i++)
			{
				if (m_ifShapeIdx[i] != (uint32_t)m_highlightedShape)
					render->draw(m_ifShapes[i]);
			}

			if (m_highlightedShape != -1)
			{
				for (uint32_t i = 0; i < m_ifShapes.size(); i++)
				{
					if (m_ifShapeIdx[i] == (uint32_t)m_highlightedShape)
						render->draw(m_ifShapes[i]);
				}
			}

			for (uint32_t i = 0; i < m_ifShapeHighlight.size(); i++)
				render->draw(m_ifShapeHighlight[i]);
		}
	}
}

void Stack::FrameUpdate(bool /*updateBlocks*/, bool forceUpdate)
{
	bool mouseDown = sf::Mouse::isButtonPressed(sf::Mouse::Left);

	if (m_dragging)
	{
		bool wantDown = false;
		
		if (m_draggingType == DraggingType::DOWN)
			wantDown = mouseDown;
		else if (m_draggingType == DraggingType::UP)
			wantDown = !mouseDown;

		if (wantDown)
		{
			m_relitivePosition.x = (Global::MousePosition.x - m_draggingMouseBegin.x) + m_setPosition.x - m_planeInnerPosition->x;
			m_relitivePosition.y = (Global::MousePosition.y - m_draggingMouseBegin.y) + m_setPosition.y - m_planeInnerPosition->y;
		}
		else
		{
			m_relitivePosition.x = (Global::MousePosition.x - m_draggingMouseBegin.x) + m_setPosition.x;
			m_relitivePosition.y = (Global::MousePosition.y - m_draggingMouseBegin.y) + m_setPosition.y;

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

	m_absolutePosition.x = m_relitivePosition.x + static_cast<int32_t>(m_planePosition->x);
	m_absolutePosition.y = m_relitivePosition.y + static_cast<int32_t>(m_planePosition->y);

	if (m_dragging)
	{
		m_highlightedShape = -1;

		for (uint32_t i = 0; i < m_ifShapeHighlight.size(); i++)
			m_ifShapeHighlight[i].setPosition(m_ifShapePositionHighlight[i] + (sf::Vector2f)m_absolutePosition);

		for (uint32_t i = 0; i < m_ifShapeHighlight.size(); i++)
		{
			if (Global::MousePosition.x > m_ifShapeHighlight[i].getPosition().x + 
				m_planePosition->x && Global::MousePosition.x < m_ifShapeHighlight[i].getPosition().x + (int64_t)m_planePosition->x + Global::BlockHeight &&
				Global::MousePosition.y > m_ifShapeHighlight[i].getPosition().y + (int64_t)m_planePosition->y && Global::MousePosition.y < m_ifShapeHighlight[i].getPosition().y + (int64_t)m_planePosition->y + Global::BlockHeight)
			{
				m_ifShapeHighlight[i].setFillColor(sf::Color(60, 60, 60));
				m_highlightedShape = static_cast<int32_t>(i);
			}
			else
				m_ifShapeHighlight[i].setFillColor(sf::Color(100, 100, 100));
		}

		for (uint32_t i = 0; i < m_ifShapes.size(); i++)
		{
			m_ifShapes[i].setPosition(m_ifShapePositions[i] + (sf::Vector2f)m_absolutePosition);

			if (m_ifShapeIdx[i] == (uint32_t)m_highlightedShape)
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
			if (Global::MousePosition.x > (int64_t)m_ifShapeHighlight[i].getPosition().x + (int64_t)m_planePosition->x && Global::MousePosition.x < (int64_t)m_ifShapeHighlight[i].getPosition().x + (int64_t)m_planePosition->x + Global::BlockHeight &&
				Global::MousePosition.y > (int64_t)m_ifShapeHighlight[i].getPosition().y + (int64_t)m_planePosition->y && Global::MousePosition.y < (int64_t)m_ifShapeHighlight[i].getPosition().y + (int64_t)m_planePosition->y + Global::BlockHeight)
			{
				m_ifShapeHighlight[i].setFillColor(sf::Color(60, 60, 60));
				m_highlightedShape = static_cast<int32_t>(i);
			}
			else
				m_ifShapeHighlight[i].setFillColor(sf::Color(100, 100, 100));
		}

		for (uint32_t i = 0; i < m_ifShapes.size(); i++)
		{
			m_ifShapes[i].setPosition(m_ifShapePositions[i] + (sf::Vector2f)m_relitivePosition);

			if (m_ifShapeIdx[i] == (uint32_t)m_highlightedShape)
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

				//m_blocks[i]->FrameUpdate(sf::Vector2f(visualOffset * 20, 0), true);

				if (un == "vin_execution_if" || un == "vin_execution_else" || un == "vin_execution_if_else")
					visualOffset++;
			}
		}
		else
		{
			/*
			for (unsigned int i = 0; i < m_blocks.size(); i++)
				m_blocks[i]->FrameUpdate(sf::Vector2f(visualOffset * 20, 0), true);
			*/
		}
	}
	else
	{
		//if (!updateBlocks || Global::SelectedStack == nullptr)
		//	return;

		int16_t visualOffset = 0;

		if (m_validHighlighting)
		{
			for (unsigned int i = 0; i < m_blocks.size(); i++)
			{
				std::string un = m_blocks[i]->GetUnlocalizedName();

				if (un == "vin_execution_if_end" || un == "vin_execution_else" || un == "vin_execution_if_else")
					visualOffset--;

				m_blocks[i]->FrameUpdate(forceUpdate || Global::SelectedBlock == m_blocks[i] || m_blocks[i]->IsBounding((sf::Vector2f)Global::MousePosition), sf::Vector2f(visualOffset * 20, 0));

				if (un == "vin_execution_if" || un == "vin_execution_else" || un == "vin_execution_if_else")
					visualOffset++;
			}
		}
		else
		{
			if (forceUpdate)
			{
				for (unsigned int i = 0; i < m_blocks.size(); i++)
				{
					m_blocks[i]->FrameUpdate(forceUpdate || Global::SelectedBlock == m_blocks[i] || m_blocks[i]->IsBounding((sf::Vector2f)Global::MousePosition), sf::Vector2f(visualOffset * 20, 0));
					//m_blocks[i]->PreRender();
				}
			}
			else
			{
				for (unsigned int i = 0; i < m_blocks.size(); i++)
					m_blocks[i]->FrameUpdate(forceUpdate || Global::SelectedBlock == m_blocks[i] || m_blocks[i]->IsBounding((sf::Vector2f)Global::MousePosition), sf::Vector2f(visualOffset * 20, 0));
			}
		}
	}

	for (uint32_t i = 0; i < m_blocks.size(); i++)
	{
		if (m_blocks[i]->GetNext())
		{
			if (i + 1 < m_blocks.size())
				m_blocks[i + 1]->SelectFirstArgument();
			else
			{
				Global::SelectedArgument = nullptr;
				Global::SelectedBlock = nullptr;
				Global::SelectedStack = nullptr;
			}

			break;
		}
	}
	
	if (forceUpdate)
		ReRender();
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

	FrameUpdate(true);
}

void* Stack::GetPlanePointer() const
{
	return m_planePtr;
}

unsigned int Stack::GetBlockCount() const
{
	return m_blocks.size();
}

sf::Vector2i Stack::GetAbsolutePosition() const
{
	return m_absolutePosition;
}

sf::Vector2i Stack::GetRelitivePosition() const
{
	return m_relitivePosition;
}

sf::Vector2i Stack::GetSetPosition() const
{
	return m_setPosition;
}

sf::Vector2u Stack::GetPlanePosition() const
{
	return *m_planePosition;
}

void Stack::SetRelitivePosition(sf::Vector2i position)
{
	m_setPosition = position;
}

unsigned int Stack::GetBlockWidth(unsigned int index) const
{
	if (index >= m_blocks.size())
		return 0;
	else
		return m_blocks[index]->GetWidth();
}

Block* Stack::GetBlock(unsigned int index) const
{
	return m_blocks[index];
}

void Stack::CopyEverything(Stack* stack, BlockRegistry* registry)
{
	m_absolutePosition = stack->GetAbsolutePosition();
	m_relitivePosition = stack->GetRelitivePosition();

	for (unsigned int i = 0; i < stack->GetBlockCount(); i++)
	{
		Block* block = new Block(stack->GetBlock(i)->GetUnlocalizedName(), registry, m_functionUpdatePreTexture, m_functionSelectStack);
		AddBlock(block);
		
		std::vector<std::string>* args = stack->GetBlock(i)->GetUsedArgumentSetup();
		block->SetArgData(*args);
		delete args;
	}

	ReloadAllBlocks();
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

std::function<void()>* Stack::GetFunctionUpdate() const
{
	return m_functionUpdatePreTexture;
}

std::function<void()>* Stack::GetFunctionSelect() const
{
	return m_functionSelectStack;
}

bool Stack::IsBounding(const sf::Vector2f& mousePos) const
{
	return mousePos.x > m_absolutePosition.x && mousePos.x < m_absolutePosition.x + static_cast<int32_t>(m_highestWidth) && mousePos.y > m_absolutePosition.y && mousePos.y < m_absolutePosition.y + static_cast<int32_t>(m_blocks.size() * Global::BlockHeight);
}                                                                                                                                                                                                    

void Stack::AddPosition(const sf::Vector2i& change)
{
	m_setPosition += change;
}

bool Stack::MouseButton(bool down, sf::Vector2i position, sf::Mouse::Button button)
{
	for (unsigned int i = 0; i < m_blocks.size(); i++)
	{
		if (m_blocks[i]->IsBounding((sf::Vector2f)position))
		{
			if (m_blocks[i]->MouseButton(down, position, button))
				return true;
		}
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

	std::vector<StatmentIf> statments = StatmentHandler::ProcessIfStatments(this);
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

std::vector<StatmentIf> Stack::GetVanity() const
{
	return m_savedVanity;
}

void Stack::ReRender()
{
	for (uint64_t i = 0; i < m_blocks.size(); i++)
		m_blocks[i]->PreRender();

	PreRender();
}

//TODO SOTP IT!!! MOD IT PLEASEE!!!
void Stack::IndexVariables()
{
	std::vector<std::string> varaibleIdx;
	varaibleIdx.push_back("");//confirm that it starts at 1 and not 0

	//TODO all of this is bad and needs to be in the modding software
	for (uint64_t i = 0; i < m_blocks.size(); i++)
	{
		//TODO add to modding software
		std::string un = m_blocks[i]->GetUnlocalizedName();
		if (un == "vin_varaibles_stack_real" || un == "vin_varaibles_stack_string" || un == "vin_varaibles_stack_bool" || un == "vin_varaibles_heap_real" || un == "vin_varaibles_heap_string" || un == "vin_varaibles_heap_bool" || un == "vin_varaibles_free_real" || un == "vin_varaibles_free_string" || un == "vin_varaibles_free_bool")
		{
			//TODO actually really add this to modding software
			std::string* vName = m_blocks[i]->GetArgument(1)->GetDataRaw();

			bool found = false;

			for (uint64_t a = 0; a < varaibleIdx.size(); a++)
			{
				if (varaibleIdx[a] == *vName)
				{
					found = true;
					m_blocks[i]->GetArgument(1)->SetData(std::to_string(a));

					break;
				}
			}

			if (!found)
			{
				varaibleIdx.push_back(*m_blocks[i]->GetArgument(1)->GetDataRaw());
				m_blocks[i]->GetArgument(1)->SetData(std::to_string(varaibleIdx.size() - 1));
			}
		}
		else
		{
			for (uint64_t a = 0; a < m_blocks[i]->GetArgumentCount(); a++)
			{
				if (*m_blocks[i]->GetArgument(a)->GetMode() == BlockArgumentVariableMode::VAR)
				{
					std::string* vName = m_blocks[i]->GetArgument(a)->GetDataRaw();

					bool found = false;

					for (uint64_t b = 0; b < varaibleIdx.size(); b++)
					{
						if (varaibleIdx[b] == *vName)
						{
							found = true;
							m_blocks[i]->GetArgument(a)->SetData(std::to_string(b));

							break;
						}
					}

					if (!found)
					{
						varaibleIdx.push_back(*m_blocks[i]->GetArgument(a)->GetDataRaw());
						m_blocks[i]->GetArgument(a)->SetData(std::to_string(varaibleIdx.size() - 1));

						Logger::Warn("detected use a variable before it it declared!");
					}
				}
			}
		}
	}
}

void Stack::PreRender()
{
	uint64_t widest = 0;
	for (unsigned int i = 0; i < m_blocks.size(); i++)
	{
		if (m_blocks[i]->GetWidth() > widest)
			widest = m_blocks[i]->GetWidth();
	}

	m_highestWidth = widest;
	
	if (widest == 0 && m_blocks.size() == 0)
		m_preTexture.create(1, 1);
	else
	{
		if (widest != m_preTexture.getSize().x || m_preTexture.getSize().y != m_blocks.size() * Global::BlockHeight)
			m_preTexture.create(widest, m_blocks.size() * Global::BlockHeight);
	}

	m_preTexture.clear(sf::Color(0, 0, 0, 0));

	for (unsigned int i = 0; i < m_blocks.size(); i++)
		m_blocks[i]->RenderToImage(&m_preTexture, i);
	m_preShape.setTexture(&m_preTexture.getTexture());
	//m_preShape.setFillColor(sf::Color::Green);
	m_preShape.setSize(sf::Vector2f(widest, m_blocks.size() * Global::BlockHeight));
	m_preShape.setTextureRect(sf::IntRect(0, static_cast<int32_t>(m_preTexture.getSize().y), static_cast<int32_t>(m_preTexture.getSize().x), -1 * static_cast<int32_t>(m_preTexture.getSize().y)));
}
