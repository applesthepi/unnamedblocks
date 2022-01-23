#include "text.hpp"

#include "rhr/registries/char_texture.hpp"
#include "rhr/rendering/renderer.hpp"
#include "rhr/rendering/vertex.hpp"
#include "rhr/stacking/block.hpp"
#include "rhr/stacking/plane.hpp"

#define EDGE_CLICK_OVERHANG 5

static i64 DEBUG_COUNT = 0;

rhr::render::object::text::text(rhr::registry::char_texture::texture_type texture_type, u16 font_size, std::function<void()>* function_update, bool read_only, bool force_register)
	: i_dicolorable(cap::color().from_normalized({0.0f, 0.0f, 0.0f, 1.0f}), cap::color().from_u8({25, 25, 25, 255}))
	, m_depth(10)
	, m_render_object_background(std::make_shared<rhr::render::object::object>(true))
	, m_render_object_text(std::make_shared<rhr::render::object::object>(true))
	, m_enable_background(true)
	, m_function_update(function_update)
	, m_read_only(read_only)
	, m_mouse_button(nullptr)
	, m_font_size(font_size)
	, m_registered(false)
	, m_force_register(force_register)
	, m_texture_type(texture_type)
{
	m_render_object_background->set_weak(m_render_object_background);
	m_render_object_text->set_weak(m_render_object_text);
	m_render_object_text->set_texture_char(texture_type, m_font_size);
	m_render_object_text->set_enabled(false);
}

rhr::render::object::text::~text() { unregister_field(); }

void rhr::render::object::text::set_weak_field(std::weak_ptr<rhr::render::interfaces::i_field>&& weak_field)
{
	m_weak_field = weak_field;

	//	if (m_force_register)
	//		register_field();
}

