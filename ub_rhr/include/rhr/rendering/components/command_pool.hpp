#pragma once
#include "config.h"

#include <cappuccino/utils.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

namespace rhr::render::component
{
///
class command_pool
{
public:
	command_pool();
	~command_pool();

	///
	void initialize_descriptor_pool();

	///
	void initialize_command_buffers();

	///
	void set_active_command_buffer(u8 idx);

	///
	vk::command_buffer* get_active_command_buffer();

	///
	vk::command_buffer* get_master_command_buffer(u8 idx);

	///
	vk::command_buffer* get_panel_command_buffer(u8 idx);

	///
	vk::command_pool& get_command_pool();

	///
	vk::descriptor_pool& get_descriptor_pool();
private:
	///
	void generate_command_buffer(u32 count, vk::command_buffer* command_buffer);

	///
	void setup_command_buffer(vk::command_buffer* command_buffer, vk::frame_buffer* frame_buffer);

	///
	std::vector<vk::command_buffer> m_command_buffer_panels;

	///
	std::vector<vk::command_buffer> m_command_buffer_master;

	///
	vk::command_buffer* m_active_command_buffer;

	///
	std::vector<vk::device_queue_create_info> m_queue_create_infos;

	///
	vk::descriptor_set_layout m_descriptor_set_layout;

	///
	vk::command_pool m_command_pool;

	///
	vk::descriptor_pool m_descriptor_pool;
};
}