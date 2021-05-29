#include "CategoryHandler.h"
#include "Global.h"
#include "RHR/registries/UIRegistry.h"
#include "ProjectHandler.h"
#include "MessageHandler.h"
#include "runtime/PreProcessor.h"
#include "RHR/stacking/Plane.h"

#include <SFML/Graphics.hpp>

#if !LINUX
#include <windows.h>
#include <shellapi.h>
#endif

CategoryHandler::CategoryHandler()
{
	m_running = false;
	m_fullBreak = false;
	m_needsUpdate = false;

	m_selectedCategory = 0;
	m_toolbarStackCount = 0;

	m_textureOpen.loadFromFile("res/dir_open.png");
	m_textureClose.loadFromFile("res/dir_close.png");

	const std::vector<ModCatagory*>& categories = BlockRegistry::GetRegistry().GetCategories();

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
			*m_modCategoryCallbacks.back() = [this, modIdx]()
			{
				ToggleMod(modIdx, m_selectedCategory);
			};

			m_modCategoryButtons.push_back(new ButtonText(m_modCategoryCallbacks.back(), "   " + categories[i]->GetModDisplayName(), 12, sf::Vector2f(m_toolbarWidth - 25, 16), MOD_BUTTON_TEXT_BG, MOD_BUTTON_TEXT_FG));
			m_modCategoryButtons.back()->setPosition(sf::Vector2f(5, offset));
			
			UIRegistry::GetRegistry().AddComponent(m_modCategoryButtons.back());

			offset += 16 + 5;

			m_modIco.push_back(sf::Sprite(m_textureClose));
			m_modOpen.push_back(false);

			m_buttons.push_back(std::vector<ButtonText*>());
			m_buttonCallbacks.push_back(std::vector<std::function<void()>*>());
		}
		
		const uint16_t catIdx = i;

		m_buttonCallbacks.back().push_back(new std::function<void()>());
		*m_buttonCallbacks.back().back() = [this, catIdx]()
		{
			UpdateBlocks(catIdx);
		};

		m_buttons.back().push_back(new ButtonText(m_buttonCallbacks.back().back(), categories[i]->GetDisplayName(), 12, sf::Vector2f(m_toolbarWidth - 25, 16), categories[i]->GetColor(), sf::Color::Black));
		m_buttons.back().back()->setPosition(20, offset);
		UIRegistry::GetRegistry().AddComponent(m_buttons.back().back());

		Collection* collection = new Collection();
		collection->SetEnabled(false);

		uint64_t idx = 0;

		for (unsigned int a = 0; a < BlockRegistry::GetRegistry().GetBlocks().size(); a++)
		{
			if (BlockRegistry::GetRegistry().GetBlocks()[a]->GetCategory() == BlockRegistry::GetRegistry().GetCategories()[i]->GetUnlocalizedName())
			{
				Stack* stack = new Stack();
				Block* block = new Block(BlockRegistry::GetRegistry().GetBlocks()[a]->GetUnlocalizedName());

				stack->setPosition(5, static_cast<int32_t>(5 + (idx * (Global::BlockHeight + 5))));

				stack->AddBlock(block);
				collection->AddStack(stack);

				idx++;
			}
		}

		Plane::ToolbarPlane->AddCollection(collection, false);
	}

	m_modOpen[0] = true;
	UpdateBlocks(0);
	const uint32_t height = UpdateButtons();

	m_background.setFillColor(MOD_BUTTON_TEXT_BG);

	m_backgroundOptions.setFillColor(MOD_BUTTON_TEXT_BG);
	m_backgroundOptions.setPosition(sf::Vector2f(0, 0));
}

void CategoryHandler::ToggleMod(uint16_t modIdx, uint64_t catIdx)
{
	m_modOpen[modIdx] = !m_modOpen[modIdx];
	UpdateBlocks(catIdx);
}

