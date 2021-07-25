#include "renderer.hpp"

#include "rhr/rendering/tools.hpp"
#include "rhr/rendering/objects/object.hpp"
#include "rhr/rendering/objects/rectangle.hpp"
#include "rhr/rendering/vertex.hpp"
#include "rhr/stacking/plane.hpp"

#include <espresso/input_handler.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void frame_buffer_resize_callback(GLFWwindow* window, i32 width, i32 height)
{
	rhr::render::renderer::frame_buffer_resized = true;
	rhr::render::renderer::window_size = { width, height };
}

void key_callback(GLFWwindow* window, i32 key, i32 scancode, i32 action, i32 mode)
{
	InputHandler::FireKey(key, action);
}

void mouse_button_callback(GLFWwindow* window, i32 button, i32 action, i32 mods)
{
	InputHandler::FireMouseButton(button, action);
}

void scroll_callback(GLFWwindow* window, f64 xoffset, f64 yoffset)
{
	InputHandler::FireMouseScroll(yoffset);
}

void cursor_position_callback(GLFWwindow* window, f64 xpos, f64 ypos)
{
	InputHandler::FireMouseMove({ xpos, ypos });
}

vk::image depthImage;
vk::device_memory depthImageMemory;

void rhr::render::renderer::initialize_window()
{
	physical_device = VK_NULL_HANDLE;
	surface = VK_NULL_HANDLE;
	frame_buffer_resized = false;
	vsync_enabled = false;

	view_matrix = glm::mat4(1.0);
	projection_matrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);

	window_size = { 1280, 720 };
	glm::vec<2, i32> window_size = window_size;

	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	char window_title[100];
	memset(window_title, 0, 100);
	sprintf(window_title, "Unnamed Blocks %s", VER_CLIENT);

	window = glfwCreateWindow(window_size.x, window_size.y, window_title, NULL, NULL);

	if (window == NULL)
		Logger::Fatal("Failed to create GLFW window");

	glfwSetFramebufferSizeCallback(window, frame_buffer_resize_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);

	init_instance();

	auto glfw_window_result = glfwCreateWindowSurface(instance, window, nullptr, &surface);

	if (glfw_window_result != VK_SUCCESS)
		Logger::Fatal(std::to_string(glfw_window_result));
}

//static std::shared_ptr<vui::RenderRectangle> testObject = std::make_shared<vui::RenderRectangle>();

void rhr::render::renderer::initialize()
{
// 	testObject->SetWeak(testObject);
// 	testObject->SetPosition({ 100, 100 });
// 	testObject->SetSize({ 256, 256 });
// 	testObject->SetColor(Color::White);
// 	testObject->SetTexture(RenderObject::TextureType::TEXT_SHEET);
// 	testObject->SetDepth(0);

	init_debug();
	init_device();

	rhr::render::tools::queue_family_indices indices = rhr::render::tools::find_queue_families(&physical_device, &surface);

	std::set<u32> unique_queue_families = { indices.graphics_family.value(), indices.present_family.value() };

	f32 queue_priority = 1.0f;
	for (u32 queue_family : unique_queue_families)
	{
		vk::device_queue_create_info queue_create_info{};
		queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queue_create_info.queueFamilyIndex = queue_family;
		queue_create_info.queueCount = 1;
		queue_create_info.pQueuePriorities = &queue_priority;
		queue_create_infos.push_back(queue_create_info);
	}

	init_logical_device();
	vkGetDeviceQueue(device, indices.present_family.value(), 0, &present_queue);

	init_swap_chain();
	init_image_views();
	init_render_pass();
	init_descriptor_set_layout();
	init_pipelines();
	init_command_pool();
	init_depth_resources();
	init_frame_buffers();
	init_texture_sampler();
	init_descriptor_pool();
	init_command_buffers();
	init_sync_objects();

	rhr::render::tools::create_aux_command_buffer();
}

void rhr::render::renderer::add_dirty(std::weak_ptr<rhr::render::interfaces::i_renderable> renderable)
{
	std::unique_lock lock(m_dirty_mutex);
	m_dirty_renderable.push_back(renderable);
}

void rhr::render::renderer::process_dirty()
{
	std::unique_lock lock(m_dirty_mutex);

	for (auto renderable : m_dirty_renderable)
	{
		if (auto object = renderable.lock())
			object->update_buffers();
	}

	m_dirty_renderable.clear();
}

