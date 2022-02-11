#include "field.hpp"

#include "rhr/handlers/context.hpp"
#include "rhr/stacking/plane.hpp"
#include "rhr/rendering/renderer.hpp"

#define DEBUG_FIELDS 0

constexpr usize FIELD_CELL_SIZE = 5000;

static void mouse_button_caller(glm::vec<2, i32> position, f32 scroll, MouseOperation operation, MouseButton button, void* data)
{
	rhr::handler::field* field = (rhr::handler::field*)data;
	field->mouse_button(position, scroll, operation, button);
}

static void text_button_caller(InputHandler::key_state state, void* data)
{
	rhr::handler::field* field = (rhr::handler::field*)data;
	field->text_button(state);
}

rhr::handler::field::field()
	: m_cell_table_size({0, 0})
	, m_mouse_down(false)
	, m_mouse_down_data(nullptr)
	, m_mouse_drag(false)
	, m_mouse_click_highlight(false)
	, m_mouse_drag_start(0)
	, m_mouse_drag_end(0)
	, m_mouse_drag_start_position({0, 0})
	, m_mouse_drag_end_position({0, 0})
	, m_cursor(false)
	, m_cursor_position({0, 0})
	, m_cursor_idx(0)
	, m_idx(0)
	, m_rectangle_cursor(std::make_shared<rhr::render::object::rectangle>())
	, m_rectangle_highlight(std::make_shared<rhr::render::object::rectangle>())
{
	resize({5, 5});

	m_rectangle_highlight->enable_border(false, rhr::render::cardinal::local_horizontal::RIGHT, rhr::render::cardinal::local_vertical::DOWN);

	m_rectangle_cursor->set_color(cap::color::text_primary_color);
	m_rectangle_cursor->set_size_local({1, rhr::stack::block::height_content}, true);
	m_rectangle_cursor->enable_border(false, rhr::render::cardinal::local_horizontal::RIGHT, rhr::render::cardinal::local_vertical::DOWN);
	m_rectangle_cursor->set_depth(rhr::render::renderer::depth_cursor);

	m_rectangle_highlight->set_color(cap::color().from_u8({30, 70, 210, 80}));

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

void rhr::handler::field::update_buffers()
{
	m_rectangle_cursor->update_buffers();
	m_rectangle_highlight->update_buffers();
}

void rhr::handler::field::reload_swap_chain()
{
	m_rectangle_cursor->reload_swap_chain();
	m_rectangle_highlight->reload_swap_chain();
}

void rhr::handler::field::mouse_button(glm::vec<2, i32> position, f32 scroll, MouseOperation operation, MouseButton button)
{
	position -= *rhr::stack::plane::primary_plane->get_offset();

	std::optional<rhr::handler::field_data::data*> data = find_first_data(position);

	if (operation == MouseOperation::Move)
	{
		if (m_mouse_down_data == nullptr || !m_mouse_down)
			return;

		if (auto lock = m_mouse_down_data->get_text_field().lock())
		{
			if (button != MouseButton::LEFT)
				return;

			std::optional<usize> idx = lock->pick_index(position, true);
			if (!idx.has_value())
				return;

			std::optional<glm::vec<2, i32>> cursor_position = lock->get_index_position(*idx);
			if (!cursor_position.has_value())
				return;

			if (m_mouse_drag_start != *idx)
				m_mouse_drag = true;

			m_mouse_drag_end		  = *idx;
			m_mouse_drag_end_position = *cursor_position;

			m_cursor_position = *cursor_position;
			m_cursor_idx	  = *idx;

			update_cursor();
			update_highlight();

			//			cap::logger::info("move from " + std::to_string(m_mouse_drag_start) + " to " + std::to_string(m_mouse_drag_end));
		}
		else
		{
			pop_data(m_mouse_down_data->get_location());
			reset_all();
		}

		return;
	}
	else if (operation == MouseOperation::Release)
	{
		m_mouse_down = false;
		return;
	}
	else if (operation == MouseOperation::Click)
		return;

	if (!data.has_value())
	{
		reset_all();
		return;
	}

	if (auto lock = data.value()->get_text_field().lock())
		lock->mouse_button(position, scroll, operation, button);

	if (operation == MouseOperation::Press)
	{
		// TODO: drag selection box like on a desktop?
		if (m_mouse_down)
			return;

		std::weak_ptr<rhr::render::interfaces::i_field> text_field_weak = data.value()->get_text_field();

		if (auto lock = text_field_weak.lock())
		{
			std::optional<usize> idx = lock->pick_index(position, false);
			if (!idx.has_value())
				return;

			std::optional<glm::vec<2, i32>> cursor_position = lock->get_index_position(*idx);
			if (!cursor_position.has_value())
				return;

			usize field_idx = idx.value();

			if (button == MouseButton::RIGHT && m_mouse_drag)
			{
				rhr::handler::context::open(
					rhr::handler::context::flag::TEXT_SELECTION_ONLY,
					[text_field_weak, field_idx](RHR_HANDLER_CONTEXT_FLAG_PREFIX flag, u8 flag_menu_item)
					{
						// if (flag == rhr::handler::context::flag::TEXT_SELECTION_ONLY)
						//{
						//	bool flag_delete_collection = false;
						//
						//	switch (rhr::handler::context::flag::hashed_menu_text[flag_menu_item])
						//	{
						//	case rhr::handler::context::flag::menu_text::PASTE:
						//
						//		if (clip::has(clip::text_format()))
						//		{
						//			std::string value;
						//			clip::get_text(value);
						//
						//			if (auto data = text_field_weak.lock())
						//				data->insert_string(value, field_idx);
						//		}
						//
						//		break;
						//	case rhr::handler::context::flag::menu_text::COPY:
						//
						//		if (auto data = text_field_weak.lock())
						//			data->
						//
						//		break;
						//	}
						// }
					});

				return;
			}

			m_mouse_down				= true;
			m_mouse_down_data			= data.value();
			m_mouse_drag				= false;
			m_mouse_drag_start			= *idx;
			m_mouse_drag_end			= *idx;
			m_mouse_drag_start_position = *cursor_position;
			m_mouse_drag_end_position	= *cursor_position;

			m_cursor		  = true;
			m_cursor_position = *cursor_position;
			m_cursor_idx	  = *idx;
			m_cursor_weak	  = data.value()->get_text_field();

			update_cursor();

			if (button == MouseButton::RIGHT)
			{
				rhr::handler::context::open(
					rhr::handler::context::flag::TEXT_STANDING_ONLY,
					[text_field_weak, field_idx](RHR_HANDLER_CONTEXT_FLAG_PREFIX flag, u8 flag_menu_item)
					{
						// if (flag == rhr::handler::context::flag::TEXT_STANDING_ONLY)
						//{
						//	bool flag_delete_collection = false;
						//
						//	switch (rhr::handler::context::flag::hashed_menu_text[flag_menu_item])
						//	{
						//	case rhr::handler::context::flag::menu_text::PASTE:
						//
						//		if (clip::has(clip::text_format()))
						//		{
						//			std::string value;
						//			clip::get_text(value);
						//
						//			if (auto data = text_field_weak.lock())
						//				data->insert_string(value, field_idx);
						//		}
						//
						//		break;
						//	}
						// }
					});

				return;
			}
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
		if (m_mouse_down)
			return;

		if (auto lock = data.value()->get_text_field().lock())
		{
			if (button != MouseButton::LEFT)
				return;

			std::optional<usize> idx = lock->pick_index(position, false);
			if (!idx.has_value())
				return;

			std::optional<glm::vec<2, i32>> cursor_position_start = lock->get_index_position(0);
			std::optional<glm::vec<2, i32>> cursor_position_end	  = lock->get_index_position(lock->get_index_count());

			if (!cursor_position_start.has_value() || !cursor_position_end.has_value())
				return;
#if DEBUG_FIELDS
			cap::logger::info("double press");
#endif
			m_mouse_down				= true;
			m_mouse_down_data			= data.value();
			m_mouse_drag				= true;
			m_mouse_drag_start			= 0;
			m_mouse_drag_end			= lock->get_index_count();
			m_mouse_drag_start_position = *cursor_position_start;
			m_mouse_drag_end_position	= *cursor_position_end;

			m_cursor		  = true;
			m_cursor_position = *cursor_position_end;
			m_cursor_idx	  = lock->get_index_count();
			m_cursor_weak	  = data.value()->get_text_field();

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
}

void rhr::handler::field::text_button(InputHandler::key_state state)
{
	if (!m_cursor)
		return;

	if (auto lock = m_mouse_down_data->get_text_field().lock())
	{
		switch (state.key)
		{
		case GLFW_KEY_BACKSPACE:
			if (m_mouse_drag)
				process_highlight(false);
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
				lock->remove_char(m_cursor_idx);
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
				m_mouse_drag_end   = 0;
				m_mouse_drag	   = true;
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
				insert_key		 = state.key;
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

rhr::handler::field_data::location
rhr::handler::field::register_field(std::weak_ptr<rhr::render::interfaces::i_field> text_field, glm::vec<2, i32> position, glm::vec<2, i32> size, u8 layer/*, glm::vec<2, i32>* plane_offset*/)
{
	glm::vec<2, usize> cell_position;

	/*if (plane_offset != nullptr)
		cell_position = calculate_cell_position(position + *plane_offset);
	else*/

	cell_position = calculate_cell_position(position);

	resize(cell_position + glm::vec<2, usize>(1, 1));

	std::optional<std::vector<std::vector<rhr::handler::field_data::data>>*> cell = get_cell(cell_position);
	if (!cell.has_value())
		cap::logger::error(cap::logger::level::SYSTEM, "rhr::handler::field::register_field failed");

	rhr::handler::field_data::location local_location = rhr::handler::field_data::location(cell_position, m_idx, layer);
	rhr::handler::field_data::data local_data(m_idx, position, /*plane_offset, */size, std::move(text_field), local_location, true);
	local_data.copy_to_host();

	push_data(local_location, local_data);
	update_guests(local_location);
	m_idx++;

	return local_location;
}

void rhr::handler::field::unregister_field(const rhr::handler::field_data::location& location)
{
	pop_data(location);
	remove_guests(location);
}

rhr::handler::field_data::location rhr::handler::field::update_field_position(const rhr::handler::field_data::location& location, glm::vec<2, i32> position/*, glm::vec<2, i32>* plane_offset*/)
{
	glm::vec<2, i32> local_position = position;

	/*if (plane_offset != nullptr)
		position += *plane_offset;*/

	if (position.x < 0 || position.y < 0)
	{
		cap::logger::warn(cap::logger::level::SYSTEM, "failed to update field position; position is less then 0");
		return location;
	}

	glm::vec<2, usize> cell_position = location.get_cell();
	cell_position.x *= FIELD_CELL_SIZE;
	cell_position.y *= FIELD_CELL_SIZE;

#if DEBUG_FIELDS
	cap::logger::info(std::to_string(cell_position.x) + ", " + std::to_string(cell_position.y));
#endif
	if (static_cast<usize>(position.x) > cell_position.x && static_cast<usize>(position.x) < cell_position.x + FIELD_CELL_SIZE && static_cast<usize>(position.y) > cell_position.y
		&& static_cast<usize>(position.y) < cell_position.y + FIELD_CELL_SIZE)
	{
		std::optional<rhr::handler::field_data::data*> data = find_data(location);
		if (!data.has_value())
			return location;

		data.value()->set_position(local_position);
		update_guests(location);

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
		local_data.set_position(local_position);
		local_data.set_location(moved_cell_location);

		remove_guests(location);
		pop_data(location);
		push_data(moved_cell_location, local_data);

		return local_data.get_location();
	}
}

void rhr::handler::field::update_field_size(const rhr::handler::field_data::location& location, glm::vec<2, i32> size)
{
	std::optional<rhr::handler::field_data::data*> data = find_data(location);
	if (!data.has_value())
		return;

	data.value()->set_size(size);
	data.value()->copy_to_host();
	update_guests(location);
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
		{
			//			cap::logger::info("first find");
			return data;
		}
	}

	//	cap::logger::info("nothing first find");
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
		glm::vec<2, i32> cell_pixel_size	 = cell.value()->at(layer)[i].get_size();

		if (position.x >= cell_pixel_position.x && position.x < cell_pixel_position.x + cell_pixel_size.x && position.y >= cell_pixel_position.y
			&& position.y < cell_pixel_position.y + cell_pixel_size.y)
			return &(cell.value()->at(layer)[i]);
	}

	return std::nullopt;
}

std::optional<rhr::handler::field_data::data*> rhr::handler::field::find_data(const rhr::handler::field_data::location& location)
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

std::optional<std::vector<std::vector<rhr::handler::field_data::data>>*> rhr::handler::field::get_cell(const glm::vec<2, usize>& cell_location)
{
	if (cell_location.x >= m_cell_table_size.x || cell_location.y >= m_cell_table_size.y)
		return std::nullopt;

	return &(m_cell_table[cell_location.x][cell_location.y]);
}

void rhr::handler::field::push_data(const rhr::handler::field_data::location& location, const rhr::handler::field_data::data& data)
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

void rhr::handler::field::pop_data(const rhr::handler::field_data::location& location)
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

	cap::logger::warn(cap::logger::level::SYSTEM, "rhr::handler::field::pop_data failed to remove data from cell");
}

glm::vec<2, usize> rhr::handler::field::calculate_cell_position(const glm::vec<2, usize>& position)
{
	return {
		static_cast<usize>(glm::floor(static_cast<f32>(position.x) / static_cast<f32>(FIELD_CELL_SIZE))),
		static_cast<usize>(glm::floor(static_cast<f32>(position.y) / static_cast<f32>(FIELD_CELL_SIZE)))};
}

void rhr::handler::field::update_cursor()
{
	m_rectangle_cursor->set_position_local_physical(
		glm::vec<2, i32>(m_cursor_position.x - 1, m_cursor_position.y) - rhr::stack::plane::primary_plane->get_position_virtual_offset() + *rhr::stack::plane::primary_plane->get_offset(), true);
}

void rhr::handler::field::update_highlight()
{
	m_rectangle_highlight->set_position_local_physical(
		glm::vec<2, i32>(m_mouse_drag_start_position.x, m_mouse_drag_start_position.y + 2) - rhr::stack::plane::primary_plane->get_position_virtual_offset() + *rhr::stack::plane::primary_plane->get_offset(), true);
	m_rectangle_highlight->set_size_local({m_mouse_drag_end_position.x - m_mouse_drag_start_position.x, rhr::stack::block::height_content - 4}, true);
}

void rhr::handler::field::reset_all()
{
	m_mouse_down				= false;
	m_mouse_down_data			= nullptr;
	m_mouse_drag				= false;
	m_mouse_drag_start			= 0;
	m_mouse_drag_end			= 0;
	m_mouse_drag_start_position = {0, 0};
	m_mouse_drag_end_position	= {0, 0};

	m_cursor		  = false;
	m_cursor_position = {0, 0};
	m_cursor_idx	  = 0;
}

void rhr::handler::field::process_highlight(bool copy)
{
	if (!m_cursor || !m_mouse_drag)
		return;

	// TODO: segfault possible, later on make this use the weakptr so no segfault possibility.
	if (auto lock = m_mouse_down_data->get_text_field().lock())
	{
		if (copy)
			cap::logger::warn(cap::logger::level::SYSTEM, "copy not implemented");
		else
		{
			if (m_mouse_drag_end > m_mouse_drag_start)
				lock->remove_string(m_mouse_drag_start, m_mouse_drag_end - m_mouse_drag_start);
			else
				lock->remove_string(m_mouse_drag_end, m_mouse_drag_start - m_mouse_drag_end);
		}

		m_cursor_idx = glm::min(m_mouse_drag_start, m_mouse_drag_end);

		m_mouse_drag	   = false;
		m_mouse_drag_start = m_cursor_idx;
		m_mouse_drag_end   = m_cursor_idx;

		std::optional<glm::vec<2, i32>> cursor_position = lock->get_index_position(m_cursor_idx);
		if (!cursor_position.has_value())
			return;

		m_cursor_position			= cursor_position.value();
		m_mouse_drag_start_position = cursor_position.value();
		m_mouse_drag_end_position	= cursor_position.value();

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
			m_mouse_drag_end		  = static_cast<i64>(m_mouse_drag_end) + idx;
			m_mouse_drag_end_position = cursor_position.value();
		}
		else
		{
			m_mouse_drag_start			= static_cast<i64>(m_mouse_drag_start) + idx;
			m_mouse_drag_start_position = cursor_position.value();

			m_mouse_drag_end		  = static_cast<i64>(m_mouse_drag_end) + idx;
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
			m_mouse_drag_end		  = idx;
			m_mouse_drag_end_position = cursor_position.value();
		}
		else
		{
			m_mouse_drag_start			= idx;
			m_mouse_drag_start_position = cursor_position.value();

			m_mouse_drag_end		  = idx;
			m_mouse_drag_end_position = cursor_position.value();
		}

		update_cursor();
		update_highlight();
	}
}

void rhr::handler::field::update_guests(const rhr::handler::field_data::location& location)
{
	std::optional<rhr::handler::field_data::data*> data = find_data(location);
	if (!data.has_value())
		return;

	glm::vec<2, usize> cell_first = calculate_cell_position(data.value()->get_position());
	glm::vec<2, usize> cell_last  = calculate_cell_position(data.value()->get_position() + data.value()->get_host_size());

	if (cell_first.x > cell_last.x || cell_first.y > cell_last.y)
	{
		cap::logger::warn(cap::logger::level::SYSTEM, "failed to update guests of field data location. host cell is after the guests cells.");
		return;
	}

	std::optional<std::vector<rhr::handler::field_data::location>*> guests = data.value()->host_get_locations();
	if (!guests.has_value())
		return;

	// remove unused guests

	for (usize i = 0; i < guests.value()->size(); i++)
	{
		rhr::handler::field_data::location& local_guest = guests.value()->at(i);
		if (local_guest.get_cell().x < cell_first.x || local_guest.get_cell().y < cell_first.y || local_guest.get_cell().x > cell_last.x || local_guest.get_cell().y > cell_last.y)
		{
#if DEBUG_FIELDS
			cap::logger::info("popping " + std::to_string(local_guest.get_cell().x) + ", " + std::to_string(local_guest.get_cell().y));
#endif
			pop_data(local_guest);
			guests.value()->erase(guests.value()->begin() + i);
			i--;
		}
	}

	for (usize y = 0; y < cell_last.y - cell_first.y + 1; y++)
	{
		for (usize x = 0; x < cell_last.x - cell_first.x + 1; x++)
		{
			bool found = false;

			// copy so the vector can be changed without breaking pointers.
			rhr::handler::field_data::location found_location({0, 0}, 0, 0);

			for (auto& guest : *guests.value())
			{
				if (guest.get_cell().x == location.get_cell().x + x && guest.get_cell().y == location.get_cell().y + y)
				{
#if DEBUG_FIELDS
					cap::logger::info("keeping " + std::to_string(guest.get_cell().x) + ", " + std::to_string(guest.get_cell().y));
#endif
					found_location = guest;
					found		   = true;
					break;
				}
			}

			glm::vec<2, usize> local_position;
			glm::vec<2, usize> local_size;

			glm::vec<2, usize> local_low;
			glm::vec<2, usize> local_high;

			// initialize local lows and highs for horizontal

			if (cell_last.x - cell_first.x == 0)
			{
				local_low.x	 = data.value()->get_position().x;
				local_high.x = data.value()->get_position().x + data.value()->get_host_size().x;
			}
			else
			{
				if (x == cell_last.x - cell_first.x)
				{
					local_low.x	 = ((location.get_cell().x + x) * FIELD_CELL_SIZE);
					local_high.x = data.value()->get_position().x + data.value()->get_host_size().x;
				}
				else if (x == 0)
				{
					local_low.x	 = data.value()->get_position().x;
					local_high.x = ((location.get_cell().x + 1) * FIELD_CELL_SIZE);
				}
				else
				{
					local_low.x	 = (location.get_cell().x + x) * FIELD_CELL_SIZE;
					local_high.x = (location.get_cell().x + x + 1) * FIELD_CELL_SIZE;
				}
			}

			// initialize local lows and highs for vertical

			if (cell_last.y - cell_first.y == 0)
			{
				local_low.y	 = data.value()->get_position().y;
				local_high.y = data.value()->get_position().y + data.value()->get_host_size().y;
			}
			else
			{
				if (y == cell_last.y - cell_first.y)
				{
					local_low.y	 = ((location.get_cell().y + y) * FIELD_CELL_SIZE);
					local_high.y = data.value()->get_position().y + data.value()->get_host_size().y;
				}
				else if (y == 0)
				{
					local_low.y	 = data.value()->get_position().y;
					local_high.y = ((location.get_cell().y + 1) * FIELD_CELL_SIZE);
				}
				else
				{
					local_low.y	 = (location.get_cell().y + y) * FIELD_CELL_SIZE;
					local_high.y = (location.get_cell().y + y + 1) * FIELD_CELL_SIZE;
				}
			}

			if (found)
			{
#if DEBUG_FIELDS
				cap::logger::info("updating guest cell " + std::to_string(found_location.get_cell().x) + ", " + std::to_string(found_location.get_cell().y));
#endif
				std::optional<rhr::handler::field_data::data*> guest_data = find_data(found_location);
				if (!guest_data.has_value())
					return;

				guest_data.value()->set_size(local_high - local_low);
				guest_data.value()->set_position(local_low);
			}
			else if (x == 0 && y == 0)
			{
#if DEBUG_FIELDS
				cap::logger::info("size is now " + std::to_string(data.value()->get_size().x) + ", " + std::to_string(data.value()->get_size().y));
#endif
				data.value()->set_size(local_high - local_low);
				data.value()->set_position(local_low);
			}
			else
			{
				usize guest_idx = m_idx++;
				rhr::handler::field_data::location guest_location({location.get_cell().x + x, location.get_cell().y + y}, guest_idx, location.get_layer());
				rhr::handler::field_data::data guest_data(guest_idx, local_low/*, data.value()->get_plane_offset()*/, local_high - local_low, std::move(data.value()->get_text_field()), guest_location, false);

#if DEBUG_FIELDS
				cap::logger::info("adding cell " + std::to_string(guest_location.get_cell().x) + ", " + std::to_string(guest_location.get_cell().y));
#endif
				guests.value()->push_back(guest_location);
				push_data(guest_location, guest_data);
			}
		}
	}
}

void rhr::handler::field::remove_guests(const rhr::handler::field_data::location& location)
{
	std::optional<rhr::handler::field_data::data*> data = find_data(location);
	if (!data.has_value())
		return;

	glm::vec<2, usize> cell_first = calculate_cell_position(data.value()->get_position());
	glm::vec<2, usize> cell_last  = calculate_cell_position(data.value()->get_position() + data.value()->get_size());

	if (cell_first.x > cell_last.x || cell_first.y > cell_last.y)
	{
		cap::logger::error(cap::logger::level::SYSTEM, "failed to remove guests of field data location. host cell is after the guests cells.");
		return;
	}

	std::optional<std::vector<rhr::handler::field_data::location>*> guests = data.value()->host_get_locations();
	if (!guests.has_value())
		return;

	// remove all guests

	for (usize i = 0; i < guests.value()->size(); i++)
	{
		rhr::handler::field_data::location& local_guest = guests.value()->at(i);
#if DEBUG_FIELDS
		cap::logger::info("poping " + std::to_string(local_guest.get_cell().x) + ", " + std::to_string(local_guest.get_cell().y));
#endif
		pop_data(local_guest);
		guests.value()->erase(guests.value()->begin() + i);
		i--;
	}
}
