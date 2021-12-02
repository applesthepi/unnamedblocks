#pragma once
#include "config.h"

#include <cappuccino/utils.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

namespace rhr::render::components
{
///
class device
{
public:
	///
	void init_instance();

	///
	static void init_debug();

	///
	static void init_device();

	///
	static void init_logical_device();

	///
	static VkInstance instance;

	///
	static VkDebugUtilsMessengerEXT debug_messenger;

	///
	static VkPhysicalDevice physical_device;

	///
	static VkDevice device_master;
};
}