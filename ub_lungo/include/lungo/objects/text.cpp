#include <lungo/handlers/panel.hpp>
#include "text.hpp"

#include "lungo/renderer.hpp"

#define EDGE_CLICK_OVERHANG 5

static i64 DEBUG_COUNT = 0;

rhr::render::object::text::text(
	rhr::registry::char_texture::texture_type texture_type,
	u16 font_size,
	bool read_only,
	glm::vec<2, i32>* plane_offset)
	: i_dicolorable(espresso::color().from_normalized({0.0f, 0.0f, 0.0f, 1.0f}), espresso::color().from_u8({25, 25, 25, 255}))
	, m_depth(10)
	, m_render_object_background(std::make_shared<rhr::render::object::object>(true))
	, m_render_object_text(std::make_shared<rhr::render::object::object>(true))
	, m_enable_background(true)
	, m_function_update(nullptr)
	, m_read_only(read_only)
	, m_mouse_button(nullptr)
	, m_font_size(font_size)
	, m_registered(false)
	, m_texture_type(texture_type)
	, m_mouse_callback_idx(0)
	, m_field(nullptr)
{
	m_render_object_text->set_texture_char(texture_type, m_font_size);
	m_render_object_text->set_enabled(false);
}

rhr::render::object::text::~text()
{
	if (!m_read_only)
		unregister_field();
}

void rhr::render::object::text::set_field_handler(rhr::handler::field* field)
{
	if (m_read_only)
		latte::logger::warn(latte::logger::level::EDITOR, __FILE__, __LINE__, "text object does not need field handler when set to read only");

	m_field = field;
}

void rhr::render::object::text::set_update_function(std::function<void()>* function_update)
{
	m_function_update = function_update;
}

void rhr::render::object::text::set_text(const std::string& text)
{
	if (text.empty())
	{
		m_text.clear();
		m_render_object_text->set_enabled(false);
		update_size();
	}
	else
	{
		m_text = text;
		m_render_object_text->set_enabled(true);
		update_size();
	}

	mark_dirty();
}

const std::string& rhr::render::object::text::get_text()
{
	return m_text;
}

void rhr::render::object::text::set_depth(i32 depth)
{
	m_depth = depth;
	mark_dirty();
}

void rhr::render::object::text::set_padding(i32 padding)
{
	m_padding = padding;
	update_size();
	mark_dirty();
}

void rhr::render::object::text::set_font_size(u16 font_size)
{
	m_font_size = font_size;
	m_render_object_text->set_texture_char_size(font_size);

	update_size();
	mark_dirty();
}

void rhr::render::object::text::enable_background(bool enable)
{
	m_enable_background = enable;
}

void rhr::render::object::text::set_mouse_button(std::function<void(rhr::handler::input::mouse_button_data)>& mouse_button)
{
	m_mouse_button = mouse_button;

	if (!m_read_only)
		register_field();
}

std::optional<usize> rhr::render::object::text::pick_index(glm::vec<2, i32> position, bool ignore_y)
{
	if (ignore_y)
	{
		i32 field_position = get_position_virtual_absolute().x + get_static_offset()->x;
		i32 delta_position = position.x - field_position;

		if (/*m_char_contacts.size() == 0 ||*/
			delta_position < -EDGE_CLICK_OVERHANG || delta_position > get_size_local().x + EDGE_CLICK_OVERHANG)
			return std::nullopt;

		// f32 running_x = static_cast<f32>(m_char_widths.front()) / 2.0f;

		if (m_text.empty())
			return 0;

		for (usize i = 0; i < m_char_contacts.size(); i++)
		{
			/*if (i > 0)
			{
				running_x += static_cast<f32>(m_char_widths[i - 1]) / 2.0f;
				running_x += static_cast<f32>(m_char_widths[i]) / 2.0f;
			}*/

			if (static_cast<i32>(m_char_contacts[i]) > delta_position)
				return i;
		}

		return m_char_contacts.size();
	}
	else
	{
		glm::vec<2, i32> field_position = get_position_virtual_absolute();
		glm::vec<2, i32> delta_position = position - field_position;

		//LOG_DEBUG_VEC2(delta_position);

		if (delta_position.x < -EDGE_CLICK_OVERHANG || delta_position.x > get_size_local().x + EDGE_CLICK_OVERHANG
			|| delta_position.y < 0 || delta_position.y > get_size_local().y)
			return std::nullopt;

		// f32 running_x = static_cast<f32>(m_char_widths.front()) / 2.0f;

		if (m_text.empty())
			return 0;

		for (usize i = 0; i < m_char_contacts.size(); i++)
		{
			/*if (i > 0)
			{
				running_x += static_cast<f32>(m_char_widths[i - 1]) / 2.0f;
				running_x += static_cast<f32>(m_char_widths[i]) / 2.0f;
			}*/

			if (static_cast<i32>(m_char_contacts[i]) > delta_position.x)
				return i;
		}

		return m_char_contacts.size();
	}
}

