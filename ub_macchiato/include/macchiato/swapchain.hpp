#pragma once
#include "config.h"

#include <latte/utils.hpp>

namespace mac::swapchain
{
///
struct swap_chain_support_details
{
	///
	vk::surface_capabilities_khr capabilities;

	///
	std::vector<vk::surface_format_khr> formats;

	///
	std::vector<vk::present_mode_khr> present_modes;
};

///
struct queue_family_indices
{
	///
	std::optional<u32> graphics_family;

	///
	std::optional<u32> present_family;

	///
	[[nodiscard]] bool complete() const
	{
		return graphics_family.has_value() && present_family.has_value();
	}
};

///
struct state
{
	///
	vk::surface_format_khr surface_format;

	///
	vk::present_mode_khr present_mode;

	///
	vk::extent_2d extent;

	///
	vk::swapchain_khr swapchain;

	///
	u32 image_count;
};

///
mac::swapchain::state* create(vk::device& logical_device, vk::physical_device& physical_device, vk::surface_khr* surface, glfw::window* glfw_window);

///
void destroy(mac::swapchain::state* swapchain_state, vk::device& logical_device);

///
mac::swapchain::swap_chain_support_details query_swapchain_support(vk::physical_device& device, vk::surface_khr* surface);

///
mac::swapchain::queue_family_indices find_queue_families(vk::physical_device& device, vk::surface_khr* surface);
}