#include "device.hpp"

#include "rhr/rendering/tools.hpp"

rhr::render::components::device::device()
	: m_instance(nullptr)
	, m_physical_device(nullptr)
	, m_device(nullptr)
	, m_debug_messenger(nullptr)
	, m_graphics_queue(nullptr)
	, m_valid(true)
{
	vk::application_info app_info = {};
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pApplicationName = "Unnamed Blocks";
	app_info.applicationVersion = VK_MAKE_API_VERSION(0, 0, 0, 0);
	app_info.pEngineName = "No Engine";
	app_info.engineVersion = VK_MAKE_API_VERSION(0, 0, 0, 0);
	app_info.apiVersion = VK_API_VERSION_1_0;
	app_info.pNext = nullptr;

	vk::instance_create_info create_info = {};
	create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	create_info.pApplicationInfo = &app_info;

	std::vector<const char*> required_extensions = rhr::render::tools::get_required_extensions();
	create_info.enabledExtensionCount = static_cast<u32>(required_extensions.size());
	create_info.ppEnabledExtensionNames = required_extensions.data();

	VkDebugUtilsMessengerCreateInfoEXT debug_create_info;
	if (validation_layers_enabled)
	{
		if (!rhr::render::tools::check_validation_layer_support())
		{
			cap::logger::error("vulkan validation layers unsupported");
			m_valid = false;
			return;
		}

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

	if (vk::create_instance(&create_info, nullptr, &m_instance) != VK_SUCCESS)
	{
		cap::logger::error("failed to create vulkan instance");
		m_valid = false;
		return;
	}

	u32 extension_count = 0;
	vk::enumerate_instance_extension_properties(nullptr, &extension_count, nullptr);

	std::vector<VkExtensionProperties> extensions(extension_count);
	vk::enumerate_instance_extension_properties(nullptr, &extension_count, extensions.data());
}

void rhr::render::components::device::initialize(vk::surface_khr* surface)
{
	m_surface = surface;

	if (!m_valid)
	{
		cap::logger::error("cannot initialize invalid device");
		return;
	}

	// Initialize Debug Operations.

	if (!validation_layers_enabled)
		return;

	vk::debug_utils_messenger_create_info debug_utils_messenger_create_info;
	rhr::render::tools::populate_debug_messenge_create_info(debug_utils_messenger_create_info);

	if (rhr::render::tools::create_debug_utils_message_ext(&m_instance, &debug_utils_messenger_create_info, nullptr, &m_debug_messenger) != VK_SUCCESS)
	{
		cap::logger::error("failed to create debug utils message EXT");
		m_valid = false;
		return;
	}

	// Initialize Vulkan Instance.

	u32 device_count = 0;
	vk::enumerate_physical_device(m_instance, &device_count, nullptr);

	if (device_count == 0)
	{
		cap::logger::error("no supported vulkan devices found");
		m_valid = false;
		return;
	}

	std::vector<vk::physical_device> devices(device_count);
	vk::enumerate_physical_device(m_instance, &device_count, devices.data());

	for (auto& device : devices)
	{
		if (rhr::render::tools::is_device_suitable(&device, m_surface))
		{
			m_physical_device = device;
			break;
		}
	}

	if (m_physical_device == nullptr)
	{
		cap::logger::error("failed to pick appropriate device");
		m_valid = false;
		return;
	}

	// Initialize Queue Properties.

	rhr::render::tools::queue_family_indices indices = rhr::render::tools::find_queue_families(&m_physical_device, m_surface);
	std::set<u32> unique_queue_families = { indices.graphics_family.value(), indices.present_family.value() };

	f32 queue_priority = 1.0f;
	for (u32 queue_family : unique_queue_families)
	{
		vk::device_queue_create_info queue_create_info = {};
		queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queue_create_info.queueFamilyIndex = queue_family;
		queue_create_info.queueCount = 1;
		queue_create_info.pQueuePriorities = &queue_priority;
		m_queue_create_infos.push_back(queue_create_info);
	}

	// Initialize Logical Device.

	vk::device_queue_create_info queue_create_info = {};
	queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queue_create_info.queueFamilyIndex = indices.graphics_family.value();
	queue_create_info.queueCount = 1;
	queue_create_info.pQueuePriorities = &queue_priority;

	vk::physical_device_features device_features = {};
	device_features.samplerAnisotropy = VK_TRUE;

	vk::device_create_info device_create_info = {};
	device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	device_create_info.queueCreateInfoCount = static_cast<u32>(m_queue_create_infos.size());
	device_create_info.pQueueCreateInfos = m_queue_create_infos.data();
	device_create_info.pEnabledFeatures = &device_features;
	device_create_info.enabledExtensionCount = static_cast<u32>(rhr::render::tools::device_extensions.size());
	device_create_info.ppEnabledExtensionNames = rhr::render::tools::device_extensions.data();

	if (validation_layers_enabled)
	{
		device_create_info.enabledLayerCount = static_cast<u32>(rhr::render::tools::validation_layers.size());
		device_create_info.ppEnabledLayerNames = rhr::render::tools::validation_layers.data();
	}
	else
		device_create_info.enabledLayerCount = 0;

	if (vk::create_device(m_physical_device, &device_create_info, nullptr, &m_device) != VK_SUCCESS)
	{
		cap::logger::error("failed to create logical device link to gpu");
		m_valid = false;
		return;
	}

	vk::get_device_queue(m_device, indices.graphics_family.value(), 0, &m_graphics_queue);
}

vk::instance& rhr::render::components::device::get_instance()
{
	if (!m_valid)
		cap::logger::warn("get_instance - device marked as invalid");

	return m_instance;
}

vk::physical_device& rhr::render::components::device::get_physical_device()
{
	return m_physical_device;
}

vk::device& rhr::render::components::device::get_device()
{
	return m_device;
}

bool rhr::render::components::device::validation_layers_enabled = true;
