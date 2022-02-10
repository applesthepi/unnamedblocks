#pragma once
#include "config.h"

#include "rhr/rendering/interfaces/i_ui.hpp"
#include "rhr/stacking/stack.hpp"

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
class collection : public rhr::render::interfaces::i_ui
{
public:
	///
	collection(glm::vec<2, i32>* plane_offset);

	///
	void set_plane_offset(glm::vec<2, i32>* plane_offset);

	///
	glm::vec<2, i32>* get_plane_offset();

	///
	void add_stack(std::shared_ptr<rhr::stack::stack> stack, bool auto_size = true);

	///
	void add_stacks(const std::vector<std::shared_ptr<rhr::stack::stack>>& stacks, bool auto_size = true);

	///
	void size_to_stacks(bool offsetStacks = true, bool usePadding = true);

	///
	void check_bounds();

	///
	void remove_stack(u64 idx, u64 block_offset = 0);

	///
	void remove_all();

	///
	void display_vanity(bool enabled);

	///
	const std::vector<std::shared_ptr<rhr::stack::stack>>& get_stacks();

private:
	///
	void ui_transform_update(i_ui::transform_update_spec transform_update_spec) override;

	///
	void ui_render() override;

	///
	void ui_reload_swap_chain() override;

	///
	void ui_update_buffers() override;

	///
	void ui_chain_update_buffers() override;

	///
	void ui_frame_update(f64 delta_time) override;

	///
	std::vector<std::shared_ptr<rhr::stack::stack>> m_stacks;

	///
	std::shared_ptr<rhr::render::object::rectangle> m_background;

	///
	bool m_display_vanity;

	///
	std::function<void()> m_function_collection_update;

	///
	glm::vec<2, i32>* m_plane_offset;
};
} // namespace rhr::stack