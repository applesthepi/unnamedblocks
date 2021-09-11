#include "button_text.hpp"

#include "rhr/rendering/renderer.hpp"
#include "rhr/stacking/arguments/argument.hpp"

rhr::render::object::button_text::button_text(const cap::color& primary_color, const cap::color& secondary_color, const std::string& text)
	: rhr::render::object::button(primary_color, secondary_color)
	, m_text(std::make_shared<rhr::render::object::text>(rhr::registry::char_texture::texture_type::BOLD_NORMAL, nullptr, nullptr, true, false))
{
	m_text->set_weak(m_text);
	m_text->set_weak_field(m_text);
	m_text->set_depth(rhr::render::renderer::depth_ui_text);
	m_text->set_color_primary(primary_color);
	m_text->set_color_secondary(cap::color::alpha);
	m_text->set_padding(rhr::stack::argument::argument::padding);
	m_text->set_text(text);
}

void rhr::render::object::button_text::on_render()
{
	rhr::render::object::button::on_render();
	m_text->render();
}

void rhr::render::object::button_text::on_update_buffers()
{
	rhr::render::object::button::on_update_buffers();
	m_text->update_buffers();
}

void rhr::render::object::button_text::on_reload_swap_chain()
{
	rhr::render::object::button::on_reload_swap_chain();
	m_text->reload_swap_chain();
}

void rhr::render::object::button_text::post_position_update()
{
	rhr::render::object::button::post_position_update();
	m_text->set_super_position(m_position + m_super_position + glm::vec<2, i32>(0, 2));
}

void rhr::render::object::button_text::post_size_update()
{
	rhr::render::object::button::post_size_update();
	m_text->set_font_size(m_size.y - 4);
}
