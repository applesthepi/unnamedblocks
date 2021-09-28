#pragma once
#include "config.h"

#include <cappuccino/utils.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

namespace rhr::render
{
///
class command
{
public:
	///
	static void init_command_pool();

	///
	static void init_descriptor_pool();

	///
	static void init_command_buffers();

	///
	static std::vector<VkCommandBuffer> command_buffers;

	///
	static VkCommandBuffer* active_command_buffer;

	///
	static std::vector<VkDeviceQueueCreateInfo> queue_create_infos;

	///
	static VkDescriptorSetLayout descriptor_set_layout;

	///
	static VkCommandPool command_pool;

	///
	static VkDescriptorPool descriptor_pool;
};
}