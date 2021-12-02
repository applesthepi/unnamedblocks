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
	explicit device();

	/// Needed for most operations. The window can exist before calling this, but every other vulkan operation requires
	/// this to be called beforehand.
	void initialize(vk::surface_khr* surface);

	///
	vk::instance& get_instance();

	///
	vk::physical_device& get_physical_device();

	///
	vk::device& get_device();

	///
	static bool validation_layers_enabled;
private:
	///
	vk::surface_khr* m_surface;

	///
	vk::instance m_instance;

	///
	vk::physical_device m_physical_device;

	///
	vk::device m_device;

	///
	vk::debug_utils_messenger_ext m_debug_messenger;

	///
	std::vector<vk::device_queue_create_info> m_queue_create_infos;

	///
	vk::queue m_graphics_queue;

	///
	bool m_valid;

    ///
    std::unique_ptr<rhr::render::components::swapchain> m_swapchain;
};
}