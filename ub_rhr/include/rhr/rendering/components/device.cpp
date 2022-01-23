#include "device.hpp"

#include "rhr/rendering/tools.hpp"

rhr::render::component::device::device()
	: m_instance {}
	, m_surface {}
	, m_physical_device {}
	, m_device {}
	, m_debug_messenger {}
	, m_graphics_queue {}
	, m_valid(true)
	, m_descriptor_set_layout {}
	, m_texture_sampler {}
{

	vk::application_info app_info = {};
	app_info.sType				  = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pApplicationName	  = "Unnamed Blocks";
	app_info.applicationVersion	  = VK_MAKE_API_VERSION(0, 0, 0, 0);
	app_info.pEngineName		  = "No Engine";
	app_info.engineVersion		  = VK_MAKE_API_VERSION(0, 0, 0, 0);
	app_info.apiVersion			  = VK_API_VERSION_1_1;
	app_info.pNext				  = nullptr;

	vk::instance_create_info create_info = {};
	create_info.sType					 = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	create_info.pApplicationInfo		 = &app_info;

	std::vector<const char*> required_extensions = rhr::render::tools::get_required_extensions();
	create_info.enabledExtensionCount			 = static_cast<u32>(required_extensions.size());
	create_info.ppEnabledExtensionNames			 = required_extensions.data();

	if (validation_layers_enabled)
	{
		VkDebugUtilsMessengerCreateInfoEXT debug_create_info = {};

		if (!rhr::render::tools::check_validation_layer_support())
		{
			cap::logger::error(cap::logger::level::SYSTEM, "vulkan validation layers unsupported");
			m_valid = false;
			return;
		}

		create_info.enabledLayerCount	= static_cast<u32>(rhr::render::tools::validation_layers.size());
		create_info.ppEnabledLayerNames = rhr::render::tools::validation_layers.data();

		rhr::render::tools::populate_debug_messenge_create_info(debug_create_info);
		create_info.pNext = &debug_create_info;
	}
	else
	{
		create_info.enabledLayerCount = 0;
		create_info.pNext			  = nullptr;
	}

	if (vk::create_instance(&create_info, nullptr, &m_instance) != VK_SUCCESS)
	{
		cap::logger::error(cap::logger::level::SYSTEM, "failed to create vulkan instance");
		m_valid = false;
		return;
	}

	u32 extension_count = 0;
	vk::enumerate_instance_extension_properties(nullptr, &extension_count, nullptr);

	std::vector<VkExtensionProperties> extensions(extension_count);
	vk::enumerate_instance_extension_properties(nullptr, &extension_count, extensions.data());
}

