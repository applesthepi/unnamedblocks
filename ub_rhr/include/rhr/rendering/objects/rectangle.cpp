#include "rectangle.hpp"

rhr::render::object::rectangle::rectangle()
	: i_colorable(cap::color().from_normalized({0.0f, 0.0f, 0.0f, 1.0f}))
	, m_depth(10)
	, m_has_color(false)
	, m_has_texture(false)
	, m_in_bounds(false)
	, m_render_object(std::make_shared<rhr::render::object::object>(true))
	, m_use_size({0, 0})
	, m_texture_type(rhr::render::object::object::texture_type::CUSTOM)
	, m_border_enabled(true)
	, m_overhang_horizontal(rhr::render::cardinal::local_horizontal::RIGHT)
	, m_overhang_vertical(rhr::render::cardinal::local_vertical::DOWN)
{}

void rhr::render::object::rectangle::set_offset(glm::vec<2, i32>* offset)
{
	m_render_object->set_offset(offset);
}

void rhr::render::object::rectangle::set_texture(const std::string& texture)
{
	m_has_texture  = true;
	m_texture	   = texture;
	m_texture_type = rhr::render::object::object::texture_type::CUSTOM;
	m_render_object->set_texture(texture);
	m_color = cap::color::white;

	mark_dirty();
}

void rhr::render::object::rectangle::set_texture(rhr::registry::char_texture::texture_type type, u16 font_size)
{
	m_has_texture = true;
	m_texture.clear();
	m_texture_type = rhr::render::object::object::texture_type::TEXT_SHEET;
	m_render_object->set_texture_char(type, font_size);

	mark_dirty();
}

void rhr::render::object::rectangle::set_depth(i32 depth)
{
	m_depth = depth;
	mark_dirty();
}

void rhr::render::object::rectangle::ui_transform_update(i_ui::transform_update_spec transform_update_spec)
{
	bool position_update = transform_update_spec & i_ui::transform_update_spec_position;
	bool size_update	 = transform_update_spec & i_ui::transform_update_spec_size;

	if (position_update)
		m_render_object->set_position(
			{get_position_physical_absolute().x, get_position_physical_absolute().y, static_cast<f64>(m_depth - 1)});

	if (!size_update)
		return;

	const glm::vec<2, i32>& size_local	   = get_size_local();
	const glm::vec<2, i32>& size_parent	   = get_size_parent();
	const glm::vec<2, i32>& position_local = get_position_local_physical();

	if (size_parent == glm::vec<2, i32>())
	{
		m_use_size	= size_local;
		m_in_bounds = true;
	}
	else
	{
		if (position_local.x > size_parent.x || position_local.y > size_parent.y)
		{
			m_use_size	= {0, 0};
			m_in_bounds = false;
		}
		else
		{
			m_use_size	= size_local;
			m_in_bounds = true;

			if (position_local.x + size_local.x > size_parent.x)
				m_use_size.x = size_parent.x - position_local.x;

			if (position_local.y + size_local.y > size_parent.y)
				m_use_size.y = size_parent.y - position_local.y;
		}
	}

	mark_dirty();
}

void rhr::render::object::rectangle::ui_render()
{
	if (m_in_bounds)
		m_render_object->render();
}

void rhr::render::object::rectangle::ui_reload_swap_chain()
{
	m_render_object->reload_swap_chain();
}