void CategoryHandler::UpdateBlocks(uint64_t catIdx)
{
	const uint32_t offset = UpdateButtons();
	
	uint32_t widest = 0;

	if (Plane::ToolbarPlane->GetCollections()[catIdx]->GetStacks().size() != m_toolbarStackCount)
	{
		uint64_t idx = 0;
		Plane::ToolbarPlane->GetCollections()[catIdx]->RemoveAll(true);

		for (unsigned int a = 0; a < BlockRegistry::GetRegistry().GetBlocks().size(); a++)
		{
			if (BlockRegistry::GetRegistry().GetBlocks()[a]->GetCategory() == BlockRegistry::GetRegistry().GetCategories()[catIdx]->GetUnlocalizedName())
			{
				Stack* stack = new Stack();
				Block* block = new Block(BlockRegistry::GetRegistry().GetBlocks()[a]->GetUnlocalizedName());

				stack->setPosition(5, static_cast<int32_t>(5 + (idx * (Global::BlockHeight + 5))));

				stack->AddBlock(block);
				Plane::ToolbarPlane->GetCollections()[catIdx]->AddStack(stack);

				idx++;
			}
		}
	}

	for (uint64_t i = 0; i < Plane::ToolbarPlane->GetCollections()[catIdx]->GetStacks().size(); i++)
	{
		if (Plane::ToolbarPlane->GetCollections()[catIdx]->GetStacks()[i]->GetBlocks().front()->GetWidth() > widest)
			widest = Plane::ToolbarPlane->GetCollections()[catIdx]->GetStacks()[i]->GetBlocks().front()->GetWidth();
	}

	Plane::ToolbarPlane->GetCollections()[m_selectedCategory]->SetEnabled(false);
	Plane::ToolbarPlane->GetCollections()[catIdx]->SetEnabled(true);

	m_selectedCategory = catIdx;

	if (widest < 250)
		m_toolbarWidth = 250;
	else
		m_toolbarWidth = widest + 10;

	Plane::ToolbarPlane->setPosition(sf::Vector2f(5, offset + 5));
	Plane::PrimaryPlane->setPosition(sf::Vector2f(m_toolbarWidth + 10, HEADER_HEIGHT + 5));
	Plane::ToolbarPlane->GetCollections()[catIdx]->setSize(sf::Vector2u(m_toolbarWidth, Plane::ToolbarPlane->GetCollections()[catIdx]->GetStacks().size() * (Global::BlockHeight + 5) - 5));

	Plane::ToolbarPlane->UpdateVAO(catIdx);

	m_toolbarStackCount = Plane::ToolbarPlane->GetCollections()[catIdx]->GetStacks().size();

	UpdateButtons();

	m_needsUpdate = true;
}

