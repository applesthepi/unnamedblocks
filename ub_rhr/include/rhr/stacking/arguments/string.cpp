#include "string.hpp"

#include "rhr/rendering/renderer.hpp"
#include "rhr/stacking/block.hpp"
#include "rhr/stacking/plane.hpp"

// TODO: remove
#include <iostream>

rhr::stack::argument::string::string(const cap::color& color, void(*update)(void*), void* data)
	: rhr::stack::argument::argument(color, update, data)
	, m_text(std::make_shared<rhr::render::object::text>(rhr::registry::char_texture::texture_type::LIGHT_NORMAL, update, data, false))
{
	m_mouse_button = [&, color](glm::vec<2, i32> position, f32 scroll, MouseOperation operation, MouseButton button)
	{
		if (button == MouseButton::MIDDLE && operation == MouseOperation::Click)
		{
			if (m_mode == BlockArgumentVariableMode::RAW)
				set_mode(BlockArgumentVariableMode::VAR);
			else if (m_mode == BlockArgumentVariableMode::VAR)
				set_mode(BlockArgumentVariableMode::RAW);
		}
	};

	m_text->set_weak(m_text);
	m_text->set_weak_field(m_text);
	m_text->set_depth(rhr::render::renderer::depth_argument_text);
	m_text->set_color_primary(cap::color::text_primary_color);
	m_text->set_color_secondary(color);
	m_text->set_padding(rhr::stack::argument::argument::padding);
	m_text->set_mouse_button(m_mouse_button);
}

BlockArgumentType rhr::stack::argument::string::get_type()
{
	return BlockArgumentType::STRING;
}

u32 rhr::stack::argument::string::get_width()
{
	return m_text->get_size().x;
}

bool rhr::stack::argument::string::has_data()
{
	return true;
}

void rhr::stack::argument::string::on_render()
{
	m_text->render();
}

void rhr::stack::argument::string::on_update_buffers()
{
	m_text->update_buffers();
}

void rhr::stack::argument::string::on_reload_swap_chain()
{
	m_text->reload_swap_chain();
}

void rhr::stack::argument::string::on_frame_update(f64 delta_time)
{

}

void rhr::stack::argument::string::post_position_update()
{
	m_text->set_super_position(m_position + m_super_position);
}

void rhr::stack::argument::string::on_set_data()
{
	m_text->set_text(m_data);
}

bool rhr::stack::argument::string::drag_bounds(glm::vec<2, i32> position)
{
	glm::vec<2, i32> arg_position = m_position + m_super_position;

	return (
		position.x > arg_position.x && position.x < arg_position.x + get_width() &&
		position.y > arg_position.y && position.y < arg_position.y + rhr::stack::block::height_content
	);
}

const std::string& rhr::stack::argument::string::get_data()
{
	return m_text->get_text();
}

void rhr::stack::argument::string::on_set_mode(BlockArgumentVariableMode mode)
{
	if (m_mode == BlockArgumentVariableMode::RAW)
		m_text->set_color_secondary(m_block_color);
	else if (m_mode == BlockArgumentVariableMode::VAR)
		m_text->set_color_secondary(cap::color().from_u8({ 100, 0, 40, 255 }));
}
