#include "line.hpp"

rhr::render::object::line::line()
	: i_colorable(cap::color().from_normalized({0.0f, 0.0f, 0.0f, 1.0f}))
	, m_depth(10)
	, m_in_bounds(false)
	, m_border_enabled(true)
	, m_render_object(std::make_shared<rhr::render::object::object>(true))
	, m_point_1({0, 0})
	, m_point_2({100, 100})
	, m_line_half_width(5)
{
}

void rhr::render::object::line::set_depth(i32 depth)
{
	m_depth = depth;
	mark_dirty();
}

void rhr::render::object::line::enable_border(bool enable_border)
{
	m_border_enabled = enable_border;
	mark_dirty();
}

void rhr::render::object::line::ui_transform_update(i_ui::transform_update_spec transform_update_spec)
{
	const glm::vec<2, i32>& size_local	   = get_size_local();
	const glm::vec<2, i32>& size_parent	   = get_size_parent();
	const glm::vec<2, i32>& position_local = get_position_local_physical();

	if (size_parent == glm::vec<2, i32>())
		m_in_bounds = true;
	else
		m_in_bounds = (position_local.x + size_local.x < size_parent.x) && (position_local.y + size_local.y < size_parent.y);

	mark_dirty();
}

void rhr::render::object::line::ui_render()
{
	if (m_in_bounds)
		m_render_object->render();
}

void rhr::render::object::line::ui_reload_swap_chain() { m_render_object->reload_swap_chain(); }

