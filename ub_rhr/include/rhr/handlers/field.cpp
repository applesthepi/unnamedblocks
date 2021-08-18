#include "field.hpp"

#include "rhr/stacking/block.hpp"

#define FIELD_CELL_SIZE 500

static void mouse_button_caller(glm::vec<2, i32> position, f32 scroll, MouseOperation operation, void* data)
{
	rhr::handler::field* field = (rhr::handler::field*)data;
	field->mouse_button(position, scroll, operation);
}

static void text_button_caller(InputHandler::key_state state, void* data)
{
	rhr::handler::field* field = (rhr::handler::field*)data;
	field->text_button(state);
}

rhr::handler::field::field()
	: m_cell_table_size({ 0, 0 })
	, m_mouse_down(false)
	, m_mouse_down_data(nullptr)
	, m_mouse_drag(false)
	, m_mouse_click_highlight(false)
	, m_mouse_drag_start(0)
	, m_mouse_drag_end(0)
	, m_mouse_drag_start_position({ 0, 0 })
	, m_mouse_drag_end_position({ 0, 0 })
	, m_cursor(false)
	, m_cursor_position({ 0, 0 })
	, m_cursor_idx(0)
	, m_idx(0)
	, m_rectangle_cursor(std::make_shared<rhr::render::object::rectangle>())
	, m_rectangle_highlight(std::make_shared<rhr::render::object::rectangle>())
{
	resize({ 5, 5 });

	m_rectangle_cursor->set_weak(m_rectangle_cursor);
	m_rectangle_highlight->set_weak(m_rectangle_highlight);

	m_rectangle_cursor->set_color(cap::color::text_secondary_color);
	m_rectangle_cursor->set_size({ 2, rhr::stack::block::height_content - 4 });
	
	m_rectangle_highlight->set_color(cap::color().from_u8({ 95, 165, 255, 60 }));

	InputHandler::RegisterMouseCallback(mouse_button_caller, this);
	InputHandler::RegisterTextCallback(text_button_caller, this);
}

void rhr::handler::field::resize(const glm::vec<2, usize>& size)
{
	glm::vec<2, usize> delta_size = size - m_cell_table_size;
	
	if (size.x > m_cell_table_size.x)
	{
		for (usize i = 0; i < delta_size.x; i++)
		{
			m_cell_table.push_back(std::vector<std::vector<std::vector<rhr::handler::field_data::data>>>());

			for (usize a = 0; a < size.y; a++)
				m_cell_table.back().push_back(std::vector<std::vector<rhr::handler::field_data::data>>());
		}

		m_cell_table_size.x = size.x;
	}

	if (size.y > m_cell_table_size.y)
	{
		for (usize i = 0; i < m_cell_table.size(); i++)
		{
			for (usize a = 0; a < delta_size.y; a++)
				m_cell_table[i].push_back(std::vector<std::vector<rhr::handler::field_data::data>>());
		}

		m_cell_table_size.y = size.y;
	}
}

void rhr::handler::field::render()
{
	if (m_cursor)
		m_rectangle_cursor->render();

	if (m_mouse_drag)
		m_rectangle_highlight->render();
}