void rhr::render::component::device::initialize(vk::surface_khr* surface)
{
	m_surface = surface;

	if (!m_valid)
	{
		cap::logger::error(cap::logger::level::SYSTEM, "cannot initialize invalid device");
		return;
	}

	// Initialize debug operations.

	if (validation_layers_enabled)
	{
		vk::debug_utils_messenger_create_info debug_utils_messenger_create_info;
		rhr::render::tools::populate_debug_messenge_create_info(debug_utils_messenger_create_info);

		if (rhr::render::tools::create_debug_utils_message_ext(&m_instance, &debug_utils_messenger_create_info, nullptr, &m_debug_messenger) != VK_SUCCESS)
		{
			cap::logger::error(cap::logger::level::SYSTEM, "failed to create debug utils message EXT");
			m_valid = false;
			return;
		}
	}

	// Initialize vulkan instance.

	u32 device_count = 0;
	vk::enumerate_physical_device(m_instance, &device_count, nullptr);

	if (device_count == 0)
	{
		cap::logger::error(cap::logger::level::SYSTEM, "no supported vulkan devices found");
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
		cap::logger::error(cap::logger::level::SYSTEM, "failed to pick appropriate device");
		m_valid = false;
		return;
	}

	// Initialize queue properties.

	rhr::render::tools::queue_family_indices indices = rhr::render::tools::find_queue_families(&m_physical_device, m_surface);
	std::set<u32> unique_queue_families				 = {indices.graphics_family.value(), indices.present_family.value()};

	f32 queue_priority = 1.0f;
	for (u32 queue_family : unique_queue_families)
	{
		vk::device_queue_create_info queue_create_info = {};
		queue_create_info.sType						   = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queue_create_info.queueFamilyIndex			   = queue_family;
		queue_create_info.queueCount				   = 1;
		queue_create_info.pQueuePriorities			   = &queue_priority;
		m_queue_create_infos.push_back(queue_create_info);
	}

	// Initialize logical device.

	vk::device_queue_create_info queue_create_info = {};
	queue_create_info.sType						   = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queue_create_info.queueFamilyIndex			   = indices.graphics_family.value();
	queue_create_info.queueCount				   = 1;
	queue_create_info.pQueuePriorities			   = &queue_priority;

	vk::physical_device_features device_features = {};
	device_features.samplerAnisotropy			 = VK_TRUE;

	vk::device_create_info device_create_info = {};
	device_create_info.sType				  = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	device_create_info.queueCreateInfoCount	   = static_cast<u32>(m_queue_create_infos.size());
	device_create_info.pQueueCreateInfos	   = m_queue_create_infos.data();
	device_create_info.pEnabledFeatures		   = &device_features;
	device_create_info.enabledExtensionCount   = static_cast<u32>(rhr::render::tools::device_extensions.size());
	device_create_info.ppEnabledExtensionNames = rhr::render::tools::device_extensions.data();

	if (validation_layers_enabled)
	{
		device_create_info.enabledLayerCount   = static_cast<u32>(rhr::render::tools::validation_layers.size());
		device_create_info.ppEnabledLayerNames = rhr::render::tools::validation_layers.data();
	}
	else
		device_create_info.enabledLayerCount = 0;

	if (vk::create_device(m_physical_device, &device_create_info, nullptr, &m_device) != VK_SUCCESS)
	{
		cap::logger::error(cap::logger::level::SYSTEM, "failed to create logical device link to gpu");
		m_valid = false;
		return;
	}

	vk::get_device_queue(m_device, indices.graphics_family.value(), 0, &m_graphics_queue);

	// Initialize other components.

	m_swapchain = std::make_unique<rhr::render::component::swapchain>();
	m_swapchain->create_render_passes();

	initialize_descriptor_set();

	m_pipeline	   = std::make_unique<rhr::render::component::pipeline>();
	m_command_pool = std::make_unique<rhr::render::component::command_pool>();
	m_swapchain->create_framebuffers();

	initialize_texture_sampler();

	m_command_pool->initialize_descriptor_pool();
	m_command_pool->initialize_command_buffers();
	m_swapchain->create_synchronization();

	m_pipeline->apply_active_pipeline("master");
}

void rhr::render::component::device::recreate_swapchain()
{
	m_swapchain.reset(nullptr);
	m_swapchain = std::make_unique<rhr::render::component::swapchain>();
	m_swapchain->create_render_passes();

	m_pipeline = std::make_unique<rhr::render::component::pipeline>();
	m_swapchain->create_framebuffers();
	m_command_pool->initialize_command_buffers();
	m_swapchain->create_synchronization();

	m_pipeline->apply_active_pipeline("master");
}

vk::instance& rhr::render::component::device::get_instance()
{
	if (!m_valid)
		cap::logger::warn(cap::logger::level::SYSTEM, "get_instance - device marked as invalid");

	return m_instance;
}

vk::physical_device& rhr::render::component::device::get_physical_device() { return m_physical_device; }

vk::device& rhr::render::component::device::get_device() { return m_device; }

vk::descriptor_set_layout& rhr::render::component::device::get_descriptor_set_layout() { return m_descriptor_set_layout; }

vk::queue& rhr::render::component::device::get_graphics_queue() { return m_graphics_queue; }

