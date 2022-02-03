#include "boolean.hpp"

#include "rhr/rendering/renderer.hpp"
#include "rhr/stacking/block.hpp"

#define ARG_BOOL_DECORE_WIDTH 10

rhr::stack::argument::boolean::boolean(const cap::color& color, std::function<void()>* function_collection_update)
	: argument(color, function_collection_update)
	, m_text(std::make_shared<rhr::render::object::text>(
		  rhr::registry::char_texture::texture_type::LIGHT_NORMAL, rhr::stack::block::height_content, function_collection_update, false, true))
	, m_decor_left(std::make_shared<rhr::render::object::object>(true))
	, m_decor_right(std::make_shared<rhr::render::object::object>(true))
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

	m_decor_left->set_weak(m_decor_left);
	m_decor_right->set_weak(m_decor_right);

	m_text->set_weak(m_text);
	m_text->set_weak_field(m_text);
	m_text->set_depth(rhr::render::renderer::depth_argument_text);
	m_text->set_color_primary(cap::color::text_primary_color);
	m_text->set_color_secondary(color);
	m_text->set_padding(padding);
	m_text->set_mouse_button(m_mouse_button);
}

cap::mod::block::block::argument::type rhr::stack::argument::boolean::get_type() { return cap::mod::block::block::argument::type::BOOL; }

u32 rhr::stack::argument::boolean::get_width() { return m_text->get_size_local().x + (2 * ARG_BOOL_DECORE_WIDTH); }

bool rhr::stack::argument::boolean::has_data() { return true; }

void rhr::stack::argument::boolean::ui_transform_update(i_ui::transform_update_spec transform_update_spec)
{
	update_child_transform(m_text);
	m_text->set_position_local_physical({ARG_BOOL_DECORE_WIDTH, 0});

	glm::vec<2, f64> pos = get_position_physical_absolute();
	m_decor_left->set_super_position({pos.x, pos.y, rhr::render::renderer::depth_argument});
	m_decor_right->set_super_position({pos.x, pos.y, rhr::render::renderer::depth_argument});

	mark_dirty();
}

void rhr::stack::argument::boolean::ui_render()
{
	m_text->render();

	m_decor_left->render();
	m_decor_right->render();
}

void rhr::stack::argument::boolean::ui_reload_swap_chain()
{
	m_text->reload_swap_chain();

	m_decor_left->reload_swap_chain();
	m_decor_right->reload_swap_chain();
}

void rhr::stack::argument::boolean::ui_update_buffers()
{
	m_text->update_buffers();

	i32 fullWidth = get_width();

	cap::color use_color = m_block_color;
	if (m_mode == cap::mod::block::block::argument::variable_mode::VAR)
		use_color = cap::color().from_u8({100, 0, 40, 255});

	rhr::render::vertex vertices[6];

	vertices[0] = rhr::render::vertex({static_cast<f32>(ARG_BOOL_DECORE_WIDTH), 0.0f, 0}, use_color.get_normalized(), {0.0f, 0.0f});
	vertices[1] = rhr::render::vertex({static_cast<f32>(0.0f), 0.0f /*static_cast<f32>(rhr::stack::block::height_content / 2)*/, 0}, use_color.get_normalized(), {1.0f, 0.0f});
	vertices[2] = rhr::render::vertex({static_cast<f32>(ARG_BOOL_DECORE_WIDTH), static_cast<f32>(rhr::stack::block::height_content), 0}, use_color.get_normalized(), {1.0f, 1.0f});

	vertices[3] = rhr::render::vertex({static_cast<f32>(fullWidth - ARG_BOOL_DECORE_WIDTH), 0.0f, 0}, use_color.get_normalized(), {0.0f, 0.0f});
	vertices[4] = rhr::render::vertex({static_cast<f32>(fullWidth), static_cast<f32>(rhr::stack::block::height_content /* / 2*/), 0}, use_color.get_normalized(), {1.0f, 0.0f});
	vertices[5] = rhr::render::vertex(
		{static_cast<f32>(fullWidth - ARG_BOOL_DECORE_WIDTH), static_cast<f32>(rhr::stack::block::height_content), 0}, use_color.get_normalized(), {1.0f, 1.0f});

	m_decor_left->update_vertices(vertices + 0, 3, true);
	m_decor_right->update_vertices(vertices + 3, 3, true);
}

void rhr::stack::argument::boolean::ui_chain_update_buffers()
{

}

void rhr::stack::argument::boolean::ui_frame_update(f64 delta_time) {}

void rhr::stack::argument::boolean::on_set_data() { m_text->set_text(m_data); }

bool rhr::stack::argument::boolean::drag_bounds(glm::vec<2, i32> position)
{
	glm::vec<2, i32> arg_position = get_position_virtual_absolute();

	return (
		position.x > arg_position.x && position.x < arg_position.x + get_width() && position.y > arg_position.y && position.y < arg_position.y + rhr::stack::block::height_content);
}

rhr::stack::argument::argument::padding_style rhr::stack::argument::boolean::get_padding_style() { return rhr::stack::argument::argument::padding_style::NONE; }

const std::string& rhr::stack::argument::boolean::get_data() { return m_text->get_text(); }

void rhr::stack::argument::boolean::on_set_mode(cap::mod::block::block::argument::variable_mode mode)
{
	if (m_mode == cap::mod::block::block::argument::variable_mode::RAW)
		m_text->set_color_secondary(m_block_color);
	else if (m_mode == cap::mod::block::block::argument::variable_mode::VAR)
		m_text->set_color_secondary(cap::color().from_u8({100, 0, 40, 255}));
}
