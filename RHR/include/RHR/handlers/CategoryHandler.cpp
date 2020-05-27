#include "CategoryHandler.h"
#include "Global.h"
#include "RHR/registries/ButtonRegistry.h"
#include "ProjectHandler.h"
#include "MessageHandler.h"
#include "runtime/PreProcessor.h"

#include <SFML/Graphics.hpp>

#ifndef LINUX
#include <windows.h>
#include <shellapi.h>
#endif

void CategoryHandler::Initialize(BlockRegistry* blockRegistry, Plane* toolbarPlane)
{
	m_running = false;
	m_fullBreak = false;
	m_needsUpdate = false;

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
	m_backgroundOptions.setSize(sf::Vector2f(window->getSize().x, HEADER_HEIGHT));
	toolbarPlane->SetSize(sf::Vector2u(Global::ToolbarWidth, window->getSize().y - (toolbarPlane->GetPosition().y + 5)));

	window->draw(m_backgroundOptions);
	window->draw(m_background);
}

void CategoryHandler::RenderPost(sf::RenderWindow* window)
{
	for (uint16_t i = 0; i < m_modIco.size(); i++)
		window->draw(m_modIco[i]);
}

void CategoryHandler::FrameUpdate(sf::RenderWindow* window, BlockRegistry* blockRegistry, Plane* toolbarPlane, Plane* primaryPlane)
{
	if (toolbarPlane->GetStackCount() != m_toolbarStackCount)
		UpdateBlocks(blockRegistry, toolbarPlane, m_selectedCategory);

	if (PreProcessor::IsFinished() && m_running)
	{
		// finish
		
		m_running = false;

		for (uint16_t i = 0; i < m_runtimeButtons.size(); i++)
			ButtonRegistry::RemoveButton(m_runtimeButtons[i]);

		for (uint16_t i = 0; i < m_editorButtons.size(); i++)
			ButtonRegistry::AddButton(m_editorButtons[i]);
	}
	else if (!PreProcessor::IsFinished() && !m_running)
	{
		// start

		m_running = true;

		for (uint16_t i = 0; i < m_runtimeButtons.size(); i++)
			ButtonRegistry::AddButton(m_runtimeButtons[i]);

		for (uint16_t i = 0; i < m_editorButtons.size(); i++)
			ButtonRegistry::RemoveButton(m_editorButtons[i]);
	}

	if (m_needsUpdate)
	{
		m_needsUpdate = false;

		primaryPlane->SetPosition(sf::Vector2u(Global::ToolbarWidth + 10, HEADER_HEIGHT + 5));
		primaryPlane->SetSize(sf::Vector2u(window->getSize().x - primaryPlane->GetPosition().x - 5, window->getSize().y - primaryPlane->GetPosition().y - 5));
	}
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
	UpdateButtons();

	m_needsUpdate = true;
}