void rhr::render::object::rectangle::ui_update_buffers()
{
	std::vector<rhr::render::vertex> vertices;
	std::vector<u32> indices;

	vertices.reserve(4);
	indices.reserve(6);

	glm::vec<2, f32> position_physical = static_cast<glm::vec<2, f32>>(get_position_physical_absolute());
	glm::vec<2, f32> use_size		   = static_cast<glm::vec<2, f32>>(m_use_size);

	rhr::render::vertex v0 =
		rhr::render::vertex({0.0, 0.0, static_cast<i32>(m_depth) * -1}, m_color.get_normalized(), {0.0f, 0.0f});
	rhr::render::vertex v1 =
		rhr::render::vertex({use_size.x, 0.0, static_cast<i32>(m_depth) * -1}, m_color.get_normalized(), {1.0f, 0.0f});
	rhr::render::vertex v2 = rhr::render::vertex(
		{use_size.x, use_size.y, static_cast<i32>(m_depth) * -1}, m_color.get_normalized(), {1.0f, 1.0f});
	rhr::render::vertex v3 =
		rhr::render::vertex({0.0, use_size.y, static_cast<i32>(m_depth) * -1}, m_color.get_normalized(), {0.0f, 1.0f});

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

	if (m_border_enabled)
	{
		rhr::render::vertex v_left0, v_left1, v_left2, v_left3;
		rhr::render::vertex v_right0, v_right1, v_right2, v_right3;
		rhr::render::vertex v_up0, v_up1, v_up2, v_up3;
		rhr::render::vertex v_down0, v_down1, v_down2, v_down3;

		if (m_overhang_horizontal == cardinal::local_horizontal::LEFT)
		{
			// overhang border to the left

			v_left0 = rhr::render::vertex({-1.0f, 0.0, 0.0}, cap::color::black.get_normalized(), {0.0f, 0.0f});
			v_left1 = rhr::render::vertex({0.0, 0.0, 0.0}, cap::color::black.get_normalized(), {1.0f, 0.0f});
			v_left2 = rhr::render::vertex({0.0, use_size.y, 0.0}, cap::color::black.get_normalized(), {1.0f, 1.0f});
			v_left3 = rhr::render::vertex({-1.0f, use_size.y, 0.0}, cap::color::black.get_normalized(), {0.0f, 1.0f});

			v_right0 =
				rhr::render::vertex({use_size.x - 1.0f, 0.0, 0.0}, cap::color::black.get_normalized(), {0.0f, 0.0f});
			v_right1 = rhr::render::vertex({use_size.x, 0.0, 0.0}, cap::color::black.get_normalized(), {1.0f, 0.0f});
			v_right2 =
				rhr::render::vertex({use_size.x, use_size.y, 0.0}, cap::color::black.get_normalized(), {1.0f, 1.0f});
			v_right3 = rhr::render::vertex(
				{use_size.x - 1.0f, use_size.y, 0.0}, cap::color::black.get_normalized(), {0.0f, 1.0f});
		}
		else if (m_overhang_horizontal == cardinal::local_horizontal::RIGHT)
		{
			// overhang border to the right

			v_left0 = rhr::render::vertex({0.0, 0.0, 0.0}, cap::color::black.get_normalized(), {0.0f, 0.0f});
			v_left1 = rhr::render::vertex({1.0f, 0.0, 0.0}, cap::color::black.get_normalized(), {1.0f, 0.0f});
			v_left2 = rhr::render::vertex({1.0f, use_size.y, 0.0}, cap::color::black.get_normalized(), {1.0f, 1.0f});
			v_left3 = rhr::render::vertex({0.0, use_size.y, 0.0}, cap::color::black.get_normalized(), {0.0f, 1.0f});

			v_right0 = rhr::render::vertex({use_size.x, 0.0, 0.0}, cap::color::black.get_normalized(), {0.0f, 0.0f});
			v_right1 =
				rhr::render::vertex({use_size.x + 1.0f, 0.0, 0.0}, cap::color::black.get_normalized(), {1.0f, 0.0f});
			v_right2 = rhr::render::vertex(
				{use_size.x + 1.0f, use_size.y, 0.0}, cap::color::black.get_normalized(), {1.0f, 1.0f});
			v_right3 =
				rhr::render::vertex({use_size.x, use_size.y, 0.0}, cap::color::black.get_normalized(), {0.0f, 1.0f});
		}

		if (m_overhang_vertical == cardinal::local_vertical::UP)
		{
			// overhang border up

			v_up0 = rhr::render::vertex({0.0, -1.0, 0.0}, cap::color::black.get_normalized(), {0.0f, 0.0f});
			v_up1 = rhr::render::vertex({use_size.x, -1.0, 0.0}, cap::color::black.get_normalized(), {1.0f, 0.0f});
			v_up2 = rhr::render::vertex({use_size.x, 0.0, 0.0}, cap::color::black.get_normalized(), {1.0f, 1.0f});
			v_up3 = rhr::render::vertex({0.0, 0.0, 0.0}, cap::color::black.get_normalized(), {0.0f, 1.0f});

			v_down0 =
				rhr::render::vertex({0.0, use_size.y - 1.0f, 0.0}, cap::color::black.get_normalized(), {0.0f, 0.0f});
			v_down1 = rhr::render::vertex(
				{use_size.x, use_size.y - 1.0f, 0.0}, cap::color::black.get_normalized(), {1.0f, 0.0f});
			v_down2 =
				rhr::render::vertex({use_size.x, use_size.y, 0.0}, cap::color::black.get_normalized(), {1.0f, 1.0f});
			v_down3 = rhr::render::vertex({0.0, use_size.y, 0.0}, cap::color::black.get_normalized(), {0.0f, 1.0f});
		}
		else if (m_overhang_vertical == cardinal::local_vertical::DOWN)
		{
			// overhang border down

			v_up0 = rhr::render::vertex({0.0, 0.0, 0.0}, cap::color::black.get_normalized(), {0.0f, 0.0f});
			v_up1 = rhr::render::vertex({use_size.x, 0.0, 0.0}, cap::color::black.get_normalized(), {1.0f, 0.0f});
			v_up2 = rhr::render::vertex({use_size.x, 1.0, 0.0}, cap::color::black.get_normalized(), {1.0f, 1.0f});
			v_up3 = rhr::render::vertex({0.0, 1.0, 0.0}, cap::color::black.get_normalized(), {0.0f, 1.0f});

			v_down0 = rhr::render::vertex({0.0, use_size.y, 0.0}, cap::color::black.get_normalized(), {0.0f, 0.0f});
			v_down1 =
				rhr::render::vertex({use_size.x, use_size.y, 0.0}, cap::color::black.get_normalized(), {1.0f, 0.0f});
			v_down2 = rhr::render::vertex(
				{use_size.x, use_size.y + 1.0f, 0.0}, cap::color::black.get_normalized(), {1.0f, 1.0f});
			v_down3 =
				rhr::render::vertex({0.0, use_size.y + 1.0f, 0.0}, cap::color::black.get_normalized(), {0.0f, 1.0f});
		}

		vertices.push_back(v_left0);
		vertices.push_back(v_left1);
		vertices.push_back(v_left2);
		vertices.push_back(v_left3);

		vertices.push_back(v_right0);
		vertices.push_back(v_right1);
		vertices.push_back(v_right2);
		vertices.push_back(v_right3);

		vertices.push_back(v_up0);
		vertices.push_back(v_up1);
		vertices.push_back(v_up2);
		vertices.push_back(v_up3);

		vertices.push_back(v_down0);
		vertices.push_back(v_down1);
		vertices.push_back(v_down2);
		vertices.push_back(v_down3);

		for (u8 i = 0; i < 4; i++)
		{
			indices.push_back(1 + 4 * (i + 1));
			indices.push_back(0 + 4 * (i + 1));
			indices.push_back(2 + 4 * (i + 1));
			indices.push_back(2 + 4 * (i + 1));
			indices.push_back(0 + 4 * (i + 1));
			indices.push_back(3 + 4 * (i + 1));
		}
	}

	m_render_object->update_vertices(&vertices, &indices, true);
}

void rhr::render::object::rectangle::ui_chain_update_buffers()
{}

void rhr::render::object::rectangle::post_color_update()
{
	m_has_color = true;
	mark_dirty();
}

void rhr::render::object::rectangle::enable_border(
	bool enable_border,
	rhr::render::cardinal::local_horizontal overhang_horizontal,
	rhr::render::cardinal::local_vertical overhang_vertical)
{
	m_border_enabled	  = enable_border;
	m_overhang_horizontal = overhang_horizontal;
	m_overhang_vertical	  = overhang_vertical;
	mark_dirty();
}
