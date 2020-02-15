#include "Block.h"
#include "args/ArgumentText.h"
#include "args/ArgumentString.h"
#include "args/ArgumetReal.h"
#include "args/ArgumentBoolean.h"
#include "handlers/Logger.h"

#include <SFML/Graphics.hpp>

Block::Block(std::string type, BlockRegistry* registry, std::function<void()>* functionUpdatePreTexture, std::function<void()>* functionSelectStack)
{
	m_unlocalizedName = type;
	m_absolutePosition = new sf::Vector2i();
	m_relitivePosition = new sf::Vector2i();
	m_selected = false;
	m_wasDown = false;
	m_next = false;
	m_functionUpdatePreTexture = functionUpdatePreTexture;
	m_functionSelectStack = functionSelectStack;

	const RegBlock* blockDetails = registry->GetBlock(type);
	if (blockDetails == nullptr)
	{
		m_unlocalizedName = "vin_null";
		blockDetails = registry->GetBlock("vin_null");
		
		if (blockDetails == nullptr)
		{
			Logger::Warn("null block does not exist!");
			return;
		}
	}

	const RegCatagory* catagoryDetails = registry->GetCatagory(blockDetails->Catagory);
	if (catagoryDetails == nullptr)
		return;

	std::vector<BlockArgument> args = blockDetails->Args;
	m_background.setFillColor(catagoryDetails->Color);

	unsigned int offset = Global::BlockBorder;

	m_functionUpdatePreTextureArgs = new std::function<void()>();
	*m_functionUpdatePreTextureArgs = [&]()
	{
		PreRender();
		(*m_functionUpdatePreTexture)();
	};

	m_functionSelect = new std::function<void()>();
	*m_functionSelect = [&]()
	{
		Global::SelectedBlock = (void*)this;
		(*m_functionSelectStack)();
	};

	for (unsigned int i = 0; i < args.size(); i++)
	{
		if (args[i].Type == BlockArgumentType::TEXT)
		{
			ArgumentText* arg = new ArgumentText(sf::Vector2u(offset, 0));

			arg->SetupInBlock(m_relitivePosition, m_absolutePosition, m_functionUpdatePreTextureArgs, m_functionSelect);
			arg->SetMode(BlockArgumentVariableMode::RAW);
			arg->SetData(args[i].Value);

			offset += arg->GetArgumentRawWidth() + Global::BlockBorder;

			m_args.push_back(arg);
		}
		else if (args[i].Type == BlockArgumentType::STRING)
		{
			ArgumentString* arg = new ArgumentString(sf::Vector2u(offset, 0));

			arg->SetupInBlock(m_relitivePosition, m_absolutePosition, m_functionUpdatePreTextureArgs, m_functionSelect);
			arg->SetMode(args[i].Value.front() == '0' ? BlockArgumentVariableMode::RAW : BlockArgumentVariableMode::VAR);
			arg->SetData(args[i].Value.substr(1, args[i].Value.length() - 1));
			arg->ReInspectData();

			offset += arg->GetArgumentRawWidth() + Global::BlockBorder;

			m_args.push_back(arg);
		}
		else if (args[i].Type == BlockArgumentType::REAL)
		{
			ArgumentReal* arg = new ArgumentReal(sf::Vector2u(offset, 0));

			arg->SetupInBlock(m_relitivePosition, m_absolutePosition, m_functionUpdatePreTextureArgs, m_functionSelect);
			arg->SetMode(args[i].Value.front() == '0' ? BlockArgumentVariableMode::RAW : BlockArgumentVariableMode::VAR);
			arg->SetData(args[i].Value.substr(1, args[i].Value.length() - 1));
			arg->ReInspectData();

			offset += arg->GetArgumentRawWidth() + Global::BlockBorder;

			m_args.push_back(arg);
		}
		else if (args[i].Type == BlockArgumentType::BOOL)
		{
			ArgumentBoolean* arg = new ArgumentBoolean(sf::Vector2u(offset, 0));

			arg->SetupInBlock(m_relitivePosition, m_absolutePosition, m_functionUpdatePreTextureArgs, m_functionSelect);
			arg->SetMode(args[i].Value.front() == '0' ? BlockArgumentVariableMode::RAW : BlockArgumentVariableMode::VAR);
			arg->SetData(args[i].Value.substr(1, args[i].Value.length() - 1));
			arg->ReInspectData();

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

void Block::UpdateShorts(std::function<void()>* functionUpdatePreTexture, std::function<void()>* functionSelectStack)
{
	m_functionUpdatePreTexture = functionUpdatePreTexture;
	m_functionSelectStack = functionSelectStack;

	for (uint64_t i = 0; i < m_args.size(); i++)
		m_args[i]->SetupInBlock(m_relitivePosition, m_absolutePosition, m_functionUpdatePreTextureArgs, m_functionSelect);
}

void Block::Render(sf::RenderTexture* render, sf::RenderWindow* window)
{
	
}

void Block::FrameUpdate(bool updateArgs, sf::Vector2f visualOffset, bool global)
{
	m_visualOffset = visualOffset;
	m_absolutePosition->x = m_stackAbsolute->x + visualOffset.x;
	m_absolutePosition->y = m_stackAbsolute->y + (int)(m_index * Global::BlockHeight) + visualOffset.y;

	m_relitivePosition->x = m_stackRelitive->x + visualOffset.x;
	m_relitivePosition->y = m_stackRelitive->y + (int)(m_index * Global::BlockHeight) + visualOffset.y;

	if (updateArgs)
	{
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
			m_background.setSize(sf::Vector2f(m_width, Global::BlockHeight));

			for (unsigned int i = 0; i < m_args.size(); i++)
			{
				m_args[i]->Update(true);
			}
		}
		else
		{
			m_background.setSize(sf::Vector2f(m_width, Global::BlockHeight));

			for (unsigned int i = 0; i < m_args.size(); i++)
			{
				m_args[i]->Update();
			}
		}
	}

	for (uint32_t i = 0; i < m_args.size(); i++)
	{
		if (m_args[i]->GetNext())
		{
			bool found = false;
			for (uint32_t a = i + 1; a < m_args.size(); a++)
			{
				if (m_args[a]->HasData())
				{
					found = true;
					m_args[a]->Select();

					break;
				}
			}

			if (!found)
				m_next = true;

			break;
		}
	}
}

void Block::SetArgData(const std::vector<BlockArgumentCaller>& data)
{
	unsigned int dataIndex = 0;

	for (unsigned int i = 0; i < m_args.size(); i++)
	{
		if (dataIndex == data.size())
			break;

		if (m_args[i]->HasData())
		{
			m_args[i]->SetMode(data[dataIndex].Mode);
			m_args[i]->SetData(data[dataIndex].Value);

			dataIndex++;
		}
	}
}

void Block::SetArgData(const std::vector<std::string> data)
{
	unsigned int dataIndex = 0;

	for (unsigned int i = 0; i < m_args.size(); i++)
	{
		if (dataIndex == data.size())
			break;

		if (m_args[i]->HasData())
		{
			m_args[i]->SetMode(data[dataIndex].front() == '0' ? BlockArgumentVariableMode::RAW : BlockArgumentVariableMode::VAR);
			m_args[i]->SetData(data[dataIndex].substr(1, data[dataIndex].length() - 1));
			dataIndex++;
		}
	}
}

void Block::RenderToImage(sf::RenderTexture* img, uint64_t idx)
{
	m_preShape.setPosition(sf::Vector2f(m_visualOffset.x, m_visualOffset.y + (idx * Global::BlockHeight)));
	img->draw(m_preShape);
}

void Block::SetupInStack(unsigned int index, sf::Vector2i* stackAbsolute, sf::Vector2i* stackRelitive, std::function<void(unsigned int index, sf::Vector2i mousePosition)>* functionSplit, std::function<void(unsigned int index, sf::Vector2i mousePosition)>* functionContext)
{
	m_stackAbsolute = stackAbsolute;
	m_stackRelitive = stackRelitive;
	m_index = index;
	m_functionSplit = functionSplit;
	m_functionContext = functionContext;

	FrameUpdate(true, sf::Vector2f(0, 0), false);
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
	std::vector<BlockArgumentCaller>* dataArgs = new std::vector<BlockArgumentCaller>();

	for (unsigned int i = 0; i < m_args.size(); i++)
	{
		if (m_args[i]->HasData())
			dataArgs->push_back(BlockArgumentCaller(m_args[i]->GetData().substr(0, 1) == "0" ? BlockArgumentVariableMode::RAW : BlockArgumentVariableMode::VAR, m_args[i]->GetData().substr(1, m_args[i]->GetData().length() - 1)));
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

	bool bounding = IsBounding((sf::Vector2f)position);

	if (bounding && !Global::Dragging && !m_wasDown)
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
				Global::SelectedArgument = nullptr;
				Global::SelectedBlock = nullptr;
				Global::SelectedStack = nullptr;

				Global::ContextActive = false;
				m_selected = false;
				(*m_functionSplit)(m_index, position);
			}
		}
	}

	return bounding;
}