vk::sampler& rhr::render::component::device::get_texture_sampler() { return m_texture_sampler; }

std::unique_ptr<rhr::render::component::swapchain>& rhr::render::component::device::get_swapchain() { return m_swapchain; }

std::unique_ptr<rhr::render::component::pipeline>& rhr::render::component::device::get_pipeline() { return m_pipeline; }

std::unique_ptr<rhr::render::component::command_pool>& rhr::render::component::device::get_command_pool() { return m_command_pool; }

void rhr::render::component::device::initialize_descriptor_set()
{
	vk::descriptor_set_layout_binding ubo_descriptor_set_layout_binding		= {};
	vk::descriptor_set_layout_binding sampler_descriptor_set_layout_binding = {};
	vk::descriptor_set_layout_create_info descriptor_set_layout_create_info = {};

	ubo_descriptor_set_layout_binding.binding			 = 0;
	ubo_descriptor_set_layout_binding.descriptorType	 = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	ubo_descriptor_set_layout_binding.descriptorCount	 = 1;
	ubo_descriptor_set_layout_binding.stageFlags		 = VK_SHADER_STAGE_VERTEX_BIT;
	ubo_descriptor_set_layout_binding.pImmutableSamplers = nullptr; // Optional

	sampler_descriptor_set_layout_binding.binding			 = 1;
	sampler_descriptor_set_layout_binding.descriptorCount	 = 1;
	sampler_descriptor_set_layout_binding.descriptorType	 = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	sampler_descriptor_set_layout_binding.pImmutableSamplers = nullptr;
	sampler_descriptor_set_layout_binding.stageFlags		 = VK_SHADER_STAGE_FRAGMENT_BIT;

	std::array<vk::descriptor_set_layout_binding, 2> descriptor_set_layout_bindings = {ubo_descriptor_set_layout_binding, sampler_descriptor_set_layout_binding};

	descriptor_set_layout_create_info.sType		   = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descriptor_set_layout_create_info.bindingCount = static_cast<u32>(descriptor_set_layout_bindings.size());
	descriptor_set_layout_create_info.pBindings	   = descriptor_set_layout_bindings.data();

	if (vk::create_descriptor_set_layout(m_device, &descriptor_set_layout_create_info, nullptr, &m_descriptor_set_layout) != VK_SUCCESS)
		cap::logger::fatal(cap::logger::level::SYSTEM, "failed to create descriptor set layout");
}

void rhr::render::component::device::initialize_texture_sampler()
{
	vk::physical_device_properties physical_device_properties = {};
	vk::sampler_create_info sampler_create_info				  = {};

	vk::get_physical_device_properties(m_physical_device, &physical_device_properties);

	sampler_create_info.sType		 = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	sampler_create_info.magFilter	 = VK_FILTER_NEAREST;
	sampler_create_info.minFilter	 = VK_FILTER_NEAREST;
	sampler_create_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	sampler_create_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	sampler_create_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

	sampler_create_info.anisotropyEnable		= VK_TRUE;
	sampler_create_info.maxAnisotropy			= physical_device_properties.limits.maxSamplerAnisotropy;
	sampler_create_info.borderColor				= VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	sampler_create_info.unnormalizedCoordinates = VK_FALSE;
	sampler_create_info.compareEnable			= VK_FALSE;
	sampler_create_info.compareOp				= VK_COMPARE_OP_ALWAYS;

	sampler_create_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	sampler_create_info.mipLodBias = 0.0f;
	sampler_create_info.minLod	   = 0.0f;
	sampler_create_info.maxLod	   = 0.0f;

	if (vk::create_sampler(m_device, &sampler_create_info, nullptr, &m_texture_sampler) != VK_SUCCESS)
		cap::logger::fatal(cap::logger::level::SYSTEM, "failed to create texture sampler");
}

#ifdef NDEBUG
bool rhr::render::component::device::validation_layers_enabled = false;
#else
bool rhr::render::component::device::validation_layers_enabled = true;
#endif