void rhr::handler::field::mouse_button(glm::vec<2, i32> position, f32 scroll, MouseOperation operation)
{
	if (operation == MouseOperation::Move)
	{
		if (m_mouse_down_data == nullptr || !m_mouse_down)
			return;

		if (auto lock = m_mouse_down_data->get_text_field().lock())
		{
			std::optional<usize> idx = lock->pick_index(position, true);
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

			update_cursor();
			update_highlight();

			Logger::Debug("move from " + std::to_string(m_mouse_drag_start) + " to " + std::to_string(m_mouse_drag_end));
		}
		else
		{
			pop_data(m_mouse_down_data->get_location());
			reset_all();
		}
	}
	else if (operation == MouseOperation::Press)
	{
		std::optional<rhr::handler::field_data::data*> data = find_first_data(position);

		// TODO: drag selection box like on a desktop?
		if (m_mouse_down)
			return;

		if (!data.has_value())
		{
			reset_all();
			return;
		}

		if (auto lock = data.value()->get_text_field().lock())
		{
			std::optional<usize> idx = lock->pick_index(position, false);
			if (!idx.has_value())
				return;

			std::optional<glm::vec<2, i32>> cursor_position = lock->get_index_position(*idx);
			if (!cursor_position.has_value())
				return;

			m_mouse_down = true;
			m_mouse_down_data = data.value();
			m_mouse_drag = false;
			m_mouse_drag_start = *idx;
			m_mouse_drag_end = *idx;
			m_mouse_drag_start_position = *cursor_position;
			m_mouse_drag_end_position = *cursor_position;

			m_cursor = true;
			m_cursor_position = *cursor_position;
			m_cursor_idx = *idx;
			m_cursor_weak = data.value()->get_text_field();

			update_cursor();
		}
		else
		{
			pop_data(data.value()->get_location());

			reset_all();
			m_mouse_down = true;
		}
	}
	else if (operation == MouseOperation::DoublePress)
	{
		std::optional<rhr::handler::field_data::data*> data = find_first_data(position);

		if (m_mouse_down)
			return;

		if (!data.has_value())
		{
			reset_all();
			return;
		}

		if (auto lock = data.value()->get_text_field().lock())
		{
			std::optional<usize> idx = lock->pick_index(position, false);
			if (!idx.has_value())
				return;

			std::optional<glm::vec<2, i32>> cursor_position_start = lock->get_index_position(0);
			std::optional<glm::vec<2, i32>> cursor_position_end = lock->get_index_position(lock->get_index_count());

			if (!cursor_position_start.has_value() || !cursor_position_end.has_value())
				return;

			Logger::Debug("double press");

			m_mouse_down = true;
			m_mouse_down_data = data.value();
			m_mouse_drag = true;
			m_mouse_drag_start = 0;
			m_mouse_drag_end = lock->get_index_count();
			m_mouse_drag_start_position = *cursor_position_start;
			m_mouse_drag_end_position = *cursor_position_end;

			m_cursor = true;
			m_cursor_position = *cursor_position_end;
			m_cursor_idx = lock->get_index_count();
			m_cursor_weak = data.value()->get_text_field();

			update_cursor();
			update_highlight();
		}
		else
		{
			pop_data(data.value()->get_location());

			reset_all();
			m_mouse_down = true;
		}
	}
	else if (operation == MouseOperation::Release)
	{
		if (!m_mouse_down)
			return;
		
		m_mouse_down = false;
	}
}

