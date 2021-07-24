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

	rhr::render::tools::rhr::render::tools::queue_family_indices indices = rhr::render::tools::find_queue_families(&physical_device, &surface);

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
	Plane::PrimaryPlane->FrameUpdate(deltaTime);
	Plane::ToolbarPlane->FrameUpdate(deltaTime);

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

	Plane::PrimaryPlane->render();
	Plane::ToolbarPlane->render();


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
	VkDescriptorSetLayoutBinding uboLayoutBinding{};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

	VkDescriptorSetLayoutBinding samplerLayoutBinding{};
	samplerLayoutBinding.binding = 1;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.pImmutableSamplers = nullptr;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };
	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<u32>(bindings.size());
	layoutInfo.pBindings = bindings.data();

	if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptor_set_layout) != VK_SUCCESS)
		Logger::Fatal("failed to set descriptor layout");
}

void rhr::render::renderer::create_pipeline(const std::string& shader, VkPipeline* pipeline, VkPipelineLayout* layout)
{
	auto vertShaderCode = rhr::render::tools::ReadFileBytes("res/shaders/" + shader + ".vert.spv");
	auto fragShaderCode = rhr::render::tools::ReadFileBytes("res/shaders/" + shader + ".frag.spv");

	VkShaderModule vertShaderModule = rhr::render::tools::create_shader_module(*vertShaderCode, &device);
	VkShaderModule fragShaderModule = rhr::render::tools::create_shader_module(*fragShaderCode, &device);

	VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;

	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

	auto bindingDescription = rhr::render::vertex::GetBindingDescription();
	auto attributeDescriptions = rhr::render::vertex::GetAttributeDescriptions();

	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<u32>(attributeDescriptions.size());
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (f32)swap_chain_extent.width;
	viewport.height = (f32)swap_chain_extent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = swap_chain_extent;

	VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

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

	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_TRUE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

	VkPipelineColorBlendStateCreateInfo colorBlending{};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f; // Optional
	colorBlending.blendConstants[1] = 0.0f; // Optional
	colorBlending.blendConstants[2] = 0.0f; // Optional
	colorBlending.blendConstants[3] = 0.0f; // Optional

	VkDynamicState dynamicStates[] = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_LINE_WIDTH
	};

	VkPipelineDynamicStateCreateInfo dynamicState{};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = 2;
	dynamicState.pDynamicStates = dynamicStates;

	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &descriptor_set_layout;
	pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
	pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

	if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, layout) != VK_SUCCESS)
		Logger::Fatal("failed to create pipeline layout");

	VkPipelineDepthStencilStateCreateInfo depthStencil{};
	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = VK_TRUE;
	depthStencil.depthWriteEnable = VK_TRUE;
	depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencil.depthBoundsTestEnable = VK_FALSE;
	depthStencil.minDepthBounds = 0.0f; // Optional
	depthStencil.maxDepthBounds = 1.0f; // Optional
	depthStencil.stencilTestEnable = VK_FALSE;
	depthStencil.front = {}; // Optional
	depthStencil.back = {}; // Optional

	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = &depthStencil;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = nullptr; // Optional

	pipelineInfo.layout = *layout;
	pipelineInfo.renderPass = render_pass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
	pipelineInfo.basePipelineIndex = -1; // Optional

	if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, pipeline) != VK_SUCCESS)
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
	rhr::render::tools::queue_family_indices queueFamilyIndices = rhr::render::tools::find_queue_families(&physical_device, &surface);

	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphics_family.value();
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; // Optional

	if (vkCreateCommandPool(device, &poolInfo, nullptr, &command_pool) != VK_SUCCESS)
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

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = render_pass;
		framebufferInfo.attachmentCount = static_cast<u32>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = swap_chain_extent.width;
		framebufferInfo.height = swap_chain_extent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swap_chain_frame_buffers[i]) != VK_SUCCESS)
			Logger::Fatal("failed to create frame buffers");
	}
}

void rhr::render::renderer::init_depth_resources()
{
	VkFormat depthFormat = rhr::render::tools::find_depth_format();
	rhr::render::tools::create_image(swap_chain_extent.width, swap_chain_extent.height, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory);
	depth_image_view = rhr::render::tools::CreateImageView(depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
	rhr::render::tools::transition_image_layout(depthImage, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
}

void rhr::render::renderer::init_texture_sampler()
{
	VkPhysicalDeviceProperties properties{};
	vkGetPhysicalDeviceProperties(physical_device, &properties);

	VkSamplerCreateInfo samplerInfo{};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_NEAREST;
	samplerInfo.minFilter = VK_FILTER_NEAREST;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

	samplerInfo.anisotropyEnable = VK_TRUE;
	samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;

	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = 0.0f;

	if (vkCreateSampler(device, &samplerInfo, nullptr, &texture_sampler) != VK_SUCCESS)
		Logger::Fatal("failed to create texture sampler");
}

void rhr::render::renderer::init_descriptor_pool()
{
	std::array<VkDescriptorPoolSize, 2> poolSizes{};

	poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSizes[0].descriptorCount = static_cast<u32>(10000);

	poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[1].descriptorCount = static_cast<u32>(10000);

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<u32>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = 10000;
	poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

	if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptor_pool) != VK_SUCCESS)
		Logger::Fatal("failed to create descriptor pool");
}

void rhr::render::renderer::init_command_buffers()
{
	command_buffers.resize(swap_chain_frame_buffers.size());

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = command_pool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (u32)command_buffers.size();

	if (vkAllocateCommandBuffers(device, &allocInfo, command_buffers.data()) != VK_SUCCESS)
		Logger::Fatal("failed to create command buffers");

	for (usize i = 0; i < command_buffers.size(); i++)
	{
		vk::command_buffer_begin_info beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = 0; // Optional
		beginInfo.pInheritanceInfo = nullptr; // Optional

		if (vkBeginCommandBuffer(command_buffers[i], &beginInfo) != VK_SUCCESS)
			Logger::Fatal("failed to start the command buffer");

		vk::render_pass_begin_info renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = render_pass;
		renderPassInfo.framebuffer = swap_chain_frame_buffers[i];

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = swap_chain_extent;

		std::array<vk::clear_value, 2> clearValues{};
		clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };

		renderPassInfo.clearValueCount = static_cast<u32>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(command_buffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
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

	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (usize i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &image_available_semaphores[i]) != VK_SUCCESS || vkCreateSemaphore(device, &semaphoreInfo, nullptr, &render_finished_semaphores[i]) != VK_SUCCESS || vkCreateFence(device, &fenceInfo, nullptr, &in_flight_fences[i]) != VK_SUCCESS)
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
//VkCommandBuffer rhr::render::renderer::AuxCommandBuffer;
//std::vector<VkBuffer> rhr::render::renderer::AuxBufferMemory;
//std::vector<vk::device_memory> rhr::render::renderer::AuxDeviceMemory;
//std::shared_mutex rhr::render::renderer::AuxCommandBufferMutex;
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
std::shared_ptr<vui::RenderFrame> rhr::render::renderer::debug_frame;

VkPipelineLayout rhr::render::renderer::blocks_pipeline_layout;
VkPipelineLayout rhr::render::renderer::ui_pipeline_layout;
VkPipelineLayout rhr::render::renderer::ui_texture_pipeline_layout;

//VkPipeline rhr::render::renderer::BlocksPipeline;
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