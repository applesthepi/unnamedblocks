#pragma once
#include "config.h"

// #include "lungo/objects/button_text.hpp"
#include "mocha/stacking/block.hpp"
#include "mocha/stacking/collection.hpp"
#include "mocha/stacking/plane.hpp"
#include "mocha/stacking/stack.hpp"

#include <latte/utils.hpp>

namespace rhr::handler
{
///
class category
{
public:
	///
	struct active
	{
		///
		u16 mod_group;

		///
		u16 mod_group_category;
	};

	///
	static void populate();

	///
	static void render();

	///
	static void select_category(active active_category);

	///
	static void toggle_category(usize category_idx);

	///
	static void reload_swap_chain();

private:
	///
	struct mod_group
	{
		///
		mod_group(const espresso::color& primary_color, const espresso::color& secondary_color)
		//			: mod_button(std::make_shared<rhr::render::object::button>(primary_color, secondary_color))
		{
			enabled = true;
		}

		///
		void toggle()
		{
			enabled = !enabled;

			//			for (auto& button : mod_category)
			//				button->set_enabled(enabled);
			//
			//			m_render_frame->enable_frame(categories_frame_idx, enabled);
		}

		///
		void reload_swap_chain()
		{
			//			mod_button->reload_swapchain();
			//			categories_render_frame->reload_swapchain();
			//			mod_button_render_frame->reload_swapchain();
			//
			//			for (auto& button : mod_category)
			//				button->reload_swapchain();

			for (auto& category : mod_category_collections)
			{
				for (auto& collection : category)
					collection->reload_swap_chain();
			}
		}

		///
		//		std::shared_ptr<rhr::render::object::button> mod_button;

		///
		//		std::shared_ptr<rhr::render::frame> categories_render_frame;

		///
		//		std::shared_ptr<rhr::render::frame> mod_button_render_frame;

		///
		//		std::vector<std::shared_ptr<rhr::render::object::button_text>> mod_category;

		///
		std::vector<std::vector<std::shared_ptr<rhr::stack::collection>>> mod_category_collections;

		///
		std::vector<std::vector<std::shared_ptr<rhr::stack::stack>>> mod_category_stacks;

		///
		std::vector<std::vector<std::shared_ptr<rhr::stack::block>>> mod_category_blocks;

		///
		std::vector<std::string> mod_category_ids;

		///
		std::vector<espresso::color> mod_category_colors;

		///
		bool enabled;

		///
		//		usize categories_frame_idx;

		///
		std::string id_mod;

		///
		u16 idx_mod;
	};

	///
	static std::vector<mod_group> m_mod_groups;

	///
	//	static std::shared_ptr<rhr::render::frame> m_render_frame;

	///
	static active m_active_category;
};
} // namespace rhr::handler