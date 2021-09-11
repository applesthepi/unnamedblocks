#include "boolean.hpp"

#include "rhr/rendering/renderer.hpp"
#include "rhr/stacking/block.hpp"

#define ARG_BOOL_DECORE_WIDTH 10

rhr::stack::argument::boolean::boolean(const cap::color& color, void(*update)(void*), void* data)
	: argument(color, update, data)
	, m_text(std::make_shared<rhr::render::object::text>(rhr::registry::char_texture::texture_type::LIGHT_NORMAL, update, data, false, true))
	, m_decor_left(std::make_shared<rhr::render::object::object>(true))
	, m_decor_right(std::make_shared<rhr::render::object::object>(true))
{
	m_decor_left->set_weak(m_decor_left);
	m_decor_right->set_weak(m_decor_right);

	m_text->set_weak(m_text);
	m_text->set_weak_field(m_text);
	m_text->set_depth(rhr::render::renderer::depth_argument_text);
	m_text->set_color_primary(cap::color::text_primary_color);
	m_text->set_color_secondary(color);
	m_text->set_padding(padding);
}

BlockArgumentType rhr::stack::argument::boolean::get_type()
{
	return BlockArgumentType::REAL;
}

u32 rhr::stack::argument::boolean::get_width()
{
	return m_text->get_size().x + (2 * ARG_BOOL_DECORE_WIDTH);
}

bool rhr::stack::argument::boolean::has_data()
{
	return true;
}

void rhr::stack::argument::boolean::on_render()
{
	m_text->render();

	m_decor_left->render();
	m_decor_right->render();
}

void rhr::stack::argument::boolean::on_update_buffers()
{
	m_text->update_buffers();

	i32 fullWidth = get_width();

	rhr::render::vertex vertices[6];

	vertices[0] = rhr::render::vertex({ static_cast<f32>(ARG_BOOL_DECORE_WIDTH), 0.0f, 0 }, m_block_color.get_normalized(), { 0.0f, 0.0f });
	vertices[1] = rhr::render::vertex({ static_cast<f32>(0.0f), 0.0f/*static_cast<f32>(rhr::stack::block::height_content / 2)*/, 0 }, m_block_color.get_normalized(), { 1.0f, 0.0f });
	vertices[2] = rhr::render::vertex({ static_cast<f32>(ARG_BOOL_DECORE_WIDTH), static_cast<f32>(rhr::stack::block::height_content), 0 }, m_block_color.get_normalized(), { 1.0f, 1.0f });

	vertices[3] = rhr::render::vertex({ static_cast<f32>(fullWidth - ARG_BOOL_DECORE_WIDTH), 0.0f, 0 }, m_block_color.get_normalized(), { 0.0f, 0.0f });
	vertices[4] = rhr::render::vertex({ static_cast<f32>(fullWidth), static_cast<f32>(rhr::stack::block::height_content/* / 2*/), 0 }, m_block_color.get_normalized(), { 1.0f, 0.0f });
	vertices[5] = rhr::render::vertex({ static_cast<f32>(fullWidth - ARG_BOOL_DECORE_WIDTH), static_cast<f32>(rhr::stack::block::height_content), 0 }, m_block_color.get_normalized(), { 1.0f, 1.0f });

	m_decor_left->update_vertices(vertices + 0, 3, true);
	m_decor_right->update_vertices(vertices + 3, 3, true);
}

void rhr::stack::argument::boolean::on_reload_swap_chain()
{
	m_text->reload_swap_chain();

	m_decor_left->reload_swap_chain();
	m_decor_right->reload_swap_chain();
}

void rhr::stack::argument::boolean::on_frame_update(f64 delta_time)
{

}

void rhr::stack::argument::boolean::post_position_update()
{
	m_text->set_super_position(m_position + m_super_position);
	m_text->set_position({ ARG_BOOL_DECORE_WIDTH, 0 });

	glm::vec<2, f64> pos = m_position + m_super_position;
	m_decor_left->set_super_position({ pos.x, pos.y, rhr::render::renderer::depth_argument });
	m_decor_right->set_super_position({ pos.x, pos.y, rhr::render::renderer::depth_argument });

	mark_dirty();
}

void rhr::stack::argument::boolean::on_set_data()
{
	m_text->set_text(m_data);
}

bool rhr::stack::argument::boolean::drag_bounds(glm::vec<2, i32> position)
{
    glm::vec<2, i32> arg_position = m_position + m_super_position;

    return (
            position.x > arg_position.x && position.x < arg_position.x + get_width() &&
            position.y > arg_position.y && position.y < arg_position.y + rhr::stack::block::height_content
            );
}