void rhr::render::renderer::render(usize idx, f64 deltaTime, bool setup, TIME_POINT& diagnostics_time)
{
	// External Frame Updating

	// if (!setup)
	// {
	// 	Client::Instance->GetPlayer()->UpdateCamera(mousePosition);
	// 	rhr::render::renderer::ViewMatrix = glm::mat4(1.0);
	// 	rhr::render::renderer::ProjectionMatrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);
	// }

	rhr::render::renderer::ui_projection_matrix = glm::ortho(0.0f, static_cast<f32>(window_size.x), 0.0f, static_cast<f32>(window_size.y), -10000.0f, 10000.0f);

	// render Frame

	vk::command_buffer_begin_info begin_info{};
	begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	begin_info.flags = 0; // Optional
	begin_info.pInheritanceInfo = nullptr; // Optional

	if (vkBeginCommandBuffer(active_command_buffer, &begin_info) != VK_SUCCESS)
		Logger::Fatal("failed to start the command buffer");

	vk::render_pass_begin_info render_pass_info{};
	render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	render_pass_info.renderPass = render_pass;
	render_pass_info.framebuffer = swap_chain_frame_buffers[idx];

	render_pass_info.renderArea.offset = { 0, 0 };
	render_pass_info.renderArea.extent = swap_chain_extent;

	std::array<vk::clear_value, 2> clear_values{};
	clear_values[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
	clear_values[1].depthStencil = { 1.0f, 0 };

	render_pass_info.clearValueCount = static_cast<u32>(clear_values.size());
	render_pass_info.pClearValues = clear_values.data();

	// Client::Instance->GetDiagnostics()->SetRenderTime(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - diagnosticsTime).count());
	// diagnosticsTime = std::chrono::high_resolution_clock::now();
	Plane::primary_plane->frame_update(deltaTime);
	Plane::toolbar_plane->frame_update(deltaTime);

	process_dirty();
	// Client::Instance->GetDiagnostics()->SetDirtyTime(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - diagnosticsTime).count());
	// diagnosticsTime = std::chrono::high_resolution_clock::now();

	vkCmdBeginRenderPass(active_command_buffer, &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);

	// if (!setup)
	// {
	// 	// ChunkManager binds BlocksPipeline
	// 	Client::Instance->GetChunkManager()->render(deltaTime);
	// 	Client::Instance->GetDispatcher()->UpdateDiagnosticData();
	// 	Client::Instance->GetDiagnostics()->UpdateProgressBars();
	// }

	Plane::primary_plane->render();
	Plane::toolbar_plane->render();


	bool erased = false;

	for (usize i = 0; i < m_layers.size(); i++)
	{
		if (erased)
		{
			erased = false;
			i--;
		}

		if (auto layer = m_layers[i].lock())
			layer->render();
		else
		{
			m_layers.erase(m_layers.begin() + i);
			erased = true;
		}
	}

	//testObject->render();

	vkCmdEndRenderPass(active_command_buffer);

	if (vkEndCommandBuffer(active_command_buffer) != VK_SUCCESS)
		Logger::Fatal("failed to close the command buffer");
}

void rhr::render::renderer::clean_up_swap_chain()
{
	vkDestroyImageView(device, depth_image_view, nullptr);
	vkDestroyImage(device, depthImage, nullptr);
	vkFreeMemory(device, depthImageMemory, nullptr);

	for (usize i = 0; i < swap_chain_frame_buffers.size(); i++)
		vkDestroyFramebuffer(device, swap_chain_frame_buffers[i], nullptr);

	vkFreeCommandBuffers(device, command_pool, static_cast<u32>(command_buffers.size()), command_buffers.data());

	//vkDestroyPipeline(Device, BlocksPipeline, nullptr);
	vkDestroyPipeline(device, ui_pipeline, nullptr);
	vkDestroyPipeline(device, ui_texture_pipeline, nullptr);

	vkDestroyPipelineLayout(device, blocks_pipeline_layout, nullptr);
	vkDestroyPipelineLayout(device, ui_pipeline_layout, nullptr);
	vkDestroyPipelineLayout(device, ui_texture_pipeline_layout, nullptr);

	vkDestroyRenderPass(device, render_pass, nullptr);

	for (usize i = 0; i < swap_chain_image_views.size(); i++)
		vkDestroyImageView(device, swap_chain_image_views[i], nullptr);

	vkDestroySwapchainKHR(device, swap_chain, nullptr);

	//for (usize i = 0; i < m_SwapChainImages.size(); i++)
	//{
	//	vkDestroyBuffer(m_Device, m_UniformBuffers[i], nullptr);
	//	vkFreeMemory(m_Device, m_UniformBuffersMemory[i], nullptr);
	//}

	vkDestroyDescriptorPool(device, descriptor_pool, nullptr);
}

void rhr::render::renderer::clean_up()
{
	clean_up_swap_chain();

	vkDestroySampler(device, texture_sampler, nullptr);
	//vkDestroyImageView(m_Device, m_TextureImageView, nullptr);

	//vkDestroyImage(m_Device, m_TextureImage, nullptr);
	//vkFreeMemory(m_Device, m_TextureImageMemory, nullptr);

	vkDestroyDescriptorSetLayout(device, descriptor_set_layout, nullptr);

	//vkDestroyBuffer(m_Device, m_IndexBuffer, nullptr);
	//vkFreeMemory(m_Device, m_IndexBufferMemory, nullptr);

	//vkDestroyBuffer(m_Device, m_VertexBuffer, nullptr);
	//vkFreeMemory(m_Device, vertexBufferMemory, nullptr);

	for (usize i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		vkDestroySemaphore(device, render_finished_semaphores[i], nullptr);
		vkDestroySemaphore(device, image_available_semaphores[i], nullptr);
		vkDestroyFence(device, in_flight_fences[i], nullptr);
	}

	vkDestroyCommandPool(device, command_pool, nullptr);

	vkDestroyDevice(device, nullptr);

	if (enable_validation_layers)
		rhr::render::tools::destroy_debug_utils_message_ext(&instance, &debug_messenger, nullptr);

	vkDestroySurfaceKHR(instance, surface, nullptr);
	vkDestroyInstance(instance, nullptr);

	glfwDestroyWindow(window);

	glfwTerminate();
}

void rhr::render::renderer::recreate_swap_chain()
{
	i32 width = 0;
	i32 height = 0;
	glfwGetFramebufferSize(window, &width, &height);

	while (width == 0 || height == 0)
	{
		glfwGetFramebufferSize(window, &width, &height);
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(device);

	clean_up_swap_chain();

	init_swap_chain();
	init_image_views();
	init_render_pass();
	init_pipelines();
	init_depth_resources();
	init_frame_buffers();
	init_descriptor_pool();
	init_command_buffers();
	init_sync_objects();
}

void rhr::render::renderer::add_layer(std::weak_ptr<rhr::render::layer> layer)
{
	m_layers.push_back(layer);
}

void rhr::render::renderer::reload_layer_swap_chains()
{
	bool erased = false;

	for (usize i = 0; i < m_layers.size(); i++)
	{
		if (erased)
		{
			erased = false;
			i--;
		}

		if (auto layer = m_layers[i].lock())
			layer->reload_swap_chain();
		else
		{
			m_layers.erase(m_layers.begin() + i);
			erased = true;
		}
	}
}

void rhr::render::renderer::init_instance()
{
	vk::application_info app_info;
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pApplicationName = "Unnamed Blocks";
	app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	app_info.pEngineName = "No Engine";
	app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
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
	if (enable_validation_layers)
	{
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
		Logger::Fatal("vulkan failed to initialize");

	u32 extension_count = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);

	std::vector<VkExtensionProperties> extensions(extension_count);
	vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());

	if (enable_validation_layers && !rhr::render::tools::check_validation_layer_support())
		Logger::Fatal("vulkan validation layers unsupported");
}

void rhr::render::renderer::init_debug()
{
	if (!enable_validation_layers)
		return;

	VkDebugUtilsMessengerCreateInfoEXT create_info;
	rhr::render::tools::populate_debug_messenge_create_info(create_info);

	if (rhr::render::tools::create_debug_utils_message_ext(&instance, &create_info, nullptr, &debug_messenger) != VK_SUCCESS)
		Logger::Fatal("failed to create debug utils message EXT");
}

void rhr::render::renderer::init_device()
{
	u32 device_count = 0;
	vkEnumeratePhysicalDevices(instance, &device_count, nullptr);

	if (device_count == 0)
		Logger::Fatal("no supported vulkan devices found");

	std::vector<vk::physical_device> devices(device_count);
	vkEnumeratePhysicalDevices(instance, &device_count, devices.data());

	for (auto& device : devices)
	{
		if (rhr::render::tools::is_device_suitable(&device, &surface))
		{
			physical_device = device;
			break;
		}
	}

	if (physical_device == VK_NULL_HANDLE)
		Logger::Fatal("failed to pick appropriate device");
}

void rhr::render::renderer::init_logical_device()
{
	rhr::render::tools::queue_family_indices indices = rhr::render::tools::find_queue_families(&physical_device, &surface);

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

	create_info.queueCreateInfoCount = static_cast<u32>(queue_create_infos.size());
	create_info.pQueueCreateInfos = queue_create_infos.data();
	create_info.pEnabledFeatures = &device_features;
	create_info.enabledExtensionCount = static_cast<u32>(rhr::render::tools::device_extensions.size());
	create_info.ppEnabledExtensionNames = rhr::render::tools::device_extensions.data();

	if (enable_validation_layers)
	{
		create_info.enabledLayerCount = static_cast<u32>(rhr::render::tools::validation_layers.size());
		create_info.ppEnabledLayerNames = rhr::render::tools::validation_layers.data();
	}
	else
		create_info.enabledLayerCount = 0;

	if (vkCreateDevice(physical_device, &create_info, nullptr, &device) != VK_SUCCESS)
		Logger::Fatal("failed to create logical device link to gpu");

	vkGetDeviceQueue(device, indices.graphics_family.value(), 0, &graphics_queue);
}

void rhr::render::renderer::init_swap_chain()
{
	rhr::render::tools::swap_chain_support_details swap_chain_support = rhr::render::tools::query_swap_chain_support(&physical_device, &surface);

	vk::surface_format_khr surfaceFormat = rhr::render::tools::choose_swap_surface_format(swap_chain_support.formats);
	vk::present_mode_khr presentMode = rhr::render::tools::choose_swap_present_mode(swap_chain_support.present_modes);
	vk::extent_2d extent = rhr::render::tools::choose_swap_extent(swap_chain_support.capabilities);

	u32 imageCount = swap_chain_support.capabilities.minImageCount + 1;

	if (swap_chain_support.capabilities.maxImageCount > 0 && imageCount > swap_chain_support.capabilities.maxImageCount)
		imageCount = swap_chain_support.capabilities.maxImageCount;

	vk::swap_chain_create_info_khr create_info{};
	create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	create_info.surface = surface;

	create_info.minImageCount = imageCount;
	create_info.imageFormat = surfaceFormat.format;
	create_info.imageColorSpace = surfaceFormat.colorSpace;
	create_info.imageExtent = extent;
	create_info.imageArrayLayers = 1;
	create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	rhr::render::tools::queue_family_indices indices = rhr::render::tools::find_queue_families(&physical_device, &surface);
	u32 queue_family_indices[] = { indices.graphics_family.value(), indices.present_family.value() };

	if (indices.graphics_family != indices.present_family)
	{
		create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		create_info.queueFamilyIndexCount = 2;
		create_info.pQueueFamilyIndices = queue_family_indices;
	}
	else
	{
		create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		create_info.queueFamilyIndexCount = 0; // Optional
		create_info.pQueueFamilyIndices = nullptr; // Optional
	}

	create_info.preTransform = swap_chain_support.capabilities.currentTransform;
	create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	create_info.presentMode = presentMode;
	create_info.clipped = VK_TRUE;
	create_info.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(device, &create_info, nullptr, &swap_chain) != VK_SUCCESS)
		Logger::Fatal("failed to create swap chain");

	vkGetSwapchainImagesKHR(device, swap_chain, &imageCount, nullptr);
	swap_chain_images.resize(imageCount);
	vkGetSwapchainImagesKHR(device, swap_chain, &imageCount, swap_chain_images.data());

	swap_chain_image_format = surfaceFormat.format;
	swap_chain_extent = extent;
}