void CategoryHandler::RegisterHeader()
{
	// ================================================================================================================================================
	// ================================================================================================================================================
	// Primary Tools
	// ================================================================================================================================================
	// ================================================================================================================================================


	{
		std::function<void()>* function = new std::function<void()>();
		*function = []()
		{
			std::function<void(bool)> cb = [&](bool result)
			{
				if (result)
				{
					Plane::PrimaryPlane->DeleteContents();
					ProjectHandler::CurrentPath.clear();
				}
			};

			MessageHandler::RegisterMessage(new MessageConfirm("unsaved progress will be lost", &cb), true);
		};

		ButtonText* button = new ButtonText(function, "new", 12, sf::Vector2f(70, 16), MOD_BUTTON_TEXT_BG, MOD_BUTTON_TEXT_FG);
		button->setPosition(sf::Vector2f(70 * 0, 0));
		UIRegistry::GetRegistry().AddComponent(button);
	}
	{
		std::function<void()>* function = new std::function<void()>();
		*function = []()
		{
			std::function<void(const std::string&)> cb = [&](const std::string& result)
			{
				if (result == "")
					return;

				std::string rc = result;

				rc += ".ub";

				Logger::Info("loading project \"" + rc + "\"");
				ProjectHandler::LoadProject(rc);
				ProjectHandler::CurrentPath = rc;
			};

			MessageHandler::RegisterMessage(new MessageInput("load path", &cb), true);
		};

		ButtonText* button = new ButtonText(function, "open", 12, sf::Vector2f(70, 16), MOD_BUTTON_TEXT_BG, MOD_BUTTON_TEXT_FG);
		button->setPosition(sf::Vector2f(70 * 1, 0));
		UIRegistry::GetRegistry().AddComponent(button);
	}
	{
		std::function<void()>* function = new std::function<void()>();
		*function = []()
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
					ProjectHandler::SaveProject(rc);
					ProjectHandler::CurrentPath = rc;
				};

				MessageHandler::RegisterMessage(new MessageInput("save path", &cb), true);
			}
			else
			{
				ProjectHandler::SaveProject(ProjectHandler::CurrentPath);
				Logger::Info("saving project \"" + ProjectHandler::CurrentPath + "\"");
			}
		};

		ButtonText* button = new ButtonText(function, "save", 12, sf::Vector2f(70, 16), MOD_BUTTON_TEXT_BG, MOD_BUTTON_TEXT_FG);
		button->setPosition(sf::Vector2f(70 * 2, 0));
		UIRegistry::GetRegistry().AddComponent(button);
	}
	{
		std::function<void()>* function = new std::function<void()>();
		*function = []()
		{
			std::function<void(const std::string&)> cb = [&](const std::string& result)
			{
				if (result == "")
					return;

				std::string rc = result;

				rc += ".ub";

				Logger::Info("saving project as \"" + rc + "\"");
				ProjectHandler::SaveProject(rc);
				ProjectHandler::CurrentPath = rc;
			};

			MessageHandler::RegisterMessage(new MessageInput("save path", &cb), true);
		};

		ButtonText* button = new ButtonText(function, "saveas", 12, sf::Vector2f(70, 16), MOD_BUTTON_TEXT_BG, MOD_BUTTON_TEXT_FG);
		button->setPosition(sf::Vector2f(70 * 3, 0));
		UIRegistry::GetRegistry().AddComponent(button);
	}
	{
		std::function<void()>* function = new std::function<void()>();
		*function = []()
		{
#if !LINUX
			ShellExecute(0, 0, "https://github.com/applesthepi/unnamedblocks/issues", 0, 0, SW_SHOW);
#else
			system("xdg-open https://github.com/applesthepi/unnamedblocks/issues");
#endif
		};

		ButtonText* button = new ButtonText(function, "issue", 12, sf::Vector2f(70, 16), MOD_BUTTON_TEXT_BG, MOD_BUTTON_TEXT_FG);
		button->setPosition(sf::Vector2f(70 * 4, 0));
		UIRegistry::GetRegistry().AddComponent(button);
	}


	// ================================================================================================================================================
	// ================================================================================================================================================
	// Debugging Tools
	// ================================================================================================================================================
	// ================================================================================================================================================


	{
		std::function<void()>* function = new std::function<void()>();
		*function = []()
		{
			if (ProjectHandler::CurrentPath == "")
				Logger::Warn("running unsaved project");
			else
				Logger::Debug("running \"" + ProjectHandler::CurrentPath + "\"");

			Plane* planeCopy = new Plane(false, *Plane::PrimaryPlane);

			PreProcessor::Cleanup();
			PreProcessor::Start(planeCopy, false);
		};

		ButtonImage* button = new ButtonImage(function, "res/deb_run_release.png", sf::Vector2f(16, 16));
		button->setPosition(sf::Vector2f((16 + 5) * 0 + 5, 16 + 6));

		UIRegistry::GetRegistry().AddComponent(button);
		m_editorButtons.push_back(button);
	}
	{
		std::function<void()>* function = new std::function<void()>();
		*function = []()
		{
			if (ProjectHandler::CurrentPath == "")
				Logger::Warn("running unsaved project");
			else
				Logger::Debug("running \"" + ProjectHandler::CurrentPath + "\"");

			Plane* planeCopy = new Plane(false, *Plane::PrimaryPlane);

			PreProcessor::Cleanup();
			PreProcessor::Start(planeCopy, true);
		};

		ButtonImage* button = new ButtonImage(function, "res/deb_run_debug.png", sf::Vector2f(16, 16));
		button->setPosition(sf::Vector2f((16 + 5) * 1 + 5, 16 + 6));

		UIRegistry::GetRegistry().AddComponent(button);
		m_editorButtons.push_back(button);
	}
	{
		std::function<void()>* function = new std::function<void()>();
		*function = []()
		{
			Logger::Debug("stopping all");
			PreProcessor::SetSuper(1, 0);
		};

		ButtonImage* button = new ButtonImage(function, "res/deb_stop.png", sf::Vector2f(16, 16));
		button->setPosition(sf::Vector2f((16 + 5) * 0 + 5, 16 + 6));

		m_runtimeButtons.push_back(button);
	}
	{
		std::function<void()>* function = new std::function<void()>();
		*function = [this]()
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

		ButtonImage* button = new ButtonImage(function, "res/deb_pause.png", sf::Vector2f(16, 16));
		button->setPosition(sf::Vector2f((16 + 5) * 1 + 5, 16 + 6));

		m_runtimeButtons.push_back(button);
	}
}

