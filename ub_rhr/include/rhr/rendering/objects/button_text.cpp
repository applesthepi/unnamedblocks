#include "button_text.hpp"

#include "rhr/rendering/renderer.hpp"
#include "rhr/stacking/arguments/argument.hpp"

rhr::render::object::button_text::button_text(const cap::color& primary_color, const cap::color& secondary_color, const std::string& text)
	: rhr::render::object::button(primary_color, secondary_color)
	, m_text(std::make_shared<rhr::render::object::text>(rhr::registry::char_texture::texture_type::BOLD_NORMAL, 4, nullptr, true, false))
{
	m_text->set_weak(m_text);
	m_text->set_weak_field(m_text);
	m_text->set_depth(rhr::render::renderer::depth_ui_text);
	m_text->set_color_primary(primary_color);
	m_text->set_color_secondary(cap::color::alpha);
	m_text->set_padding(rhr::stack::argument::argument::padding);
	m_text->set_text(text);
}

void rhr::render::object::button_text::ui_transform_update(i_ui::transform_update_spec transform_update_spec)
{
	bool position_update = transform_update_spec & i_ui::transform_update_spec_position;
	bool size_update     = transform_update_spec & i_ui::transform_update_spec_size;

	if (size_update && !position_update)
	{
		// Update only size.

		update_child_transform(m_text, 0x0);

		i32 font_size = get_size_local().y - 4;
		if (font_size <= 4)
			m_text->set_font_size(static_cast<u16>(get_size_local().y));
		else
			m_text->set_font_size(get_size_local().y - 4);
	}
	else if (size_update)
	{
		// Update size and position.

		update_child_transform(m_text, 0x0);
		m_text->set_position_parent_physical(m_text->get_position_parent_physical() + glm::vec<2, i32>(0, 2), true);

		i32 font_size = get_size_local().y - 4;
		if (font_size <= 4)
			m_text->set_font_size(static_cast<u16>(get_size_local().y));
		else
			m_text->set_font_size(get_size_local().y - 4);
	}
	else if (position_update)
	{
		// Update only position.

		update_child_transform(m_text, 0x0);
		m_text->set_position_parent_physical(m_text->get_position_parent_physical() + glm::vec<2, i32>(0, 2), true);
	}
}

void rhr::render::object::button_text::ui_render()
{
	rhr::render::object::button::ui_render();
	m_text->render();
}

void rhr::render::object::button_text::ui_reload_swap_chain()
{
	rhr::render::object::button::ui_reload_swap_chain();
	m_text->reload_swap_chain();
}

void rhr::render::object::button_text::ui_update_buffers() {}

void rhr::render::object::button_text::ui_chain_update_buffers()
{
	m_text->update_buffers();
}

void rhr::render::object::button_text::ui_frame_update(f64 delta_time) { rhr::render::object::button::ui_frame_update(delta_time); }
