#include "category.hpp"

#include "rhr/registries/char_texture.hpp"
#include "rhr/handlers/project.hpp"
#include "rhr/handlers/message.hpp"
#include "rhr/handlers/build.hpp"
#include "rhr/stacking/plane.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

static void button_callback_category(void* data)
{
	rhr::handler::category::active* active_catagory = (rhr::handler::category::active*) data;
	rhr::handler::category::select_category(*active_catagory);
}

static void button_callback_mod(void* data)
{
	usize* category_idx = (usize*) data;
	rhr::handler::category::toggle_category(*category_idx);
}

void rhr::handler::category::populate()
{
//	m_render_frame = render_frame;
//	m_render_frame->set_padding(0);
//	m_render_frame->enable_background(cap::color::background_color_2);
//	m_render_frame->disable_bar_movement();

	const std::vector<rhr::registry::block::category_info>& category_infos = rhr::registry::block::get_registry()
		.get_categories();
	const std::vector<rhr::registry::block::block_info>& block_infos = rhr::registry::block::get_registry()
		.get_blocks();

	//std::vector<std::string> binnedMods;
	std::vector<std::vector<esp::mod::category*>> binned_catagories;
	std::vector<std::vector<std::vector<cap::mod::block::block*>>> binned_blocks;

	bool found = false;

	// TODO: this modularization should be done in rhr::registry::block

	for (usize i = 0; i < rhr::handler::project::mods.size(); i++)
	{
		binned_catagories.emplace_back();
		binned_blocks.emplace_back();

		usize ac = 0;
		bool first_mod_category = false;

		for (usize a = 0; a < category_infos.size(); a++, ac++)
		{
			if (category_infos[a].category_mod_unlocalized_name == rhr::handler::project::mods[i])
			{
				if (!first_mod_category)
				{
					first_mod_category = true;
					ac = 0;
				}

				binned_catagories[i].push_back(category_infos[a].category_mod_category);
				binned_blocks[i].push_back(std::vector<cap::mod::block::block*>());

				for (usize b = 0; b < block_infos.size(); b++)
				{
					if (block_infos[b].block_mod_unlocalized_name == rhr::handler::project::mods[i] &&
						block_infos[b].block_mod_block->get_category() ==
							category_infos[a].category_mod_category->get_unlocalized_name())
					{
						binned_blocks[i][ac].push_back(block_infos[b].block_mod_block);
					}
				}
			}
		}
	}

	m_mod_groups.reserve(rhr::handler::project::mods.size());
	i32 offset = rhr::stack::block::padding / 2;

	for (usize i = 0; i < rhr::handler::project::mods.size(); i++)
	{
		offset += rhr::stack::block::padding / 2;

		usize* category_idx = new usize;
		*category_idx = i;

		mod_group group = mod_group(cap::color::text_primary_color, cap::color::background_color_3);

		group.id_mod = rhr::handler::project::mods[i];
		group.idx_mod = i;

// 		group.mod_category.reserve(binned_catagories[i].size());

//		group.mod_button->set_size_local({ 100, 16 });
//		group.mod_button->enable_fill_width(true);
//		group.mod_button->set_callback(button_callback_mod, category_idx);
//
//		group.mod_button_render_frame = std::make_shared<rhr::render::frame>();
//		group.mod_button_render_frame->set_weak(group.mod_button_render_frame);
//
//		group.categories_render_frame = std::make_shared<rhr::render::frame>();
//		group.categories_render_frame->set_weak(group.categories_render_frame);

//		m_render_frame->add_frame(group.mod_button_render_frame, rhr::render::cardinal::local::DOWN);
//		group.categories_frame_idx = m_render_frame->add_frame(group.categories_render_frame, rhr::render::cardinal::local::DOWN);

//		group.mod_button_render_frame->add_content(group.mod_button, std::weak_ptr<rhr::render::interfaces::i_updateable>(), group.mod_button, group.mod_button, rhr::render::cardinal::local::DOWN);

		offset += rhr::stack::block::padding / 2;

		active* active_catagories = new active[binned_catagories[i].size()];

		for (usize a = 0; a < binned_catagories[i].size(); a++)
		{
			group.mod_category_ids.push_back(binned_catagories[i][a]->get_display_name());
			group.mod_category_colors.push_back(binned_catagories[i][a]->get_color());

			active_catagories[a].mod_group = static_cast<u16>(i);
			active_catagories[a].mod_group_category = static_cast<u16>(a);

			std::shared_ptr<rhr::render::object::button_text> button = std::make_shared<rhr::render::object::button_text>(
				cap::color::black, cap::color::background_color_3,
				binned_catagories[i][a]->get_display_name());
			button->set_weak(button);
			button->set_color_secondary(binned_catagories[i][a]->get_color());
			button->enable_fill_width(true);
			button->set_callback(button_callback_category, active_catagories + a);
			button->set_size_local({200, 16}, false);
			button->set_position_local_physical({16, 0}, false);

//			group.categories_render_frame->add_content(button, std::weak_ptr<rhr::render::interfaces::i_updateable>(), button, button, rhr::render::cardinal::local::DOWN);

			offset += rhr::stack::block::padding / 2 + button->get_size_local().y;
//			group.mod_category.push_back(std::move(button));

			group.mod_category_collections.emplace_back();
			group.mod_category_stacks.emplace_back();
			group.mod_category_blocks.emplace_back();

			for (usize b = 0; b < binned_blocks[i][a].size(); b++)
			{
				std::shared_ptr<rhr::stack::collection> collection = std::make_shared<rhr::stack::collection>();
				collection->set_weak(collection);
				collection->display_vanity(false);
				collection->set_position_local_physical(
					{0, (rhr::stack::block::height + (rhr::stack::block::padding * 2)) * b}
				);

				std::shared_ptr<rhr::stack::stack> stack = std::make_shared<rhr::stack::stack>();
				stack->set_weak(stack);

				std::shared_ptr<rhr::stack::block> block = std::make_shared<rhr::stack::block>(
					binned_blocks[i][a][b]->get_unlocalized_name());
				block->set_weak(block);

				stack->add_block(block);
				collection->add_stack(stack);

				collection->set_enabled(false);

				group.mod_category_collections.back().push_back(collection);
				group.mod_category_stacks.back().push_back(stack);
				group.mod_category_blocks.back().push_back(block);
			}
		}

//		m_render_frame->add_frame(group.mod_button_render_frame, rhr::render::cardinal::local::DOWN);
//		group.categories_frame_idx = m_render_frame->add_frame(group.categories_render_frame, rhr::render::cardinal::local::DOWN);
//		m_render_frame->set_bar(0, 30);

		m_mod_groups.push_back(std::move(group));
	}

	select_category(m_active_category);
}

