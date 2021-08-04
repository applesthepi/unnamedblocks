#include "field.hpp"

#define FIELD_CELL_SIZE 500

static void mouse_button_caller(glm::vec<2, i32> position, f32 scroll, MouseOperation operation, void* data)
{
	rhr::handler::field* field = (rhr::handler::field*)data;
	field->mouse_button(position, scroll, operation);
}

rhr::handler::field::field()
	: m_cell_table_size({ 0, 0 })
	, m_mouse_down(false)
	, m_mouse_drag(false)
	, m_mouse_drag_start(0)
	, m_mouse_drag_end(0)
	, m_mouse_drag_start_position({ 0, 0 })
	, m_mouse_drag_end_position({ 0, 0 })
	, m_cursor(false)
	, m_cursor_position({ 0, 0 })
	, m_cursor_idx(0)
	, m_idx(0)
{
	resize({ 5, 5 });
	InputHandler::RegisterMouseCallback(mouse_button_caller, this);
}

void rhr::handler::field::resize(const glm::vec<2, usize>& size)
{
	glm::vec<2, usize> delta_size = size - m_cell_table_size;
	
	if (size.x > m_cell_table_size.x)
	{
		for (usize i = 0; i < delta_size.x; i++)
		{
			m_cell_table.push_back(std::vector<std::vector<rhr::handler::field_data::data>>());

			for (usize a = 0; a < size.y; a++)
				m_cell_table.back().push_back(std::vector<rhr::handler::field_data::data>());
		}

		m_cell_table_size.x = size.x;
	}

	if (size.y > m_cell_table_size.y)
	{
		for (usize i = 0; i < m_cell_table.size(); i++)
		{
			for (usize a = 0; a < delta_size.y; a++)
				m_cell_table[i].push_back(std::vector<rhr::handler::field_data::data>());
		}

		m_cell_table_size.y = size.y;
	}
}

void rhr::handler::field::mouse_button(glm::vec<2, i32> position, f32 scroll, MouseOperation operation)
{
	if (operation == MouseOperation::Move)
	{
		std::optional<rhr::handler::field_data::data*> data = find_data(position);
		if (!data.has_value() || !m_mouse_down)
			return;

		if (auto lock = data.value()->get_text_field().lock())
		{
			std::optional<usize> idx = lock->pick_index(position, false);
			if (!idx.has_value())
				return;

			std::optional<glm::vec<2, i32>> cursor_position = lock->get_index_position(*idx);
			if (!cursor_position.has_value())
				return;

			if (m_mouse_drag_start != *idx)
				m_mouse_drag = true;

			m_mouse_drag_end = *idx;
			m_mouse_drag_end_position = *cursor_position;

			m_cursor_position = *cursor_position;
			m_cursor_idx = *idx;

			Logger::Debug("move from " + std::to_string(m_mouse_drag_start) + " to " + std::to_string(m_mouse_drag_end));
		}
		else
		{
			pop_data(data.value()->get_location());

			m_mouse_down = false;
			m_mouse_drag = false;
			m_mouse_drag_start = 0;
			m_mouse_drag_end = 0;
			m_mouse_drag_start_position = { 0, 0 };
			m_mouse_drag_end_position = { 0, 0 };

			m_cursor = false;
			m_cursor_position = { 0, 0 };
			m_cursor_idx = 0;
		}
	}
	else if (operation == MouseOperation::Press)
	{
		std::optional<rhr::handler::field_data::data*> data = find_data(position);

		// TODO: drag selection box like on a desktop?
		if (!data.has_value() || m_mouse_down)
			return;

		if (auto lock = data.value()->get_text_field().lock())
		{
			std::optional<usize> idx = lock->pick_index(position, false);
			if (!idx.has_value())
				return;

			std::optional<glm::vec<2, i32>> cursor_position = lock->get_index_position(*idx);
			if (!cursor_position.has_value())
				return;

			Logger::Debug("press " + std::to_string(*idx));

			m_mouse_down = true;
			m_mouse_drag = false;
			m_mouse_drag_start = *idx;
			m_mouse_drag_end = *idx;
			m_mouse_drag_start_position = *cursor_position;
			m_mouse_drag_end_position = *cursor_position;

			m_cursor = true;
			m_cursor_position = *cursor_position;
			m_cursor_idx = *idx;
			m_cursor_weak = data.value()->get_text_field();
		}
		else
		{
			pop_data(data.value()->get_location());

			m_mouse_down = true;
			m_mouse_drag = false;
			m_mouse_drag_start = 0;
			m_mouse_drag_end = 0;
			m_mouse_drag_start_position = { 0, 0 };
			m_mouse_drag_end_position = { 0, 0 };

			m_cursor = false;
			m_cursor_position = { 0, 0 };
			m_cursor_idx = 0;
		}
	}
	else if (operation == MouseOperation::Release)
	{
		std::optional<rhr::handler::field_data::data*> data = find_data(position);
		if (!data.has_value() || !m_mouse_down)
			return;

		if (m_mouse_drag)
		{
			m_mouse_down = false;
		}
		else
		{
			m_mouse_down = false;
		}
	}
}