void rhr::render::renderer::init_image_views()
{
	swap_chain_image_views.resize(swap_chain_images.size());

	for (u32 i = 0; i < swap_chain_images.size(); i++)
		swap_chain_image_views[i] = rhr::render::tools::create_image_view(swap_chain_images[i], swap_chain_image_format, VK_IMAGE_ASPECT_COLOR_BIT);
}

void rhr::render::renderer::init_render_pass()
{
	VkSubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcAccessMask = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

	VkAttachmentDescription color_attachment{};
	color_attachment.format = swap_chain_image_format;
	color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
	color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentDescription depth_attachment{};
	depth_attachment.format = rhr::render::tools::find_depth_format();
	depth_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
	depth_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depth_attachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depth_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depth_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depth_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depth_attachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference color_attachment_ref{};
	color_attachment_ref.attachment = 0;
	color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depth_attachment_ref{};
	depth_attachment_ref.attachment = 1;
	depth_attachment_ref.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &color_attachment_ref;
	subpass.pDepthStencilAttachment = &depth_attachment_ref;

	std::array<VkAttachmentDescription, 2> attachments = { color_attachment, depth_attachment };

	VkRenderPassCreateInfo render_pass_info{};
	render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	render_pass_info.attachmentCount = static_cast<u32>(attachments.size());
	render_pass_info.pAttachments = attachments.data();
	render_pass_info.subpassCount = 1;
	render_pass_info.pSubpasses = &subpass;
	render_pass_info.dependencyCount = 1;
	render_pass_info.pDependencies = &dependency;

	if (vkCreateRenderPass(device, &render_pass_info, nullptr, &render_pass) != VK_SUCCESS)
		Logger::Fatal("failed to create render pass");
}

