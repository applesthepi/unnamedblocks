#pragma once
#include "config.h"

#include "field_data.hpp"

#include <cappuccino/utils.hpp>
#include <espresso/input_handler.hpp>

namespace rhr::handler
{
///
class field
{
public:
	///
	field();

	///
	void mouse_button(glm::vec<2, i32> position, f32 scroll, MouseOperation operation);

	///
	rhr::handler::field_data::location register_field(std::weak_ptr<rhr::render::interfaces::i_field> text_field, glm::vec<2, i32> position);

	///
	void unregister_field(rhr::handler::field_data::location location);

	///
	rhr::handler::field_data::location update_field_position(rhr::handler::field_data::location location, glm::vec<2, i32> position);

	///
	void update_field_size(rhr::handler::field_data::location location, glm::vec<2, i32> size);
private:
	///
	void resize(const glm::vec<2, usize>& size);

	///
	std::optional<rhr::handler::field_data::data*> find_data(glm::vec<2, i32> position);

	///
	std::optional<rhr::handler::field_data::data*> find_data(rhr::handler::field_data::location location);

	///
	std::optional<std::vector<rhr::handler::field_data::data>*> get_cell(glm::vec<2, usize> cell_location);

	///
	void push_data(glm::vec<2, usize> cell_location, rhr::handler::field_data::data data);

	///
	void pop_data(rhr::handler::field_data::location location);

	///
	glm::vec<2, usize> calculate_cell_position(glm::vec<2, usize> position);

	///
	std::vector<std::vector<std::vector<rhr::handler::field_data::data>>> m_cell_table;

	///
	glm::vec<2, usize> m_cell_table_size;

	///
	bool m_mouse_down;

	///
	bool m_mouse_drag;

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
};
}