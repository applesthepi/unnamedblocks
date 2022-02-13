#include "real.hpp"

#include "rhr/rendering/renderer.hpp"
#include "rhr/stacking/block.hpp"

#define ARG_REAL_DECORE_WIDTH 6

rhr::stack::argument::real::real(
	const cap::color& color, std::function<void()>* function_collection_update, glm::vec<2, i32>* plane_offset)
	: rhr::stack::argument::argument(color, function_collection_update, plane_offset)
	, m_text(std::make_shared<rhr::render::object::text>(
		  rhr::registry::char_texture::texture_type::LIGHT_NORMAL,
		  rhr::stack::block::height_content,
		  false,
		  true,
		  plane_offset))
	, m_decor_left_top(std::make_shared<rhr::render::object::object>(true))
	, m_decor_left_bottom(std::make_shared<rhr::render::object::object>(true))
	, m_decor_right_top(std::make_shared<rhr::render::object::object>(true))
	, m_decor_right_bottom(std::make_shared<rhr::render::object::object>(true))
{
	m_mouse_button = [&, color](glm::vec<2, i32> position, f32 scroll, rhr::handler::input::mouse_operation operation, rhr::handler::input::mouse_button button)
	{
		if (button == rhr::handler::input::mouse_button::MIDDLE && operation == rhr::handler::input::mouse_operation::PRESS)
		{
			if (m_mode == cap::mod::block::block::argument::variable_mode::RAW)
				set_mode(cap::mod::block::block::argument::variable_mode::VAR);
			else if (m_mode == cap::mod::block::block::argument::variable_mode::VAR)
				set_mode(cap::mod::block::block::argument::variable_mode::RAW);
		}
	};

	m_function_text_update = [&]()
	{
		set_data(m_text->get_text());
		(*m_function_collection_update)();
	};

	m_decor_left_top->set_offset(plane_offset);
	m_decor_left_bottom->set_offset(plane_offset);
	m_decor_right_top->set_offset(plane_offset);
	m_decor_right_bottom->set_offset(plane_offset);

	m_text->set_offset(plane_offset);
	m_text->set_update_function(&m_function_text_update);
	m_text->set_weak_field(m_text);
	m_text->set_depth(rhr::render::renderer::depth_argument_text);
	m_text->set_color_primary(cap::color::text_primary_color);
	m_text->set_color_secondary(color);
	m_text->set_padding(rhr::stack::argument::argument::padding);
	m_text->set_mouse_button(m_mouse_button);
}

cap::mod::block::block::argument::type rhr::stack::argument::real::get_type()
{
	return cap::mod::block::block::argument::type::REAL;
}

u32 rhr::stack::argument::real::get_width()
{
	return m_text->get_size_local().x + (2 * ARG_REAL_DECORE_WIDTH);
}

bool rhr::stack::argument::real::has_data()
{
	return true;
}

void rhr::stack::argument::real::ui_transform_update(i_ui::transform_update_spec transform_update_spec)
{
	update_child_transform(m_text, false);
	m_text->set_position_local_physical({ARG_REAL_DECORE_WIDTH, 0}, true);

	glm::vec<2, f64> pos = get_position_physical_absolute();
	m_decor_left_top->set_super_position({pos.x, pos.y, rhr::render::renderer::depth_argument});
	m_decor_left_bottom->set_super_position({pos.x, pos.y, rhr::render::renderer::depth_argument});
	m_decor_right_top->set_super_position({pos.x, pos.y, rhr::render::renderer::depth_argument});
	m_decor_right_bottom->set_super_position({pos.x, pos.y, rhr::render::renderer::depth_argument});

	mark_dirty();
}

void rhr::stack::argument::real::ui_render()
{
	m_text->render();

	m_decor_left_top->render();
	m_decor_left_bottom->render();
	m_decor_right_top->render();
	m_decor_right_bottom->render();
}

void rhr::stack::argument::real::ui_reload_swap_chain()
{
	m_text->reload_swap_chain();

	m_decor_left_top->reload_swap_chain();
	m_decor_left_bottom->reload_swap_chain();
	m_decor_right_top->reload_swap_chain();
	m_decor_right_bottom->reload_swap_chain();
}

