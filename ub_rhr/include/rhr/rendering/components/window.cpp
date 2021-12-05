#include "window.hpp"

#include "rhr/handlers/input.hpp"

namespace rhr::render::component::window_callback
{
void framebuffer_resize(glfw::window* glfw_window, i32 width, i32 height)
{
	auto window = reinterpret_cast<rhr::render::component::window*>(glfw::get_window_user_pointer(glfw_window));
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
	auto window = reinterpret_cast<rhr::render::component::window*>(glfw::get_window_user_pointer(glfw_window));
	window->update_window_position({ x, y });
}
}

rhr::render::component::window::window(std::string title, glm::vec<2, i32> window_size)
	: m_window{}
	, m_recreate_swapchain(false)
	, m_valid(false)
	, m_window_size(window_size)
	, m_window_position({ 0, 0 })
    , m_window_title(std::move(title))
	, m_surface{}
{
	glfw::set_window_hint(GLFW_CLIENT_API, GLFW_NO_API);
	glfw::set_window_hint(GLFW_RESIZABLE, GL_TRUE);

	m_window = glfw::create_window(m_window_size.x, m_window_size.y, m_window_title.c_str(), nullptr, nullptr);

	if (m_window == nullptr)
	{
		cap::logger::error("failed to create window");
		return;
	}

	m_device = std::make_unique<rhr::render::component::device>();

	if (glfw::create_window_surface(m_device->get_instance(), m_window, nullptr, &m_surface) != VK_SUCCESS)
	{
		cap::logger::error("failed to create window surface");
		return;
	}

	glfw::set_framebuffer_size_callback(m_window, rhr::render::component::window_callback::framebuffer_resize);
	glfw::set_key_callback(m_window, rhr::render::component::window_callback::key);
	glfw::set_mouse_button_callback(m_window, rhr::render::component::window_callback::mouse_button);
	glfw::set_scroll_callback(m_window, rhr::render::component::window_callback::scroll);
	glfw::set_cursor_position_callback(m_window, rhr::render::component::window_callback::cursor_position);
	glfw::set_window_position_callback(m_window, rhr::render::component::window_callback::window_position);

	glfw::set_window_user_pointer(m_window, reinterpret_cast<void*>(this));
	glfw::get_window_position(m_window, &m_window_position.x, &m_window_position.y);
}

void rhr::render::component::window::initialize_components()
{
	m_device->initialize(&m_surface);
	m_valid = true;
}

void rhr::render::component::window::bind_color_pipeline(vk::descriptor_set* descriptor_set)
{
	m_device->get_pipeline()->bind_color_pipeline(descriptor_set);
}

void rhr::render::component::window::bind_texture_pipeline(vk::descriptor_set* descriptor_set)
{
	m_device->get_pipeline()->bind_texture_pipeline(descriptor_set);
}

void rhr::render::component::window::set_active_command_buffer(u8 idx)
{
	m_device->get_command_pool()->set_active_command_buffer(idx);
}

void rhr::render::component::window::set_window_position(glm::vec<2, i32> position)
{
	m_window_position = std::move(position);
}

void rhr::render::component::window::apply_active_pipeline(const std::string& name)
{
	m_device->get_pipeline()->apply_active_pipeline(name);
}

glfw::window* rhr::render::component::window::get_window()
{
	return m_window;
}

const glm::vec<2, i32>& rhr::render::component::window::get_window_size()
{
	return m_window_size;
}

const glm::vec<2, i32>& rhr::render::component::window::get_window_position()
{
	return m_window_position;
}

vk::surface_khr* rhr::render::component::window::get_surface()
{
	return &m_surface;
}

vk::surface_format_khr* rhr::render::component::window::get_surface_format()
{
	return &m_device->get_swapchain()->get_surface_format();
}

vk::instance* rhr::render::component::window::get_instance()
{
	return &m_device->get_instance();
}

vk::physical_device* rhr::render::component::window::get_physical_device()
{
	return &m_device->get_physical_device();
}

vk::device* rhr::render::component::window::get_device()
{
	return &m_device->get_device();
}

vk::format* rhr::render::component::window::get_swapchain_format()
{
	return &m_device->get_swapchain()->get_format();
}

vk::extent_2d* rhr::render::component::window::get_swapchain_extent()
{
	return &m_device->get_swapchain()->get_extent();
}

vk::descriptor_set_layout* rhr::render::component::window::get_descriptor_set_layout()
{
	return &m_device->get_descriptor_set_layout();
}

vk::render_pass* rhr::render::component::window::get_render_pass(u8 idx)
{
	return &m_device->get_swapchain()->get_render_pass(idx);
}

u8 rhr::render::component::window::get_framebuffer_count()
{
	return m_device->get_swapchain()->get_framebuffer_count();
}

vk::frame_buffer* rhr::render::component::window::get_framebuffer(u8 idx)
{
	return &m_device->get_swapchain()->get_framebuffer(idx);
}

vk::pipeline* rhr::render::component::window::get_color_pipeline(const std::string& name)
{
	return &m_device->get_pipeline()->get_color_pipeline(name);
}

vk::pipeline* rhr::render::component::window::get_texture_pipeline(const std::string& name)
{
	return &m_device->get_pipeline()->get_texture_pipeline(name);
}

vk::command_buffer* rhr::render::component::window::get_active_command_buffer()
{
	return m_device->get_command_pool()->get_active_command_buffer();
}

vk::swapchain_khr* rhr::render::component::window::get_swapchain()
{
	return &m_device->get_swapchain()->get_swapchain();
}

vk::present_mode_khr* rhr::render::component::window::get_present_mode()
{
	return &m_device->get_swapchain()->get_present_mode();
}

vk::queue* rhr::render::component::window::get_graphics_queue()
{
	return &m_device->get_graphics_queue();
}

vk::command_pool* rhr::render::component::window::get_command_pool()
{
	return &m_device->get_command_pool()->get_command_pool();
}

vk::command_buffer* rhr::render::component::window::get_master_command_buffer(u8 idx)
{
	return m_device->get_command_pool()->get_master_command_buffer(idx);
}

vk::command_buffer* rhr::render::component::window::get_panel_command_buffer(u8 idx)
{
	return m_device->get_command_pool()->get_panel_command_buffer(idx);
}

rhr::render::component::frame* rhr::render::component::window::get_frame(u8 idx)
{
	return &m_device->get_swapchain()->get_frame(idx);
}

vk::descriptor_pool* rhr::render::component::window::get_descriptor_pool()
{
	return &m_device->get_command_pool()->get_descriptor_pool();
}

vk::sampler* rhr::render::component::window::get_texture_sampler()
{
	return &m_device->get_texture_sampler();
}

void rhr::render::component::window::flag_swapchain_recreation(glm::vec<2, i32> window_size)
{
	m_window_size = window_size;
	m_recreate_swapchain = true;
}

void rhr::render::component::window::flag_swapchain_recreation()
{
	m_recreate_swapchain = true;
}

void rhr::render::component::window::flag_clear_swapchain_recreation()
{
	m_recreate_swapchain = false;
}

void rhr::render::component::window::update_window_position(glm::vec<2, i32> window_position)
{
	m_window_position = window_position;
}

bool rhr::render::component::window::get_swapchain_recreation_flag()
{
	return m_recreate_swapchain;
}

void rhr::render::component::window::register_paired_pipeline(
	const std::string& name, const std::string& shader_color, const std::string& shader_texture,
	vk::cull_mode_flags cull_mode_flags
)
{
	m_device->get_pipeline()->register_paired_pipeline(name, shader_color, shader_texture, cull_mode_flags);
}
