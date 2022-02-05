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

void rhr::render::object::button_image::ui_transform_update(i_ui::transform_update_spec transform_update_spec)
{
	bool position_update = transform_update_spec & i_ui::transform_update_spec_position;
	bool size_update     = transform_update_spec & i_ui::transform_update_spec_size;

	if (size_update && !position_update)
	{
		// Update only size.

		update_child_transform(m_rectangle, 0x0);
		m_rectangle->set_size_max(true);
	}
	else if (size_update)
	{
		// Update size and position.

		update_child_transform(m_rectangle, i_ui::transform_update_spec_position);
		m_rectangle->set_size_max(true);
	}
	else if (position_update)
	{
		// Update only position.
		update_child_transform(m_rectangle, i_ui::transform_update_spec_position);
	}
}

void rhr::render::object::button_image::ui_render() { m_rectangle->render(); }

void rhr::render::object::button_image::ui_reload_swap_chain() { m_rectangle->reload_swap_chain(); }

void rhr::render::object::button_image::ui_update_buffers() {}

void rhr::render::object::button_image::ui_chain_update_buffers() { m_rectangle->update_buffers(); }
