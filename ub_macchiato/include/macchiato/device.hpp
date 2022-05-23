#pragma once
#include "config.h"

#include "validation_layer.hpp"

#include <latte/utils.hpp>

namespace mac::device
{
///
struct state
{
	///
	vk::instance instance;

	///
	bool validation;

	///
	std::vector<mac::validation_layer> validation_layers;

	///
	std::vector<const char*> validation_layers_simplified;

	///
	vk::debug_utils_messenger_ext debug_utils_messenger_ext;

	///
	vk::physical_device physical_device;

	///
	vk::device logical_device;

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
VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
	VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
	VkDebugUtilsMessageTypeFlagsEXT message_type,
	const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
	void* user_data);
}
