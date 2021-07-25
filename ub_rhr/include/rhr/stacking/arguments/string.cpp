#include "string.hpp"

#include "rhr/rendering/renderer.hpp"
#include "rhr/stacking/block.hpp"

rhr::stack::argument::string::string(const cap::color& color)
	: rhr::stack::argument::argument(color)
	, m_text(std::make_shared<rhr::render::object::text>())
{
	m_text->set_weak(m_text);
	m_text->set_depth(rhr::render::renderer::depth_argument_text);
	m_text->set_color_primary(cap::color::text_primary_color);
	m_text->set_color_secondary(color);
	m_text->set_padding(rhr::stack::argument::argument::padding);
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