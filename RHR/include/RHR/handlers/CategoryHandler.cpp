#include "CategoryHandler.h"
#include "Global.h"
#include "RHR/registries/ButtonRegistry.h"
#include "ProjectHandler.h"
#include "MessageHandler.h"
#include "runtime/PreProcessor.h"

void CategoryHandler::Initialize(BlockRegistry* blockRegistry, Plane* toolbarPlane)
{
	m_selectedCategory = 0;
	m_toolbarStackCount = 0;

	m_textureOpen.loadFromFile("res/dir_open.png");
	m_textureClose.loadFromFile("res/dir_close.png");

	const std::vector<ModCatagory*>& categories = blockRegistry->GetCategories();

	if (categories.size() == 0)
	{
		Logger::Warn("no categories loaded!");
		return;
	}

	std::string lastCategoryMod;
	uint32_t offset = 16 + 10;

	for (uint64_t i = 0; i < categories.size(); i++)
	{
		if (categories[i]->GetModUnlocalizedName() != lastCategoryMod)
		{
			lastCategoryMod = categories[i]->GetModUnlocalizedName();

			const uint16_t modIdx = m_modCategoryButtons.size();

			m_modCategoryCallbacks.push_back(new std::function<void()>());
			*m_modCategoryCallbacks.back() = [modIdx, blockRegistry, toolbarPlane]()
			{
				ToggleMod(modIdx, blockRegistry, toolbarPlane, m_selectedCategory);
			};

			m_modCategoryButtons.push_back(new Button(sf::Vector2i(5, offset), sf::Vector2u(Global::ToolbarWidth - 25, 16), m_modCategoryCallbacks.back()));
			m_modCategoryButtons.back()->SetButtonModeText("   " + categories[i]->GetModDisplayName(), MOD_BUTTON_TEXT_BG, MOD_BUTTON_TEXT_FG, 12);
			ButtonRegistry::AddButton(m_modCategoryButtons.back());
			offset += 16 + 5;

			m_modIco.push_back(sf::Sprite(m_textureClose));

			m_modOpen.push_back(false);

			m_buttons.push_back(std::vector<Button*>());
			m_buttonCallbacks.push_back(std::vector<std::function<void()>*>());
		}
		
		const uint16_t catIdx = i;

		m_buttonCallbacks.back().push_back(new std::function<void()>());
		*m_buttonCallbacks.back().back() = [blockRegistry, toolbarPlane, catIdx]()
		{
			UpdateBlocks(blockRegistry, toolbarPlane, catIdx);
		};

		m_buttons.back().push_back(new Button(sf::Vector2i(20, offset), sf::Vector2u(Global::ToolbarWidth - 25, 16), m_buttonCallbacks.back().back()));
		m_buttons.back().back()->SetButtonModeText(categories[i]->GetDisplayName(), categories[i]->GetColor(), sf::Color::Black, 12);
		ButtonRegistry::AddButton(m_buttons.back().back());
	}

	m_modOpen[0] = true;
	UpdateBlocks(blockRegistry, toolbarPlane, 0);
	const uint32_t height = UpdateButtons();

	m_background.setFillColor(MOD_BUTTON_TEXT_BG);

	m_backgroundOptions.setFillColor(MOD_BUTTON_TEXT_BG);
	m_backgroundOptions.setPosition(sf::Vector2f(0, 0));
}

void CategoryHandler::Render(sf::RenderWindow* window, Plane* toolbarPlane)
{
	m_backgroundOptions.setSize(sf::Vector2f(window->getSize().x, 16));
	toolbarPlane->SetSize(sf::Vector2u(Global::ToolbarWidth, window->getSize().y - (toolbarPlane->GetPosition().y + 5)));

	window->draw(m_backgroundOptions);
	window->draw(m_background);
}

void CategoryHandler::RenderPost(sf::RenderWindow* window)
{
	for (uint16_t i = 0; i < m_modIco.size(); i++)
		window->draw(m_modIco[i]);
}

void CategoryHandler::FrameUpdate(BlockRegistry* blockRegistry, Plane* toolbarPlane)
{
	if (toolbarPlane->GetStackCount() != m_toolbarStackCount)
		UpdateBlocks(blockRegistry, toolbarPlane, m_selectedCategory);
}

void CategoryHandler::ToggleMod(const uint16_t& modIdx, BlockRegistry* blockRegistry, Plane* toolbarPlane, const uint64_t& catIdx)
{
	m_modOpen[modIdx] = !m_modOpen[modIdx];
	UpdateBlocks(blockRegistry, toolbarPlane, catIdx);
}

