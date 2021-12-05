#pragma once
#include "config.h"

#include "device.hpp"

#include <cappuccino/utils.hpp>

namespace rhr::render::component
{
///
class window
{
public:
	window(std::string title, glm::vec<2, i32> window_size);

	///
	void initialize_components();

	///
	void register_paired_pipeline(
		const std::string& name, const std::string& shader_color, const std::string& shader_texture,
		vk::cull_mode_flags cull_mode_flags = VK_CULL_MODE_NONE
	);

	///
	void bind_color_pipeline(vk::descriptor_set* descriptor_set);

	///
	void bind_texture_pipeline(vk::descriptor_set* descriptor_set);

	///
	void set_active_command_buffer(u8 idx);

	///
	void set_window_position(glm::vec<2, i32> position);

	///
	void apply_active_pipeline(const std::string& name);

	///
	glfw::window* get_window();

	///
	const glm::vec<2, i32>& get_window_size();

	///
	const glm::vec<2, i32>& get_window_position();

	///
	bool get_swapchain_recreation_flag();

	///
	vk::surface_khr* get_surface();

	///
	vk::surface_format_khr* get_surface_format();

	///
	vk::instance* get_instance();

	///
	vk::physical_device* get_physical_device();

	///
	vk::device* get_device();

	///
	vk::format* get_swapchain_format();

	///
	vk::extent_2d* get_swapchain_extent();

	///
	vk::descriptor_set_layout* get_descriptor_set_layout();

	///
	vk::render_pass* get_render_pass(u8 idx);

	///
	u8 get_framebuffer_count();

	///
	vk::frame_buffer* get_framebuffer(u8 idx);

	///
	vk::pipeline* get_color_pipeline(const std::string& name);

	///
	vk::pipeline* get_texture_pipeline(const std::string& name);

	///
	vk::command_buffer* get_active_command_buffer();

	///
	vk::swapchain_khr* get_swapchain();

	///
	vk::present_mode_khr* get_present_mode();

	///
	vk::queue* get_graphics_queue();

	///
	vk::command_pool* get_command_pool();

	///
	vk::command_buffer* get_master_command_buffer(u8 idx);

	///
	vk::command_buffer* get_panel_command_buffer(u8 idx);

	///
	rhr::render::component::frame* get_frame(u8 idx);

	///
	vk::descriptor_pool* get_descriptor_pool();

	///
	vk::sampler* get_texture_sampler();

	///
	void flag_swapchain_recreation(glm::vec<2, i32> window_size);

	///
	void flag_swapchain_recreation();

	///
	void flag_clear_swapchain_recreation();

	///
	void update_window_position(glm::vec<2, i32> window_position);
private:
	///
	glfw::window* m_window;

	///
	bool m_recreate_swapchain;

	///
	bool m_valid;

	///
	glm::vec<2, i32> m_window_size;

	///
	glm::vec<2, i32> m_window_position;

    ///
    std::string m_window_title;

	///
	std::unique_ptr<rhr::render::component::device> m_device;

	///
	vk::surface_khr m_surface;
};
}