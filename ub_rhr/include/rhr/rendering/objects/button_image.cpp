#include "button_image.hpp"

#include "rhr/rendering/renderer.hpp"
#include "rhr/stacking/arguments/argument.hpp"

rhr::render::object::button_image::button_image(const std::string& texture)
	: rhr::render::object::button(cap::color::alpha, cap::color::alpha)
	, m_rectangle(std::make_shared<rhr::render::object::rectangle>())
{
	m_rectangle->set_weak(m_rectangle);
	m_rectangle->set_depth(rhr::render::renderer::depth_ui_background);
	m_rectangle->set_texture(texture);
}

void rhr::render::object::button_image::on_render()
{
	m_rectangle->render();
}

void rhr::render::object::button_image::on_update_buffers()
{
	m_rectangle->update_buffers();
}

void rhr::render::object::button_image::on_reload_swap_chain()
{
	m_rectangle->reload_swap_chain();
}

void rhr::render::object::button_image::post_transform_update()
{
	update_child_transform(m_rectangle, false);
	m_rectangle->set_size_max();
}