#include "device.hpp"

#include "swapchain.hpp"

mac::device::state* mac::device::create(bool enable_validation, const std::string& application_name)
{
	auto* device_state = new mac::device::state {
		.instance = nullptr,
		.validation = enable_validation,
	};

	// VALIDATION DATA

	std::vector<const char*> std_glfw_extensions;
	vk::debug_utils_messenger_create_info debug_utils_messenger_create_info = {};

	device_state->validation_layers = {
		{ .name = "VK_LAYER_KHRONOS_validation", .validation_layer_importance = mac::validation_layer::importance::REQUIRED },
		{ .name = "VK_LAYER_KHRONOS_synchronization2", .validation_layer_importance = mac::validation_layer::importance::OPTIONAL },
		{ .name = "VK_LAYER_LUNARG_monitor", .validation_layer_importance = mac::validation_layer::importance::OPTIONAL },
		{ .name = "VK_LAYER_LUNARG_screenshot", .validation_layer_importance = mac::validation_layer::importance::OPTIONAL },
		{ .name = "VK_LAYER_NV_optimus", .validation_layer_importance = mac::validation_layer::importance::OPTIONAL }
	};

	// BUILD INSTANCE

	vk::application_info application_info = {};
	vk::instance_create_info instance_create_info = {};

	application_info.sType				  = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	application_info.pApplicationName	  = application_name.c_str();
	application_info.applicationVersion	  = VK_MAKE_API_VERSION(0, 0, 0, 0);
	application_info.pEngineName		  = "No Engine";
	application_info.engineVersion		  = VK_MAKE_API_VERSION(0, 0, 0, 0);
	application_info.apiVersion			  = VK_API_VERSION_1_2;
	application_info.pNext				  = nullptr;

	u32 glfw_extension_count = 0;
	const char** glfw_extensions;
	glfw_extensions = glfw::get_required_instance_extensions(&glfw_extension_count);

	instance_create_info.sType					 = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instance_create_info.pApplicationInfo		 = &application_info;

	if (enable_validation)
	{
		std_glfw_extensions = std::vector<const char*>(glfw_extensions, glfw_extensions + glfw_extension_count);
		std_glfw_extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

		instance_create_info.enabledExtensionCount		 = static_cast<u32>(std_glfw_extensions.size());
		instance_create_info.ppEnabledExtensionNames	 = std_glfw_extensions.data();
	}
	else
	{
		instance_create_info.enabledExtensionCount		 = glfw_extension_count;
		instance_create_info.ppEnabledExtensionNames	 = glfw_extensions;
	}

	if (enable_validation)
	{
		populate_debug_utils_messenger_create_info(debug_utils_messenger_create_info);

		mac::device::shave_validation_layer_support(device_state->validation_layers);
		mac::validation_layer::simplify(device_state->validation_layers, device_state->validation_layers_simplified);

		instance_create_info.enabledLayerCount   = static_cast<u32>(device_state->validation_layers_simplified.size());
		instance_create_info.ppEnabledLayerNames = device_state->validation_layers_simplified.data();
		instance_create_info.pNext = &debug_utils_messenger_create_info;
	}
	else
	{
		instance_create_info.enabledLayerCount   = 0;
		instance_create_info.ppEnabledLayerNames = nullptr;
		instance_create_info.pNext = nullptr;
	}

	if (vk::create_instance(&instance_create_info, nullptr, &device_state->instance) != VK_SUCCESS)
	{
		latte::logger::fatal(latte::logger::level::SYSTEM, "failed to create vulkan instance");
		delete device_state;
		return nullptr;
	}

	return device_state;
}

void mac::device::destroy(mac::device::state* device_state)
{
	delete device_state;
}

