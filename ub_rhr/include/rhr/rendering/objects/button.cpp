#include "button.hpp"

#include "rhr/rendering/renderer.hpp"

static void mouse_update_caller(glm::vec<2, i32> position, f32 scroll, MouseOperation operation, MouseButton button, void* data)
{
	rhr::render::object::button* button_object = (rhr::render::object::button*)data;
	button_object->mouse_update(position, scroll, operation);
}

rhr::render::object::button::button(const cap::color& primary_color, const cap::color& secondary_color)
	: i_dicolorable(primary_color, secondary_color)
	, m_background(std::make_shared<rhr::render::object::rectangle>())
	, m_callback(nullptr)
	, m_callback_data(nullptr)
	, m_enable_fill_width(false)
{
	m_background->set_weak(m_background);
	m_background->set_color(m_color_secondary);
	m_background->set_depth(rhr::render::renderer::depth_ui_background);

	set_size_local({ 100, 20 });

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
	if (!get_enabled())
		return;

	if (operation == MouseOperation::Click)
	{
		glm::vec<2, i32> button_position = get_position_physical_absolute();

		if (position.x >= button_position.x && position.x < button_position.x + get_size_local().x &&
			position.y >= button_position.y && position.y < button_position.y + get_size_local().y)
		{
			if (m_callback == nullptr)
				cap::logger::warn("button callback is nullptr");
			else
				m_callback(m_callback_data);
		}
	}
}

void rhr::render::object::button::enable_fill_width(bool enable)
{
	m_enable_fill_width = enable;
}

void rhr::render::object::button::ui_transform_update()
{
	if (m_enable_fill_width)
		set_size_local(get_size_local() + glm::vec<2, i32>(get_size_parent().x - get_position_local_physical().x, 0), false);

	update_child_transform(m_background, false);
	m_background->set_size_max();
}

void rhr::render::object::button::ui_render()
{
	m_background->render();
}

void rhr::render::object::button::ui_reload_swap_chain()
{
	m_background->reload_swap_chain();
}

void rhr::render::object::button::ui_update_buffers()
{
	m_background->update_buffers();
}

void rhr::render::object::button::ui_frame_update(f64 delta_time)
{

}

void rhr::render::object::button::post_color_update()
{
	m_background->set_color(m_color_secondary);
}
