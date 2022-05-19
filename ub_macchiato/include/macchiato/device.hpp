#pragma once
#include "config.h"

#include "validation_layer.hpp"

#include <latte/utils.hpp>

namespace mac::device
{
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
struct state
{
	///
	vk::instance instance;

	///
	bool validation;

	///
	std::vector<const char*> validation_layers_simplified;

	///
	vk::debug_utils_messenger_ext debug_utils_messenger_ext;

	///
	vk::physical_device physical_device;

	///
	vk::device device;

	///
	vk::queue graphics_queue;
};

///
mac::device::state* create(bool enable_validation, const std::string& application_name);

///
void destroy(mac::device::state* device_state);

///
void setup_rendering(mac::device::state* device_state, vk::surface_khr* surface);

///
bool shave_validation_layer_support(std::vector<mac::validation_layer>& validation_layers);

///
void populate_debug_utils_messenger_create_info(vk::debug_utils_messenger_create_info& debug_utils_messenger_create_info);

///
mac::device::queue_family_indices find_queue_families(vk::physical_device& device, vk::surface_khr* surface);

///
mac::device::swap_chain_support_details query_swap_chain_support(vk::physical_device& device, vk::surface_khr* surface);

///
VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
	VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
	VkDebugUtilsMessageTypeFlagsEXT message_type,
	const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
	void* user_data);
}
