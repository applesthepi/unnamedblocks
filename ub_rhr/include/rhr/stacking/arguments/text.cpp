#include "text.hpp"

#include "rhr/rendering/renderer.hpp"

rhr::stack::argument::text::text(const cap::color& color, void(*update)(void*), void* data)
	: rhr::stack::argument::argument(color, update, data)
	, m_text(std::make_shared<rhr::render::object::text>(rhr::registry::char_texture::texture_type::BOLD_NORMAL, update, data, true, false))
{
	m_text->set_weak(m_text);
	m_text->set_weak_field(m_text);
	m_text->set_depth(rhr::render::renderer::depth_argument_text);
	m_text->set_color_primary(cap::color::black);
	m_text->set_color_secondary(color);
	m_text->set_padding(rhr::stack::argument::text::padding);
	m_text->enable_background(false);
}

BlockArgumentType rhr::stack::argument::text::get_type()
{
	return BlockArgumentType::TEXT;
}

u32 rhr::stack::argument::text::get_width()
{
	return m_text->get_size().x;
}

bool rhr::stack::argument::text::has_data()
{
	return true;
}

void rhr::stack::argument::text::on_render()
{
	m_text->render();
}

void rhr::stack::argument::text::on_update_buffers()
{
	m_text->update_buffers();
}

void rhr::stack::argument::text::on_reload_swap_chain()
{
	m_text->reload_swap_chain();
}

void rhr::stack::argument::text::on_frame_update(f64 delta_time)
{
	
}

void rhr::stack::argument::text::post_position_update()
{
	m_text->set_super_position(m_position + m_super_position);
}

void rhr::stack::argument::text::on_set_data()
{
	m_text->set_text(m_data);
}
