#include "CategoryHandler.hpp"

#include "registries/UIRegistry.hpp"
#include "handlers/ProjectHandler.hpp"
#include "handlers/MessageHandler.hpp"
#include "handlers/runtime/PreProcessor.hpp"
#include "stacking/Plane.hpp"

#if !LINUX
#include <windows.h>
#include <shellapi.h>
#endif



/*
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
*/

static void ButtonCatagoryCallback(void* data)
{
	CatagoryHandler::ActiveCatagory* activeCatagory = (CatagoryHandler::ActiveCatagory*)data;
	CatagoryHandler::SelectCatagory(*activeCatagory);
}

void CatagoryHandler::Populate(std::shared_ptr<vui::RenderFrame>& renderFrame)
{
	m_RenderFrame = renderFrame;
	m_RenderFrame->SetPadding(4);
	m_RenderFrame->EnableBackground(Color::BackgroundColor2);
	m_RenderFrame->DisableBarMovement();

	const std::vector<CatagoryInfo>& categoryInfos = BlockRegistry::GetRegistry().GetCategories();
	const std::vector<BlockInfo>& blockInfos = BlockRegistry::GetRegistry().GetBlocks();

	//std::vector<std::string> binnedMods;
	std::vector<std::vector<ModCatagory*>> binnedCatagories;
	std::vector<std::vector<std::vector<ModBlock*>>> binnedBlocks;

	bool found = false;

	// TODO: this modularization should be done in BlockRegistry

	for (size_t i = 0; i < ProjectHandler::Mods.size(); i++)
	{
		binnedCatagories.push_back(std::vector<ModCatagory*>());
		binnedBlocks.push_back(std::vector<std::vector<ModBlock*>>());

		for (size_t a = 0; a < categoryInfos.size(); a++)
		{
			if (categoryInfos[a].CatagoryModUnlocalizedName == ProjectHandler::Mods[i])
			{
				binnedCatagories[i].push_back(categoryInfos[a].CatagoryModCatagory);
				binnedBlocks[i].push_back(std::vector<ModBlock*>());

				for (size_t b = 0; b < blockInfos.size(); b++)
				{
					if (blockInfos[b].BlockModUnlocalizedName == ProjectHandler::Mods[i] &&
						blockInfos[b].BlockModBlock->GetCategory() == categoryInfos[a].CatagoryModCatagory->GetUnlocalizedName())
					{
						binnedBlocks[i][a].push_back(blockInfos[b].BlockModBlock);
					}
				}
			}
		}
	}

	//for (auto& info : catagoryInfos)
	//{
	//	for (size_t i = 0; i < binnedMods.size(); i++)
	//	{
	//		if (info.CatagoryModUnlocalizedName == binnedMods[i])
	//		{
	//			binnedCatagories[i].push_back(info.CatagoryModCatagory);
	//			binnedBlocks[i].push_back(std::vector<ModBlock*>());

	//			for (auto& blockInfo : blockInfos)
	//			{
	//				if (blockInfo.BlockModUnlocalizedName == binnedMods[i] &&
	//					blockInfo.BlockModBlock->GetCategory() == info.CatagoryModCatagory->GetUnlocalizedName())
	//					binnedBlocks[i].back().push_back(blockInfo.BlockModBlock);
	//			}

	//			found = true;
	//		}
	//	}

	//	if (!found)
	//	{
	//		binnedMods.push_back(info.CatagoryModUnlocalizedName);
	//		binnedCatagories.push_back(std::vector<ModCatagory*>());
	//		binnedBlocks.push_back(std::vector<std::vector<ModBlock*>>());

	//		binnedCatagories.back().push_back(info.CatagoryModCatagory);

	//		for (auto& blockInfo : blockInfos)
	//		{
	//			if (blockInfo.BlockModUnlocalizedName == binnedMods.back() &&
	//				blockInfo.BlockModBlock->GetCategory() == info.CatagoryModCatagory->GetUnlocalizedName())
	//				binnedBlocks.back().back().push_back(blockInfo.BlockModBlock);
	//		}
	//	}
	//	else
	//		found = false;
	//}

	m_ModGroups.reserve(ProjectHandler::Mods.size());

	int32_t offset = Block::Padding / 2;

	for (size_t i = 0; i < ProjectHandler::Mods.size(); i++)
	{
		offset += Block::Padding / 2;

 		ModGroup group = ModGroup(Color::TextPrimaryColor, Color::BackgroundColor3);
		group.ModCategories.reserve(binnedCatagories[i].size());
		group.ModButton->SetSize({ 200, 16 });
		group.ModButton->EnableFillWidth();
		m_RenderFrame->AddContent(group.ModButton, std::weak_ptr<IUpdatable>(), group.ModButton, group.ModButton, vui::LocalCardinal::DOWN);

		offset += Block::Padding / 2;
 		
		ActiveCatagory* activeCatagories = new ActiveCatagory[binnedCatagories[i].size()];

		for (size_t a = 0; a < binnedCatagories[i].size(); a++)
		{
			activeCatagories[a].ActiveModGroup = static_cast<uint16_t>(i);
			activeCatagories[a].ActiveModGroupCatagory = static_cast<uint16_t>(a);

			std::shared_ptr<vui::RenderButton> button = std::make_shared<vui::RenderButton>(Color::TextPrimaryColor, Color::BackgroundColor3);
			button->SetWeak(button);
			button->SetSize({ 200, 16 });
			button->SetPosition({ 16, 0 });
			button->SetColorSecondary(binnedCatagories[i][a]->GetColor());
			button->EnableFillWidth();
			button->SetCallback(ButtonCatagoryCallback, activeCatagories + a);
			m_RenderFrame->AddContent(button, std::weak_ptr<IUpdatable>(), button, button, vui::LocalCardinal::DOWN);

			offset += Block::Padding / 2 + button->GetSize().y;
			group.ModCategories.push_back(std::move(button));

			group.ModCategoryCollections.push_back(std::vector<std::shared_ptr<Collection>>());
			group.ModCategoryStacks.push_back(std::vector<std::shared_ptr<Stack>>());
			group.ModCategoryBlocks.push_back(std::vector<std::shared_ptr<Block>>());

			for (auto binnedBlock : binnedBlocks[i][a])
			{
				std::shared_ptr<Collection> collection = std::make_shared<Collection>();
				collection->SetWeak(collection);

				std::shared_ptr<Stack> stack = std::make_shared<Stack>();
				stack->SetWeak(stack);

				std::shared_ptr<Block> block = std::make_shared<Block>(binnedBlock->GetUnlocalizedName());
				block->SetWeak(block);
				
				stack->AddBlock(block);
				collection->AddStack(stack);

				group.ModCategoryCollections.back().push_back(collection);
				group.ModCategoryStacks.back().push_back(stack);
				group.ModCategoryBlocks.back().push_back(block);
			}
		}

		m_ModGroups.push_back(std::move(group));
	}

	SelectCatagory(m_ActiveCatagory);
}