const uint32_t CategoryHandler::UpdateButtons()
{
	uint64_t offset = HEADER_HEIGHT + 10;

	for (uint64_t i = 0; i < m_modCategoryButtons.size(); i++)
	{
		m_modCategoryButtons[i]->SetPosition(sf::Vector2i(10, offset));
		m_modCategoryButtons[i]->SetSize(sf::Vector2u(Global::ToolbarWidth - 10, 16));

		m_modIco[i].setPosition(sf::Vector2f(10, offset));

		if (m_modOpen[i])
			m_modIco[i].setTexture(m_textureOpen);
		else
			m_modIco[i].setTexture(m_textureClose);

		offset += 16 + 5;

		if (m_modOpen[i])
		{
			for (uint64_t a = 0; a < m_buttons[i].size(); a++)
			{
				m_buttons[i][a]->SetEnabled(true);
				m_buttons[i][a]->SetPosition(sf::Vector2i(10 + 20, offset));
				m_buttons[i][a]->SetSize(sf::Vector2u(Global::ToolbarWidth - (10 + 20), 16));
				offset += 16 + 5;
			}
		}
		else
		{
			for (uint64_t a = 0; a < m_buttons[i].size(); a++)
				m_buttons[i][a]->SetEnabled(false);
		}
	}

	m_background.setPosition(sf::Vector2f(5, HEADER_HEIGHT + 5));
	m_background.setSize(sf::Vector2f(Global::ToolbarWidth, offset - (16 + 5)));

	return offset;
}

void CategoryHandler::UpdateBlocks(BlockRegistry* blockRegistry, Plane* toolbarPlane, const uint64_t& catIdx)
{
	const uint32_t offset = UpdateButtons();
	toolbarPlane->DeleteAllBlocks();

	uint32_t idx = 0;
	uint32_t widest = 0;
	for (unsigned int a = 0; a < blockRegistry->GetBlocks().size(); a++)
	{
		if (blockRegistry->GetBlocks()[a]->GetCategory() == blockRegistry->GetCategories()[catIdx]->GetUnlocalizedName())
		{
			Stack* stack = new Stack(sf::Vector2i(5, static_cast<int32_t>(5 + (idx * (Global::BlockHeight + 5)))), blockRegistry);
			Block* block = new Block(blockRegistry->GetBlocks()[a]->GetUnlocalizedName(), blockRegistry, stack->GetFunctionUpdate(), stack->GetFunctionSelect());

			toolbarPlane->AddStack(stack);
			stack->AddBlock(block);

			stack->ReRender();
			stack->ReloadVanity();

			if (block->GetWidth() > widest)
				widest = block->GetWidth();

			idx++;
		}
	}
	
	m_toolbarStackCount = toolbarPlane->GetStackCount();
	m_selectedCategory = catIdx;

	if (widest < 250)
		Global::ToolbarWidth = 250;
	else
		Global::ToolbarWidth = widest + 10;

	toolbarPlane->SetPosition(sf::Vector2u(5, offset + 5));
}