void rhr::handler::field::text_button(InputHandler::key_state state)
{
	if (!m_cursor)
		return;

	/*switch (key)
	{
	case GLFW_KEY_ESCAPE:
		break;
	case GLFW_KEY_ENTER:
		break;
	case GLFW_KEY_RIGHT:
		break;
	case GLFW_KEY_LEFT:
		break;
	case GLFW_KEY_UP:
		break;
	case GLFW_KEY_DOWN:
		break;
	case GLFW_KEY_HOME:
		break;
	case GLFW_KEY_END:
		break;
	case GLFW_KEY_LEFT_SHIFT:
		break;
	default:
		process_highlight(false);
		break;
	}*/

	if (auto lock = m_mouse_down_data->get_text_field().lock())
	{
		switch (state.key)
		{
		case GLFW_KEY_BACKSPACE:
			if (m_mouse_drag)
			{
				process_highlight(false);
				
			}
			else
			{
				lock->remove_char(m_cursor_idx - 1);
				advance_cursor(-1, true);
			}
			return;
		case GLFW_KEY_DELETE:
			if (m_mouse_drag)
				process_highlight(false);
			else
				lock->remove_char(m_cursor_idx + 1);
			return;
		case GLFW_KEY_ESCAPE:
			reset_all();
			return;
		case GLFW_KEY_ENTER:
			reset_all();
			return;
		case GLFW_KEY_RIGHT:
			if (state.shift)
				m_mouse_drag = true;
			else if (m_mouse_drag)
			{
				m_mouse_drag = false;
				set_cursor(glm::max(m_mouse_drag_start, m_mouse_drag_end), !state.shift);
				return;
			}
			advance_cursor(1, !state.shift);
			return;
		case GLFW_KEY_LEFT:
			if (state.shift)
				m_mouse_drag = true;
			else if (m_mouse_drag)
			{
				m_mouse_drag = false;
				set_cursor(glm::min(m_mouse_drag_start, m_mouse_drag_end), !state.shift);
				return;
			}
			advance_cursor(-1, !state.shift);
			return;
		case GLFW_KEY_UP:
			set_cursor(0, !state.shift);
			return;
		case GLFW_KEY_DOWN:
			set_cursor(lock->get_index_count(), !state.shift);
			return;
		case GLFW_KEY_HOME:
			set_cursor(0, !state.shift);
			return;
		case GLFW_KEY_END:
			set_cursor(lock->get_index_count(), !state.shift);
			return;
		case 'A':
			if (state.ctrl)
			{
				set_cursor(0, true);
				m_mouse_drag_start = 0;
				m_mouse_drag_end = 0;
				m_mouse_drag = true;
				set_cursor(lock->get_index_count(), false);
				return;
			}
		}

		if (state.key >= 32 && state.key <= 96)
			process_highlight(false);

		if (state.key < 32 || state.key > 96)
			return;

		if (state.shift)
		{
			char insert_key;
			bool insert_key_valid = true;

			switch (state.key)
			{
			case '\'':
				insert_key = '"';
				break;
			case ',':
				insert_key = '<';
				break;
			case '-':
				insert_key = '_';
				break;
			case '.':
				insert_key = '>';
				break;
			case '/':
				insert_key = '?';
				break;
			case '0':
				insert_key = ')';
				break;
			case '1':
				insert_key = '!';
				break;
			case '2':
				insert_key = '@';
				break;
			case '3':
				insert_key = '#';
				break;
			case '4':
				insert_key = '$';
				break;
			case '5':
				insert_key = '%';
				break;
			case '6':
				insert_key = '^';
				break;
			case '7':
				insert_key = '&';
				break;
			case '8':
				insert_key = '*';
				break;
			case '9':
				insert_key = '(';
				break;
			case ';':
				insert_key = ':';
				break;
			case '=':
				insert_key = '+';
				break;
			case '[':
				insert_key = '{';
				break;
			case '\\':
				insert_key = '|';
				break;
			case ']':
				insert_key = '}';
				break;
			case '`':
				insert_key = '~';
				break;
			default:
				insert_key_valid = false;
				break;
			}

			if (state.key >= 'A' && state.key <= 'Z')
			{
				insert_key = state.key;
				insert_key_valid = true;
			}

			if (!insert_key_valid)
				return;

			lock->insert_char(insert_key, m_cursor_idx);
		}
		else
		{
			if (state.key >= 'A' && state.key <= 'Z')
				lock->insert_char(state.key + 32 /* make lowercase */, m_cursor_idx);
			else
				lock->insert_char(state.key, m_cursor_idx);
		}
	}

	advance_cursor(1, false);
}

rhr::handler::field_data::location rhr::handler::field::register_field(std::weak_ptr<rhr::render::interfaces::i_field> text_field, glm::vec<2, i32> position, u8 layer)
{
	glm::vec<2, usize> cell_position = calculate_cell_position(position);
	resize(cell_position + glm::vec<2, usize>(1, 1));

	std::optional<std::vector<std::vector<rhr::handler::field_data::data>>*> cell = get_cell(cell_position);
	if (!cell.has_value())
		Logger::Error("rhr::handler::field::register_field failed");

	rhr::handler::field_data::location location = rhr::handler::field_data::location(cell_position, m_idx, layer);
	push_data(location, rhr::handler::field_data::data(m_idx, position, { 0, 0 }, text_field, location));
	m_idx++;

	return location;
}

void rhr::handler::field::unregister_field(rhr::handler::field_data::location location)
{
	pop_data(location);
}

