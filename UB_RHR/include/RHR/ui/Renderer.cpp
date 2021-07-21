#include "Renderer.hpp"

#include "ui/RenderTools.hpp"
#include "ui/RenderObject.hpp"
#include "ui/RenderRectangle.hpp"
#include "ui/Vertex.hpp"
#include "stacking/Plane.hpp"

#include <Espresso/InputHandler.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

const std::vector<Vertex> testVertices = {
	{ { -0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
	{ { 0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f } },
	{ { 0.5f, 0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
	{ { -0.5f, 0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } },

	{ { -0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
	{ { 0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f } },
	{ { 0.5f, 0.5f, -0.5f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
	{ { -0.5f, 0.5f, -0.5f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }
};

const std::vector<uint32_t> testIndices = {
	0, 1, 2, 2, 3, 0,
	4, 5, 6, 6, 7, 4
};

void frame_buffer_resize_callback(GLFWwindow* window, int width, int height)
{
	Renderer::FramebufferResized = true;
	Renderer::WindowSize = { width, height };
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	InputHandler::FireKey(key, action);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	InputHandler::FireMouseButton(button, action);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	InputHandler::FireMouseScroll(yoffset);
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	InputHandler::FireMouseMove({ xpos, ypos });
}

VkImage depthImage;
VkDeviceMemory depthImageMemory;

void Renderer::InitializeWindow()
{
	PhysicalDevice = VK_NULL_HANDLE;
	Surface = VK_NULL_HANDLE;
	FramebufferResized = false;
	VsyncEnabled = false;

	ViewMatrix = glm::mat4(1.0);
	ProjectionMatrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);

	WindowSize = { 1280, 720 };
	glm::vec<2, int32_t> windowSize = WindowSize;

	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	char windowTitle[100];
	memset(windowTitle, 0, 100);
	sprintf(windowTitle, "Unnamed Blocks %s", VER_CLIENT);

	Window = glfwCreateWindow(windowSize.x, windowSize.y, windowTitle, NULL, NULL);

	if (Window == NULL)
		Logger::Fatal("Failed to create GLFW window");

	glfwSetFramebufferSizeCallback(Window, frame_buffer_resize_callback);
	glfwSetKeyCallback(Window, key_callback);
	glfwSetMouseButtonCallback(Window, mouse_button_callback);
	glfwSetScrollCallback(Window, scroll_callback);
	glfwSetCursorPosCallback(Window, cursor_position_callback);

	InitInstance();

	auto glfwWindowResult = glfwCreateWindowSurface(Instance, Window, nullptr, &Surface);

	if (glfwWindowResult != VK_SUCCESS)
		Logger::Fatal(std::to_string(glfwWindowResult));
}

static std::shared_ptr<vui::RenderRectangle> testObject = std::make_shared<vui::RenderRectangle>();

void Renderer::Initialization()
{
	testObject->SetWeak(testObject);
	testObject->SetPosition({ 0, 0 });
	testObject->SetSize({ 256, 256 });
	testObject->SetColor(Color::White);
	testObject->SetTexture(RenderObject::TextureType::TEXT_SHEET);

	InitDebug();
	InitDevice();

	QueueFamilyIndices indices = RenderTools::FindQueueFamilies(&PhysicalDevice, &Surface);

	std::set<uint32_t> uniqueQueueFamilies = { indices.GraphicsFamily.value(), indices.PresentFamily.value() };

	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		QueueCreateInfos.push_back(queueCreateInfo);
	}

	InitLogicalDevice();
	vkGetDeviceQueue(Device, indices.PresentFamily.value(), 0, &PresentQueue);

	InitSwapChain();
	InitImageViews();
	InitRenderPass();
	InitDescriptorSetLayout();
	InitPipelines();
	InitCommandPool();
	InitDepthResources();
	InitFrameBuffers();
	InitTextureSampler();
	InitDescriptorPool();
	InitCommandBuffers();
	InitSyncObjects();

	RenderTools::CreateAuxCommandBuffer();
}

void Renderer::AddDirty(std::weak_ptr<IRenderable> renderable)
{
	std::unique_lock lock(m_DirtyMutex);
	m_DirtyRenderable.push_back(renderable);
}

void Renderer::ProcessDirty()
{
	std::unique_lock lock(m_DirtyMutex);

	for (auto renderable : m_DirtyRenderable)
	{
		if (auto object = renderable.lock())
			object->UpdateBuffers();
	}

	m_DirtyRenderable.clear();
}

void Renderer::Render(size_t idx, double deltaTime, bool setup, TIME_POINT& diagnosticsTime)
{
	// External Frame Updating

	// if (!setup)
	// {
	// 	Client::Instance->GetPlayer()->UpdateCamera(mousePosition);
	// 	Renderer::ViewMatrix = glm::mat4(1.0);
	// 	Renderer::ProjectionMatrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);
	// }

	Renderer::UIProjectionMatrix = glm::ortho(0.0f, static_cast<float>(WindowSize.x), 0.0f, static_cast<float>(WindowSize.y), -10000.0f, 10000.0f);

	// Render Frame

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0; // Optional
	beginInfo.pInheritanceInfo = nullptr; // Optional

	if (vkBeginCommandBuffer(ActiveCommandBuffer, &beginInfo) != VK_SUCCESS)
		Logger::Fatal("failed to start the command buffer");

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = RenderPass;
	renderPassInfo.framebuffer = SwapChainFramebuffers[idx];

	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = SwapChainExtent;

	std::array<VkClearValue, 2> clearValues{};
	clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
	clearValues[1].depthStencil = { 1.0f, 0 };

	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	// Client::Instance->GetDiagnostics()->SetRenderTime(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - diagnosticsTime).count());
	// diagnosticsTime = std::chrono::high_resolution_clock::now();
	Plane::PrimaryPlane->FrameUpdate(deltaTime);
	Plane::ToolbarPlane->FrameUpdate(deltaTime);

	ProcessDirty();
	// Client::Instance->GetDiagnostics()->SetDirtyTime(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - diagnosticsTime).count());
	// diagnosticsTime = std::chrono::high_resolution_clock::now();

	vkCmdBeginRenderPass(ActiveCommandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	// if (!setup)
	// {
	// 	// ChunkManager binds BlocksPipeline
	// 	Client::Instance->GetChunkManager()->Render(deltaTime);
	// 	Client::Instance->GetDispatcher()->UpdateDiagnosticData();
	// 	Client::Instance->GetDiagnostics()->UpdateProgressBars();
	// }

	Plane::PrimaryPlane->Render();
	Plane::ToolbarPlane->Render();

	testObject->Render();

	bool erased = false;

	for (size_t i = 0; i < m_Layers.size(); i++)
	{
		if (erased)
		{
			erased = false;
			i--;
		}

		if (auto layer = m_Layers[i].lock())
			layer->Render();
		else
		{
			m_Layers.erase(m_Layers.begin() + i);
			erased = true;
		}
	}

	vkCmdEndRenderPass(ActiveCommandBuffer);

	if (vkEndCommandBuffer(ActiveCommandBuffer) != VK_SUCCESS)
		Logger::Fatal("failed to close the command buffer");
}

void Renderer::CleanupSwapChain()
{
	vkDestroyImageView(Device, DepthImageView, nullptr);
	vkDestroyImage(Device, depthImage, nullptr);
	vkFreeMemory(Device, depthImageMemory, nullptr);

	for (size_t i = 0; i < SwapChainFramebuffers.size(); i++)
	{
		vkDestroyFramebuffer(Device, SwapChainFramebuffers[i], nullptr);
	}

	vkFreeCommandBuffers(Device, CommandPool, static_cast<uint32_t>(CommandBuffers.size()), CommandBuffers.data());

	//vkDestroyPipeline(Device, BlocksPipeline, nullptr);
	vkDestroyPipeline(Device, UIPipeline, nullptr);
	vkDestroyPipeline(Device, UITexturePipeline, nullptr);

	vkDestroyPipelineLayout(Device, BlocksPipelineLayout, nullptr);
	vkDestroyPipelineLayout(Device, UIPipelineLayout, nullptr);
	vkDestroyPipelineLayout(Device, UITexturePipelineLayout, nullptr);

	vkDestroyRenderPass(Device, RenderPass, nullptr);

	for (size_t i = 0; i < SwapChainImageViews.size(); i++)
	{
		vkDestroyImageView(Device, SwapChainImageViews[i], nullptr);
	}

	vkDestroySwapchainKHR(Device, SwapChain, nullptr);

	//for (size_t i = 0; i < m_SwapChainImages.size(); i++)
	//{
	//	vkDestroyBuffer(m_Device, m_UniformBuffers[i], nullptr);
	//	vkFreeMemory(m_Device, m_UniformBuffersMemory[i], nullptr);
	//}

	vkDestroyDescriptorPool(Device, DescriptorPool, nullptr);
}

void Renderer::Cleanup()
{
	CleanupSwapChain();

	vkDestroySampler(Device, TextureSampler, nullptr);
	//vkDestroyImageView(m_Device, m_TextureImageView, nullptr);

	//vkDestroyImage(m_Device, m_TextureImage, nullptr);
	//vkFreeMemory(m_Device, m_TextureImageMemory, nullptr);

	vkDestroyDescriptorSetLayout(Device, DescriptorSetLayout, nullptr);

	//vkDestroyBuffer(m_Device, m_IndexBuffer, nullptr);
	//vkFreeMemory(m_Device, m_IndexBufferMemory, nullptr);

	//vkDestroyBuffer(m_Device, m_VertexBuffer, nullptr);
	//vkFreeMemory(m_Device, vertexBufferMemory, nullptr);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		vkDestroySemaphore(Device, RenderFinishedSemaphores[i], nullptr);
		vkDestroySemaphore(Device, ImageAvailableSemaphores[i], nullptr);
		vkDestroyFence(Device, InFlightFences[i], nullptr);
	}

	vkDestroyCommandPool(Device, CommandPool, nullptr);

	vkDestroyDevice(Device, nullptr);

	if (EnableValidationLayers)
		RenderTools::DestroyDebugUtilsMessengerEXT(&Instance, &DebugMessenger, nullptr);

	vkDestroySurfaceKHR(Instance, Surface, nullptr);
	vkDestroyInstance(Instance, nullptr);

	glfwDestroyWindow(Window);

	glfwTerminate();
}

void Renderer::RecreateSwapChain()
{
	int width = 0, height = 0;
	glfwGetFramebufferSize(Window, &width, &height);

	while (width == 0 || height == 0)
	{
		glfwGetFramebufferSize(Window, &width, &height);
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(Device);

	CleanupSwapChain();

	InitSwapChain();
	InitImageViews();
	InitRenderPass();
	InitPipelines();
	InitDepthResources();
	InitFrameBuffers();
	InitDescriptorPool();
	InitCommandBuffers();
	InitSyncObjects();
}

void Renderer::AddLayer(std::weak_ptr<vui::RenderLayer> layer)
{
	m_Layers.push_back(layer);
}

void Renderer::ReloadLayerSwapChains()
{
	bool erased = false;

	for (size_t i = 0; i < m_Layers.size(); i++)
	{
		if (erased)
		{
			erased = false;
			i--;
		}

		if (auto layer = m_Layers[i].lock())
			layer->ReloadSwapChain();
		else
		{
			m_Layers.erase(m_Layers.begin() + i);
			erased = true;
		}
	}
}

void Renderer::InitInstance()
{
	VkApplicationInfo appInfo;
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "MCC";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;
	appInfo.pNext = NULL;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	auto requiredExtensions = RenderTools::GetRequiredExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
	createInfo.ppEnabledExtensionNames = requiredExtensions.data();

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
	if (EnableValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(RenderTools::ValidationLayers.size());
		createInfo.ppEnabledLayerNames = RenderTools::ValidationLayers.data();

		RenderTools::PopulateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
	}
	else
	{
		createInfo.enabledLayerCount = 0;
		createInfo.pNext = nullptr;
	}

	if (vkCreateInstance(&createInfo, nullptr, &Instance) != VK_SUCCESS)
		Logger::Fatal("vulkan failed to initialize");

	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

	if (EnableValidationLayers && !RenderTools::CheckValidationLayerSupport())
		Logger::Fatal("vulkan validation layers unsupported");
}

void Renderer::InitDebug()
{
	if (!EnableValidationLayers)
		return;

	VkDebugUtilsMessengerCreateInfoEXT createInfo;
	RenderTools::PopulateDebugMessengerCreateInfo(createInfo);

	if (RenderTools::CreateDebugUtilsMessengerEXT(&Instance, &createInfo, nullptr, &DebugMessenger) != VK_SUCCESS)
		Logger::Fatal("failed to create debug utils message EXT");
}

void Renderer::InitDevice()
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(Instance, &deviceCount, nullptr);

	if (deviceCount == 0)
		Logger::Fatal("no supported vulkan devices found");

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(Instance, &deviceCount, devices.data());

	for (auto& device : devices)
	{
		if (RenderTools::IsDeviceSuitable(&device, &Surface))
		{
			PhysicalDevice = device;
			break;
		}
	}

	if (PhysicalDevice == VK_NULL_HANDLE)
		Logger::Fatal("failed to pick appropriate device");
}

void Renderer::InitLogicalDevice()
{
	QueueFamilyIndices indices = RenderTools::FindQueueFamilies(&PhysicalDevice, &Surface);

	VkDeviceQueueCreateInfo queueCreateInfo{};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = indices.GraphicsFamily.value();
	queueCreateInfo.queueCount = 1;

	float queuePriority = 1.0f;
	queueCreateInfo.pQueuePriorities = &queuePriority;

	VkPhysicalDeviceFeatures deviceFeatures{};
	deviceFeatures.samplerAnisotropy = VK_TRUE;

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	createInfo.queueCreateInfoCount = static_cast<uint32_t>(QueueCreateInfos.size());
	createInfo.pQueueCreateInfos = QueueCreateInfos.data();
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = static_cast<uint32_t>(RenderTools::DeviceExtensions.size());
	createInfo.ppEnabledExtensionNames = RenderTools::DeviceExtensions.data();

	if (EnableValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(RenderTools::ValidationLayers.size());
		createInfo.ppEnabledLayerNames = RenderTools::ValidationLayers.data();
	}
	else
		createInfo.enabledLayerCount = 0;

	if (vkCreateDevice(PhysicalDevice, &createInfo, nullptr, &Device) != VK_SUCCESS)
		Logger::Fatal("failed to create logical device link to gpu");

	vkGetDeviceQueue(Device, indices.GraphicsFamily.value(), 0, &GraphicsQueue);
}

void Renderer::InitSwapChain()
{
	SwapChainSupportDetails swapChainSupport = RenderTools::QuerySwapChainSupport(&PhysicalDevice, &Surface);

	VkSurfaceFormatKHR surfaceFormat = RenderTools::ChooseSwapSurfaceFormat(swapChainSupport.Formats);
	VkPresentModeKHR presentMode = RenderTools::ChooseSwapPresentMode(swapChainSupport.PresentModes);
	VkExtent2D extent = RenderTools::ChooseSwapExtent(swapChainSupport.Capabilities);

	uint32_t imageCount = swapChainSupport.Capabilities.minImageCount + 1;

	if (swapChainSupport.Capabilities.maxImageCount > 0 && imageCount > swapChainSupport.Capabilities.maxImageCount)
		imageCount = swapChainSupport.Capabilities.maxImageCount;

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = Surface;

	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices indices = RenderTools::FindQueueFamilies(&PhysicalDevice, &Surface);
	uint32_t queueFamilyIndices[] = { indices.GraphicsFamily.value(), indices.PresentFamily.value() };

	if (indices.GraphicsFamily != indices.PresentFamily)
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0; // Optional
		createInfo.pQueueFamilyIndices = nullptr; // Optional
	}

	createInfo.preTransform = swapChainSupport.Capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(Device, &createInfo, nullptr, &SwapChain) != VK_SUCCESS)
		Logger::Fatal("failed to create swap chain");

	vkGetSwapchainImagesKHR(Device, SwapChain, &imageCount, nullptr);
	SwapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(Device, SwapChain, &imageCount, SwapChainImages.data());

	SwapChainImageFormat = surfaceFormat.format;
	SwapChainExtent = extent;
}

void Renderer::InitImageViews()
{
	SwapChainImageViews.resize(SwapChainImages.size());

	for (uint32_t i = 0; i < SwapChainImages.size(); i++)
		SwapChainImageViews[i] = RenderTools::CreateImageView(SwapChainImages[i], SwapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
}

void Renderer::InitRenderPass()
{
	VkSubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcAccessMask = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

	VkAttachmentDescription colorAttachment{};
	colorAttachment.format = SwapChainImageFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentDescription depthAttachment{};
	depthAttachment.format = RenderTools::FindDepthFormat();
	depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference colorAttachmentRef{};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthAttachmentRef{};
	depthAttachmentRef.attachment = 1;
	depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	subpass.pDepthStencilAttachment = &depthAttachmentRef;

	std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };
	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	if (vkCreateRenderPass(Device, &renderPassInfo, nullptr, &RenderPass) != VK_SUCCESS)
		Logger::Fatal("failed to create render pass");
}

void Renderer::InitDescriptorSetLayout()
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
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	layoutInfo.pBindings = bindings.data();

	if (vkCreateDescriptorSetLayout(Device, &layoutInfo, nullptr, &DescriptorSetLayout) != VK_SUCCESS)
		Logger::Fatal("failed to set descriptor layout");
}

void Renderer::CreatePipeline(const std::string& shader, VkPipeline* pipeline, VkPipelineLayout* layout)
{
	auto vertShaderCode = RenderTools::ReadFileBytes("res/shaders/" + shader + ".vert.spv");
	auto fragShaderCode = RenderTools::ReadFileBytes("res/shaders/" + shader + ".frag.spv");

	VkShaderModule vertShaderModule = RenderTools::CreateShaderModule(*vertShaderCode, &Device);
	VkShaderModule fragShaderModule = RenderTools::CreateShaderModule(*fragShaderCode, &Device);

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

	auto bindingDescription = Vertex::GetBindingDescription();
	auto attributeDescriptions = Vertex::GetAttributeDescriptions();

	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)SwapChainExtent.width;
	viewport.height = (float)SwapChainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = SwapChainExtent;

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
	pipelineLayoutInfo.pSetLayouts = &DescriptorSetLayout;
	pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
	pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

	if (vkCreatePipelineLayout(Device, &pipelineLayoutInfo, nullptr, layout) != VK_SUCCESS)
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
	pipelineInfo.renderPass = RenderPass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
	pipelineInfo.basePipelineIndex = -1; // Optional

	if (vkCreateGraphicsPipelines(Device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, pipeline) != VK_SUCCESS)
		Logger::Fatal("failed to create graphics pipeline");
}

void Renderer::InitPipelines()
{
	//CreatePipeline("blocks", &BlocksPipeline, &BlocksPipelineLayout);
	CreatePipeline("ui", &UIPipeline, &UIPipelineLayout);
	CreatePipeline("ui_texture", &UITexturePipeline, &UITexturePipelineLayout);
}

void Renderer::InitCommandPool()
{
	QueueFamilyIndices queueFamilyIndices = RenderTools::FindQueueFamilies(&PhysicalDevice, &Surface);

	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = queueFamilyIndices.GraphicsFamily.value();
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; // Optional

	if (vkCreateCommandPool(Device, &poolInfo, nullptr, &CommandPool) != VK_SUCCESS)
		Logger::Fatal("failed to create command pool");
}

void Renderer::InitFrameBuffers()
{
	SwapChainFramebuffers.resize(SwapChainImageViews.size());

	for (size_t i = 0; i < SwapChainImageViews.size(); i++)
	{
		std::array<VkImageView, 2> attachments = {
			SwapChainImageViews[i],
			DepthImageView
		};

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = RenderPass;
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = SwapChainExtent.width;
		framebufferInfo.height = SwapChainExtent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(Device, &framebufferInfo, nullptr, &SwapChainFramebuffers[i]) != VK_SUCCESS)
			Logger::Fatal("failed to create frame buffers");
	}
}

void Renderer::InitDepthResources()
{
	VkFormat depthFormat = RenderTools::FindDepthFormat();
	RenderTools::CreateImage(SwapChainExtent.width, SwapChainExtent.height, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory);
	DepthImageView = RenderTools::CreateImageView(depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
	RenderTools::TransitionImageLayout(depthImage, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
}

void Renderer::InitTextureSampler()
{
	VkPhysicalDeviceProperties properties{};
	vkGetPhysicalDeviceProperties(PhysicalDevice, &properties);

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

	if (vkCreateSampler(Device, &samplerInfo, nullptr, &TextureSampler) != VK_SUCCESS)
		Logger::Fatal("failed to create texture sampler");
}

void Renderer::InitDescriptorPool()
{
	std::array<VkDescriptorPoolSize, 2> poolSizes{};

	poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSizes[0].descriptorCount = static_cast<uint32_t>(10000);

	poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[1].descriptorCount = static_cast<uint32_t>(10000);

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = 10000;
	poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

	if (vkCreateDescriptorPool(Device, &poolInfo, nullptr, &DescriptorPool) != VK_SUCCESS)
		Logger::Fatal("failed to create descriptor pool");
}

void Renderer::InitCommandBuffers()
{
	CommandBuffers.resize(SwapChainFramebuffers.size());

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = CommandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)CommandBuffers.size();

	if (vkAllocateCommandBuffers(Device, &allocInfo, CommandBuffers.data()) != VK_SUCCESS)
		Logger::Fatal("failed to create command buffers");

	for (size_t i = 0; i < CommandBuffers.size(); i++)
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = 0; // Optional
		beginInfo.pInheritanceInfo = nullptr; // Optional

		if (vkBeginCommandBuffer(CommandBuffers[i], &beginInfo) != VK_SUCCESS)
			Logger::Fatal("failed to start the command buffer");

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = RenderPass;
		renderPassInfo.framebuffer = SwapChainFramebuffers[i];

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = SwapChainExtent;

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };

		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(CommandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		vkCmdBindPipeline(CommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, UIPipeline);
		vkCmdEndRenderPass(CommandBuffers[i]);

		if (vkEndCommandBuffer(CommandBuffers[i]) != VK_SUCCESS)
			Logger::Fatal("failed to close the command buffer");
	}
}

void Renderer::InitSyncObjects()
{
	for (size_t i = 0; i < RenderFinishedSemaphores.size(); i++)
	{
		vkDestroySemaphore(Device, RenderFinishedSemaphores[i], nullptr);
		vkDestroySemaphore(Device, ImageAvailableSemaphores[i], nullptr);
		vkDestroyFence(Device, InFlightFences[i], nullptr);
	}

	ImageAvailableSemaphores.clear();
	RenderFinishedSemaphores.clear();
	InFlightFences.clear();
	ImagesInFlight.clear();

	ImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	RenderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	InFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
	ImagesInFlight.resize(SwapChainImages.size(), VK_NULL_HANDLE);

	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		if (vkCreateSemaphore(Device, &semaphoreInfo, nullptr, &ImageAvailableSemaphores[i]) != VK_SUCCESS || vkCreateSemaphore(Device, &semaphoreInfo, nullptr, &RenderFinishedSemaphores[i]) != VK_SUCCESS || vkCreateFence(Device, &fenceInfo, nullptr, &InFlightFences[i]) != VK_SUCCESS)
			Logger::Fatal("failed to create semaphores");
	}
}

VkInstance Renderer::Instance;
VkDebugUtilsMessengerEXT Renderer::DebugMessenger;
VkPhysicalDevice Renderer::PhysicalDevice;
VkDevice Renderer::Device;
VkQueue Renderer::GraphicsQueue;
VkQueue Renderer::PresentQueue;
VkSurfaceKHR Renderer::Surface;
VkSwapchainKHR Renderer::SwapChain;
GLFWwindow* Renderer::Window;
std::vector<VkImage> Renderer::SwapChainImages;
std::vector<VkImageView> Renderer::SwapChainImageViews;
VkFormat Renderer::SwapChainImageFormat;
VkExtent2D Renderer::SwapChainExtent;
std::vector<VkFramebuffer> Renderer::SwapChainFramebuffers;
std::vector<VkCommandBuffer> Renderer::CommandBuffers;
VkCommandBuffer Renderer::ActiveCommandBuffer;
//VkCommandBuffer Renderer::AuxCommandBuffer;
//std::vector<VkBuffer> Renderer::AuxBufferMemory;
//std::vector<VkDeviceMemory> Renderer::AuxDeviceMemory;
//std::shared_mutex Renderer::AuxCommandBufferMutex;
std::vector<VkDeviceQueueCreateInfo> Renderer::QueueCreateInfos;
VkRenderPass Renderer::RenderPass;
VkDescriptorSetLayout Renderer::DescriptorSetLayout;
VkCommandPool Renderer::CommandPool;
VkDescriptorPool Renderer::DescriptorPool;
VkSampler Renderer::TextureSampler;
VkImageView Renderer::DepthImageView;
std::vector<VkSemaphore> Renderer::ImageAvailableSemaphores;
std::vector<VkSemaphore> Renderer::RenderFinishedSemaphores;
std::vector<VkFence> Renderer::InFlightFences;
std::vector<VkFence> Renderer::ImagesInFlight;
bool Renderer::FramebufferResized;
glm::mat4 Renderer::ViewMatrix;
glm::mat4 Renderer::ProjectionMatrix;
glm::mat4 Renderer::UIProjectionMatrix;
bool Renderer::VsyncEnabled;
glm::vec<2, uint32_t> Renderer::WindowSize;
std::shared_ptr<vui::RenderFrame> Renderer::DebugFrame;

VkPipelineLayout Renderer::BlocksPipelineLayout;
VkPipelineLayout Renderer::UIPipelineLayout;
VkPipelineLayout Renderer::UITexturePipelineLayout;

//VkPipeline Renderer::BlocksPipeline;
VkPipeline Renderer::UIPipeline;
VkPipeline Renderer::UITexturePipeline;

uint32_t Renderer::DepthBackground = 100;
uint32_t Renderer::DepthPlane = 95;
uint32_t Renderer::DepthCollection = 90;
uint32_t Renderer::DepthStack = 85;
uint32_t Renderer::DepthBlock = 80;
uint32_t Renderer::DepthArgument = 75;
uint32_t Renderer::DepthArgumentText = 70;

std::vector<std::weak_ptr<IRenderable>> Renderer::m_DirtyRenderable;
std::vector<std::weak_ptr<vui::RenderLayer>> Renderer::m_Layers;
std::shared_mutex Renderer::m_DirtyMutex;