void CategoryHandler::RegisterHeader(BlockRegistry* blockRegistry, Plane* primaryPlane)
{
	{
		std::function<void()>* function = new std::function<void()>();
		*function = [primaryPlane]()
		{
			bool* result = new bool(false);

			MessageHandler::RegisterMessageSync(new Message("all unsaved progress will be lost if you continue", result));

			if (*result)
			{
				primaryPlane->DeleteAllBlocks();
			}
		};

		Button* button = new Button(sf::Vector2i(105 * 0, 0), sf::Vector2u(100, 16), function);
		button->SetButtonModeText("new", MOD_BUTTON_TEXT_BG, MOD_BUTTON_TEXT_FG, 12);

		ButtonRegistry::AddButton(button);
	}
	{
		std::function<void()>* function = new std::function<void()>();
		*function = [primaryPlane, &blockRegistry]()
		{
			std::string* result = new std::string();

			MessageHandler::RegisterMessageSync(new Message("load file path:", result));
			if (*result == "")
			{
				delete result;
				return;
			}
			*result += ".ub";

			Logger::Info("loading project from \"" + *result + "\"");
			ProjectHandler::LoadProject(*result, primaryPlane, blockRegistry);
			ProjectHandler::CurrentPath = *result;

			delete result;
		};

		Button* button = new Button(sf::Vector2i(105 * 1, 0), sf::Vector2u(100, 16), function);
		button->SetButtonModeText("open", MOD_BUTTON_TEXT_BG, MOD_BUTTON_TEXT_FG, 12);

		ButtonRegistry::AddButton(button);
	}
	{
		std::function<void()>* function = new std::function<void()>();
		*function = [primaryPlane]()
		{
			if (ProjectHandler::CurrentPath == "")
			{
				std::string* result = new std::string();

				MessageHandler::RegisterMessageSync(new Message("save file path:", result));
				if (*result == "")
				{
					delete result;
					return;
				}
				*result += ".ub";

				Logger::Info("saveas to \"" + *result + "\"");
				ProjectHandler::SaveProject(*result, primaryPlane);
				ProjectHandler::CurrentPath = *result;

				delete result;
			}
			else
			{
				ProjectHandler::SaveProject(ProjectHandler::CurrentPath, primaryPlane);
				Logger::Info("save to \"" + ProjectHandler::CurrentPath + "\"");
			}
		};

		Button* button = new Button(sf::Vector2i(105 * 2, 0), sf::Vector2u(100, 16), function);
		button->SetButtonModeText("save", MOD_BUTTON_TEXT_BG, MOD_BUTTON_TEXT_FG, 12);

		ButtonRegistry::AddButton(button);
	}
	{
		std::function<void()>* function = new std::function<void()>();
		*function = [primaryPlane]()
		{
			std::string* result = new std::string();

			MessageHandler::RegisterMessageSync(new Message("save file path:", result));
			if (*result == "")
			{
				delete result;
				return;
			}
			*result += ".ub";

			Logger::Info("saveas to \"" + *result + "\"");
			ProjectHandler::SaveProject(*result, primaryPlane);
			ProjectHandler::CurrentPath = *result;

			delete result;
		};

		Button* button = new Button(sf::Vector2i(105 * 3, 0), sf::Vector2u(100, 16), function);
		button->SetButtonModeText("saveas", MOD_BUTTON_TEXT_BG, MOD_BUTTON_TEXT_FG, 12);

		ButtonRegistry::AddButton(button);
	}
	{
		std::function<void()>* function = new std::function<void()>();
		*function = [primaryPlane, blockRegistry]()
		{
			if (ProjectHandler::CurrentPath == "")
				Logger::Info("running unsaved project");
			else
				Logger::Info("running \"" + ProjectHandler::CurrentPath + "\"");

			Plane* planeCopy = new Plane(sf::Vector2u(0, 0), sf::Vector2u(0, 0));
			planeCopy->CopyEverything(primaryPlane, blockRegistry);

			PreProcessor::Cleanup();
			PreProcessor::Start(planeCopy, blockRegistry, true);
		};

		Button* button = new Button(sf::Vector2i(105 * 4, 0), sf::Vector2u(100, 16), function);
		button->SetButtonModeText("debug", MOD_BUTTON_TEXT_BG, MOD_BUTTON_TEXT_FG, 12);

		ButtonRegistry::AddButton(button);
	}
	{
		std::function<void()>* function = new std::function<void()>();
		*function = [primaryPlane, blockRegistry]()
		{
			if (ProjectHandler::CurrentPath == "")
				Logger::Info("running unsaved project");
			else
				Logger::Info("running \"" + ProjectHandler::CurrentPath + "\"");

			Plane* planeCopy = new Plane(sf::Vector2u(0, 0), sf::Vector2u(0, 0));
			planeCopy->CopyEverything(primaryPlane, blockRegistry);

			PreProcessor::Cleanup();
			PreProcessor::Start(planeCopy, blockRegistry, false);
		};

		Button* button = new Button(sf::Vector2i(105 * 5, 0), sf::Vector2u(100, 16), function);
		button->SetButtonModeText("release", MOD_BUTTON_TEXT_BG, MOD_BUTTON_TEXT_FG, 12);

		ButtonRegistry::AddButton(button);
	}
}

std::vector<Button*> CategoryHandler::m_modCategoryButtons;

std::vector<std::vector<Button*>> CategoryHandler::m_buttons;

std::vector<std::function<void()>*> CategoryHandler::m_modCategoryCallbacks;

std::vector<std::vector<std::function<void()>*>> CategoryHandler::m_buttonCallbacks;

std::vector<bool> CategoryHandler::m_modOpen;

std::vector<sf::Sprite> CategoryHandler::m_modIco;

sf::Texture CategoryHandler::m_textureOpen;

sf::Texture CategoryHandler::m_textureClose;

sf::RectangleShape CategoryHandler::m_background;

sf::RectangleShape CategoryHandler::m_backgroundOptions;

uint64_t CategoryHandler::m_toolbarStackCount;

uint64_t CategoryHandler::m_selectedCategory;