void rhr::render::renderer::init_descriptor_set_layout()
{
	VkDescriptorSetLayoutBinding ubo_layout_binding{};
	ubo_layout_binding.binding = 0;
	ubo_layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	ubo_layout_binding.descriptorCount = 1;
	ubo_layout_binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	ubo_layout_binding.pImmutableSamplers = nullptr; // Optional

	VkDescriptorSetLayoutBinding sampler_layout_binding{};
	sampler_layout_binding.binding = 1;
	sampler_layout_binding.descriptorCount = 1;
	sampler_layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	sampler_layout_binding.pImmutableSamplers = nullptr;
	sampler_layout_binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	std::array<VkDescriptorSetLayoutBinding, 2> bindings = { ubo_layout_binding, sampler_layout_binding };
	VkDescriptorSetLayoutCreateInfo layout_info{};
	layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layout_info.bindingCount = static_cast<u32>(bindings.size());
	layout_info.pBindings = bindings.data();

	if (vkCreateDescriptorSetLayout(device, &layout_info, nullptr, &descriptor_set_layout) != VK_SUCCESS)
		Logger::Fatal("failed to set descriptor layout");
}

void rhr::render::renderer::create_pipeline(const std::string& shader, VkPipeline* pipeline, VkPipelineLayout* layout)
{
	auto vert_shader_code = rhr::render::tools::read_file_bytes("res/shaders/" + shader + ".vert.spv");
	auto frag_shader_code = rhr::render::tools::read_file_bytes("res/shaders/" + shader + ".frag.spv");

	VkShaderModule vert_shader_module = rhr::render::tools::create_shader_module(*vert_shader_code, &device);
	VkShaderModule frag_shader_module = rhr::render::tools::create_shader_module(*frag_shader_code, &device);

	VkPipelineShaderStageCreateInfo vert_shader_stage_info{};
	vert_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vert_shader_stage_info.stage = VK_SHADER_STAGE_VERTEX_BIT;

	vert_shader_stage_info.module = vert_shader_module;
	vert_shader_stage_info.pName = "main";

	VkPipelineShaderStageCreateInfo frag_shader_stage_info{};
	frag_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	frag_shader_stage_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	frag_shader_stage_info.module = frag_shader_module;
	frag_shader_stage_info.pName = "main";

	VkPipelineShaderStageCreateInfo shader_stages[] = { vert_shader_stage_info, frag_shader_stage_info };

	auto binding_description = rhr::render::vertex::get_binding_description();
	auto attribute_descriptions = rhr::render::vertex::get_binding_description();

	VkPipelineVertexInputStateCreateInfo vertex_input_info{};
	vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertex_input_info.vertexBindingDescriptionCount = 1;
	vertex_input_info.vertexAttributeDescriptionCount = static_cast<u32>(attribute_descriptions.size());
	vertex_input_info.pVertexBindingDescriptions = &binding_description;
	vertex_input_info.pVertexAttributeDescriptions = attribute_descriptions.data();

	VkPipelineInputAssemblyStateCreateInfo input_assembly{};
	input_assembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	input_assembly.primitiveRestartEnable = VK_FALSE;

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<f32>(swap_chain_extent.width);
	viewport.height = static_cast<f32>(swap_chain_extent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = swap_chain_extent;

	VkPipelineViewportStateCreateInfo viewport_state{};
	viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewport_state.viewportCount = 1;
	viewport_state.pViewports = &viewport;
	viewport_state.scissorCount = 1;
	viewport_state.pScissors = &scissor;

	VkPipelineRasterizationStateCreateInfo rasterizer{};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;

	rasterizer.cullMode = VK_CULL_MODE_NONE;
	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;

	rasterizer.depthBiasEnable = VK_FALSE;
	rasterizer.depthBiasConstantFactor = 0.0f; // Optional
	rasterizer.depthBiasClamp = 0.0f; // Optional
	rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

	VkPipelineMultisampleStateCreateInfo multisampling{};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampling.minSampleShading = 1.0f; // Optional
	multisampling.pSampleMask = nullptr; // Optional
	multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
	multisampling.alphaToOneEnable = VK_FALSE; // Optional

	VkPipelineColorBlendAttachmentState color_blend_attachment{};
	color_blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	color_blend_attachment.blendEnable = VK_TRUE;
	color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	color_blend_attachment.colorBlendOp = VK_BLEND_OP_ADD;
	color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	color_blend_attachment.alphaBlendOp = VK_BLEND_OP_ADD;

	VkPipelineColorBlendStateCreateInfo color_blending{};
	color_blending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	color_blending.logicOpEnable = VK_FALSE;
	color_blending.logicOp = VK_LOGIC_OP_COPY; // Optional
	color_blending.attachmentCount = 1;
	color_blending.pAttachments = &color_blend_attachment;
	color_blending.blendConstants[0] = 0.0f; // Optional
	color_blending.blendConstants[1] = 0.0f; // Optional
	color_blending.blendConstants[2] = 0.0f; // Optional
	color_blending.blendConstants[3] = 0.0f; // Optional

	VkDynamicState dynamic_states[] = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_LINE_WIDTH
	};

	VkPipelineDynamicStateCreateInfo dynamic_state{};
	dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamic_state.dynamicStateCount = 2;
	dynamic_state.pDynamicStates = dynamic_states;

	VkPipelineLayoutCreateInfo pipeline_layout_info{};
	pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipeline_layout_info.setLayoutCount = 1;
	pipeline_layout_info.pSetLayouts = &descriptor_set_layout;
	pipeline_layout_info.pushConstantRangeCount = 0; // Optional
	pipeline_layout_info.pPushConstantRanges = nullptr; // Optional

	if (vkCreatePipelineLayout(device, &pipeline_layout_info, nullptr, layout) != VK_SUCCESS)
		Logger::Fatal("failed to create pipeline layout");

	VkPipelineDepthStencilStateCreateInfo depth_stencil{};
	depth_stencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depth_stencil.depthTestEnable = VK_TRUE;
	depth_stencil.depthWriteEnable = VK_TRUE;
	depth_stencil.depthCompareOp = VK_COMPARE_OP_LESS;
	depth_stencil.depthBoundsTestEnable = VK_FALSE;
	depth_stencil.minDepthBounds = 0.0f; // Optional
	depth_stencil.maxDepthBounds = 1.0f; // Optional
	depth_stencil.stencilTestEnable = VK_FALSE;
	depth_stencil.front = {}; // Optional
	depth_stencil.back = {}; // Optional

	VkGraphicsPipelineCreateInfo pipeline_info{};
	pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipeline_info.stageCount = 2;
	pipeline_info.pStages = shader_stages;
	pipeline_info.pVertexInputState = &vertex_input_info;
	pipeline_info.pInputAssemblyState = &input_assembly;
	pipeline_info.pViewportState = &viewport_state;
	pipeline_info.pRasterizationState = &rasterizer;
	pipeline_info.pMultisampleState = &multisampling;
	pipeline_info.pDepthStencilState = &depth_stencil;
	pipeline_info.pColorBlendState = &color_blending;
	pipeline_info.pDynamicState = nullptr; // Optional

	pipeline_info.layout = *layout;
	pipeline_info.renderPass = render_pass;
	pipeline_info.subpass = 0;
	pipeline_info.basePipelineHandle = VK_NULL_HANDLE; // Optional
	pipeline_info.basePipelineIndex = -1; // Optional

	if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipeline_info, nullptr, pipeline) != VK_SUCCESS)
		Logger::Fatal("failed to create graphics pipeline");
}