rhr::handler::field_data::location rhr::handler::field::update_field_position(rhr::handler::field_data::location location, glm::vec<2, i32> position)
{
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

		rhr::handler::field_data::location moved_cell_location = rhr::handler::field_data::location(cell_position, location.get_idx(), location.get_layer());

		rhr::handler::field_data::data local_data = *data.value();
		local_data.set_position(position);
		local_data.set_location(moved_cell_location);

		pop_data(location);
		push_data(moved_cell_location, local_data);

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

std::optional<rhr::handler::field_data::data*> rhr::handler::field::find_first_data(glm::vec<2, i32> position)
{
	glm::vec<2, usize> cell_position = calculate_cell_position(position);

	std::optional<std::vector<std::vector<rhr::handler::field_data::data>>*> cell = get_cell(cell_position);
	if (!cell.has_value())
		return std::nullopt;

	for (u8 i = 0; i < static_cast<u8>(cell.value()->size()); i++)
	{
		std::optional<rhr::handler::field_data::data*> data = find_data(position, i);
		if (data.has_value())
			return data;
	}

	return std::nullopt;
}

std::optional<rhr::handler::field_data::data*> rhr::handler::field::find_data(glm::vec<2, i32> position, u8 layer)
{
	glm::vec<2, usize> cell_position = calculate_cell_position(position);
	resize(cell_position + glm::vec<2, usize>(1, 1));

	std::optional<std::vector<std::vector<rhr::handler::field_data::data>>*> cell = get_cell(cell_position);
	if (!cell.has_value())
		return std::nullopt;

	for (usize i = 0; i < cell.value()->at(layer).size(); i++)
	{
		glm::vec<2, i32> cell_pixel_position = cell.value()->at(layer)[i].get_position();
		glm::vec<2, i32> cell_pixel_size = cell.value()->at(layer)[i].get_size();

		if (position.x >= cell_pixel_position.x && position.x < cell_pixel_position.x + cell_pixel_size.x &&
			position.y >= cell_pixel_position.y && position.y < cell_pixel_position.y + cell_pixel_size.y)
			return &(cell.value()->at(layer)[i]);
	}

	return std::nullopt;
}

std::optional<rhr::handler::field_data::data*> rhr::handler::field::find_data(rhr::handler::field_data::location location)
{
	resize(location.get_cell() + glm::vec<2, usize>(1, 1));

	std::optional<std::vector<std::vector<rhr::handler::field_data::data>>*> cell = get_cell(location.get_cell());
	if (!cell.has_value())
		return std::nullopt;

	for (usize i = 0; i < cell.value()->at(location.get_layer()).size(); i++)
	{
		if (cell.value()->at(location.get_layer())[i].get_idx() == location.get_idx())
			return &(cell.value()->at(location.get_layer())[i]);
	}

	return std::nullopt;
}

std::optional<std::vector<std::vector<rhr::handler::field_data::data>>*> rhr::handler::field::get_cell(glm::vec<2, usize> cell_location)
{
	if (cell_location.x >= m_cell_table_size.x || cell_location.y >= m_cell_table_size.y)
		return std::nullopt;

	return &(m_cell_table[cell_location.x][cell_location.y]);
}

void rhr::handler::field::push_data(rhr::handler::field_data::location location, rhr::handler::field_data::data data)
{
	std::optional<std::vector<std::vector<rhr::handler::field_data::data>>*> cell = get_cell(location.get_cell());
	if (!cell.has_value())
		return;

	if (location.get_layer() >= static_cast<u8>(cell.value()->size()))
	{
		u8 delta_size = location.get_layer() - static_cast<u8>(cell.value()->size());
 		for (u8 i = 0; i < delta_size + 1; i++)
			cell.value()->push_back(std::vector<rhr::handler::field_data::data>());
	}

	cell.value()->at(location.get_layer()).push_back(data);
}

void rhr::handler::field::pop_data(rhr::handler::field_data::location location)
{
	std::optional<std::vector<std::vector<rhr::handler::field_data::data>>*> cell = get_cell(location.get_cell());
	if (!cell.has_value())
		return;

	for (usize i = 0; i < cell.value()->at(location.get_layer()).size(); i++)
	{
		if (cell.value()->at(location.get_layer())[i].get_idx() == location.get_idx())
		{
			cell.value()->at(location.get_layer()).erase(cell.value()->at(location.get_layer()).begin() + i);
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

void rhr::handler::field::update_cursor()
{
	m_rectangle_cursor->set_position({ m_cursor_position.x - 1, m_cursor_position.y + 2 });
}

void rhr::handler::field::update_highlight()
{
	m_rectangle_highlight->set_position({ m_mouse_drag_start_position.x, m_mouse_drag_start_position.y + 2 });
	m_rectangle_highlight->set_size({ m_mouse_drag_end_position.x - m_mouse_drag_start_position.x, rhr::stack::block::height_content - 4 });
}

void rhr::handler::field::reset_all()
{
	m_mouse_down = false;
	m_mouse_down_data = nullptr;
	m_mouse_drag = false;
	m_mouse_drag_start = 0;
	m_mouse_drag_end = 0;
	m_mouse_drag_start_position = { 0, 0 };
	m_mouse_drag_end_position = { 0, 0 };

	m_cursor = false;
	m_cursor_position = { 0, 0 };
	m_cursor_idx = 0;
}

void rhr::handler::field::process_highlight(bool copy)
{
	if (!m_cursor || !m_mouse_drag)
		return;

	// TODO: segfault possible, later on make this use the weakptr so no segfault possibility.
	if (auto lock = m_mouse_down_data->get_text_field().lock())
	{
		if (copy)
			Logger::Debug("copy not implemented");
		else
		{
			if (m_mouse_drag_end > m_mouse_drag_start)
				lock->remove_string(m_mouse_drag_start, m_mouse_drag_end - m_mouse_drag_start);
			else
				lock->remove_string(m_mouse_drag_end, m_mouse_drag_start - m_mouse_drag_end);
		}

		m_cursor_idx = glm::min(m_mouse_drag_start, m_mouse_drag_end);

		m_mouse_drag = false;
		m_mouse_drag_start = m_cursor_idx;
		m_mouse_drag_end = m_cursor_idx;

		std::optional<glm::vec<2, i32>> cursor_position = lock->get_index_position(m_cursor_idx);
		if (!cursor_position.has_value())
			return;

		m_cursor_position = cursor_position.value();
		m_mouse_drag_start_position = cursor_position.value();
		m_mouse_drag_end_position = cursor_position.value();

		update_cursor();
		update_highlight();
	}
}

void rhr::handler::field::advance_cursor(i64 idx, bool break_highlight)
{
	if (!m_cursor)
		return;

	if (auto lock = m_mouse_down_data->get_text_field().lock())
	{
		if (static_cast<i64>(m_cursor_idx) + idx < 0 || static_cast<i64>(m_cursor_idx) + idx > lock->get_index_count())
			return;

		m_cursor_idx = static_cast<i64>(m_cursor_idx) + idx;

		std::optional<glm::vec<2, i32>> cursor_position = lock->get_index_position(m_cursor_idx);
		if (!cursor_position.has_value())
			return;

		m_cursor_position = cursor_position.value();
		
		if (m_mouse_drag && !break_highlight)
		{
			m_mouse_drag_end = static_cast<i64>(m_mouse_drag_end) + idx;
			m_mouse_drag_end_position = cursor_position.value();
		}
		else
		{
			m_mouse_drag_start = static_cast<i64>(m_mouse_drag_start) + idx;
			m_mouse_drag_start_position = cursor_position.value();

			m_mouse_drag_end = static_cast<i64>(m_mouse_drag_end) + idx;
			m_mouse_drag_end_position = cursor_position.value();
		}

		update_cursor();
		update_highlight();
	}
}

void rhr::handler::field::set_cursor(usize idx, bool break_highlight)
{
	if (!m_cursor)
		return;

	if (auto lock = m_mouse_down_data->get_text_field().lock())
	{
		if (idx < 0 || idx > lock->get_index_count())
			return;

		m_cursor_idx = idx;

		std::optional<glm::vec<2, i32>> cursor_position = lock->get_index_position(m_cursor_idx);
		if (!cursor_position.has_value())
			return;

		m_cursor_position = cursor_position.value();

		if (m_mouse_drag && !break_highlight)
		{
			m_mouse_drag_end = idx;
			m_mouse_drag_end_position = cursor_position.value();
		}
		else
		{
			m_mouse_drag_start = idx;
			m_mouse_drag_start_position = cursor_position.value();

			m_mouse_drag_end = idx;
			m_mouse_drag_end_position = cursor_position.value();
		}

		update_cursor();
		update_highlight();
	}
}
