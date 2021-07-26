#include "category.hpp"

#include "rhr/registries/char_texture.hpp"
#include "rhr/handlers/project.hpp"
#include "rhr/handlers/message.hpp"
#include "rhr/handlers/preprocessor.hpp"
#include "rhr/stacking/plane.hpp"

//#if !LINUX
//#include <windows.h>
//#include <shellapi.h>
//#endif



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

	const std::vector<ModCatagory*>& categories = rhr::registry::block::GetRegistry().GetCategories();

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

			const u16 modIdx = m_modCategoryButtons.size();

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

		const u16 catIdx = i;

		m_buttonCallbacks.back().push_back(new std::function<void()>());
		*m_buttonCallbacks.back().back() = [this, catIdx]()
		{
			UpdateBlocks(catIdx);
		};

		m_buttons.back().push_back(new ButtonText(m_buttonCallbacks.back().back(), categories[i]->GetDisplayName(), 12, sf::Vector2f(m_toolbarWidth - 25, 16), categories[i]->GetColor(), sf::cap::color::Black));
		m_buttons.back().back()->setPosition(20, offset);
		UIRegistry::GetRegistry().AddComponent(m_buttons.back().back());

		rhr::stack::collection* collection = new rhr::stack::collection();
		collection->SetEnabled(false);

		uint64_t idx = 0;

		for (unsigned int a = 0; a < rhr::registry::block::GetRegistry().GetBlocks().size(); a++)
		{
			if (rhr::registry::block::GetRegistry().GetBlocks()[a]->GetCategory() == rhr::registry::block::GetRegistry().GetCategories()[i]->GetUnlocalizedName())
			{
				rhr::stack::stack* stack = new rhr::stack::stack();
				rhr::stack::block* block = new rhr::stack::block(rhr::registry::block::GetRegistry().GetBlocks()[a]->GetUnlocalizedName());

				stack->setPosition(5, static_cast<i32>(5 + (idx * (Global::BlockHeight + 5))));

				stack->add_block(block);
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

void CategoryHandler::ToggleMod(u16 modIdx, uint64_t catIdx)
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

		for (unsigned int a = 0; a < rhr::registry::block::GetRegistry().GetBlocks().size(); a++)
		{
			if (rhr::registry::block::GetRegistry().GetBlocks()[a]->GetCategory() == rhr::registry::block::GetRegistry().GetCategories()[catIdx]->GetUnlocalizedName())
			{
				rhr::stack::stack* stack = new rhr::stack::stack();
				rhr::stack::block* block = new rhr::stack::block(rhr::registry::block::GetRegistry().GetBlocks()[a]->GetUnlocalizedName());

				stack->setPosition(5, static_cast<i32>(5 + (idx * (Global::BlockHeight + 5))));

				stack->add_block(block);
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

u16 CategoryHandler::GetToolbarWidth()
{
	return m_toolbarWidth;
}

CategoryHandler& CategoryHandler::GetHandler()
{
	return *m_handler;
}

void CategoryHandler::PostRender(sf::RenderWindow* window)
{
	for (u16 i = 0; i < m_modIco.size(); i++)
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

		for (u16 i = 0; i < m_runtimeButtons.size(); i++)
			UIRegistry::GetRegistry().RemoveComponent(m_runtimeButtons[i]);

		for (u16 i = 0; i < m_editorButtons.size(); i++)
			UIRegistry::GetRegistry().AddComponent(m_editorButtons[i]);
	}
	else if (!PreProcessor::IsFinished() && !m_running)
	{
		// start

		m_running = true;

		for (u16 i = 0; i < m_runtimeButtons.size(); i++)
			UIRegistry::GetRegistry().AddComponent(m_runtimeButtons[i]);

		for (u16 i = 0; i < m_editorButtons.size(); i++)
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

static void button_callback_category(void* data)
{
	rhr::handler::category::active* active_catagory = (rhr::handler::category::active*)data;
	rhr::handler::category::select_category(*active_catagory);
}

void rhr::handler::category::populate(std::shared_ptr<rhr::render::frame>& render_frame)
{
	m_render_frame = render_frame;
	m_render_frame->set_padding(4);
	m_render_frame->enable_background(cap::color::background_color_2);
	m_render_frame->disable_bar_movement();

	const std::vector<rhr::registry::block::CatagoryInfo>& category_infos = rhr::registry::block::get_registry().get_categories();
	const std::vector<rhr::registry::block::BlockInfo>& block_infos = rhr::registry::block::get_registry().get_blocks();

	//std::vector<std::string> binnedMods;
	std::vector<std::vector<ModCatagory*>> binned_catagories;
	std::vector<std::vector<std::vector<ModBlock*>>> binned_blocks;

	bool found = false;

	// TODO: this modularization should be done in rhr::registry::block

	for (usize i = 0; i < ProjectHandler::mods.size(); i++)
	{
		binned_catagories.push_back(std::vector<ModCatagory*>());
		binned_blocks.push_back(std::vector<std::vector<ModBlock*>>());

		for (usize a = 0; a < category_infos.size(); a++)
		{
			if (category_infos[a].CatagoryModUnlocalizedName == ProjectHandler::mods[i])
			{
				binned_catagories[i].push_back(category_infos[a].CatagoryModCatagory);
				binned_blocks[i].push_back(std::vector<ModBlock*>());

				for (usize b = 0; b < block_infos.size(); b++)
				{
					if (block_infos[b].BlockModUnlocalizedName == ProjectHandler::mods[i] &&
						block_infos[b].BlockModBlock->get_categories() == category_infos[a].CatagoryModCatagory->GetUnlocalizedName())
					{
						binned_blocks[i][a].push_back(block_infos[b].BlockModBlock);
					}
				}
			}
		}
	}

	//for (auto& info : catagoryInfos)
	//{
	//	for (usize i = 0; i < binnedMods.size(); i++)
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

	m_mod_groups.reserve(ProjectHandler::mods.size());

	i32 offset = rhr::stack::block::padding / 2;

	for (usize i = 0; i < ProjectHandler::mods.size(); i++)
	{
		offset += rhr::stack::block::padding / 2;

 		mod_group group = mod_group(cap::color::text_primary_color, cap::color::background_color_3);
		group.mod_category.reserve(binned_catagories[i].size());
		group.mod_button->set_size({ 200, 16 });
		group.mod_button->enable_fill_width(true);
		m_render_frame->add_content(group.mod_button, std::weak_ptr<rhr::render::interfaces::i_updateable>(), group.mod_button, group.mod_button, rhr::render::cardinal::local::DOWN);

		offset += rhr::stack::block::padding / 2;
 		
		active* active_catagories = new active[binned_catagories[i].size()];

		for (usize a = 0; a < binned_catagories[i].size(); a++)
		{
			active_catagories[a].mod_group = static_cast<u16>(i);
			active_catagories[a].mod_group_category = static_cast<u16>(a);

			std::shared_ptr<rhr::render::object::button> button = std::make_shared<rhr::render::object::button>(cap::color::text_primary_color, cap::color::background_color_3);
			button->set_weak(button);
			button->set_size({ 200, 16 });
			button->set_position({ 16, 0 });
			button->set_color_secondary(binned_catagories[i][a]->GetColor());
			button->enable_fill_width(true);
			button->set_callback(button_callback_category, active_catagories + a);
			m_render_frame->add_content(button, std::weak_ptr<rhr::render::interfaces::i_updateable>(), button, button, rhr::render::cardinal::local::DOWN);

			offset += rhr::stack::block::padding / 2 + button->get_size().y;
			group.mod_category.push_back(std::move(button));

			group.mod_category_collections.push_back(std::vector<std::shared_ptr<rhr::stack::collection>>());
			group.mod_category_stacks.push_back(std::vector<std::shared_ptr<rhr::stack::stack>>());
			group.mod_category_blocks.push_back(std::vector<std::shared_ptr<rhr::stack::block>>());

			for (usize b = 0; b < binned_blocks[i][a].size(); b++)
			{
				std::shared_ptr<rhr::stack::collection> collection = std::make_shared<rhr::stack::collection>();
				collection->set_weak(collection);
				collection->display_vanity(false);
				collection->set_position({ 0, (rhr::stack::block::height + (rhr::stack::block::padding * 2)) * b });

				std::shared_ptr<rhr::stack::stack> stack = std::make_shared<rhr::stack::stack>();
				stack->set_weak(stack);

				std::shared_ptr<rhr::stack::block> block = std::make_shared<rhr::stack::block>(binned_blocks[i][a][b]->GetUnlocalizedName());
				block->set_weak(block);
				
				stack->add_block(block);
				collection->add_stack(stack);

				group.mod_category_collections.back().push_back(collection);
				group.mod_category_stacks.back().push_back(stack);
				group.mod_category_blocks.back().push_back(block);
			}
		}

		m_mod_groups.push_back(std::move(group));
	}

	select_category(m_active_category);
}

void rhr::handler::category::render()
{
	for (auto& group : m_mod_groups)
	{
		group.mod_button->render();

		for (auto& catagory : group.mod_category)
			catagory->render();
	}
}

void rhr::handler::category::select_category(active active_category)
{
	m_active_category = active_category;

	if (m_active_category.mod_group >= m_mod_groups.size())
	{
		Logger::Warn("Failed to load catagory. Mod idx out of range (" + std::to_string(m_active_category.mod_group) + " >= " + std::to_string(m_mod_groups.size()) + ")");
		return;
	}

	if (m_active_category.mod_group_category >= m_mod_groups[m_active_category.mod_group].mod_category.size())
	{
		Logger::Warn("Failed to load catagory. Category idx out of range (" + std::to_string(m_active_category.mod_group_category) + " >= " + std::to_string(m_mod_groups[m_active_category.mod_group].mod_category.size()) + ")");
		return;
	}

	rhr::stack::plane::toolbar_plane->delete_contents();

	for (auto collection : m_mod_groups[m_active_category.mod_group].mod_category_collections[m_active_category.mod_group_category])
		rhr::stack::plane::toolbar_plane->add_collection(collection, false);
}

std::vector<rhr::handler::category::mod_group> rhr::handler::category::m_mod_groups;

std::shared_ptr<rhr::render::frame> rhr::handler::category::m_render_frame;

rhr::handler::category::active rhr::handler::category::m_active_category = { 0, 0 };