std::optional<glm::vec<2, i32>> rhr::render::object::text::get_index_position(usize idx)
{
	if (/*m_char_offsets.size() == 0 ||*/ idx > m_char_offsets.size())
		return std::nullopt;

	glm::vec<2, i32> field_position = get_position_physical_absolute();

	if (idx > 0)
		field_position.x += static_cast<i32>(m_char_offsets[idx - 1]);
	else
		field_position.x += m_padding;

	return field_position;
}

usize rhr::render::object::text::get_index_count()
{
	return m_text.size();
}

void rhr::render::object::text::insert_char(char charactor, usize idx)
{
	if (idx > m_text.size())
		return;

	m_text.insert(m_text.begin() + idx, charactor);

	update_size();
	if (m_function_update != nullptr)
		(*m_function_update)();

	mark_dirty();
}

void rhr::render::object::text::insert_string(const std::string& string, usize idx)
{
	if (idx > m_text.size())
		return;

	m_text.insert(idx, string);

	update_size();
	if (m_function_update != nullptr)
		(*m_function_update)();
	mark_dirty();
}

bool rhr::render::object::text::remove_char(usize idx)
{
	if (idx >= m_text.size())
		return false;

	m_text.erase(m_text.begin() + idx);

	update_size();
	if (m_function_update != nullptr)
		(*m_function_update)();
	mark_dirty();

	return true;
}

bool rhr::render::object::text::remove_string(usize idx, usize size)
{
	if (idx >= m_text.size() || idx + size > m_text.size())
		return false;

	m_text.erase(idx, size);

	update_size();
	if (m_function_update != nullptr)
		(*m_function_update)();
	mark_dirty();

	return true;
}

void rhr::render::object::text::ui_initialize()
{

}

void rhr::render::object::text::ui_transform_update(i_ui::transform_update_spec transform_update_spec)
{
	const glm::vec<2, i32>& position_physical = get_position_physical_absolute();
	const glm::vec<2, i32>& position_virtual  = get_position_virtual_absolute();

	bool position_update = transform_update_spec & i_ui::transform_update_spec_position;
	bool size_update	 = transform_update_spec & i_ui::transform_update_spec_size;

	if (position_update)
	{
		m_render_object_background->set_position(
			{static_cast<f64>(position_physical.x), static_cast<f64>(position_physical.y), static_cast<f64>(m_depth)});
		m_render_object_text->set_position(
			{static_cast<f64>(position_physical.x),
				static_cast<f64>(position_physical.y),
				static_cast<f64>(m_depth) - 0.1});
	}

	// TODO: fields fix
	if (!m_read_only && !m_registered && position_virtual.x >= 0 && position_virtual.y >= 0)
		register_field();

	if (!m_read_only && m_registered)
	{
		if (position_virtual.x < 0 || position_virtual.y < 0)
			unregister_field();
		else
			m_location = m_field->update_field_position(
				m_location.value(), position_virtual, position_physical);

	}
}

void rhr::render::object::text::ui_frame_update(f64 delta_time)
{}

void rhr::render::object::text::ui_render()
{
	if (get_enabled())
	{
		if (m_enable_background)
			m_render_object_background->render();

		m_render_object_text->render();
	}
}

void rhr::render::object::text::ui_reload_swap_chain()
{
	m_render_object_background->reload_swap_chain();
	m_render_object_text->reload_swap_chain();
}