uint32_t CategoryHandler::UpdateButtons()
{
	uint64_t offset = HEADER_HEIGHT + 10;

	for (uint64_t i = 0; i < m_modCategoryButtons.size(); i++)
	{
		m_modCategoryButtons[i]->setPosition(sf::Vector2f(10, offset));
		m_modCategoryButtons[i]->setSize(sf::Vector2u(m_toolbarWidth - 10, 16));

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
				m_buttons[i][a]->setEnabled(true);
				m_buttons[i][a]->setPosition(sf::Vector2f(10 + 20, offset));
				m_buttons[i][a]->setSize(sf::Vector2u(m_toolbarWidth - (10 + 20), 16));
				offset += 16 + 5;
			}
		}
		else
		{
			for (uint64_t a = 0; a < m_buttons[i].size(); a++)
				m_buttons[i][a]->setEnabled(false);
		}
	}

	m_background.setPosition(sf::Vector2f(5, HEADER_HEIGHT + 5));
	m_background.setSize(sf::Vector2f(m_toolbarWidth, offset - (HEADER_HEIGHT + 5)));

	return offset;
}

uint16_t CategoryHandler::GetToolbarWidth()
{
	return m_toolbarWidth;
}

CategoryHandler& CategoryHandler::GetHandler()
{
	return *m_handler;
}

void CategoryHandler::PostRender(sf::RenderWindow* window)
{
	for (uint16_t i = 0; i < m_modIco.size(); i++)
		window->draw(m_modIco[i]);
}

void CategoryHandler::frameUpdate(double deltaTime)
{
	if (Plane::ToolbarPlane->GetCollections().size() > m_selectedCategory && Plane::ToolbarPlane->GetCollections()[m_selectedCategory]->GetStacks().size() != m_toolbarStackCount)
		UpdateBlocks(m_selectedCategory);

	if (PreProcessor::IsFinished() && m_running)
	{
		// finish

		m_running = false;

		for (uint16_t i = 0; i < m_runtimeButtons.size(); i++)
			UIRegistry::GetRegistry().RemoveComponent(m_runtimeButtons[i]);

		for (uint16_t i = 0; i < m_editorButtons.size(); i++)
			UIRegistry::GetRegistry().AddComponent(m_editorButtons[i]);
	}
	else if (!PreProcessor::IsFinished() && !m_running)
	{
		// start

		m_running = true;

		for (uint16_t i = 0; i < m_runtimeButtons.size(); i++)
			UIRegistry::GetRegistry().AddComponent(m_runtimeButtons[i]);

		for (uint16_t i = 0; i < m_editorButtons.size(); i++)
			UIRegistry::GetRegistry().RemoveComponent(m_editorButtons[i]);
	}

	if (m_needsUpdate)
	{
		m_needsUpdate = false;

		Plane::PrimaryPlane->setPosition(sf::Vector2f(m_toolbarWidth + 10, HEADER_HEIGHT + 5));
		Plane::PrimaryPlane->setSize(sf::Vector2u(Global::WindowSize.x - Plane::PrimaryPlane->getPosition().x - 5, Global::WindowSize.y - Plane::PrimaryPlane->getPosition().y - 5));
	}

	m_backgroundOptions.setSize(sf::Vector2f(Global::WindowSize.x, HEADER_HEIGHT));
	Plane::ToolbarPlane->setSize(sf::Vector2u(m_toolbarWidth, Global::WindowSize.y - (Plane::ToolbarPlane->getPosition().y + 5)));
}

void CategoryHandler::CreateHandler()
{
	m_handler = new CategoryHandler();
}

void CategoryHandler::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_backgroundOptions);
	target.draw(m_background);
}

CategoryHandler* CategoryHandler::m_handler;