void CatagoryHandler::Render()
{
	for (auto& group : m_ModGroups)
	{
		group.ModButton->Render();

		for (auto& catagory : group.ModCategories)
			catagory->Render();
	}
}

void CatagoryHandler::SelectCatagory(ActiveCatagory activeCatagory)
{
	m_ActiveCatagory = activeCatagory;

	if (m_ActiveCatagory.ActiveModGroup >= m_ModGroups.size())
	{
		Logger::Warn("Failed to load catagory. Mod idx out of range (" + std::to_string(m_ActiveCatagory.ActiveModGroup) + " >= " + std::to_string(m_ModGroups.size()) + ")");
		return;
	}

	if (m_ActiveCatagory.ActiveModGroupCatagory >= m_ModGroups[m_ActiveCatagory.ActiveModGroup].ModCategories.size())
	{
		Logger::Warn("Failed to load catagory. Category idx out of range (" + std::to_string(m_ActiveCatagory.ActiveModGroupCatagory) + " >= " + std::to_string(m_ModGroups[m_ActiveCatagory.ActiveModGroup].ModCategories.size()) + ")");
		return;
	}

	Plane::ToolbarPlane->DeleteContents();

	for (auto collection : m_ModGroups[m_ActiveCatagory.ActiveModGroup].ModCategoryCollections[m_ActiveCatagory.ActiveModGroupCatagory])
		Plane::ToolbarPlane->AddCollection(collection, false);
}

std::vector<CatagoryHandler::ModGroup> CatagoryHandler::m_ModGroups;

std::shared_ptr<vui::RenderFrame> CatagoryHandler::m_RenderFrame;

CatagoryHandler::ActiveCatagory CatagoryHandler::m_ActiveCatagory = { 0, 0 };