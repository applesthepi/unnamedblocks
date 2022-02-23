#pragma once
#include "config.h"

#include "mocha/handlers/field_data.hpp"
#include "lungo/objects/rectangle.hpp"

#include "mocha/handlers/input.hpp"
#include <latte/utils.hpp>

namespace rhr::handler
{
/// TODO: Tie field handler to each *panel* with there own offset pointers through an interface.
///
class field
{
public:
	///
	field();

	///
	void render();

	///
	void update_buffers();

	///
	void reload_swap_chain();

	///
	void mouse_button(glm::vec<2, i32> position, f32 scroll, rhr::handler::input::mouse_operation operation, rhr::handler::input::mouse_button button);

	///
	void text_button(rhr::handler::input::key_state state);

	///
	rhr::handler::field_data::location register_field(
		std::weak_ptr<rhr::render::interfaces::i_field> text_field,
		glm::vec<2, i32> position,
		glm::vec<2, i32> size,
		u8 layer /*, glm::vec<2, i32>* plane_offset*/);

	///
	void unregister_field(const rhr::handler::field_data::location& location);

	///
	rhr::handler::field_data::location update_field_position(
		const rhr::handler::field_data::location& location,
		glm::vec<2, i32> position /*, glm::vec<2, i32>* plane_offset*/);

	///
	void update_field_size(const rhr::handler::field_data::location& location, glm::vec<2, i32> size);

private:
	///
	void resize(const glm::vec<2, usize>& size);

	///
	std::optional<rhr::handler::field_data::data*> find_first_data(glm::vec<2, i32> position);

	///
	std::optional<rhr::handler::field_data::data*> find_data(glm::vec<2, i32> position, u8 layer);

	///
	std::optional<rhr::handler::field_data::data*> find_data(const rhr::handler::field_data::location& location);

	///
	std::optional<std::vector<std::vector<rhr::handler::field_data::data>>*>
	get_cell(const glm::vec<2, usize>& cell_location);

	///
	void push_data(const rhr::handler::field_data::location& location, const rhr::handler::field_data::data& data);

	///
	void pop_data(const rhr::handler::field_data::location& location);

	///
	glm::vec<2, usize> calculate_cell_position(const glm::vec<2, usize>& position);

	///
	void update_cursor();

	///
	void update_highlight();

	///
	void reset_all();

	///
	void process_highlight(bool copy);

	///
	void advance_cursor(i64 idx, bool break_highlight);

	///
	void set_cursor(usize idx, bool break_highlight);

	///
	void update_guests(const rhr::handler::field_data::location& location);

	///
	void remove_guests(const rhr::handler::field_data::location& location);

	///
	std::vector<std::vector<std::vector<std::vector<rhr::handler::field_data::data>>>> m_cell_table;

	///
	glm::vec<2, usize> m_cell_table_size;

	///
	bool m_mouse_down;

	///
	rhr::handler::field_data::data* m_mouse_down_data;

	///
	bool m_mouse_drag;

	///
	bool m_mouse_click_highlight;

	///
	usize m_mouse_drag_start;

	///
	usize m_mouse_drag_end;

	///
	glm::vec<2, i32> m_mouse_drag_start_position;

	///
	glm::vec<2, i32> m_mouse_drag_end_position;

	///
	bool m_cursor;

	///
	glm::vec<2, i32> m_cursor_position;

	///
	usize m_cursor_idx;

	///
	std::weak_ptr<rhr::render::interfaces::i_field> m_cursor_weak;

	///
	usize m_idx;

	///
	std::shared_ptr<rhr::render::object::rectangle> m_rectangle_cursor;

	///
	std::shared_ptr<rhr::render::object::rectangle> m_rectangle_highlight;
};
} // namespace rhr::handler