void rhr::handler::category::render()
{
	ImGui::Begin("mod_categories");

	if (ImGui::CollapsingHeader("enabled mods"))
	{
		for (auto& group: m_mod_groups)
		{
			std::string mod_cat_count = std::to_string(group.mod_category_ids.size()) + " categories";
			ImGui::Selectable(group.id_mod.c_str(), &group.enabled);
			ImGui::SameLine(100);
			ImGui::Text(mod_cat_count.c_str());
		}
	}

	for (auto& group: m_mod_groups)
	{
		if (ImGui::CollapsingHeader(group.id_mod.c_str(), &group.enabled))
		{
			ImGui::Indent(15.0f);

			for (u16 i = 0; i < static_cast<u16>(group.mod_category_ids.size()); i++)
			{
				auto category_color_100 = group.mod_category_colors[i].get_normalized();
				auto category_color_10 = group.mod_category_colors[i].get_normalized_scaled(0.1f, false);

				std::string cb_id = "cb_" + group.mod_category_ids[i];
				ImGui::ColorButton(cb_id.c_str(), { category_color_100.x, category_color_100.y, category_color_100.z, category_color_100.w });

				ImGui::SameLine();

				ImGuiStyle& style = ImGui::GetStyle();
				glm::vec<4, f32> base_color = { style.Colors[ImGuiCol_Button].x, style.Colors[ImGuiCol_Button].y, style.Colors[ImGuiCol_Button].z, style.Colors[ImGuiCol_Button].w };
				base_color *= 0.8f;

				ImGui::PushStyleColor(
					ImGuiCol_Button, { category_color_10.x + base_color.x, category_color_10.y + base_color.y, category_color_10.z + base_color.z, category_color_10.w + base_color.w }
				);

				if (ImGui::Button(group.mod_category_ids[i].c_str()))
					select_category({group.idx_mod, i});

				ImGui::PopStyleColor();
			}

			ImGui::Unindent(15.0f);
		}
	}

	ImGui::End();
}

void rhr::handler::category::select_category(active active_category)
{
	m_active_category = active_category;

	if (m_active_category.mod_group >= m_mod_groups.size())
	{
		cap::logger::warn(
			cap::logger::level::EDITOR,
			"Failed to load category. Mod idx out of range (" + std::to_string(m_active_category.mod_group) + " >= " +
				std::to_string(m_mod_groups.size()) + ")"
		);
		return;
	}

	if (m_active_category.mod_group_category >= m_mod_groups[m_active_category.mod_group].mod_category_ids.size())
	{
		cap::logger::warn(
			cap::logger::level::EDITOR,
			"Failed to load category. Category idx out of range (" +
				std::to_string(m_active_category.mod_group_category) +
				" >= " + std::to_string(m_mod_groups[m_active_category.mod_group].mod_category_ids.size()) + ")"
		);
		return;
	}

	rhr::stack::plane::toolbar_plane->delete_contents(true);

	for (auto collection: m_mod_groups[m_active_category.mod_group].mod_category_collections[m_active_category
		.mod_group_category])
	{
		collection->set_enabled(true);
		rhr::stack::plane::toolbar_plane->add_collection(collection, false);
	}
}

void rhr::handler::category::toggle_category(usize category_idx)
{
	if (category_idx >= m_mod_groups.size())
	{
		cap::logger::warn(cap::logger::level::EDITOR, "failed to toggle category out of bounds");
		return;
	}

	m_mod_groups[category_idx].toggle();
}

void rhr::handler::category::reload_swap_chain()
{
	for (auto& mod_group: m_mod_groups)
		mod_group.reload_swap_chain();
}

std::vector<rhr::handler::category::mod_group> rhr::handler::category::m_mod_groups;
rhr::handler::category::active rhr::handler::category::m_active_category = {0, 0};