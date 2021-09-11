#include "button.hpp"

#include "rhr/rendering/renderer.hpp"

static void mouse_update_caller(glm::vec<2, i32> position, f32 scroll, MouseOperation operation, MouseButton button, void* data)
{
	rhr::render::object::button* button_object = (rhr::render::object::button*)data;
	button_object->mouse_update(position, scroll, operation);
}

rhr::render::object::button::button(const cap::color& primary_color, const cap::color& secondary_color)
	: i_enableable(true)
	, i_dicolorable(primary_color, secondary_color)
	, m_background(std::make_shared<rhr::render::object::rectangle>())
	, m_callback(nullptr)
	, m_callback_data(nullptr)
	, m_enable_fill_width(false)
{
	m_background->set_weak(m_background);
	m_background->set_color(m_color_secondary);
	m_background->set_depth(rhr::render::renderer::depth_ui_background);

	set_position({ 0, 0 });
	set_size({ 100, 20 });

	InputHandler::RegisterMouseCallback(mouse_update_caller, this);
}

rhr::render::object::button::~button()
{
	InputHandler::UnregisterMouseCallback(mouse_update_caller);
}

void rhr::render::object::button::set_depth(i32 depth)
{
//	m_background->set_depth(depth);
}

void rhr::render::object::button::set_callback(void(*callback)(void*), void* data)
{
	m_callback = callback;
	m_callback_data = data;
}

void rhr::render::object::button::mouse_update(glm::vec<2, i32> position, f32 scroll, MouseOperation operation)
{
	if (!m_enabled)
		return;

	if (operation == MouseOperation::Click)
	{
		glm::vec<2, i32> button_position = m_position + m_super_position;

		if (position.x >= button_position.x && position.x < button_position.x + m_size.x &&
			position.y >= button_position.y && position.y < button_position.y + m_size.y)
		{
			if (m_callback == nullptr)
				Logger::Warn("button callback is nullptr");
			else
				m_callback(m_callback_data);
		}
	}
}

void rhr::render::object::button::enable_fill_width(bool enable)
{
	m_enable_fill_width = enable;
}

void rhr::render::object::button::on_render()
{
	m_background->render();
}

void rhr::render::object::button::on_update_buffers()
{
	m_background->update_buffers();
}

void rhr::render::object::button::on_reload_swap_chain()
{
	m_background->reload_swap_chain();
}

void rhr::render::object::button::post_position_update()
{
	m_background->set_super_position(m_position + m_super_position);
}

void rhr::render::object::button::post_size_update()
{
	if (m_enable_fill_width)
		m_size.x = m_super_bounds.x - m_position.x;

	m_background->set_size(m_size);
}

void rhr::render::object::button::post_color_update()
{
	m_background->set_color(m_color_secondary);
}