void mac::device::setup_rendering(mac::device::state* device_state, vk::surface_khr* surface)
{
	// SETUP DEBUG OPERATIONS

	if (device_state->validation)
	{
		vk::debug_utils_messenger_create_info debug_utils_messenger_create_info = {};
		populate_debug_utils_messenger_create_info(debug_utils_messenger_create_info);

		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vk::get_instance_proc_addr(device_state->instance, "vkCreateDebugUtilsMessengerEXT");

		if (func == nullptr)
		{
			latte::logger::fatal(latte::logger::level::SYSTEM, __FILE__, __LINE__,"failed to setup debug utils messenger");
			return;
		}

		if (func(device_state->instance, &debug_utils_messenger_create_info, nullptr, &device_state->debug_utils_messenger_ext) != VK_SUCCESS)
		{
			latte::logger::fatal(latte::logger::level::SYSTEM, __FILE__, __LINE__,"failed to setup debug utils messenger");
			return;
		}
	}

	// CHOOSE PHYSICAL DEVICE

	std::array<const char*, 1> device_extensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	u32 device_count = 0;
	vk::enumerate_physical_device(device_state->instance, &device_count, nullptr);

	if (device_count == 0)
	{
		latte::logger::fatal(latte::logger::level::SYSTEM, "no supported vulkan devices found");
		return;
	}

	std::vector<vk::physical_device> devices(device_count);
	vk::enumerate_physical_device(device_state->instance, &device_count, devices.data());

	auto function_is_device_suitable = [surface, &device_extensions](vk::physical_device& device)
	{
		auto device_queue_family_indices = mac::swapchain::find_queue_families(device, surface);

		u32 extension_count;
		vk::enumerate_device_extension_properties(device, nullptr, &extension_count, nullptr);

		std::vector<vk::extension_properties> available_extensions(extension_count);
		vk::enumerate_device_extension_properties(device, nullptr, &extension_count, available_extensions.data());

		for (const auto& required_extension : device_extensions)
		{
			bool found_extension = false;
			for (const auto& extension : available_extensions)
			{
				if (strcmp(extension.extensionName, required_extension) == 0)
				{
					found_extension = true;
					break;
				}
			}

			if (!found_extension)
			{
				latte::logger::fatal(latte::logger::level::SYSTEM, "failed to present required device extensions");
				return false;
			}
		}

		auto swap_chain_support = mac::swapchain::query_swapchain_support(device, surface);
		bool swap_chain_adequate = !swap_chain_support.formats.empty() && !swap_chain_support.present_modes.empty();

		vk::physical_device_features physical_device_features;
		vk::get_physical_device_features(device, &physical_device_features);

		return device_queue_family_indices.complete() && swap_chain_adequate && physical_device_features.samplerAnisotropy;
	};

	for (auto& device : devices)
	{
		if (function_is_device_suitable(device))
		{
			device_state->physical_device = device;
			break;
		}
	}

	if (device_state->physical_device == nullptr)
	{
		latte::logger::fatal(latte::logger::level::SYSTEM, "failed to pick appropriate device");
		return;
	}

	// CREATE LOGICAL DEVICE

	auto indices = mac::swapchain::find_queue_families(device_state->physical_device, surface);
	std::set<u32> unique_queue_families = {indices.graphics_family.value(), indices.present_family.value()};
	std::vector<vk::device_queue_create_info> device_queue_create_infos;

	f32 queue_priority = 1.0f;
	for (u32 queue_family : unique_queue_families)
	{
		vk::device_queue_create_info queue_create_info = {};
		queue_create_info.sType						   = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queue_create_info.queueFamilyIndex			   = queue_family;
		queue_create_info.queueCount				   = 1;
		queue_create_info.pQueuePriorities			   = &queue_priority;
		device_queue_create_infos.emplace_back(queue_create_info);
	}

	vk::device_queue_create_info device_queue_create_info = {};
	vk::physical_device_features physical_device_features = {};
	vk::device_create_info device_create_info 			  = {};

	device_queue_create_info.sType					   = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	device_queue_create_info.queueFamilyIndex		   = indices.graphics_family.value();
	device_queue_create_info.queueCount				   = 1;
	device_queue_create_info.pQueuePriorities		   = &queue_priority;

	physical_device_features.samplerAnisotropy = VK_TRUE;

	device_create_info.sType				   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	device_create_info.queueCreateInfoCount	   = static_cast<u32>(device_queue_create_infos.size());
	device_create_info.pQueueCreateInfos	   = device_queue_create_infos.data();
	device_create_info.pEnabledFeatures		   = &physical_device_features;
	device_create_info.enabledExtensionCount   = static_cast<u32>(device_extensions.size());
	device_create_info.ppEnabledExtensionNames = device_extensions.data();

	if (device_state->validation)
	{
		device_create_info.enabledLayerCount   = static_cast<u32>(device_state->validation_layers_simplified.size());
		device_create_info.ppEnabledLayerNames = device_state->validation_layers_simplified.data();
	}
	else
		device_create_info.enabledLayerCount = 0;

	if (vk::create_device(device_state->physical_device, &device_create_info, nullptr, &device_state->logical_device) != VK_SUCCESS)
	{
		latte::logger::fatal(latte::logger::level::SYSTEM, "failed to create logical device link to gpu");
		return;
	}

	vk::get_device_queue(device_state->logical_device, indices.graphics_family.value(), 0, &device_state->graphics_queue);
}

