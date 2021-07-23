#include "rectangle.hpp"

#include "rhr/rendering/renderer.hpp"
#include "rhr/rendering/vertex.hpp"

rhr::render::object::rectangle::rectangle()
	: i_enableable(true)
	, i_colorable(cap::color().from_normalized({ 0.0f, 0.0f, 0.0f, 1.0f }))
	, m_depth(10)
	, m_has_color(false)
	, m_has_texture(false)
	, m_in_bounds(false)
	, m_render_object(std::make_shared<rhr::render::object::object>(true))
	, m_use_size({ 0, 0 })
	, m_texture_type(rhr::render::object::object::texture_type::CUSTOM)
{
	m_render_object->set_weak(m_render_object);
}

void rhr::render::object::rectangle::set_texture(const std::string& texture)
{
	m_has_texture = true;
	m_texture = texture;
	m_texture_type = rhr::render::object::object::texture_type::CUSTOM;
	m_render_object->set_texture(texture);

	mark_dirty();
}

void rhr::render::object::rectangle::set_texture(rhr::render::object::object::texture_type type)
{
	m_has_texture = true;
	m_texture.clear();
	m_texture_type = rhr::render::object::object::texture_type::TEXT_SHEET;
	m_render_object->set_texture(type);

	mark_dirty();
}

void rhr::render::object::rectangle::set_depth(i32 depth)
{
	m_depth = depth;
	mark_dirty();
}

void rhr::render::object::rectangle::on_render()
{
	if (m_enabled && m_in_bounds)
		m_render_object->render();
}

void rhr::render::object::rectangle::on_update_buffers()
{
	std::vector<Vertex> vertices;
	std::vector<u32> indices;

	vertices.reserve(4);
	indices.reserve(6);

	glm::vec<2, i32> position = m_position + m_super_position;
	glm::vec<3, f32> color = { m_color.get_normalized().r, m_color.get_normalized().g, m_color.get_normalized().b };

	// TODO: remove
	//std::cout << m_SuperOffset.x << ", " << m_SuperOffset.y << std::endl;

	Vertex v0 = Vertex({ static_cast<float>(position.x), static_cast<float>(position.y), static_cast<i32>(m_depth) * -1 }, color, { 0.0f, 0.0f });
	Vertex v1 = Vertex({ static_cast<float>(position.x + m_use_size.x), static_cast<float>(position.y), static_cast<i32>(m_depth) * -1 }, color, { 1.0f, 0.0f });
	Vertex v2 = Vertex({ static_cast<float>(position.x + m_use_size.x), static_cast<float>(position.y + m_use_size.y), static_cast<i32>(m_depth) * -1 }, color, { 1.0f, 1.0f });
	Vertex v3 = Vertex({ static_cast<float>(position.x), static_cast<float>(position.y + m_use_size.y), static_cast<i32>(m_depth) * -1 }, color, { 0.0f, 1.0f });

	vertices.push_back(v0);
	vertices.push_back(v1);
	vertices.push_back(v2);
	vertices.push_back(v3);

	indices.push_back(1);
	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(2);
	indices.push_back(0);
	indices.push_back(3);

	m_render_object->update_vertices(&vertices, &indices, true);
}

void rhr::render::object::rectangle::on_reload_swap_chain()
{
	m_render_object->reload_swap_chain();
}

void rhr::render::object::rectangle::post_position_update()
{
	mark_dirty();
}

void rhr::render::object::rectangle::post_color_update()
{
	mark_dirty();
}

void rhr::render::object::rectangle::post_size_update()
{
	if (m_super_bounds.x == 0 && m_super_bounds.y == 0)
	{
		m_use_size = m_size;
		m_in_bounds = true;

		mark_dirty();
	}
	else
	{
		if (m_position.x > m_super_bounds.x ||
			m_position.y > m_super_bounds.y)
		{
			m_use_size = { 0, 0 };
			m_in_bounds = false;
		}
		else
		{
			m_use_size = m_size;
			m_in_bounds = true;

			if (m_position.x + m_size.x > m_super_bounds.x)
				m_use_size.x = m_super_bounds.x - m_position.x;

			if (m_position.y + m_size.y > m_super_bounds.y)
				m_use_size.y = m_super_bounds.y - m_position.y;

			mark_dirty();
		}
	}
}
