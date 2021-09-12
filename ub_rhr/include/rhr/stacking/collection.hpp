#pragma once
#include "config.h"

#include "rhr/stacking/stack.hpp"
#include "rhr/rendering/interfaces/i_positionable.hpp"
#include "rhr/rendering/interfaces/i_sizeable.hpp"
#include "rhr/rendering/interfaces/i_renderable.hpp"
#include "rhr/rendering/interfaces/i_updateable.hpp"
#include "rhr/rendering/interfaces/i_enableable.hpp"

#include <cappuccino/utils.hpp>

//#define COLLECTION_COLOR_OUTLINE_R 61
//#define COLLECTION_COLOR_OUTLINE_G 61
//#define COLLECTION_COLOR_OUTLINE_B 81
//#define COLLECTION_COLOR_OUTLINE sf::Color(COLLECTION_COLOR_OUTLINE_R, COLLECTION_COLOR_OUTLINE_G, COLLECTION_COLOR_OUTLINE_B)
//
//#define COLLECTION_COLOR_FILL_R 23
//#define COLLECTION_COLOR_FILL_G 23
//#define COLLECTION_COLOR_FILL_B 28
//#define COLLECTION_COLOR_FILL sf::Color(COLLECTION_COLOR_FILL_R, COLLECTION_COLOR_FILL_G, COLLECTION_COLOR_FILL_B)
//
//#define COLLECTION_OUTLINE_WIDTH 4
//#define COLLECTION_TAB_PART 30
//#define COLLECTION_TAB_PART_COUNT 3

#define COLLECTION_EMPTY_SPACE 50

namespace rhr::stack
{
///
class collection : public rhr::render::interfaces::i_positionable<2, i32>, public rhr::render::interfaces::i_sizeable<2, i32>, public rhr::render::interfaces::i_renderable, public rhr::render::interfaces::i_updateable, public rhr::render::interfaces::i_enableable
{
public:
	///
	collection();

	///
	void add_stack(std::shared_ptr<rhr::stack::stack> stack, bool auto_size = true);

	///
	void add_stacks(const std::vector<std::shared_ptr<rhr::stack::stack>>& stacks, bool auto_size = true);

	///
	void size_to_stacks(bool offsetStacks = true, bool usePadding = true);

	///
	void check_bounds();

	///
	void remove_stack(u64 idx);

	///
	void remove_all();

	///
	void display_vanity(bool enabled);

	///
	const std::vector<std::shared_ptr<rhr::stack::stack>>& get_stacks();

	///
	void frame_update(f64 delta_time) override;
private:
	///
	void on_render() override;

	///
	void on_update_buffers() override;

	///
	void on_reload_swap_chain() override;

	///
	void post_position_update() override;

	///
	void post_size_update() override;

	///
	std::vector<std::shared_ptr<rhr::stack::stack>> m_stacks;

	///
	std::shared_ptr<rhr::render::object::rectangle> m_background;

	///
	bool m_display_vanity;

	///
	std::function<void()> m_function_collection_update;
};
}