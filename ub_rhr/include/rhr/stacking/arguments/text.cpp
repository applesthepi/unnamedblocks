#include "text.hpp"

#include "rhr/rendering/renderer.hpp"
#include "rhr/stacking/block.hpp"

rhr::stack::argument::text::text(const cap::color& color, std::function<void()>* function_collection_update, glm::vec<2, i32>* plane_offset)
	: rhr::stack::argument::argument(color, function_collection_update, plane_offset)
	, m_text(std::make_shared<rhr::render::object::text>(
		  rhr::registry::char_texture::texture_type::BOLD_NORMAL, rhr::stack::block::height_content, true, false))
{
	m_text->set_weak(m_text);
	m_text->set_offset(plane_offset);
	m_text->set_weak_field(m_text);
	m_text->set_depth(rhr::render::renderer::depth_argument_text);
	m_text->set_color_primary(cap::color::black);
	m_text->set_color_secondary(color);
	m_text->set_padding(0);
	m_text->enable_background(false);
}

cap::mod::block::block::argument::type rhr::stack::argument::text::get_type() { return cap::mod::block::block::argument::type::TEXT; }

u32 rhr::stack::argument::text::get_width()
{
	//	cap::logger::info("arg text width: " + std::to_string(m_text->get_size_local().x));
	return m_text->get_size_local().x;
}

bool rhr::stack::argument::text::has_data() { return true; }

rhr::stack::argument::argument::padding_style rhr::stack::argument::text::get_padding_style() { return rhr::stack::argument::argument::padding_style::SOFT; }

void rhr::stack::argument::text::ui_transform_update(i_ui::transform_update_spec transform_update_spec) { update_child_transform(m_text, true); }

void rhr::stack::argument::text::ui_render() { m_text->render(); }

void rhr::stack::argument::text::ui_reload_swap_chain() { m_text->reload_swap_chain(); }

void rhr::stack::argument::text::ui_update_buffers() {}

void rhr::stack::argument::text::ui_chain_update_buffers()
{
	m_text->update_buffers();
}

void rhr::stack::argument::text::ui_frame_update(f64 delta_time) {}

void rhr::stack::argument::text::on_set_data() { m_text->set_text(m_data); }

void rhr::stack::argument::text::set_plane_offset(glm::vec<2, i32>* plane_offset)
{
	argument::set_plane_offset(plane_offset);
	m_text->set_offset(plane_offset);
}