void rhr::render::object::text::ui_update_buffers()
{
	const glm::vec<2, i32>& size_local = get_size_local();

	{
		rhr::render::vertex vertices[4];
		u32 indices[6];

		vertices[0] = rhr::render::vertex({0.0f, 0.0f, 0.0f}, m_color_secondary.get_normalized(), {0.0f, 0.0f});
		vertices[1] = rhr::render::vertex(
			{static_cast<f32>(size_local.x), 0.0f, 0.0f}, m_color_secondary.get_normalized(), {1.0f, 0.0f});
		vertices[2] = rhr::render::vertex(
			{static_cast<f32>(size_local.x), static_cast<f32>(size_local.y), 0.0f},
			m_color_secondary.get_normalized(),
			{1.0f, 1.0f});
		vertices[3] = rhr::render::vertex(
			{0.0f, static_cast<f32>(size_local.y), 0.0f}, m_color_secondary.get_normalized(), {0.0f, 1.0f});

		indices[0] = 1;
		indices[1] = 0;
		indices[2] = 2;
		indices[3] = 2;
		indices[4] = 0;
		indices[5] = 3;

		m_render_object_background->update_vertices(vertices, 4, indices, 6, true);
	}

	m_render_object_text->set_enabled(m_text.size() > 0);

	if (m_render_object_text->get_enabled())
	{
		rhr::render::vertex* vertices = (rhr::render::vertex*)alloca(sizeof(rhr::render::vertex) * m_text.size() * 4);
		u32* indices				  = (u32*)alloca(sizeof(u32) * m_text.size() * 6);
		f32 running_x				  = static_cast<f32>(m_padding);

		for (usize i = 0; i < m_text.size(); i++)
		{
			rhr::registry::char_texture::char_data char_data =
				rhr::registry::char_texture::get()->get_texture_map(m_font_size)->map[m_texture_type].char_map[m_text[i]];
			f32 y_offset = static_cast<f32>(m_font_size) - static_cast<f32>(char_data.offset.y)
				- static_cast<f32>(BLOCK_PADDING);

			vertices[i * 4 + 0] = rhr::render::vertex(
				{static_cast<f32>(running_x + char_data.offset.x), y_offset, 0.0f},
				m_color_primary.get_normalized(),
				{char_data.first.x, char_data.first.y});
			vertices[i * 4 + 1] = rhr::render::vertex(
				{static_cast<f32>(running_x + char_data.offset.x + char_data.size.x), y_offset, 0.0f},
				m_color_primary.get_normalized(),
				{char_data.second.x, char_data.first.y});
			vertices[i * 4 + 2] = rhr::render::vertex(
				{static_cast<f32>(running_x + char_data.offset.x + char_data.size.x),
					static_cast<f32>(char_data.size.y) + y_offset,
					0.0f},
				m_color_primary.get_normalized(),
				{char_data.second.x, char_data.second.y});
			vertices[i * 4 + 3] = rhr::render::vertex(
				{static_cast<f32>(running_x + char_data.offset.x), static_cast<f32>(char_data.size.y) + y_offset, 0.0f},
				m_color_primary.get_normalized(),
				{char_data.first.x, char_data.second.y});

			indices[i * 6 + 0] = i * 4 + 1;
			indices[i * 6 + 1] = i * 4 + 0;
			indices[i * 6 + 2] = i * 4 + 2;
			indices[i * 6 + 3] = i * 4 + 2;
			indices[i * 6 + 4] = i * 4 + 0;
			indices[i * 6 + 5] = i * 4 + 3;

			running_x += static_cast<f32>(char_data.advance.x >> 6);
		}

		m_render_object_text->update_vertices(vertices, m_text.size() * 4, indices, m_text.size() * 6, true);
		set_size_local({(running_x + static_cast<f32>(m_padding)), BLOCK_HEIGHT_CONTENT}, false);
	}
	else
		set_size_local({(static_cast<f32>(m_padding) * 2.0f), BLOCK_HEIGHT_CONTENT}, false);

	// TODO: fields fix
	if (!m_read_only && m_registered)
		m_field->update_field_size(m_location.value(), size_local);
}

void rhr::render::object::text::ui_chain_update_buffers()
{}

void rhr::render::object::text::ui_static_offset_update()
{
	m_render_object_background->set_offset(get_static_offset());
	m_render_object_text->set_offset(get_static_offset());
}

void rhr::render::object::text::ui_serialize(latte::serializer::node& node)
{

}

void rhr::render::object::text::ui_deserialize(latte::serializer::node& node)
{

}

void rhr::render::object::text::post_color_update()
{
	mark_dirty();
}

void rhr::render::object::text::update_size()
{
	m_char_offsets.clear();
	m_char_contacts.clear();

	f32 running_char_offsets  = static_cast<f32>(m_padding);
	f32 running_char_contacts = static_cast<f32>(m_padding);

	for (usize i = 0; i < m_text.size(); i++)
	{
		i16 char_width =
			rhr::registry::char_texture::get()->get_texture_map(m_font_size)->map[m_texture_type].char_map[m_text[i]].advance.x
				>> 6;

		running_char_offsets += static_cast<f32>(char_width);
		running_char_contacts += static_cast<f32>(char_width) / 2.0f;

		if (i > 0)
		{
			i16 last_char_width = rhr::registry::char_texture::get()->get_texture_map(m_font_size)
				->map[m_texture_type]
				.char_map[m_text[i - 1]]
				.advance.x
				>> 6;
			running_char_contacts += static_cast<f32>(last_char_width) / 2.0f;
		}

		m_char_offsets.push_back(static_cast<i16>(running_char_offsets));
		m_char_contacts.push_back(static_cast<i16>(running_char_contacts));
	}

	if (m_text.empty())
	{
		m_char_offsets.push_back(static_cast<i16>(running_char_offsets));
		m_char_contacts.push_back(static_cast<i16>(running_char_contacts));
	}

	set_size_local({static_cast<i32>(running_char_offsets) + m_padding, m_font_size}, false);
}

void rhr::render::object::text::register_field()
{
	if (m_registered)
		return;

	if (m_mouse_button == nullptr)
	{
		latte::logger::error(latte::logger::level::EDITOR, __FILE__, __LINE__, "failed to register mouse callbacks for text object because no callback is given");
		return;
	}

	if (m_field == nullptr)
	{
		latte::logger::error(latte::logger::level::EDITOR, __FILE__, __LINE__, "failed to register field of text because field was never set");
		return;
	}

	m_registered = true;
	m_mouse_callback_idx = rhr::handler::input::register_mouse_callback(m_mouse_button, nullptr);
	m_location = m_field->register_field(this, get_position_virtual_absolute(), get_position_physical_absolute(), get_size_local(), rhr::render::renderer::depth_ui_text);
}

void rhr::render::object::text::unregister_field()
{
	if (!m_registered)
		return;

	m_registered = false;
	rhr::handler::input::unregister_mouse_callback(m_mouse_callback_idx);
	m_field->unregister_field(m_location.value());
}