void rhr::render::renderer::init_pipelines()
{
	//CreatePipeline("blocks", &BlocksPipeline, &BlocksPipelineLayout);
	create_pipeline("ui", &ui_pipeline, &ui_pipeline_layout);
	create_pipeline("ui_texture", &ui_texture_pipeline, &ui_texture_pipeline_layout);
}

void rhr::render::renderer::init_command_pool()
{
	rhr::render::tools::queue_family_indices queue_family_indices = rhr::render::tools::find_queue_families(&physical_device, &surface);

	VkCommandPoolCreateInfo pool_info{};
	pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	pool_info.queueFamilyIndex = queue_family_indices.graphics_family.value();
	pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; // Optional

	if (vkCreateCommandPool(device, &pool_info, nullptr, &command_pool) != VK_SUCCESS)
		Logger::Fatal("failed to create command pool");
}

void rhr::render::renderer::init_frame_buffers()
{
	swap_chain_frame_buffers.resize(swap_chain_image_views.size());

	for (usize i = 0; i < swap_chain_image_views.size(); i++)
	{
		std::array<VkImageView, 2> attachments = {
			swap_chain_image_views[i],
			depth_image_view
		};

		VkFramebufferCreateInfo frame_buffer_info{};
		frame_buffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		frame_buffer_info.renderPass = render_pass;
		frame_buffer_info.attachmentCount = static_cast<u32>(attachments.size());
		frame_buffer_info.pAttachments = attachments.data();
		frame_buffer_info.width = swap_chain_extent.width;
		frame_buffer_info.height = swap_chain_extent.height;
		frame_buffer_info.layers = 1;

		if (vkCreateFramebuffer(device, &frame_buffer_info, nullptr, &swap_chain_frame_buffers[i]) != VK_SUCCESS)
			Logger::Fatal("failed to create frame buffers");
	}
}

