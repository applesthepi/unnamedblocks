#include "Block.h"
#include "args/ArgumentText.h"
#include "args/ArgumentString.h"
#include "args/ArgumetReal.h"
#include "args/ArgumentBoolean.h"

#include <SFML/Graphics.hpp>

Block::Block(std::string type)
{
	m_unlocalizedName = type;
	m_absolutePosition = new sf::Vector2i();
	m_relitivePosition = new sf::Vector2i();
	m_selected = false;
	m_wasDown = false;

	const RegBlock* blockDetails = BlockRegistry::GetBlock(type);
	if (blockDetails == nullptr)
		return;

	const RegCatagory* catagoryDetails = BlockRegistry::GetCatagory(blockDetails->Catagory);
	if (catagoryDetails == nullptr)
		return;

	std::vector<BlockArgument> args = blockDetails->Args;
	m_background.setFillColor(catagoryDetails->Color);

	unsigned int offset = Global::BlockBorder;

	for (unsigned int i = 0; i < args.size(); i++)
	{
		if (args[i].Type == BlockArgumentType::TEXT)
		{
			ArgumentText* arg = new ArgumentText(sf::Vector2u(offset, 0), args[i].Value);
			arg->SetupInBlock(m_relitivePosition, m_absolutePosition);
			offset += arg->GetArgumentRawWidth() + Global::BlockBorder;

			m_args.push_back(arg);
		}
		else if (args[i].Type == BlockArgumentType::STRING)
		{
			ArgumentString* arg = new ArgumentString(sf::Vector2u(offset, 0), args[i].Value);
			arg->SetupInBlock(m_relitivePosition, m_absolutePosition);
			offset += arg->GetArgumentRawWidth() + Global::BlockBorder;

			m_args.push_back(arg);
		}
		else if (args[i].Type == BlockArgumentType::REAL)
		{
			ArgumentReal* arg = new ArgumentReal(sf::Vector2u(offset, 0), args[i].Value);
			arg->SetupInBlock(m_relitivePosition, m_absolutePosition);
			offset += arg->GetArgumentRawWidth() + Global::BlockBorder;

			m_args.push_back(arg);
		}
		else if (args[i].Type == BlockArgumentType::BOOL)
		{
			ArgumentBoolean* arg = new ArgumentBoolean(sf::Vector2u(offset, 0), args[i].Value);
			arg->SetupInBlock(m_relitivePosition, m_absolutePosition);
			offset += arg->GetArgumentRawWidth() + Global::BlockBorder;

			m_args.push_back(arg);
		}
	}

	m_width = offset;
}

Block::~Block()
{
	delete m_absolutePosition;
	delete m_relitivePosition;

	for (unsigned int i = 0; i < m_args.size(); i++)
	{
		m_args[i]->Deallocate();
		delete m_args[i];
	}
}

void Block::Render(sf::RenderTexture* render, sf::RenderWindow* window)
{
	if (render == nullptr)
	{
		window->draw(m_background);

		for (unsigned int i = 0; i < m_args.size(); i++)
		{
			m_args[i]->Render(nullptr, window);
		}
	}
	else
	{
		render->draw(m_background);

		for (unsigned int i = 0; i < m_args.size(); i++)
		{
			m_args[i]->Render(render, window);
		}
	}
}

void Block::FrameUpdate(sf::RenderWindow* window, bool global)
{
	//std::cout << m_stackAbsolute->x << ", " << m_stackAbsolute->y << std::endl;
	m_absolutePosition->x = m_stackAbsolute->x;
	m_absolutePosition->y = m_stackAbsolute->y + (m_index * Global::BlockHeight);

	m_relitivePosition->x = m_stackRelitive->x;
	m_relitivePosition->y = m_stackRelitive->y + (m_index * Global::BlockHeight);

	{
		unsigned int offset = Global::BlockBorder;

		for (unsigned int i = 0; i < m_args.size(); i++)
		{
			m_args[i]->SetRelitivePosition(sf::Vector2u(offset, 0));
			offset += m_args[i]->GetArgumentRawWidth() + Global::BlockBorder;
		}

		m_width = offset;
	}

	if (global)
	{
		m_background.setPosition(m_absolutePosition->x, m_absolutePosition->y);
		m_background.setSize(sf::Vector2f(m_width, Global::BlockHeight));

		for (unsigned int i = 0; i < m_args.size(); i++)
		{
			m_args[i]->Update(window, true);
		}
	}
	else
	{
		m_background.setPosition(m_relitivePosition->x, m_relitivePosition->y);
		m_background.setSize(sf::Vector2f(m_width, Global::BlockHeight));

		for (unsigned int i = 0; i < m_args.size(); i++)
		{
			m_args[i]->Update(window);
		}
	}
}

void Block::SetArgData(std::vector<std::string>* data)
{
	unsigned int dataIndex = 0;

	for (unsigned int i = 0; i < m_args.size(); i++)
	{
		if (dataIndex == data->size())
			break;

		if (m_args[i]->HasData())
		{
			m_args[i]->SetData((*data)[dataIndex]);
			dataIndex++;
		}
	}
}

void Block::SetupInStack(unsigned int index, sf::Vector2i* stackAbsolute, sf::Vector2i* stackRelitive, std::function<void(unsigned int index, sf::Vector2i mousePosition)>* functionSplit, std::function<void(unsigned int index, sf::Vector2i mousePosition)>* functionContext)
{
	m_stackAbsolute = stackAbsolute;
	m_stackRelitive = stackRelitive;
	m_index = index;
	m_functionSplit = functionSplit;
	m_functionContext = functionContext;
}

unsigned int Block::GetWidth()
{
	return m_width;
}

Argument* Block::GetArgument(unsigned int index)
{
	return m_args[index];
}

BlockRuntimeReturn Block::GetUsedArgumentsRuntime()
{
	std::vector<std::string>* dataArgs = new std::vector<std::string>();

	for (unsigned int i = 0; i < m_args.size(); i++)
	{
		if (m_args[i]->HasData())
			dataArgs->push_back(m_args[i]->GetData());
	}

	return BlockRuntimeReturn(dataArgs, dataArgs->size());
}

std::vector<std::string>* Block::GetUsedArgumentSetup()
{
	std::vector<std::string>* argData = new std::vector<std::string>();
	for (unsigned int i = 0; i < m_args.size(); i++)
	{
		if (m_args[i]->HasData())
			argData->push_back(m_args[i]->GetData());
	}

	return argData;
}

unsigned int Block::GetArgumentCount()
{
	return m_args.size();
}

std::string Block::GetUnlocalizedName()
{
	return m_unlocalizedName;
}

bool Block::MouseButton(bool down, sf::Vector2i position, sf::Mouse::Button button)
{
	if (!down)
	{
		if (m_wasDown)
			m_wasDown = false;
		return true;
	}

	bool over = position.x > m_absolutePosition->x&& position.x < m_absolutePosition->x + (int)GetWidth() &&
		position.y > m_absolutePosition->y&& position.y < m_absolutePosition->y + (int)Global::BlockHeight;

	if (over && !Global::Dragging && !m_wasDown)
	{
		bool attempt = true;

		for (unsigned int i = 0; i < m_args.size(); i++)
		{
			if (m_args[i]->MouseButton(down, position, button))
			{
				attempt = false;
				m_selected = true;
				break;
			}
		}

		if (attempt)
		{
			if (button == sf::Mouse::Right)
			{
				m_selected = false;
				(*m_functionContext)(m_index, position);
			}
			else
			{
				Global::ContextActive = false;
				m_selected = false;
				(*m_functionSplit)(m_index, position);
			}
		}
	}

	return over;
}