void rhr::render::object::line::ui_update_buffers()
{
	double point_1_angle_1 = atan2(static_cast<double>(m_point_2.y - m_point_1.y), static_cast<double>(m_point_2.x - m_point_1.x));
	double point_1_angle_2 = glm::radians(90.0) - point_1_angle_1;
	double half_width	   = static_cast<double>(m_line_half_width);

	glm::vec<2, i32> point_1_top = {static_cast<i32>(glm::cos(point_1_angle_1) * half_width), static_cast<i32>(glm::sin(point_1_angle_1) * half_width * -1.0)};

	glm::vec<2, i32> point_1_right = {static_cast<i32>(glm::cos(point_1_angle_2) * half_width * -1.0), static_cast<i32>(glm::sin(point_1_angle_2) * half_width * -1.0)};

	glm::vec<2, i32> point_1_left = {static_cast<i32>(glm::cos(point_1_angle_2) * half_width), static_cast<i32>(glm::sin(point_1_angle_2) * half_width)};

	double point_2_angle_1 = atan2(static_cast<double>(m_point_2.y - m_point_1.y), static_cast<double>(m_point_2.x - m_point_1.x));
	double point_2_angle_2 = glm::radians(90.0) - point_2_angle_1;

	glm::vec<2, i32> point_2_top = {static_cast<i32>(glm::cos(point_2_angle_1) * half_width), static_cast<i32>(glm::sin(point_2_angle_1) * half_width * -1.0)};

	glm::vec<2, i32> point_2_right = {static_cast<i32>(glm::cos(point_2_angle_2) * half_width * -1.0), static_cast<i32>(glm::sin(point_2_angle_2) * half_width * -1.0)};

	glm::vec<2, i32> point_2_left = {static_cast<i32>(glm::cos(point_2_angle_2) * half_width), static_cast<i32>(glm::sin(point_2_angle_2) * half_width)};

	//	cap::logger::info("point_2_top: ", point_2_left);

	std::vector<rhr::render::vertex> vertices;
	std::vector<u32> indices;

	vertices.reserve(4 * 3);
	indices.reserve(6 * 3);

	// point 1 termination

	{
		rhr::render::vertex pt = rhr::render::vertex({m_point_1.x, m_point_1.y, static_cast<i32>(m_depth) * -1}, m_color.get_normalized(), {0.0f, 0.0f});

		rhr::render::vertex pt_left =
			rhr::render::vertex({m_point_1.x - point_1_left.x, m_point_1.y + point_1_left.y, static_cast<i32>(m_depth) * -1}, m_color.get_normalized(), {0.0f, 0.0f});

		rhr::render::vertex pt_top =
			rhr::render::vertex({m_point_1.x - point_1_top.x, m_point_1.y + point_1_top.y, static_cast<i32>(m_depth) * -1}, m_color.get_normalized(), {0.0f, 0.0f});

		rhr::render::vertex pt_right =
			rhr::render::vertex({m_point_1.x - point_1_right.x, m_point_1.y + point_1_right.y, static_cast<i32>(m_depth) * -1}, m_color.get_normalized(), {0.0f, 0.0f});

		vertices.push_back(pt);
		vertices.push_back(pt_left);
		vertices.push_back(pt_top);
		vertices.push_back(pt_right);

		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);

		indices.push_back(0);
		indices.push_back(2);
		indices.push_back(3);
	}

	// point 2 termination

	{
		rhr::render::vertex pt = rhr::render::vertex({m_point_2.x, m_point_2.y, static_cast<i32>(m_depth) * -1}, m_color.get_normalized(), {0.0f, 0.0f});

		rhr::render::vertex pt_left =
			rhr::render::vertex({m_point_2.x + point_2_left.x, m_point_2.y - point_2_left.y, static_cast<i32>(m_depth) * -1}, m_color.get_normalized(), {0.0f, 0.0f});

		rhr::render::vertex pt_top =
			rhr::render::vertex({m_point_2.x + point_2_top.x, m_point_2.y - point_2_top.y, static_cast<i32>(m_depth) * -1}, m_color.get_normalized(), {0.0f, 0.0f});

		rhr::render::vertex pt_right =
			rhr::render::vertex({m_point_2.x + point_2_right.x, m_point_2.y - point_2_right.y, static_cast<i32>(m_depth) * -1}, m_color.get_normalized(), {0.0f, 0.0f});

		vertices.push_back(pt);
		vertices.push_back(pt_left);
		vertices.push_back(pt_top);
		vertices.push_back(pt_right);

		indices.push_back(0 + 4);
		indices.push_back(1 + 4);
		indices.push_back(2 + 4);

		indices.push_back(0 + 4);
		indices.push_back(2 + 4);
		indices.push_back(3 + 4);
	}

	// fill

	{
		rhr::render::vertex pt_1_left =
			rhr::render::vertex({m_point_1.x - point_1_left.x, m_point_1.y + point_1_left.y, static_cast<i32>(m_depth) * -1}, m_color.get_normalized(), {0.0f, 0.0f});

		rhr::render::vertex pt_1_right =
			rhr::render::vertex({m_point_1.x - point_1_right.x, m_point_1.y + point_1_right.y, static_cast<i32>(m_depth) * -1}, m_color.get_normalized(), {0.0f, 0.0f});

		rhr::render::vertex pt_2_left =
			rhr::render::vertex({m_point_2.x + point_2_left.x, m_point_2.y - point_2_left.y, static_cast<i32>(m_depth) * -1}, m_color.get_normalized(), {0.0f, 0.0f});

		rhr::render::vertex pt_2_right =
			rhr::render::vertex({m_point_2.x + point_2_right.x, m_point_2.y - point_2_right.y, static_cast<i32>(m_depth) * -1}, m_color.get_normalized(), {0.0f, 0.0f});

		vertices.push_back(pt_1_left);
		vertices.push_back(pt_1_right);
		vertices.push_back(pt_2_left);
		vertices.push_back(pt_2_right);

		indices.push_back(0 + 8);
		indices.push_back(1 + 8);
		indices.push_back(2 + 8);

		indices.push_back(0 + 8);
		indices.push_back(2 + 8);
		indices.push_back(3 + 8);
	}

