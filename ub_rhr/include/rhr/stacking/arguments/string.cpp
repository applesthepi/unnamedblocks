#include "string.hpp"

#include "rhr/rendering/renderer.hpp"
#include "rhr/stacking/block.hpp"
#include "rhr/stacking/plane.hpp"

rhr::stack::argument::string::string(const cap::color& color, std::function<void()>* function_collection_update)
	: rhr::stack::argument::argument(color, function_collection_update)
	, m_text(std::make_shared<rhr::render::object::text>(
		  rhr::registry::char_texture::texture_type::LIGHT_NORMAL, rhr::stack::block::height_content, function_collection_update, false, true))
{
	m_mouse_button = [&, color](glm::vec<2, i32> position, f32 scroll, MouseOperation operation, MouseButton button)
	{
		if (button == MouseButton::MIDDLE && operation == MouseOperation::Press)
		{
			if (m_mode == cap::mod::block::block::argument::variable_mode::RAW)
				set_mode(cap::mod::block::block::argument::variable_mode::VAR);
			else if (m_mode == cap::mod::block::block::argument::variable_mode::VAR)
				set_mode(cap::mod::block::block::argument::variable_mode::RAW);
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

cap::mod::block::block::argument::type rhr::stack::argument::string::get_type() { return cap::mod::block::block::argument::type::STRING; }

u32 rhr::stack::argument::string::get_width() { return m_text->get_size_local().x; }

bool rhr::stack::argument::string::has_data() { return true; }

void rhr::stack::argument::string::ui_transform_update(i_ui::transform_update_spec transform_update_spec) { update_child_transform(m_text, true); }

void rhr::stack::argument::string::ui_render() { m_text->render(); }

void rhr::stack::argument::string::ui_reload_swap_chain() { m_text->reload_swap_chain(); }

void rhr::stack::argument::string::ui_update_buffers() {}

void rhr::stack::argument::string::ui_chain_update_buffers()
{
	m_text->update_buffers();
}

void rhr::stack::argument::string::ui_frame_update(f64 delta_time) {}

void rhr::stack::argument::string::on_set_data() { m_text->set_text(m_data); }

bool rhr::stack::argument::string::drag_bounds(glm::vec<2, i32> position)
{
	const glm::vec<2, i32>& arg_position = get_position_virtual_absolute();

	return (
		position.x > arg_position.x && position.x < arg_position.x + get_width() && position.y > arg_position.y && position.y < arg_position.y + rhr::stack::block::height_content);
}

rhr::stack::argument::argument::padding_style rhr::stack::argument::string::get_padding_style() { return rhr::stack::argument::argument::padding_style::HARD; }

const std::string& rhr::stack::argument::string::get_data() { return m_text->get_text(); }

void rhr::stack::argument::string::on_set_mode(cap::mod::block::block::argument::variable_mode mode)
{
	if (m_mode == cap::mod::block::block::argument::variable_mode::RAW)
		m_text->set_color_secondary(m_block_color);
	else if (m_mode == cap::mod::block::block::argument::variable_mode::VAR)
		m_text->set_color_secondary(cap::color().from_u8({100, 0, 40, 255}));
}
