#pragma once
#include "config.h"

#include "rhr/registries/block.hpp"
#include "rhr/stacking/Plane.hpp"
#include "rhr/stacking/Collection.hpp"
#include "rhr/stacking/Stack.hpp"
#include "rhr/stacking/Block.hpp"
#include "rhr/rendering/objects/button.hpp"
#include "rhr/rendering/frame.hpp"

#include <cappuccino/utils.hpp>

namespace rhr::handler
{
///
static class category
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
private:
	///
	struct mod_group
	{
		///
		mod_group(const cap::color& primary_color, const cap::color& secondary_color)
			: mod_button(std::make_shared<rhr::render::object::button>(primary_color, secondary_color))
		{
			mod_button->set_weak(mod_button);
		}

		///
		std::shared_ptr<rhr::render::object::button> mod_button;

		///
		std::vector<std::shared_ptr<rhr::render::object::button>> mod_category;

		///
		std::vector<std::vector<std::shared_ptr<rhr::stack::collection>>> mod_category_collections;

		///
		std::vector<std::vector<std::shared_ptr<rhr::stack::stack>>> mod_category_stacks;

		///
		std::vector<std::vector<std::shared_ptr<rhr::stack::block>>> mod_category_blocks;
	};

	///
	static std::vector<mod_group> m_mod_groups;

	///
	static std::shared_ptr<rhr::render::frame> m_render_frame;

	///
	static active m_active_category;
};
}