#if 0
	rhr::render::vertex v0 = rhr::render::vertex({ position_physical.x, position_physical.y, static_cast<i32>(m_depth) * -1 }, m_color.get_normalized(), { 0.0f, 0.0f });
	rhr::render::vertex v1 = rhr::render::vertex({ position_physical.x + size_local.x, position_physical.y, static_cast<i32>(m_depth) * -1 }, m_color.get_normalized(), { 1.0f, 0.0f });
	rhr::render::vertex v2 = rhr::render::vertex({ position_physical.x + size_local.x, position_physical.y + size_local.y, static_cast<i32>(m_depth) * -1 }, m_color.get_normalized(), { 1.0f, 1.0f });
	rhr::render::vertex v3 = rhr::render::vertex({ position_physical.x, position_physical.y + size_local.y, static_cast<i32>(m_depth) * -1 }, m_color.get_normalized(), { 0.0f, 1.0f });

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

		v_left0 = rhr::render::vertex({ position_physical.x, position_physical.y, static_cast<f32>(m_depth - 1) * -1.0f}, cap::color::black.get_normalized(), { 0.0f, 0.0f });
		v_left1 = rhr::render::vertex({ position_physical.x + 1.0f, position_physical.y, static_cast<f32>(m_depth - 1) * -1.0f}, cap::color::black.get_normalized(), { 1.0f, 0.0f });
		v_left2 = rhr::render::vertex({ position_physical.x + 1.0f, position_physical.y + size_local.y, static_cast<f32>(m_depth - 1) * -1.0f}, cap::color::black.get_normalized(), { 1.0f, 1.0f });
		v_left3 = rhr::render::vertex({ position_physical.x, position_physical.y + size_local.y, static_cast<f32>(m_depth - 1) * -1.0f}, cap::color::black.get_normalized(), { 0.0f, 1.0f });

		v_right0 = rhr::render::vertex({ position_physical.x + size_local.x - 1.0f, position_physical.y, static_cast<f32>(m_depth - 1) * -1.0f}, cap::color::black.get_normalized(), { 0.0f, 0.0f });
		v_right1 = rhr::render::vertex({ position_physical.x + size_local.x, position_physical.y, static_cast<f32>(m_depth - 1) * -1.0f}, cap::color::black.get_normalized(), { 1.0f, 0.0f });
		v_right2 = rhr::render::vertex({ position_physical.x + size_local.x, position_physical.y + size_local.y, static_cast<f32>(m_depth - 1) * -1.0f}, cap::color::black.get_normalized(), { 1.0f, 1.0f });
		v_right3 = rhr::render::vertex({ position_physical.x + size_local.x - 1.0f, position_physical.y + size_local.y, static_cast<f32>(m_depth - 1) * -1.0f}, cap::color::black.get_normalized(), { 0.0f, 1.0f });

		v_up0 = rhr::render::vertex({ position_physical.x, position_physical.y, static_cast<f32>(m_depth - 1) * -1.0f}, cap::color::black.get_normalized(), { 0.0f, 0.0f });
		v_up1 = rhr::render::vertex({ position_physical.x + size_local.x, position_physical.y, static_cast<f32>(m_depth - 1) * -1.0f}, cap::color::black.get_normalized(), { 1.0f, 0.0f });
		v_up2 = rhr::render::vertex({ position_physical.x + size_local.x, position_physical.y + 1.0f, static_cast<f32>(m_depth - 1) * -1.0f}, cap::color::black.get_normalized(), { 1.0f, 1.0f });
		v_up3 = rhr::render::vertex({ position_physical.x, position_physical.y + 1.0f, static_cast<f32>(m_depth - 1) * -1.0f}, cap::color::black.get_normalized(), { 0.0f, 1.0f });

		v_down0 = rhr::render::vertex({ position_physical.x, position_physical.y + size_local.y - 1.0f, static_cast<f32>(m_depth - 1) * -1.0f}, cap::color::black.get_normalized(), { 0.0f, 0.0f });
		v_down1 = rhr::render::vertex({ position_physical.x + size_local.x, position_physical.y + size_local.y - 1.0f, static_cast<f32>(m_depth - 1) * -1.0f}, cap::color::black.get_normalized(), { 1.0f, 0.0f });
		v_down2 = rhr::render::vertex({ position_physical.x + size_local.x, position_physical.y + size_local.y, static_cast<f32>(m_depth - 1) * -1.0f}, cap::color::black.get_normalized(), { 1.0f, 1.0f });
		v_down3 = rhr::render::vertex({ position_physical.x, position_physical.y + size_local.y, static_cast<f32>(m_depth - 1) * -1.0f}, cap::color::black.get_normalized(), { 0.0f, 1.0f });

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
#endif
	m_render_object->update_vertices(&vertices, &indices, true);
}

void rhr::render::object::line::ui_chain_update_buffers()
{

}

void rhr::render::object::line::post_color_update() { mark_dirty(); }

void rhr::render::object::line::set_point_1(const glm::vec<2, i32>& point)
{
	//	cap::logger::info("set_point_1", point);
	m_point_1 = point;
	mark_dirty();
}

void rhr::render::object::line::set_point_2(const glm::vec<2, i32>& point)
{
	//	cap::logger::info("set_point_2", point);
	m_point_2 = point;
	mark_dirty();
}

void rhr::render::object::line::set_line_half_width(u8 line_half_width) { m_line_half_width = line_half_width; }
