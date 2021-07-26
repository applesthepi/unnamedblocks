#include "real.hpp"

#include "rhr/rendering/renderer.hpp"
#include "rhr/stacking/block.hpp"

#define ARG_REAL_DECORE_WIDTH 6

rhr::stack::argument::real::real(const cap::color& color)
	: rhr::stack::argument::argument(color)
	, m_text(std::make_shared<rhr::render::object::text>())
	, m_decor_left_top(std::make_shared<rhr::render::object::object>(true))
	, m_decor_left_bottom(std::make_shared<rhr::render::object::object>(true))
	, m_decor_right_top(std::make_shared<rhr::render::object::object>(true))
	, m_decor_right_bottom(std::make_shared<rhr::render::object::object>(true))
{
	m_decor_left_top->set_weak(m_decor_left_top);
	m_decor_left_bottom->set_weak(m_decor_left_bottom);
	m_decor_right_top->set_weak(m_decor_right_top);
	m_decor_right_bottom->set_weak(m_decor_right_bottom);

	m_text->set_weak(m_text);
	m_text->set_depth(rhr::render::renderer::depth_argument_text);
	m_text->set_color_primary(cap::color::text_primary_color);
	m_text->set_color_secondary(color);
	m_text->set_padding(rhr::stack::argument::argument::padding);
}

BlockArgumentType rhr::stack::argument::real::get_type()
{
	return BlockArgumentType::REAL;
}

u32 rhr::stack::argument::real::get_width()
{
	return m_text->get_size().x + (2 * ARG_REAL_DECORE_WIDTH);
}

bool rhr::stack::argument::real::has_data()
{
	return true;
}

void rhr::stack::argument::real::on_render()
{
	m_text->render();

	m_decor_left_top->render();
	m_decor_left_bottom->render();
	m_decor_right_top->render();
	m_decor_right_bottom->render();
}

void rhr::stack::argument::real::on_update_buffers()
{
	m_text->update_buffers();

	i32 full_width = get_width();

	rhr::render::vertex vertices[12];
	f32 other_side = ARG_REAL_DECORE_WIDTH + m_text->get_size().x;

	vertices[0] = rhr::render::vertex({ static_cast<f32>(ARG_REAL_DECORE_WIDTH), 0, 0 }, m_block_color.get_normalized(), { 0.0f, 0.0f });
	vertices[1] = rhr::render::vertex({ 0, 0, 0 }, m_block_color.get_normalized(), { 0.0f, 0.0f });
	vertices[2] = rhr::render::vertex({ static_cast<f32>(ARG_REAL_DECORE_WIDTH), static_cast<f32>(rhr::stack::block::height_content / 2), 0 }, m_block_color.get_normalized(), { 0.0f, 0.0f });

	vertices[3] = rhr::render::vertex({ static_cast<f32>(ARG_REAL_DECORE_WIDTH), static_cast<f32>(rhr::stack::block::height_content / 2), 0 }, m_block_color.get_normalized(), { 0.0f, 0.0f });
	vertices[4] = rhr::render::vertex({ 0, static_cast<f32>(rhr::stack::block::height_content), 0 }, m_block_color.get_normalized(), { 0.0f, 0.0f });
	vertices[5] = rhr::render::vertex({ static_cast<f32>(ARG_REAL_DECORE_WIDTH), static_cast<f32>(rhr::stack::block::height_content), 0 }, m_block_color.get_normalized(), { 0.0f, 0.0f });

	vertices[6] = rhr::render::vertex({ other_side, 0, 0 }, m_block_color.get_normalized(), { 0.0f, 0.0f });
	vertices[7] = rhr::render::vertex({ other_side + static_cast<f32>(ARG_REAL_DECORE_WIDTH), 0, 0 }, m_block_color.get_normalized(), { 0.0f, 0.0f });
	vertices[8] = rhr::render::vertex({ other_side, static_cast<f32>(rhr::stack::block::height_content / 2), 0 }, m_block_color.get_normalized(), { 0.0f, 0.0f });

	vertices[9] = rhr::render::vertex({ other_side, static_cast<f32>(rhr::stack::block::height_content / 2), 0 }, m_block_color.get_normalized(), { 0.0f, 0.0f });
	vertices[10] = rhr::render::vertex({ other_side + static_cast<f32>(ARG_REAL_DECORE_WIDTH), static_cast<f32>(rhr::stack::block::height_content), 0 }, m_block_color.get_normalized(), { 0.0f, 0.0f });
	vertices[11] = rhr::render::vertex({ other_side, static_cast<f32>(rhr::stack::block::height_content), 0 }, m_block_color.get_normalized(), { 0.0f, 0.0f });

	m_decor_left_top->update_vertices(vertices + 0, 3, true);
	m_decor_left_bottom->update_vertices(vertices + 3, 3, true);
	m_decor_right_top->update_vertices(vertices + 6, 3, true);
	m_decor_right_bottom->update_vertices(vertices + 9, 3, true);
}

void rhr::stack::argument::real::on_reload_swap_chain()
{
	m_text->reload_swap_chain();

	m_decor_left_top->reload_swap_chain();
	m_decor_left_bottom->reload_swap_chain();
	m_decor_right_top->reload_swap_chain();
	m_decor_right_bottom->reload_swap_chain();
}

void rhr::stack::argument::real::on_frame_update(f64 delta_time)
{

}

void rhr::stack::argument::real::post_position_update()
{
	m_text->set_super_position(m_position + m_super_position);
	m_text->set_position({ ARG_REAL_DECORE_WIDTH, 0 });

	glm::vec<2, f64> pos = m_position + m_super_position;
	m_decor_left_top->set_super_position({ pos.x, pos.y, rhr::render::renderer::depth_argument });
	m_decor_left_bottom->set_super_position({ pos.x, pos.y, rhr::render::renderer::depth_argument });
	m_decor_right_top->set_super_position({ pos.x, pos.y, rhr::render::renderer::depth_argument });
	m_decor_right_bottom->set_super_position({ pos.x, pos.y, rhr::render::renderer::depth_argument });

	mark_dirty();
}

void rhr::stack::argument::real::on_set_data()
{
	m_text->set_text(m_data);
}
