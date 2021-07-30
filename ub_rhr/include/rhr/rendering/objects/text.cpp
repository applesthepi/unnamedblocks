#include "text.hpp"

#include "rhr/rendering/renderer.hpp"
#include "rhr/rendering/vertex.hpp"
#include "rhr/stacking/block.hpp"
#include "rhr/registries/char_texture.hpp"

#define EDGE_CLICK_OVERHANG 5

rhr::render::object::text::text(rhr::registry::char_texture::texture_type texture_type)
	: i_dicolorable(cap::color().from_normalized({ 0.0f, 0.0f, 0.0f, 1.0f }), cap::color().from_u8({ 25, 25, 25, 255 }))
	, i_enableable(true)
	, m_depth(10)
	, m_render_object_background(std::make_shared<rhr::render::object::object>(true))
	, m_render_object_text(std::make_shared<rhr::render::object::object>(true))
	, m_enable_background(true)
{
	m_render_object_background->set_weak(m_render_object_background);
	m_render_object_text->set_weak(m_render_object_text);
	m_render_object_text->set_texture_char(texture_type);
	m_render_object_text->set_enabled(false);
}

void rhr::render::object::text::set_text(const std::string& text)
{
	if (text.size() == 0)
	{
		m_text.clear();
		m_char_widths.clear();
		m_render_object_text->set_enabled(false);
		m_size = { 10, rhr::stack::block::height - (rhr::stack::block::padding * 2) };
	}
	else
	{
		m_text = text;
		m_render_object_text->set_enabled(true);
		update_size();
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

void rhr::render::object::text::enable_background(bool enable)
{
	m_enable_background = enable;
}

void rhr::render::object::text::update_size()
{
	i32 running_x = m_padding;
	m_char_widths.clear();

	for (usize i = 0; i < m_text.size(); i++)
	{
		i16 char_width = rhr::registry::char_texture::texture_map[rhr::registry::char_texture::texture_type::LIGHT_NORMAL].char_map[m_text[i]].advance.x >> 6;
		running_x += static_cast<i32>(char_width);
		m_char_widths.push_back(running_x);
	}
	
	m_size = { running_x + m_padding, rhr::stack::block::height - (rhr::stack::block::padding * 2) };
}

void rhr::render::object::text::on_render()
{
	if (m_enabled)
	{
		if (m_enable_background)
		{
			vkCmdBindPipeline(rhr::render::renderer::active_command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, rhr::render::renderer::ui_pipeline);
			m_render_object_background->render();
		}

		vkCmdBindPipeline(rhr::render::renderer::active_command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, rhr::render::renderer::ui_texture_pipeline);
		m_render_object_text->render();
	}
}

void rhr::render::object::text::on_update_buffers()
{
	clear_dirty();

	{
		rhr::render::vertex vertices[4];
		u32 indices[6];

		vertices[0] = rhr::render::vertex({ 0.0f, 0.0f, 0.0f }, m_color_secondary.get_normalized(), { 0.0f, 0.0f });
		vertices[1] = rhr::render::vertex({ static_cast<f32>(m_size.x), 0.0f, 0.0f }, m_color_secondary.get_normalized(), { 1.0f, 0.0f });
		vertices[2] = rhr::render::vertex({ static_cast<f32>(m_size.x), static_cast<f32>(m_size.y), 0.0f }, m_color_secondary.get_normalized(), { 1.0f, 1.0f });
		vertices[3] = rhr::render::vertex({ 0.0f, static_cast<f32>(m_size.y), 0.0f }, m_color_secondary.get_normalized(), { 0.0f, 1.0f });

		indices[0] = 1;
		indices[1] = 0;
		indices[2] = 2;
		indices[3] = 2;
		indices[4] = 0;
		indices[5] = 3;

		m_render_object_background->update_vertices(vertices, 4, indices, 6, true);
	}

	if (m_render_object_text->get_enabled())
	{
		rhr::render::vertex* vertices = (rhr::render::vertex*)alloca(sizeof(rhr::render::vertex) * m_text.size() * 4);
		u32* indices = (u32*)alloca(sizeof(u32) * m_text.size() * 6);

		i32 running_x = m_padding;

		for (usize i = 0; i < m_text.size(); i++)
		{
			rhr::registry::char_texture::char_data char_data = rhr::registry::char_texture::texture_map[rhr::registry::char_texture::texture_type::BOLD_NORMAL].char_map[m_text[i]];
			f32 y_offset = static_cast<f32>(rhr::stack::block::height_content) + (-1.0f * static_cast<f32>(char_data.offset.y)) - static_cast<f32>(rhr::stack::block::padding);

			vertices[i * 4 + 0] = rhr::render::vertex({ static_cast<f32>(running_x + char_data.offset.x), y_offset, 0.0f }, m_color_primary.get_normalized(), { char_data.first.x, char_data.first.y });
			vertices[i * 4 + 1] = rhr::render::vertex({ static_cast<f32>(running_x + char_data.offset.x + char_data.size.x), y_offset, 0.0f }, m_color_primary.get_normalized(), { char_data.second.x, char_data.first.y });
			vertices[i * 4 + 2] = rhr::render::vertex({ static_cast<f32>(running_x + char_data.offset.x + char_data.size.x), static_cast<f32>(char_data.size.y) + y_offset, 0.0f }, m_color_primary.get_normalized(), { char_data.second.x, char_data.second.y });
			vertices[i * 4 + 3] = rhr::render::vertex({ static_cast<f32>(running_x + char_data.offset.x), static_cast<f32>(char_data.size.y) + y_offset, 0.0f }, m_color_primary.get_normalized(), { char_data.first.x, char_data.second.y });

			indices[i * 6 + 0] = i * 4 + 1;
			indices[i * 6 + 1] = i * 4 + 0;
			indices[i * 6 + 2] = i * 4 + 2;
			indices[i * 6 + 3] = i * 4 + 2;
			indices[i * 6 + 4] = i * 4 + 0;
			indices[i * 6 + 5] = i * 4 + 3;

			running_x += char_data.advance.x >> 6;
		}

		m_render_object_text->update_vertices(vertices, m_text.size() * 4, indices, m_text.size() * 6, true);
		m_size = { running_x + m_padding, rhr::stack::block::height - (rhr::stack::block::padding * 2) };
	}
}

void rhr::render::object::text::on_reload_swap_chain()
{
	m_render_object_background->reload_swap_chain();
	m_render_object_text->reload_swap_chain();
}

void rhr::render::object::text::post_position_update()
{
	glm::vec<2, i32> position = m_position + m_super_position;
	m_render_object_background->set_super_position({static_cast<f64>(position.x), static_cast<f64>(position.y), static_cast<f64>(m_depth) });
	m_render_object_text->set_super_position({ static_cast<f64>(position.x), static_cast<f64>(position.y), static_cast<f64>(m_depth) - 0.1 });

	mark_dirty();
}

void rhr::render::object::text::post_size_update()
{
	mark_dirty();
}

std::optional<usize> rhr::render::object::text::pick_index(glm::vec<2, i32> position, bool ignore_y)
{
	if (ignore_y)
	{
		i32 field_position = m_position.x + m_super_position.x;
		i32 delta_position = position.x - field_position;

		if (m_char_widths.size() == 0 ||
			delta_position < -EDGE_CLICK_OVERHANG || delta_position > m_size.x + EDGE_CLICK_OVERHANG)
			return std::nullopt;

		f32 running_x = static_cast<f32>(m_char_widths.front()) / 2.0f;

		for (usize i = 0; i < m_char_widths.size(); i++)
		{
			if (i > 0)
			{
				running_x += static_cast<f32>(m_char_widths[i - 1]) / 2.0f;
				running_x += static_cast<f32>(m_char_widths[i]) / 2.0f;
			}

			if (running_x > static_cast<f32>(delta_position))
				return i;
		}

		return m_char_widths.size();
	}
	else
	{
		glm::vec<2, i32> field_position = m_position + m_super_position;
		glm::vec<2, i32> delta_position = position - field_position;

		if (m_char_widths.size() == 0 ||
			delta_position.x < -EDGE_CLICK_OVERHANG || delta_position.x > m_size.x + EDGE_CLICK_OVERHANG ||
			delta_position.y < 0 || delta_position.y > m_size.y)
			return std::nullopt;

		f32 running_x = static_cast<f32>(m_char_widths.front()) / 2.0f;

		for (usize i = 0; i < m_char_widths.size(); i++)
		{
			if (i > 0)
			{
				running_x += static_cast<f32>(m_char_widths[i - 1]) / 2.0f;
				running_x += static_cast<f32>(m_char_widths[i]) / 2.0f;
			}

			if (running_x > static_cast<f32>(delta_position.x))
				return i;
		}

		return m_char_widths.size();
	}
}

std::optional<glm::vec<2, i32>> rhr::render::object::text::get_index_position(usize idx)
{
	if (m_char_widths.size() == 0 || idx > m_char_widths.size())
		return std::nullopt;

	i32 running_x = 0;

	for (usize i = 0; i < idx; i++)
		running_x += static_cast<i32>(m_char_widths[i]);

	glm::vec<2, i32> field_position = m_position + m_super_position;
	field_position.x += running_x;
	
	return field_position;
}
