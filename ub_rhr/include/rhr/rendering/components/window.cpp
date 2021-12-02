#include "window.hpp"

#include "rhr/handlers/input.hpp"

///
namespace rhr::render::components::window_callback
{
void framebuffer_resize(glfw::window* glfw_window, i32 width, i32 height)
{
	auto window = reinterpret_cast<rhr::render::components::window*>(glfw::get_window_user_pointer(glfw_window));
	window->flag_swapchain_recreation({ width, height });
}

void key(glfw::window* glfw_window, i32 key, i32 scancode, i32 action, i32 mode)
{
	InputHandler::FireKey(static_cast<i16>(key), action);
}

void mouse_button(glfw::window* glfw_window, i32 button, i32 action, i32 mods)
{
	InputHandler::FireMouseButton(button, action);
}

void scroll(glfw::window* glfw_window, f64 x_offset, f64 y_offset)
{
	InputHandler::FireMouseScroll(static_cast<f32>(y_offset));
}

void cursor_position(glfw::window* glfw_window, f64 x_position, f64 y_position)
{
	InputHandler::FireMouseMove({ static_cast<i32>(x_position), static_cast<i32>(y_position) });
}

void window_position(glfw::window* glfw_window, i32 x, i32 y)
{
	auto window = reinterpret_cast<rhr::render::components::window*>(glfw::get_window_user_pointer(glfw_window));
	window->update_window_position({ x, y });
}
}

rhr::render::components::window::window(const std::string& title, const glm::vec<2, i32>& window_size)
	: m_window(nullptr)
	, m_recreate_swapchain(false)
	, m_window_size(window_size)
	, m_window_position({ 0, 0 })
{
	m_window = glfw::create_window(window_size.x, window_size.y, title.c_str(), nullptr, nullptr);

	if (m_window == nullptr)
	{
		cap::logger::error("failed to create window");
		return;
	}

	glfw::set_framebuffer_size_callback(m_window, rhr::render::components::window_callback::framebuffer_resize);
	glfw::set_key_callback(m_window, rhr::render::components::window_callback::key);
	glfw::set_mouse_button_callback(m_window, rhr::render::components::window_callback::mouse_button);
	glfw::set_scroll_callback(m_window, rhr::render::components::window_callback::scroll);
	glfw::set_cursor_position_callback(m_window, rhr::render::components::window_callback::cursor_position);
	glfw::set_window_position_callback(m_window, rhr::render::components::window_callback::window_position);

	glfw::set_window_user_pointer(m_window, reinterpret_cast<void*>(this));
	glfw::get_window_position(m_window, &m_window_position.x, &m_window_position.y);
}

void rhr::render::components::window::flag_swapchain_recreation(const glm::vec<2, i32>& window_size)
{
	m_window_size = window_size;
	m_recreate_swapchain = true;
}

void rhr::render::components::window::update_window_position(const glm::vec<2, i32>& window_position)
{
	m_window_position = window_position;
}