rhr::handler::field_data::location rhr::handler::field::register_field(std::weak_ptr<rhr::render::interfaces::i_field> text_field, glm::vec<2, i32> position)
{
	glm::vec<2, usize> cell_position = calculate_cell_position(position);
	resize(cell_position + glm::vec<2, usize>(1, 1));

	std::optional<std::vector<rhr::handler::field_data::data>*> cell = get_cell(cell_position);
	if (!cell.has_value())
		Logger::Error("rhr::handler::field::register_field failed");

	rhr::handler::field_data::location location = rhr::handler::field_data::location(cell_position, m_idx);
	push_data(cell_position, rhr::handler::field_data::data(m_idx, position, { 0, 0 }, text_field, location));
	m_idx++;

	return location;
}

void rhr::handler::field::unregister_field(rhr::handler::field_data::location location)
{
	pop_data(location);
}

rhr::handler::field_data::location rhr::handler::field::update_field_position(rhr::handler::field_data::location location, glm::vec<2, i32> position)
{
	//Logger::Debug(std::to_string(position.x) + ", " + std::to_string(position.y));

	glm::vec<2, i32> cell_position = location.get_cell();
	cell_position.x *= FIELD_CELL_SIZE;
	cell_position.y *= FIELD_CELL_SIZE;

	if (position.x > cell_position.x && position.x < cell_position.x + FIELD_CELL_SIZE &&
		position.y > cell_position.y && position.y < cell_position.y + FIELD_CELL_SIZE)
	{
		std::optional<rhr::handler::field_data::data*> data = find_data(location);
		if (!data.has_value())
			return location;

		data.value()->set_position(position);
		return location;
	}
	else
	{
		std::optional<rhr::handler::field_data::data*> data = find_data(location);
		if (!data.has_value())
			return location;

		glm::vec<2, usize> cell_position = calculate_cell_position(position);
		resize(cell_position + glm::vec<2, usize>(1, 1));

		rhr::handler::field_data::data local_data = *data.value();
		local_data.set_position(position);
		local_data.set_location(rhr::handler::field_data::location(cell_position, location.get_idx()));

		pop_data(location);
		push_data(cell_position, local_data);

		return local_data.get_location();
	}
}

void rhr::handler::field::update_field_size(rhr::handler::field_data::location location, glm::vec<2, i32> size)
{
	std::optional<rhr::handler::field_data::data*> data = find_data(location);
	if (!data.has_value())
		return;

	data.value()->set_size(size);
}

std::optional<rhr::handler::field_data::data*> rhr::handler::field::find_data(glm::vec<2, i32> position)
{
	glm::vec<2, usize> cell_position = calculate_cell_position(position);
	resize(cell_position + glm::vec<2, usize>(1, 1));

	std::optional<std::vector<rhr::handler::field_data::data>*> cell = get_cell(cell_position);
	if (!cell.has_value())
		return std::nullopt;

	for (usize i = 0; i < cell.value()->size(); i++)
	{
		if (position.x >= cell.value()->at(i).get_position().x && position.x < cell.value()->at(i).get_position().x + cell.value()->at(i).get_size().x &&
			position.y >= cell.value()->at(i).get_position().y && position.y < cell.value()->at(i).get_position().y + cell.value()->at(i).get_size().y)
			return &(cell.value()->at(i));
	}

	return std::nullopt;
}

std::optional<rhr::handler::field_data::data*> rhr::handler::field::find_data(rhr::handler::field_data::location location)
{
	glm::vec<2, usize> cell_position = location.get_cell();
	resize(cell_position + glm::vec<2, usize>(1, 1));

	std::optional<std::vector<rhr::handler::field_data::data>*> cell = get_cell(location.get_cell());
	if (!cell.has_value())
		return std::nullopt;

	for (usize i = 0; i < cell.value()->size(); i++)
	{
		if (cell.value()->at(i).get_idx() == location.get_idx())
			return &(cell.value()->at(i));
	}

	return std::nullopt;
}

std::optional<std::vector<rhr::handler::field_data::data>*> rhr::handler::field::get_cell(glm::vec<2, usize> cell_location)
{
	if (cell_location.x >= m_cell_table_size.x || cell_location.y >= m_cell_table_size.y)
		return std::nullopt;

	return &(m_cell_table[cell_location.x][cell_location.y]);
}

void rhr::handler::field::push_data(glm::vec<2, usize> cell_location, rhr::handler::field_data::data data)
{
	std::optional<std::vector<rhr::handler::field_data::data>*> cell = get_cell(cell_location);
	if (!cell.has_value())
		return;

	cell.value()->push_back(data);
}

void rhr::handler::field::pop_data(rhr::handler::field_data::location location)
{
	std::optional<std::vector<rhr::handler::field_data::data>*> cell = get_cell(location.get_cell());
	if (!cell.has_value())
		return;

	for (usize i = 0; i < cell.value()->size(); i++)
	{
		if (cell.value()->at(i).get_idx() == location.get_idx())
		{
			cell.value()->erase(cell.value()->begin() + i);
			return;
		}
	}

	Logger::Warn("rhr::handler::field::pop_data failed to remove data from cell");
}

glm::vec<2, usize> rhr::handler::field::calculate_cell_position(glm::vec<2, usize> position)
{
	return { static_cast<usize>(glm::floor(static_cast<f32>(position.x) / static_cast<f32>(FIELD_CELL_SIZE))),
			 static_cast<usize>(glm::floor(static_cast<f32>(position.y) / static_cast<f32>(FIELD_CELL_SIZE)))
	};
}