void CategoryHandler::RegisterHeader(BlockRegistry* blockRegistry, Plane* primaryPlane)
{
	// ================================================================================================================================================
	// ================================================================================================================================================
	// Primary Tools
	// ================================================================================================================================================
	// ================================================================================================================================================


	{
		std::function<void()>* function = new std::function<void()>();
		*function = [primaryPlane]()
		{
			std::function<void(const bool&)> cb = [&](const bool& result)
			{
				if (result)
				{
					primaryPlane->DeleteAllBlocks();
					ProjectHandler::CurrentPath.clear();
				}
			};

			MessageHandler::RegisterMessage(new MessageConfirm("unsaved progress will be lost", &cb), true);
		};

		Button* button = new Button(sf::Vector2i(70 * 0, 0), sf::Vector2u(70, 16), function);
		button->SetButtonModeText("new", MOD_BUTTON_TEXT_BG, MOD_BUTTON_TEXT_FG, 12);

		ButtonRegistry::AddButton(button);
	}
	{
		std::function<void()>* function = new std::function<void()>();
		*function = [primaryPlane, blockRegistry]()
		{
			std::function<void(const std::string&)> cb = [&](const std::string& result)
			{
				if (result == "")
					return;

				std::string rc = result;

				rc += ".ub";

				Logger::Info("loading project \"" + rc + "\"");
				ProjectHandler::LoadProject(rc, primaryPlane, blockRegistry);
				ProjectHandler::CurrentPath = rc;
			};

			MessageHandler::RegisterMessage(new MessageInput("load path", &cb), true);
		};

		Button* button = new Button(sf::Vector2i(70 * 1, 0), sf::Vector2u(70, 16), function);
		button->SetButtonModeText("open", MOD_BUTTON_TEXT_BG, MOD_BUTTON_TEXT_FG, 12);

		ButtonRegistry::AddButton(button);
	}
	{
		std::function<void()>* function = new std::function<void()>();
		*function = [primaryPlane]()
		{
			if (ProjectHandler::CurrentPath == "")
			{
				std::function<void(const std::string&)> cb = [&](const std::string& result)
				{
					if (result == "")
						return;

					std::string rc = result;

					rc += ".ub";

					Logger::Info("saving project as \"" + rc + "\"");
					ProjectHandler::SaveProject(rc, primaryPlane);
					ProjectHandler::CurrentPath = rc;
				};

				MessageHandler::RegisterMessage(new MessageInput("save path", &cb), true);
			}
			else
			{
				ProjectHandler::SaveProject(ProjectHandler::CurrentPath, primaryPlane);
				Logger::Info("saving project \"" + ProjectHandler::CurrentPath + "\"");
			}
		};

		Button* button = new Button(sf::Vector2i(70 * 2, 0), sf::Vector2u(70, 16), function);
		button->SetButtonModeText("save", MOD_BUTTON_TEXT_BG, MOD_BUTTON_TEXT_FG, 12);

		ButtonRegistry::AddButton(button);
	}
	{
		std::function<void()>* function = new std::function<void()>();
		*function = [primaryPlane]()
		{
			std::function<void(const std::string&)> cb = [&](const std::string& result)
			{
				if (result == "")
					return;

				std::string rc = result;

				rc += ".ub";

				Logger::Info("saving project as \"" + rc + "\"");
				ProjectHandler::SaveProject(rc, primaryPlane);
				ProjectHandler::CurrentPath = rc;
			};

			MessageHandler::RegisterMessage(new MessageInput("save path", &cb), true);
		};

		Button* button = new Button(sf::Vector2i(70 * 3, 0), sf::Vector2u(70, 16), function);
		button->SetButtonModeText("saveas", MOD_BUTTON_TEXT_BG, MOD_BUTTON_TEXT_FG, 12);

		ButtonRegistry::AddButton(button);
	}
	{
		std::function<void()>* function = new std::function<void()>();
		*function = [primaryPlane]()
		{
#ifndef LINUX
			ShellExecute(0, 0, "https://github.com/applesthepi/unnamedblocks/issues", 0, 0, SW_SHOW);
#else
			system("xdg-open https://github.com/applesthepi/unnamedblocks/issues");
#endif
		};

		Button* button = new Button(sf::Vector2i(70 * 4, 0), sf::Vector2u(70, 16), function);
		button->SetButtonModeText("issue", MOD_BUTTON_TEXT_BG, MOD_BUTTON_TEXT_FG, 12);

		ButtonRegistry::AddButton(button);
	}


	// ================================================================================================================================================
	// ================================================================================================================================================
	// Debugging Tools
	// ================================================================================================================================================
	// ================================================================================================================================================


	{
		std::function<void()>* function = new std::function<void()>();
		*function = [primaryPlane, blockRegistry]()
		{
			if (ProjectHandler::CurrentPath == "")
				Logger::Warn("running unsaved project");
			else
				Logger::Debug("running \"" + ProjectHandler::CurrentPath + "\"");

			Plane* planeCopy = new Plane(sf::Vector2u(0, 0), sf::Vector2u(0, 0));
			planeCopy->CopyEverything(primaryPlane, blockRegistry);

			PreProcessor::Cleanup();
			PreProcessor::Start(planeCopy, blockRegistry, false);
		};

		Button* button = new Button(sf::Vector2i((16 + 5) * 0 + 5, 16 + 6), sf::Vector2u(16, 16), function);
		button->SetButtonModeImage("res/deb_run_release.png");

		ButtonRegistry::AddButton(button);
		m_editorButtons.push_back(button);
	}
	{
		std::function<void()>* function = new std::function<void()>();
		*function = [primaryPlane, blockRegistry]()
		{
			if (ProjectHandler::CurrentPath == "")
				Logger::Warn("running unsaved project");
			else
				Logger::Debug("running \"" + ProjectHandler::CurrentPath + "\"");

			Plane* planeCopy = new Plane(sf::Vector2u(0, 0), sf::Vector2u(0, 0));
			planeCopy->CopyEverything(primaryPlane, blockRegistry);

			PreProcessor::Cleanup();
			PreProcessor::Start(planeCopy, blockRegistry, true);
		};

		Button* button = new Button(sf::Vector2i((16 + 5) * 1 + 5, 16 + 6), sf::Vector2u(16, 16), function);
		button->SetButtonModeImage("res/deb_run_debug.png");

		ButtonRegistry::AddButton(button);
		m_editorButtons.push_back(button);
	}
	{
		std::function<void()>* function = new std::function<void()>();
		*function = [primaryPlane, blockRegistry]()
		{
			Logger::Debug("stopping all");
			PreProcessor::SetSuper(1, 0);
		};

		Button* button = new Button(sf::Vector2i((16 + 5) * 0 + 5, 16 + 6), sf::Vector2u(16, 16), function);
		button->SetButtonModeImage("res/deb_stop.png");

		m_runtimeButtons.push_back(button);
	}
	{
		std::function<void()>* function = new std::function<void()>();
		*function = [primaryPlane, blockRegistry]()
		{
			if (!m_fullBreak)
			{
				m_fullBreak = true;

				Logger::Debug("breaking all");
				PreProcessor::SetSuper(2, 0);

				m_runtimeButtons[1]->SetImage("res/deb_resume.png");
			}
			else
			{
				m_fullBreak = false;

				Logger::Debug("resuming all");
				PreProcessor::SetSuper(3, 0);

				m_runtimeButtons[1]->SetImage("res/deb_pause.png");
			}
		};

		Button* button = new Button(sf::Vector2i((16 + 5) * 1 + 5, 16 + 6), sf::Vector2u(16, 16), function);
		button->SetButtonModeImage("res/deb_pause.png");

		m_runtimeButtons.push_back(button);
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

bool CategoryHandler::m_running;

bool CategoryHandler::m_fullBreak;

bool CategoryHandler::m_needsUpdate;

std::vector<Button*> CategoryHandler::m_editorButtons;

std::vector<Button*> CategoryHandler::m_runtimeButtons;
