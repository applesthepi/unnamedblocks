#include "Stack.h"
#include "Global.h"
#include "Plane.h"

#include <iostream>

Stack::Stack(sf::Vector2i relitivePosition)
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
		Global::ContextActive = true;
		Global::Context.Callback = m_functionContextCallback;
		Global::Context.Type = ContextType::BLOCK;
		Global::Context.Position.x = mousePosition.x + 5;
		Global::Context.Position.y = mousePosition.y + 5;
		Global::ContextData = std::to_string(index);
	};

	*m_functionContextCallback = [&](unsigned int index)
	{
		Global::Context.Type = ContextType::NONE;
		unsigned int blockIndex = std::stoul(Global::ContextData);

		if (index == 0)
		{
			Stack* stack = new Stack(m_setPosition + sf::Vector2i(0, blockIndex * Global::BlockHeight));
			(*m_functionAdd)(stack);

			for (unsigned int i = blockIndex; i < m_blocks.size(); i++)
			{
				Block* block = new Block(m_blocks[i]->GetUnlocalizedName());
				block->SetArgData(m_blocks[i]->GetUsedArgumentsRuntime().Args);
				stack->AddBlock(block);
			}

			stack->ReloadAllBlocks();
			stack->DragUp(Global::MousePosition);
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
			Stack* stack = new Stack(m_setPosition + sf::Vector2i(0, blockIndex * Global::BlockHeight));
			(*m_functionAdd)(stack);

			Block* block = new Block(m_blocks[blockIndex]->GetUnlocalizedName());
			block->SetArgData(m_blocks[blockIndex]->GetUsedArgumentsRuntime().Args);
			stack->AddBlock(block);

			stack->ReloadAllBlocks();
			stack->DragUp(Global::MousePosition);
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
			//TODO fix memory leak
			m_planePosition = new sf::Vector2u(m_planePosition->x, m_planePosition->y);
		}
		else
		{
			Stack* stayStack = new Stack(m_setPosition);
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
		}
	};
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

	if (Global::Dragging && Global::DraggingStack == this)
	{
		for (unsigned int i = 0; i < m_blocks.size(); i++)
		{
			m_blocks[i]->FrameUpdate(window, true);
		}
	}
	else
	{
		for (unsigned int i = 0; i < m_blocks.size(); i++)
		{
			m_blocks[i]->FrameUpdate(window);
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

void Stack::CopyEverything(Stack* stack)
{
	m_absolutePosition = stack->GetAbsolutePosition();
	m_relitivePosition = stack->GetRelitivePosition();

	for (unsigned int i = 0; i < stack->GetBlockCount(); i++)
	{
		Block* block = new Block(stack->GetBlock(i)->GetUnlocalizedName());
		AddBlock(block);
		
		std::vector<std::string>* args = stack->GetBlock(i)->GetUsedArgumentSetup();
		block->SetArgData(args);
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