bool Block::GetNext()
{
	if (m_next)
	{
		m_next = false;
		return true;
	}
	else
		return false;
}

void Block::SelectFirstArgument()
{
	bool found = false;
	for (uint32_t a = 0; a < m_args.size(); a++)
	{
		if (m_args[a]->HasData())
		{
			found = true;
			m_args[a]->Select();

			break;
		}
	}

	if (!found)
		m_next = true;
}

void Block::PreRender()
{
	if (m_width != m_preTexture.getSize().x)
		m_preTexture.create(m_width, Global::BlockHeight);

	m_preTexture.clear();
	m_preTexture.draw(m_background);

	for (unsigned int i = 0; i < m_args.size(); i++)
		m_args[i]->Render(&m_preTexture);

	m_preShape.setTexture(&m_preTexture.getTexture());
	m_preShape.setSize((sf::Vector2f)m_preTexture.getSize());
	m_preShape.setTextureRect(sf::IntRect(0, m_preTexture.getSize().y, m_preTexture.getSize().x, -1 * m_preTexture.getSize().y));
}

bool Block::IsBounding(const sf::Vector2f& mousePos)
{
	return (int64_t)mousePos.x > (int64_t)m_absolutePosition->x && (int64_t)mousePos.x < (int64_t)m_absolutePosition->x + (int64_t)m_width && (int64_t)mousePos.y > (int64_t)m_absolutePosition->y && (int64_t)mousePos.y < (int64_t)m_absolutePosition->y + (int64_t)Global::BlockHeight;
}

void Block::UpdateInner()
{

}