bool mac::device::shave_validation_layer_support(std::vector<mac::validation_layer>& validation_layers)
{
	std::vector<vk::layer_properties> available_validation_layers;

	u32 layer_count;
	vk::enumerate_instance_layer_properties(&layer_count, nullptr);
	available_validation_layers.resize(layer_count);
	vk::enumerate_instance_layer_properties(&layer_count, available_validation_layers.data());

	for (i32 i = 0; i < validation_layers.size(); i++)
	{
		auto& validation_layer = validation_layers[i];
		bool found_validation_layer = false;

		for (const auto& available_validation_layer : available_validation_layers)
		{
			if (validation_layer.name == available_validation_layer.layerName)
			{
				latte::logger::info(latte::logger::level::SYSTEM, "validation layer \"" + validation_layer.name + "\" loaded");
				found_validation_layer = true;
				break;
			}
		}

		if (!found_validation_layer)
		{
			if (validation_layer.validation_layer_importance == mac::validation_layer::importance::OPTIONAL)
			{
				latte::logger::warn(latte::logger::level::SYSTEM, "optional validation layer \"" + validation_layer.name + "\" is not supported");
				found_validation_layer = false;
				break;
			}
			else if (validation_layer.validation_layer_importance == mac::validation_layer::importance::REQUIRED)
			{
				latte::logger::fatal(latte::logger::level::SYSTEM, "required validation layer \"" + validation_layer.name + "\" is not supported");
				return false;
			}

			validation_layers.erase(validation_layers.begin() + i--);
		}
	}

	return true;
}

void mac::device::populate_debug_utils_messenger_create_info(
	vk::debug_utils_messenger_create_info& debug_utils_messenger_create_info
)
{
	debug_utils_messenger_create_info					= {};
	debug_utils_messenger_create_info.sType			    = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	debug_utils_messenger_create_info.messageSeverity   = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	debug_utils_messenger_create_info.messageType       = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	debug_utils_messenger_create_info.pfnUserCallback   = debug_callback;
}

VkBool32 mac::device::debug_callback(
	VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, VkDebugUtilsMessageTypeFlagsEXT message_type,
	const VkDebugUtilsMessengerCallbackDataEXT* callback_data, void* user_data
)
{
	if (message_severity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
		latte::logger::warn(latte::logger::level::SYSTEM, std::string(callback_data->pMessage));
	else if (message_severity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
		latte::logger::error(latte::logger::level::SYSTEM, std::string(callback_data->pMessage));

	return VK_FALSE;
}
