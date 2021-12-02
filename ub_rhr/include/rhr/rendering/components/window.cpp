#include "window.hpp"

#include <utility>

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

rhr::render::components::window::window(std::string title, glm::vec<2, i32> window_size)
	: m_window(nullptr)
	, m_recreate_swapchain(false)
	, m_valid(false)
	, m_window_size(window_size)
	, m_window_position({ 0, 0 })
    , m_window_title(std::move(title))
	, m_surface(nullptr)
{
	setup();
}

void rhr::render::components::window::create_systems()
{
	//rhr::render::swap_chain::init_swap_chain();
	//rhr::render::swap_chain::init_image_views();
	//rhr::render::render_pass::init_render_pass();
	//init_descriptor_set_layout();
	//rhr::render::pipeline::initialize();
	//rhr::render::command::init_command_pool();
	//init_depth_resources();
	//rhr::render::swap_chain::init_frame_buffers();
	//init_texture_sampler();
	//rhr::render::command::init_descriptor_pool();
	//rhr::render::command::init_command_buffers();
	//rhr::render::swap_chain::init_sync_objects();
}

void rhr::render::components::window::delete_systems()
{

}

vk::surface_khr* rhr::render::components::window::get_surface()
{
	return &m_surface;
}

vk::physical_device* rhr::render::components::window::get_physical_device()
{
	return &m_device->get_physical_device();
}

vk::device* rhr::render::components::window::get_device()
{
	return &m_device->get_device();
}

void rhr::render::components::window::flag_swapchain_recreation(glm::vec<2, i32> window_size)
{
	m_window_size = window_size;
	m_recreate_swapchain = true;
}

void rhr::render::components::window::update_window_position(glm::vec<2, i32> window_position)
{
	m_window_position = window_position;
}

void rhr::render::components::window::setup()
{
    m_window = glfw::create_window(m_window_size.x, m_window_size.y, m_window_title.c_str(), nullptr, nullptr);

    if (m_window == nullptr)
    {
        cap::logger::error("failed to create window");
        return;
    }

    m_device = std::make_unique<rhr::render::components::device>();

    if (glfw::create_window_surface(m_device->get_instance(), m_window, nullptr, &m_surface) != VK_SUCCESS)
    {
        cap::logger::error("failed to create window surface");
        return;
    }

    m_device->initialize(&m_surface);

    glfw::set_framebuffer_size_callback(m_window, rhr::render::components::window_callback::framebuffer_resize);
    glfw::set_key_callback(m_window, rhr::render::components::window_callback::key);
    glfw::set_mouse_button_callback(m_window, rhr::render::components::window_callback::mouse_button);
    glfw::set_scroll_callback(m_window, rhr::render::components::window_callback::scroll);
    glfw::set_cursor_position_callback(m_window, rhr::render::components::window_callback::cursor_position);
    glfw::set_window_position_callback(m_window, rhr::render::components::window_callback::window_position);

    glfw::set_window_user_pointer(m_window, reinterpret_cast<void*>(this));
    glfw::get_window_position(m_window, &m_window_position.x, &m_window_position.y);

    m_valid = true;
}
