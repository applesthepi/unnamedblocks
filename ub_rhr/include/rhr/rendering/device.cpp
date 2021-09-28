#include "device.hpp"

#include "rhr/rendering/swap_chain.hpp"
#include "rhr/rendering/tools.hpp"
#include "rhr/rendering/renderer.hpp"
#include "rhr/rendering/command.hpp"

void rhr::render::device::init_instance()
{
	vk::application_info app_info;
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pApplicationName = "Unnamed Blocks";
	app_info.applicationVersion = VK_MAKE_API_VERSION(0, 0, 0, 0);
	app_info.pEngineName = "No Engine";
	app_info.engineVersion = VK_MAKE_API_VERSION(0, 0, 0, 0);
	app_info.apiVersion = VK_API_VERSION_1_0;
	app_info.pNext = NULL;

	vk::instance_create_info create_info{};
	create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	create_info.pApplicationInfo = &app_info;

	u32 glfw_extension_count = 0;
	const char** glfw_extensions;

	glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

	auto required_extensions = rhr::render::tools::get_required_extensions();
	create_info.enabledExtensionCount = static_cast<u32>(required_extensions.size());
	create_info.ppEnabledExtensionNames = required_extensions.data();

	VkDebugUtilsMessengerCreateInfoEXT debug_create_info;
	if (rhr::render::renderer::enable_validation_layers)
	{
		//		rhr::render::tools::validation_layers.push_back("VK_LAYER_LUNARG_standard_validation");
		//		rhr::render::tools::validation_layers.push_back("VK_EXT_debug_report");

		create_info.enabledLayerCount = static_cast<u32>(rhr::render::tools::validation_layers.size());
		create_info.ppEnabledLayerNames = rhr::render::tools::validation_layers.data();

		rhr::render::tools::populate_debug_messenge_create_info(debug_create_info);
		create_info.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debug_create_info;
	}
	else
	{
		create_info.enabledLayerCount = 0;
		create_info.pNext = nullptr;
	}

	if (vkCreateInstance(&create_info, nullptr, &instance) != VK_SUCCESS)
		cap::logger::fatal("vulkan failed to initialize");

	u32 extension_count = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);

	std::vector<VkExtensionProperties> extensions(extension_count);
	vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());

	if (rhr::render::renderer::enable_validation_layers && !rhr::render::tools::check_validation_layer_support())
		cap::logger::fatal("vulkan validation layers unsupported");
}

void rhr::render::device::init_debug()
{
	if (!rhr::render::renderer::enable_validation_layers)
		return;

	VkDebugUtilsMessengerCreateInfoEXT create_info;
	rhr::render::tools::populate_debug_messenge_create_info(create_info);

	if (rhr::render::tools::create_debug_utils_message_ext(&instance, &create_info, nullptr, &debug_messenger) != VK_SUCCESS)
		cap::logger::fatal("failed to create debug utils message EXT");
}

void rhr::render::device::init_device()
{
	u32 device_count = 0;
	vkEnumeratePhysicalDevices(instance, &device_count, nullptr);

	if (device_count == 0)
		cap::logger::fatal("no supported vulkan devices found");

	std::vector<vk::physical_device> devices(device_count);
	vkEnumeratePhysicalDevices(instance, &device_count, devices.data());

	for (auto& device : devices)
	{
		if (rhr::render::tools::is_device_suitable(&device, &rhr::render::renderer::surface))
		{
			physical_device = device;
			break;
		}
	}

	if (physical_device == VK_NULL_HANDLE)
		cap::logger::fatal("failed to pick appropriate device");
}

void rhr::render::device::init_logical_device()
{
	rhr::render::tools::queue_family_indices indices = rhr::render::tools::find_queue_families(&physical_device, &rhr::render::renderer::surface);

	vk::device_queue_create_info queue_create_info{};
	queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queue_create_info.queueFamilyIndex = indices.graphics_family.value();
	queue_create_info.queueCount = 1;

	f32 queue_priority = 1.0f;
	queue_create_info.pQueuePriorities = &queue_priority;

	vk::physical_device_features device_features{};
	device_features.samplerAnisotropy = VK_TRUE;

	vk::device_create_info create_info{};
	create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	create_info.queueCreateInfoCount = static_cast<u32>(rhr::render::command::queue_create_infos.size());
	create_info.pQueueCreateInfos = rhr::render::command::queue_create_infos.data();
	create_info.pEnabledFeatures = &device_features;
	create_info.enabledExtensionCount = static_cast<u32>(rhr::render::tools::device_extensions.size());
	create_info.ppEnabledExtensionNames = rhr::render::tools::device_extensions.data();

	if (rhr::render::renderer::enable_validation_layers)
	{
		create_info.enabledLayerCount = static_cast<u32>(rhr::render::tools::validation_layers.size());
		create_info.ppEnabledLayerNames = rhr::render::tools::validation_layers.data();
	}
	else
		create_info.enabledLayerCount = 0;

	if (vkCreateDevice(physical_device, &create_info, nullptr, &device_master) != VK_SUCCESS)
		cap::logger::fatal("failed to create logical device link to gpu");

	vkGetDeviceQueue(device_master, indices.graphics_family.value(), 0, &rhr::render::renderer::graphics_queue);
}

VkInstance rhr::render::device::instance;
VkDebugUtilsMessengerEXT rhr::render::device::debug_messenger;
VkPhysicalDevice rhr::render::device::physical_device;
VkDevice rhr::render::device::device_master;