void rhr::stack::argument::real::ui_update_buffers()
{
	m_text->update_buffers();

	i32 full_width = get_width();

	rhr::render::vertex vertices[12];
	f32 other_side = ARG_REAL_DECORE_WIDTH + m_text->get_size_local().x;

	cap::color use_color = m_block_color;
	if (m_mode == cap::mod::block::block::argument::variable_mode::VAR)
		use_color = cap::color().from_u8({100, 0, 40, 255});

	vertices[0] =
		rhr::render::vertex({static_cast<f32>(ARG_REAL_DECORE_WIDTH), 0, 0}, use_color.get_normalized(), {0.0f, 0.0f});
	vertices[1] = rhr::render::vertex({0, 0, 0}, use_color.get_normalized(), {0.0f, 0.0f});
	vertices[2] = rhr::render::vertex(
		{static_cast<f32>(ARG_REAL_DECORE_WIDTH), static_cast<f32>(rhr::stack::block::height_content / 2), 0},
		use_color.get_normalized(),
		{0.0f, 0.0f});

	vertices[3] = rhr::render::vertex(
		{static_cast<f32>(ARG_REAL_DECORE_WIDTH), static_cast<f32>(rhr::stack::block::height_content / 2), 0},
		use_color.get_normalized(),
		{0.0f, 0.0f});
	vertices[4] = rhr::render::vertex(
		{0, static_cast<f32>(rhr::stack::block::height_content), 0}, use_color.get_normalized(), {0.0f, 0.0f});
	vertices[5] = rhr::render::vertex(
		{static_cast<f32>(ARG_REAL_DECORE_WIDTH), static_cast<f32>(rhr::stack::block::height_content), 0},
		use_color.get_normalized(),
		{0.0f, 0.0f});

	vertices[6] = rhr::render::vertex({other_side, 0, 0}, use_color.get_normalized(), {0.0f, 0.0f});
	vertices[7] = rhr::render::vertex(
		{other_side + static_cast<f32>(ARG_REAL_DECORE_WIDTH), 0, 0}, use_color.get_normalized(), {0.0f, 0.0f});
	vertices[8] = rhr::render::vertex(
		{other_side, static_cast<f32>(rhr::stack::block::height_content / 2), 0},
		use_color.get_normalized(),
		{0.0f, 0.0f});

	vertices[9] = rhr::render::vertex(
		{other_side, static_cast<f32>(rhr::stack::block::height_content / 2), 0},
		use_color.get_normalized(),
		{0.0f, 0.0f});
	vertices[10] = rhr::render::vertex(
		{other_side + static_cast<f32>(ARG_REAL_DECORE_WIDTH), static_cast<f32>(rhr::stack::block::height_content), 0},
		use_color.get_normalized(),
		{0.0f, 0.0f});
	vertices[11] = rhr::render::vertex(
		{other_side, static_cast<f32>(rhr::stack::block::height_content), 0}, use_color.get_normalized(), {0.0f, 0.0f});

	m_decor_left_top->update_vertices(vertices + 0, 3, true);
	m_decor_left_bottom->update_vertices(vertices + 3, 3, true);
	m_decor_right_top->update_vertices(vertices + 6, 3, true);
	m_decor_right_bottom->update_vertices(vertices + 9, 3, true);
}

void rhr::stack::argument::real::ui_chain_update_buffers()
{}

void rhr::stack::argument::real::ui_frame_update(f64 delta_time)
{}

void rhr::stack::argument::real::on_set_data()
{
	m_text->set_text(m_data);
}

bool rhr::stack::argument::real::drag_bounds(glm::vec<2, i32> position)
{
	glm::vec<2, i32> arg_position = get_position_virtual_absolute();

	return (
		position.x > arg_position.x && position.x < arg_position.x + get_width() && position.y > arg_position.y
		&& position.y < arg_position.y + rhr::stack::block::height_content);
}

rhr::stack::argument::argument::padding_style rhr::stack::argument::real::get_padding_style()
{
	return rhr::stack::argument::argument::padding_style::HARD;
}

void rhr::stack::argument::real::set_plane_offset(glm::vec<2, i32>* plane_offset)
{
	argument::set_plane_offset(plane_offset);

	m_text->set_offset(plane_offset);

	m_decor_left_top->set_offset(plane_offset);
	m_decor_left_bottom->set_offset(plane_offset);
	m_decor_right_top->set_offset(plane_offset);
	m_decor_right_bottom->set_offset(plane_offset);
}

void rhr::stack::argument::real::on_set_mode(cap::mod::block::block::argument::variable_mode mode)
{
	if (m_mode == cap::mod::block::block::argument::variable_mode::RAW)
		m_text->set_color_secondary(m_block_color);
	else if (m_mode == cap::mod::block::block::argument::variable_mode::VAR)
		m_text->set_color_secondary(cap::color().from_u8({100, 0, 40, 255}));
}