void rhr::render::renderer::init_depth_resources()
{
	VkFormat depthFormat = rhr::render::tools::find_depth_format();
	rhr::render::tools::create_image(swap_chain_extent.width, swap_chain_extent.height, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory);
	depth_image_view = rhr::render::tools::create_image_view(depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
	rhr::render::tools::transition_image_layout(depthImage, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
}

void rhr::render::renderer::init_texture_sampler()
{
	VkPhysicalDeviceProperties properties{};
	vkGetPhysicalDeviceProperties(physical_device, &properties);

	VkSamplerCreateInfo sampler_info{};
	sampler_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	sampler_info.magFilter = VK_FILTER_NEAREST;
	sampler_info.minFilter = VK_FILTER_NEAREST;
	sampler_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	sampler_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	sampler_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

	sampler_info.anisotropyEnable = VK_TRUE;
	sampler_info.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
	sampler_info.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	sampler_info.unnormalizedCoordinates = VK_FALSE;
	sampler_info.compareEnable = VK_FALSE;
	sampler_info.compareOp = VK_COMPARE_OP_ALWAYS;

	sampler_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	sampler_info.mipLodBias = 0.0f;
	sampler_info.minLod = 0.0f;
	sampler_info.maxLod = 0.0f;

	if (vkCreateSampler(device, &sampler_info, nullptr, &texture_sampler) != VK_SUCCESS)
		Logger::Fatal("failed to create texture sampler");
}

void rhr::render::renderer::init_descriptor_pool()
{
	std::array<VkDescriptorPoolSize, 2> pool_sizes{};

	pool_sizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	pool_sizes[0].descriptorCount = static_cast<u32>(10000);

	pool_sizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	pool_sizes[1].descriptorCount = static_cast<u32>(10000);

	VkDescriptorPoolCreateInfo pool_info{};
	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_info.poolSizeCount = static_cast<u32>(pool_sizes.size());
	pool_info.pPoolSizes = pool_sizes.data();
	pool_info.maxSets = 10000;
	pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

	if (vkCreateDescriptorPool(device, &pool_info, nullptr, &descriptor_pool) != VK_SUCCESS)
		Logger::Fatal("failed to create descriptor pool");
}

void rhr::render::renderer::init_command_buffers()
{
	command_buffers.resize(swap_chain_frame_buffers.size());

	VkCommandBufferAllocateInfo alloc_info{};
	alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	alloc_info.commandPool = command_pool;
	alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	alloc_info.commandBufferCount = (u32)command_buffers.size();

	if (vkAllocateCommandBuffers(device, &alloc_info, command_buffers.data()) != VK_SUCCESS)
		Logger::Fatal("failed to create command buffers");

	for (usize i = 0; i < command_buffers.size(); i++)
	{
		vk::command_buffer_begin_info begin_info{};
		begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		begin_info.flags = 0; // Optional
		begin_info.pInheritanceInfo = nullptr; // Optional

		if (vkBeginCommandBuffer(command_buffers[i], &begin_info) != VK_SUCCESS)
			Logger::Fatal("failed to start the command buffer");

		vk::render_pass_begin_info render_pass_info{};
		render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		render_pass_info.renderPass = render_pass;
		render_pass_info.framebuffer = swap_chain_frame_buffers[i];

		render_pass_info.renderArea.offset = { 0, 0 };
		render_pass_info.renderArea.extent = swap_chain_extent;

		std::array<vk::clear_value, 2> clear_values{};
		clear_values[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
		clear_values[1].depthStencil = { 1.0f, 0 };

		render_pass_info.clearValueCount = static_cast<u32>(clear_values.size());
		render_pass_info.pClearValues = clear_values.data();

		vkCmdBeginRenderPass(command_buffers[i], &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);
		vkCmdBindPipeline(command_buffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, ui_pipeline);
		vkCmdEndRenderPass(command_buffers[i]);

		if (vkEndCommandBuffer(command_buffers[i]) != VK_SUCCESS)
			Logger::Fatal("failed to close the command buffer");
	}
}

void rhr::render::renderer::init_sync_objects()
{
	for (usize i = 0; i < render_finished_semaphores.size(); i++)
	{
		vkDestroySemaphore(device, render_finished_semaphores[i], nullptr);
		vkDestroySemaphore(device, image_available_semaphores[i], nullptr);
		vkDestroyFence(device, in_flight_fences[i], nullptr);
	}

	image_available_semaphores.clear();
	render_finished_semaphores.clear();
	in_flight_fences.clear();
	images_in_flight.clear();

	image_available_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
	render_finished_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
	in_flight_fences.resize(MAX_FRAMES_IN_FLIGHT);
	images_in_flight.resize(swap_chain_images.size(), VK_NULL_HANDLE);

	VkSemaphoreCreateInfo semaphore_info{};
	semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fence_info{};
	fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (usize i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		if (vkCreateSemaphore(device, &semaphore_info, nullptr, &image_available_semaphores[i]) != VK_SUCCESS || vkCreateSemaphore(device, &semaphore_info, nullptr, &render_finished_semaphores[i]) != VK_SUCCESS || vkCreateFence(device, &fence_info, nullptr, &in_flight_fences[i]) != VK_SUCCESS)
			Logger::Fatal("failed to create semaphores");
	}
}

VkInstance rhr::render::renderer::instance;
VkDebugUtilsMessengerEXT rhr::render::renderer::debug_messenger;
VkPhysicalDevice rhr::render::renderer::physical_device;
VkDevice rhr::render::renderer::device;
VkQueue rhr::render::renderer::graphics_queue;
VkQueue rhr::render::renderer::present_queue;
VkSurfaceKHR rhr::render::renderer::surface;
VkSwapchainKHR rhr::render::renderer::swap_chain;
GLFWwindow* rhr::render::renderer::window;
std::vector<vk::image> rhr::render::renderer::swap_chain_images;
std::vector<VkImageView> rhr::render::renderer::swap_chain_image_views;
VkFormat rhr::render::renderer::swap_chain_image_format;
VkExtent2D rhr::render::renderer::swap_chain_extent;
std::vector<VkFramebuffer> rhr::render::renderer::swap_chain_frame_buffers;
std::vector<VkCommandBuffer> rhr::render::renderer::command_buffers;
VkCommandBuffer rhr::render::renderer::active_command_buffer;
std::vector<vk::device_queue_create_info> rhr::render::renderer::queue_create_infos;
VkRenderPass rhr::render::renderer::render_pass;
VkDescriptorSetLayout rhr::render::renderer::descriptor_set_layout;
VkCommandPool rhr::render::renderer::command_pool;
VkDescriptorPool rhr::render::renderer::descriptor_pool;
VkSampler rhr::render::renderer::texture_sampler;
VkImageView rhr::render::renderer::depth_image_view;
std::vector<VkSemaphore> rhr::render::renderer::image_available_semaphores;
std::vector<VkSemaphore> rhr::render::renderer::render_finished_semaphores;
std::vector<VkFence> rhr::render::renderer::in_flight_fences;
std::vector<VkFence> rhr::render::renderer::images_in_flight;
bool rhr::render::renderer::frame_buffer_resized;
glm::mat4 rhr::render::renderer::view_matrix;
glm::mat4 rhr::render::renderer::projection_matrix;
glm::mat4 rhr::render::renderer::ui_projection_matrix;
bool rhr::render::renderer::vsync_enabled;
glm::vec<2, u32> rhr::render::renderer::window_size;
std::shared_ptr<rhr::render::frame> rhr::render::renderer::debug_frame;

VkPipelineLayout rhr::render::renderer::blocks_pipeline_layout;
VkPipelineLayout rhr::render::renderer::ui_pipeline_layout;
VkPipelineLayout rhr::render::renderer::ui_texture_pipeline_layout;

VkPipeline rhr::render::renderer::ui_pipeline;
VkPipeline rhr::render::renderer::ui_texture_pipeline;

u32 rhr::render::renderer::depth_background = 100;
u32 rhr::render::renderer::depth_plane = 95;
u32 rhr::render::renderer::depth_collection = 90;
u32 rhr::render::renderer::depth_stack = 85;
u32 rhr::render::renderer::depth_block = 80;
u32 rhr::render::renderer::depth_argument = 75;
u32 rhr::render::renderer::depth_argument_text = 70;

std::vector<std::weak_ptr<rhr::render::interfaces::i_renderable>> rhr::render::renderer::m_dirty_renderable;
std::vector<std::weak_ptr<rhr::render::layer>> rhr::render::renderer::m_layers;
std::shared_mutex rhr::render::renderer::m_dirty_mutex;