void rhr::render::object::text::set_text(const std::string& text)
{
	if (text.size() == 0)
	{
		m_text.clear();
		m_render_object_text->set_enabled(false);
		update_size();
		register_field();
	}
	else
	{
		m_text = text;
		m_render_object_text->set_enabled(true);
		update_size();
		register_field();
	}

	mark_dirty();
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

void rhr::render::object::text::enable_background(bool enable) { m_enable_background = enable; }

std::optional<usize> rhr::render::object::text::pick_index(glm::vec<2, i32> position, bool ignore_y)
{
	if (ignore_y)
	{
		i32 field_position = get_position_virtual_absolute().x;
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

		if (delta_position.x < -EDGE_CLICK_OVERHANG || delta_position.x > get_size_local().x + EDGE_CLICK_OVERHANG || delta_position.y < 0 || delta_position.y > get_size_local().y)
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

	glm::vec<2, i32> field_position = get_position_virtual_absolute();

	if (idx > 0)
		field_position.x += static_cast<i32>(m_char_offsets[idx - 1]);
	else
		field_position.x += m_padding;

	return field_position;
}

usize rhr::render::object::text::get_index_count() { return m_text.size(); }

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

void rhr::render::object::text::update_size()
{
	m_char_offsets.clear();
	m_char_contacts.clear();

	f32 running_char_offsets  = static_cast<f32>(m_padding);
	f32 running_char_contacts = static_cast<f32>(m_padding);

	for (usize i = 0; i < m_text.size(); i++)
	{
		i16 char_width = rhr::registry::char_texture::get_texture_map(m_font_size)->map[m_texture_type].char_map[m_text[i]].advance.x >> 6;

		running_char_offsets += static_cast<f32>(char_width);
		running_char_contacts += static_cast<f32>(char_width) / 2.0f;

		if (i > 0)
		{
			i16 last_char_width = rhr::registry::char_texture::get_texture_map(m_font_size)->map[m_texture_type].char_map[m_text[i - 1]].advance.x >> 6;
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

void rhr::render::object::text::ui_transform_update()
{
	const glm::vec<2, i32>& position_physical = get_position_physical_absolute();
	const glm::vec<2, i32>& position_virtual  = get_position_virtual_absolute();

	m_render_object_background->set_super_position({static_cast<f64>(position_physical.x), static_cast<f64>(position_physical.y), static_cast<f64>(m_depth)});
	m_render_object_text->set_super_position({static_cast<f64>(position_physical.x), static_cast<f64>(position_physical.y), static_cast<f64>(m_depth) - 0.1});

	if (!m_read_only && !m_registered && position_virtual.x >= 0 && position_virtual.y >= 0)
		register_field();

	if (!m_read_only && m_registered)
	{
		if (position_virtual.x < 0 || position_virtual.y < 0)
			unregister_field();
		else
			m_location = rhr::stack::plane::primary_plane->get_field().update_field_position(m_location.value(), position_virtual);
	}

	mark_dirty();
}

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
	//	update_size();

	const glm::vec<2, i32>& size_local = get_size_local();

	{
		rhr::render::vertex vertices[4];
		u32 indices[6];

		vertices[0] = rhr::render::vertex({0.0f, 0.0f, 0.0f}, m_color_secondary.get_normalized(), {0.0f, 0.0f});
		vertices[1] = rhr::render::vertex({static_cast<f32>(size_local.x), 0.0f, 0.0f}, m_color_secondary.get_normalized(), {1.0f, 0.0f});
		vertices[2] = rhr::render::vertex({static_cast<f32>(size_local.x), static_cast<f32>(size_local.y), 0.0f}, m_color_secondary.get_normalized(), {1.0f, 1.0f});
		vertices[3] = rhr::render::vertex({0.0f, static_cast<f32>(size_local.y), 0.0f}, m_color_secondary.get_normalized(), {0.0f, 1.0f});

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
			rhr::registry::char_texture::char_data char_data = rhr::registry::char_texture::get_texture_map(m_font_size)->map[m_texture_type].char_map[m_text[i]];
			f32 y_offset									 = static_cast<f32>(m_font_size) - static_cast<f32>(char_data.offset.y) - static_cast<f32>(rhr::stack::block::padding);

			vertices[i * 4 + 0] =
				rhr::render::vertex({static_cast<f32>(running_x + char_data.offset.x), y_offset, 0.0f}, m_color_primary.get_normalized(), {char_data.first.x, char_data.first.y});
			vertices[i * 4 + 1] = rhr::render::vertex(
				{static_cast<f32>(running_x + char_data.offset.x + char_data.size.x), y_offset, 0.0f}, m_color_primary.get_normalized(), {char_data.second.x, char_data.first.y});
			vertices[i * 4 + 2] = rhr::render::vertex(
				{static_cast<f32>(running_x + char_data.offset.x + char_data.size.x), static_cast<f32>(char_data.size.y) + y_offset, 0.0f},
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
		set_size_local({(running_x + static_cast<f32>(m_padding)), rhr::stack::block::height_content}, false);
	}
	else
		set_size_local({(static_cast<f32>(m_padding) * 2.0f), rhr::stack::block::height_content}, false);

	if (!m_read_only && m_registered)
		rhr::stack::plane::primary_plane->get_field().update_field_size(m_location.value(), size_local);
}

void rhr::render::object::text::ui_frame_update(f64 delta_time) {}

const std::string& rhr::render::object::text::get_text() { return m_text; }

void rhr::render::object::text::mouse_button(glm::vec<2, i32> position, f32 scroll, MouseOperation operation, MouseButton button)
{
	if (m_mouse_button != nullptr)
		m_mouse_button(position, scroll, operation, button);
}

void rhr::render::object::text::set_mouse_button(std::function<void(glm::vec<2, i32> position, f32 scroll, MouseOperation operation, MouseButton button)>& mouse_button)
{
	m_mouse_button = mouse_button;
}

void rhr::render::object::text::post_color_update() { mark_dirty(); }

void rhr::render::object::text::set_font_size(u16 font_size)
{
	m_font_size = font_size;
	m_render_object_text->set_texture_char_size(font_size);

	update_size();
	mark_dirty();
}

void rhr::render::object::text::register_field()
{
	if (!m_registered)
	{
		m_registered = true;

		if (!m_read_only)
		{
			m_location =
				rhr::stack::plane::primary_plane->get_field().register_field(m_weak_field, get_position_virtual_absolute(), get_size_local(), InputHandler::BullishLayerArguments);
		}
	}
}

void rhr::render::object::text::unregister_field()
{
	if (!m_read_only && m_registered)
	{
		m_registered = false;
		rhr::stack::plane::primary_plane->get_field().unregister_field(m_location.value());
	}
}
