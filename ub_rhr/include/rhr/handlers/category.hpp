#pragma once
#include "config.h"

#include "rhr/registries/block.hpp"
#include "rhr/stacking/plane.hpp"
#include "rhr/stacking/collection.hpp"
#include "rhr/stacking/stack.hpp"
#include "rhr/stacking/block.hpp"
#include "rhr/rendering/objects/button_text.hpp"
#include "rhr/rendering/frame.hpp"

#include <cappuccino/utils.hpp>

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
	static void populate(std::shared_ptr<rhr::render::frame>& render_frame);

	///
	static void render();

	///
	static void select_category(active active_category);

	///
	static void toggle_category(usize category_idx);
private:
	///
	struct mod_group
	{
		///
		mod_group(const cap::color& primary_color, const cap::color& secondary_color)
			: mod_button(std::make_shared<rhr::render::object::button>(primary_color, secondary_color))
		{
			mod_button->set_weak(mod_button);
			m_enabled = true;
		}

		void toggle()
		{
			m_enabled = !m_enabled;

			for (auto button : mod_category)
				button->set_enabled(m_enabled);
		}

		///
		std::shared_ptr<rhr::render::object::button> mod_button;

		///
		std::vector<std::shared_ptr<rhr::render::object::button_text>> mod_category;

		///
		std::vector<std::vector<std::shared_ptr<rhr::stack::collection>>> mod_category_collections;

		///
		std::vector<std::vector<std::shared_ptr<rhr::stack::stack>>> mod_category_stacks;

		///
		std::vector<std::vector<std::shared_ptr<rhr::stack::block>>> mod_category_blocks;

		///
		bool m_enabled;
	};

	///
	static std::vector<mod_group> m_mod_groups;

	///
	static std::shared_ptr<rhr::render::frame> m_render_frame;

	///
	static active m_active_category;
};
}