#include "renderer.hpp"

#include "rhr/rendering/tools.hpp"
#include "rhr/rendering/objects/object.hpp"
#include "rhr/rendering/objects/rectangle.hpp"
#include "rhr/rendering/vertex.hpp"
#include "rhr/stacking/plane.hpp"
#include "rhr/handlers/category.hpp"
#include "rhr/rendering/swap_chain.hpp"
#include "rhr/rendering/render_pass.hpp"
#include "rhr/rendering/command.hpp"
#include "rhr/rendering/pipeline.hpp"
#include "rhr/rendering/device.hpp"
#include "rhr/rendering/panel.hpp"

#include <espresso/input_handler.hpp>
#include <cappuccino/utils.hpp>
#include <iostream>

//#define STB_IMAGE_IMPLEMENTATION
//#include <stb_image.h>

static bool run_first_render_pass = false;

static void check_vk_result(VkResult err)
{
	if (err != VK_SUCCESS)
		cap::logger::error("vulkan error code \"" + std::to_string(err) + "\"");
}

void frame_buffer_resize_callback(GLFWwindow* window, i32 width, i32 height)
{
	rhr::render::renderer::frame_buffer_resized = true;
	rhr::render::renderer::window_size = { width, height };
}

void key_callback(GLFWwindow* window, i32 key, i32 scancode, i32 action, i32 mode)
{
	if (action == GLFW_PRESS && key == GLFW_KEY_Q)
		run_first_render_pass = !run_first_render_pass;

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

void window_position_callback(GLFWwindow* window, i32 x, i32 y)
{
	rhr::render::renderer::window_position = { x, y };
}

vk::image depthImage;
vk::device_memory depthImageMemory;

void rhr::render::renderer::initialize_window()
{
	rhr::render::device::physical_device = VK_NULL_HANDLE;
	surface = VK_NULL_HANDLE;
	frame_buffer_resized = false;
	vsync_enabled = false;

	view_matrix = glm::mat4(1.0);
	projection_matrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);

	window_size = { 1280, 720 };

	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	char window_title[100];
	memset(window_title, 0, 100);
	sprintf(window_title, "Unnamed Blocks %s", VER_CLIENT);

	window = glfwCreateWindow(window_size.x, window_size.y, window_title, NULL, NULL);

	if (window == NULL)
		cap::logger::fatal("Failed to create GLFW window");

	glfwSetFramebufferSizeCallback(window, frame_buffer_resize_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetWindowPosCallback(window, window_position_callback);

	rhr::render::device::init_instance();

	auto glfw_window_result = glfwCreateWindowSurface(rhr::render::device::instance, window, nullptr, &surface);

	if (glfw_window_result != VK_SUCCESS)
		cap::logger::fatal(std::to_string(glfw_window_result));
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

	rhr::render::device::init_debug();
	rhr::render::device::init_device();

	rhr::render::tools::queue_family_indices indices = rhr::render::tools::find_queue_families(&rhr::render::device::physical_device, &surface);
	std::set<u32> unique_queue_families = { indices.graphics_family.value(), indices.present_family.value() };

	f32 queue_priority = 1.0f;
	for (u32 queue_family : unique_queue_families)
	{
		vk::device_queue_create_info queue_create_info{};
		queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queue_create_info.queueFamilyIndex = queue_family;
		queue_create_info.queueCount = 1;
		queue_create_info.pQueuePriorities = &queue_priority;
		rhr::render::command::queue_create_infos.push_back(queue_create_info);
	}

	rhr::render::device::init_logical_device();
	rhr::render::swap_chain::init_swap_chain();
	rhr::render::swap_chain::init_image_views();
	rhr::render::render_pass::init_render_pass();
	init_descriptor_set_layout();
	rhr::render::pipeline::init_pipelines();
	rhr::render::command::init_command_pool();
	init_depth_resources();
	rhr::render::swap_chain::init_frame_buffers();
	init_texture_sampler();
	rhr::render::command::init_descriptor_pool();
	rhr::render::command::init_command_buffers();
	rhr::render::swap_chain::init_sync_objects();

	rhr::render::tools::swap_chain_support_details swap_chain_support = rhr::render::tools::query_swap_chain_support(&rhr::render::device::physical_device, &surface);

	u32 imageCount = swap_chain_support.capabilities.minImageCount + 1;

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	// Setup Dear ImGui style
//	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	{
		ImFontAtlas *fontAtlas = io.Fonts;
		ImFontConfig fontConfig;
		fontConfig.GlyphRanges = fontAtlas->GetGlyphRangesCyrillic();
		fontConfig.MergeMode = false;
		fontConfig.PixelSnapH = false;

		io.FontDefault = io.Fonts->AddFontFromFileTTF("res/Roboto-Regular.ttf", 16, &fontConfig);
	}

	{
		ImGuiStyle &style = ImGui::GetStyle();

		const ImVec4 dockBgColor = ImVec4(0.098f, 0.098f, 0.1019f, 1.0f);
		const ImVec4 bgColor = ImVec4(0.145f, 0.145f, 0.149f, 1.0f);
		const ImVec4 lightBgColor = ImVec4(0.321f, 0.321f, 0.333f, 1.0f);
		const ImVec4 lighterBgColor = ImVec4(0.353f, 0.353f, 0.372f, 1.0f);

		const ImVec4 panelColor = ImVec4(0.2f, 0.2f, 0.21f, 1.0f);
		const ImVec4 panelHoverColor = ImVec4(0.114f, 0.592f, 0.925f, 1.0f);
		const ImVec4 panelActiveColor = ImVec4(0.0f, 0.466f, 0.784f, 1.0f);

		const ImVec4 textColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		const ImVec4 textDisabledColor = ImVec4(0.592f, 0.592f, 0.592f, 1.0f);
		const ImVec4 borderColor = ImVec4(0.305f, 0.305f, 0.305f, 1.0f);

		style.Colors[ImGuiCol_Text] = textColor;
		style.Colors[ImGuiCol_TextDisabled] = textDisabledColor;
		style.Colors[ImGuiCol_TextSelectedBg] = panelActiveColor;
		style.Colors[ImGuiCol_WindowBg] = bgColor;
		style.Colors[ImGuiCol_ChildBg] = bgColor;
		style.Colors[ImGuiCol_PopupBg] = bgColor;
		style.Colors[ImGuiCol_Border] = borderColor;
		style.Colors[ImGuiCol_BorderShadow] = borderColor;
		style.Colors[ImGuiCol_FrameBg] = panelColor;
		style.Colors[ImGuiCol_FrameBgHovered] = panelHoverColor;
		style.Colors[ImGuiCol_FrameBgActive] = panelActiveColor;
		style.Colors[ImGuiCol_TitleBg] = bgColor;
		style.Colors[ImGuiCol_TitleBgActive] = bgColor;
		style.Colors[ImGuiCol_TitleBgCollapsed] = bgColor;
		style.Colors[ImGuiCol_MenuBarBg] = panelColor;
		style.Colors[ImGuiCol_ScrollbarBg] = panelColor;
		style.Colors[ImGuiCol_ScrollbarGrab] = lightBgColor;
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = lighterBgColor;
		style.Colors[ImGuiCol_ScrollbarGrabActive] = lighterBgColor;
		style.Colors[ImGuiCol_CheckMark] = panelActiveColor;
		style.Colors[ImGuiCol_SliderGrab] = panelHoverColor;
		style.Colors[ImGuiCol_SliderGrabActive] = panelActiveColor;
		style.Colors[ImGuiCol_Button] = panelColor;
		style.Colors[ImGuiCol_ButtonHovered] = panelHoverColor;
		style.Colors[ImGuiCol_ButtonActive] = panelHoverColor;
		style.Colors[ImGuiCol_Header] = panelColor;
		style.Colors[ImGuiCol_HeaderHovered] = panelHoverColor;
		style.Colors[ImGuiCol_HeaderActive] = panelActiveColor;
		style.Colors[ImGuiCol_Separator] = borderColor;
		style.Colors[ImGuiCol_SeparatorHovered] = borderColor;
		style.Colors[ImGuiCol_SeparatorActive] = borderColor;
		style.Colors[ImGuiCol_ResizeGrip] = bgColor;
		style.Colors[ImGuiCol_ResizeGripHovered] = panelColor;
		style.Colors[ImGuiCol_ResizeGripActive] = lightBgColor;
		style.Colors[ImGuiCol_PlotLines] = panelActiveColor;
		style.Colors[ImGuiCol_PlotLinesHovered] = panelHoverColor;
		style.Colors[ImGuiCol_PlotHistogram] = panelActiveColor;
		style.Colors[ImGuiCol_PlotHistogramHovered] = panelHoverColor;
		style.Colors[ImGuiCol_DragDropTarget] = bgColor;
		style.Colors[ImGuiCol_NavHighlight] = bgColor;
		style.Colors[ImGuiCol_Tab] = bgColor;
		style.Colors[ImGuiCol_TabActive] = panelActiveColor;
		style.Colors[ImGuiCol_TabUnfocused] = bgColor;
		style.Colors[ImGuiCol_TabUnfocusedActive] = panelActiveColor;
		style.Colors[ImGuiCol_TabHovered] = panelHoverColor;

		style.Colors[ImGuiCol_DockingEmptyBg] = dockBgColor;

		style.ScrollbarSize = 10.0f;
		style.WindowRounding = 1.0f;
		style.ChildRounding = 1.0f;
		style.FrameRounding = 1.0f;
		style.GrabRounding = 1.0f;
		style.PopupRounding = 1.0f;
		style.ScrollbarRounding = 1.0f;
		style.TabRounding = 1.0f;
		style.WindowMenuButtonPosition = ImGuiDir_Right;
	}

	imgui_local = new imgui_data(io, style);

	imgui_local->frames = new ImGui_ImplVulkanH_Frame[imageCount];
	imgui_local->semaphores = new ImGui_ImplVulkanH_FrameSemaphores[imageCount];

	for (u32 i = 0; i < imageCount; i++)
	{
		imgui_local->frames[i].CommandPool = rhr::render::command::command_pool;
		imgui_local->frames[i].CommandBuffer = rhr::render::command::command_buffers[i];
		imgui_local->frames[i].Fence = rhr::render::swap_chain::in_flight_fences[i];
		imgui_local->frames[i].Backbuffer = rhr::render::swap_chain::swap_chain_images[i];
		imgui_local->frames[i].BackbufferView = rhr::render::swap_chain::swap_chain_image_views[i];
		imgui_local->frames[i].Framebuffer = rhr::render::swap_chain::swap_chain_frame_buffers[i];
	}

	for (u32 i = 0; i < imageCount; i++)
	{
		imgui_local->semaphores[i].ImageAcquiredSemaphore = rhr::render::swap_chain::image_available_semaphores[i];
		imgui_local->semaphores[i].RenderCompleteSemaphore = rhr::render::swap_chain::render_finished_semaphores[i];
	}

	imgui_local->data.Width = rhr::render::renderer::window_size.x;
	imgui_local->data.Height = rhr::render::renderer::window_size.y;
	imgui_local->data.Swapchain = rhr::render::swap_chain::swapchain_khr;
	imgui_local->data.Surface = surface;
	imgui_local->data.SurfaceFormat = surface_format;
	imgui_local->data.PresentMode = present_mode;
	imgui_local->data.RenderPass = rhr::render::render_pass::render_pass_master;
	imgui_local->data.Pipeline = rhr::render::pipeline::ui_pipeline;
//	imgui_local->data.ClearEnable = false;
//	imgui_local->data.ClearValue = VkClearValue(0.0f, 0.0f, 0.0f, 1.0f);
	imgui_local->data.FrameIndex = 0;
	imgui_local->data.ImageCount = rhr::render::swap_chain::swap_chain_images.size();
	imgui_local->data.SemaphoreIndex = 0;
	imgui_local->data.Frames = imgui_local->frames;
	imgui_local->data.FrameSemaphores = imgui_local->semaphores;

	ImGui_ImplVulkan_InitInfo imgui_info{
		.Instance = rhr::render::device::instance,
		.PhysicalDevice = rhr::render::device::physical_device,
		.Device = rhr::render::device::device_master,
		.QueueFamily = static_cast<uint32_t>(indices.graphics_family.value()),
		.Queue = graphics_queue,
		.PipelineCache = nullptr,
		.DescriptorPool = rhr::render::command::descriptor_pool,
		.MinImageCount = swap_chain_support.capabilities.minImageCount,
		.ImageCount = static_cast<uint32_t>(rhr::render::swap_chain::swap_chain_images.size()),
		.MSAASamples = VK_SAMPLE_COUNT_1_BIT,
		.Allocator = nullptr,
		.CheckVkResultFn = [](VkResult result) {
			if (result != VK_SUCCESS)
				cap::logger::fatal("imgui fatal error: " + std::to_string(static_cast<i32>(result)));
		}
	};

	ImGui_ImplGlfw_InitForVulkan(window, true);
	ImGui_ImplVulkan_Init(&imgui_info, rhr::render::render_pass::render_pass_master);

	{
		VkResult err;

		err = vkResetCommandPool(rhr::render::device::device_master, rhr::render::command::command_pool, 0);
		check_vk_result(err);

		// Use any command queue
		VkCommandPool command_pool = imgui_local->data.Frames[imgui_local->data.FrameIndex].CommandPool;
		VkCommandBuffer command_buffer = imgui_local->data.Frames[imgui_local->data.FrameIndex].CommandBuffer;

		err = vkResetCommandPool(rhr::render::device::device_master, command_pool, 0);
		check_vk_result(err);

		VkCommandBufferBeginInfo begin_info = {};
		begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		begin_info.flags = 0;

		err = vkBeginCommandBuffer(command_buffer, &begin_info);
		check_vk_result(err);

		ImGui_ImplVulkan_CreateFontsTexture(command_buffer);

		VkSubmitInfo end_info = {};
		end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		end_info.commandBufferCount = 1;
		end_info.pCommandBuffers = &command_buffer;

		err = vkEndCommandBuffer(command_buffer);
		check_vk_result(err);

		err = vkQueueSubmit(graphics_queue, 1, &end_info, VK_NULL_HANDLE);
		check_vk_result(err);

		err = vkDeviceWaitIdle(rhr::render::device::device_master);
		check_vk_result(err);

		ImGui_ImplVulkan_DestroyFontUploadObjects();
	}
}

void rhr::render::renderer::add_dirty(std::weak_ptr<rhr::render::interfaces::i_renderable> renderable)
{
	std::unique_lock lock(m_dirty_mutex);
	m_dirty_renderable.push_back(renderable);
}

void rhr::render::renderer::add_dirty(std::weak_ptr<rhr::render::interfaces::i_ui> ui)
{
	std::unique_lock lock(m_dirty_mutex);
	m_dirty_ui.push_back(ui);
}

void rhr::render::renderer::process_dirty()
{
	std::unique_lock lock(m_dirty_mutex);

	for (const auto& renderable : m_dirty_renderable)
	{
		if (auto object = renderable.lock())
			object->update_buffers();
	}

	for (const auto& ui : m_dirty_ui)
	{
		if (auto object = ui.lock())
			object->update_buffers();
	}

	m_dirty_renderable.clear();
	m_dirty_ui.clear();
}

void rhr::render::renderer::render_pass_plane()
{
	rhr::render::renderer::ui_projection_matrix = glm::ortho(0.0f, static_cast<f32>(window_size.x), 0.0f, static_cast<f32>(window_size.y), -10000.0f, 10000.0f);

	VkResult err;

	VkSemaphore image_acquired_semaphore  = imgui_local->data.FrameSemaphores[imgui_local->data.SemaphoreIndex].ImageAcquiredSemaphore;
	VkSemaphore render_complete_semaphore = imgui_local->data.FrameSemaphores[imgui_local->data.SemaphoreIndex].RenderCompleteSemaphore;

	err = vkAcquireNextImageKHR(rhr::render::device::device_master, imgui_local->data.Swapchain, UINT64_MAX, image_acquired_semaphore, VK_NULL_HANDLE, &imgui_local->data.FrameIndex);

	if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
	{
		reload_swap_chain_flag = true;
		return;
	}

	check_vk_result(err);

	ImGui_ImplVulkanH_Frame* fd = &imgui_local->data.Frames[imgui_local->data.FrameIndex];

	// frame sync

	err = vkWaitForFences(rhr::render::device::device_master, 1, &fd->Fence, VK_TRUE, UINT64_MAX);    // wait indefinitely instead of periodically checking
	check_vk_result(err);

	err = vkResetFences(rhr::render::device::device_master, 1, &fd->Fence);
	check_vk_result(err);

	// initialize command buffer for render

	//	VkResult err;

	std::array<VkClearValue, 2> clear_values{};
	clear_values[0].color = { { 0.0f, 0.0f, 0.0f, 1.0f } };
	clear_values[1].depthStencil = { 1.0f, 0 };

	//	VkSemaphore image_acquired_semaphore  = imgui_local->data.FrameSemaphores[imgui_local->data.SemaphoreIndex].ImageAcquiredSemaphore;
	//	VkSemaphore render_complete_semaphore = imgui_local->data.FrameSemaphores[imgui_local->data.SemaphoreIndex].RenderCompleteSemaphore;

	//	ImGui_ImplVulkanH_Frame* fd = &imgui_local->data.Frames[imgui_local->data.FrameIndex];

	{
		VkCommandBufferBeginInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		info.flags = 0;

		err = vkBeginCommandBuffer(fd->CommandBuffer, &info);
		rhr::render::command::active_command_buffer = &(fd->CommandBuffer);
		check_vk_result(err);
	}

	rhr::stack::plane::primary_plane->frame_update(1.0);
	rhr::stack::plane::toolbar_plane->frame_update(1.0);

	process_dirty();
}

void rhr::render::renderer::render_pass_master()
{
	VkResult err;

	std::array<VkClearValue, 2> clear_values{};
	clear_values[0].color = { { 0.0f, 0.0f, 0.0f, 1.0f } };
	clear_values[1].depthStencil = { 1.0f, 0 };

	ImGui_ImplVulkanH_Frame* fd = &imgui_local->data.Frames[imgui_local->data.FrameIndex];

	VkSemaphore image_acquired_semaphore  = imgui_local->data.FrameSemaphores[imgui_local->data.SemaphoreIndex].ImageAcquiredSemaphore;
	VkSemaphore render_complete_semaphore = imgui_local->data.FrameSemaphores[imgui_local->data.SemaphoreIndex].RenderCompleteSemaphore;

	// final render pass

	{
		VkRenderPassBeginInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		info.renderPass = imgui_local->data.RenderPass;
		info.framebuffer = fd->Framebuffer;
		info.renderArea.extent.width = imgui_local->data.Width;
		info.renderArea.extent.height = imgui_local->data.Height;
		info.clearValueCount = clear_values.size();
		info.pClearValues = clear_values.data();

		vkCmdBeginRenderPass(fd->CommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);

		ImGui_ImplVulkan_RenderDrawData(imgui_draw_data, fd->CommandBuffer);

		vkCmdEndRenderPass(fd->CommandBuffer);
	}

	// submit command buffer and queue to gpu

	{
		VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		VkSubmitInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		info.waitSemaphoreCount = 1;
		info.pWaitSemaphores = &image_acquired_semaphore;
		info.pWaitDstStageMask = &wait_stage;
		info.commandBufferCount = 1;
		info.pCommandBuffers = &fd->CommandBuffer;
		info.signalSemaphoreCount = 1;
		info.pSignalSemaphores = &render_complete_semaphore;

		err = vkEndCommandBuffer(fd->CommandBuffer);
		check_vk_result(err);

		err = vkQueueSubmit(graphics_queue, 1, &info, fd->Fence);
		check_vk_result(err);
	}
}

void rhr::render::renderer::frame_present()
{
	if (reload_swap_chain_flag)
		return;
	
	VkSemaphore render_complete_semaphore = imgui_local->data.FrameSemaphores[imgui_local->data.SemaphoreIndex].RenderCompleteSemaphore;

	VkPresentInfoKHR info = {};
	info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	info.waitSemaphoreCount = 1;
	info.pWaitSemaphores = &render_complete_semaphore;
	info.swapchainCount = 1;
	info.pSwapchains = &imgui_local->data.Swapchain;
	info.pImageIndices = &imgui_local->data.FrameIndex;

	VkResult err = vkQueuePresentKHR(graphics_queue, &info);

	if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
	{
		reload_swap_chain_flag = true;
		return;
	}

	vkQueueWaitIdle(graphics_queue);
	check_vk_result(err);

	err = vkResetCommandPool(rhr::render::device::device_master, rhr::render::command::command_pool, 0);
	check_vk_result(err);

	imgui_local->data.SemaphoreIndex = (imgui_local->data.SemaphoreIndex + 1) % imgui_local->data.ImageCount;
}

void rhr::render::renderer::clean_up_swap_chain()
{
	vkDestroyImageView(rhr::render::device::device_master, depth_image_view, nullptr);
	vkDestroyImage(rhr::render::device::device_master, depthImage, nullptr);
	vkFreeMemory(rhr::render::device::device_master, depthImageMemory, nullptr);

	for (usize i = 0; i < rhr::render::swap_chain::swap_chain_frame_buffers.size(); i++)
		vkDestroyFramebuffer(rhr::render::device::device_master, rhr::render::swap_chain::swap_chain_frame_buffers[i], nullptr);

	vkFreeCommandBuffers(rhr::render::device::device_master, rhr::render::command::command_pool, static_cast<u32>(rhr::render::command::command_buffers.size()), rhr::render::command::command_buffers.data());

	//vkDestroyPipeline(Device, BlocksPipeline, nullptr);
	vkDestroyPipeline(rhr::render::device::device_master, rhr::render::pipeline::ui_pipeline, nullptr);
	vkDestroyPipeline(rhr::render::device::device_master, rhr::render::pipeline::ui_texture_pipeline, nullptr);

//	vkDestroyPipelineLayout(device, rhr::render::pipeline::blocks_pipeline_layout, nullptr);
	vkDestroyPipelineLayout(rhr::render::device::device_master, rhr::render::pipeline::ui_pipeline_layout, nullptr);
	vkDestroyPipelineLayout(rhr::render::device::device_master, rhr::render::pipeline::ui_texture_pipeline_layout, nullptr);

	vkDestroyRenderPass(rhr::render::device::device_master, rhr::render::render_pass::render_pass_master, nullptr);

	for (usize i = 0; i < rhr::render::swap_chain::swap_chain_image_views.size(); i++)
		vkDestroyImageView(rhr::render::device::device_master, rhr::render::swap_chain::swap_chain_image_views[i], nullptr);

	vkDestroySwapchainKHR(rhr::render::device::device_master, rhr::render::swap_chain::swapchain_khr, nullptr);

	//for (usize i = 0; i < m_SwapChainImages.size(); i++)
	//{
	//	vkDestroyBuffer(m_Device, m_UniformBuffers[i], nullptr);
	//	vkFreeMemory(m_Device, m_UniformBuffersMemory[i], nullptr);
	//}

	vkDestroyDescriptorPool(rhr::render::device::device_master, rhr::render::command::descriptor_pool, nullptr);
}

void rhr::render::renderer::clean_up()
{
	clean_up_swap_chain();

	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	vkDestroySampler(rhr::render::device::device_master, texture_sampler, nullptr);
	//vkDestroyImageView(m_Device, m_TextureImageView, nullptr);

	//vkDestroyImage(m_Device, m_TextureImage, nullptr);
	//vkFreeMemory(m_Device, m_TextureImageMemory, nullptr);

	vkDestroyDescriptorSetLayout(rhr::render::device::device_master, rhr::render::command::descriptor_set_layout, nullptr);

	//vkDestroyBuffer(m_Device, m_IndexBuffer, nullptr);
	//vkFreeMemory(m_Device, m_IndexBufferMemory, nullptr);

	//vkDestroyBuffer(m_Device, m_VertexBuffer, nullptr);
	//vkFreeMemory(m_Device, vertexBufferMemory, nullptr);

	for (usize i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		vkDestroySemaphore(rhr::render::device::device_master, rhr::render::swap_chain::render_finished_semaphores[i], nullptr);
		vkDestroySemaphore(rhr::render::device::device_master, rhr::render::swap_chain::image_available_semaphores[i], nullptr);
		vkDestroyFence(rhr::render::device::device_master, rhr::render::swap_chain::in_flight_fences[i], nullptr);
	}

	vkDestroyCommandPool(rhr::render::device::device_master, rhr::render::command::command_pool, nullptr);

	vkDestroyDevice(rhr::render::device::device_master, nullptr);

	if (enable_validation_layers)
		rhr::render::tools::destroy_debug_utils_message_ext(&rhr::render::device::instance, &rhr::render::device::debug_messenger, nullptr);

	vkDestroySurfaceKHR(rhr::render::device::instance, surface, nullptr);
	vkDestroyInstance(rhr::render::device::instance, nullptr);

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

	vkDeviceWaitIdle(rhr::render::device::device_master);

	clean_up_swap_chain();

	rhr::render::swap_chain::init_swap_chain();
	rhr::render::swap_chain::init_image_views();
	rhr::render::render_pass::init_render_pass();
	rhr::render::pipeline::init_pipelines();
	init_depth_resources();
	rhr::render::swap_chain::init_frame_buffers();
	rhr::render::command::init_descriptor_pool();
	rhr::render::command::init_command_buffers();
	rhr::render::swap_chain::init_sync_objects();

	rhr::render::tools::swap_chain_support_details swap_chain_support = rhr::render::tools::query_swap_chain_support(&rhr::render::device::physical_device, &surface);
	rhr::render::tools::queue_family_indices indices = rhr::render::tools::find_queue_families(&rhr::render::device::physical_device, &surface);

	std::set<u32> unique_queue_families = { indices.graphics_family.value(), indices.present_family.value() };

	imgui_local->data.Width = width;
	imgui_local->data.Height = height;

	ImGui_ImplVulkan_SetMinImageCount(swap_chain_support.capabilities.minImageCount);
	ImGui_ImplVulkanH_CreateOrResizeWindow(
		rhr::render::device::instance,
		rhr::render::device::physical_device,
		rhr::render::device::device_master,
		&imgui_local->data,
		static_cast<u32>(indices.graphics_family.value()),
		nullptr,
		width,
		height,
		swap_chain_support.capabilities.minImageCount
		);
}

void rhr::render::renderer::add_layer(std::weak_ptr<rhr::render::layer> layer)
{
//	m_layers.push_back(layer);
}

void rhr::render::renderer::reload_layer_swap_chains()
{
//	bool erased = false;
//
//	for (usize i = 0; i < m_layers.size(); i++)
//	{
//		if (erased)
//		{
//			erased = false;
//			i--;
//		}
//
//		if (auto layer = m_layers[i].lock())
//			layer->reload_swap_chain();
//		else
//		{
//			m_layers.erase(m_layers.begin() + i);
//			erased = true;
//		}
//	}
//
//	rhr::handler::category::reload_swap_chain();
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

	if (vkCreateDescriptorSetLayout(rhr::render::device::device_master, &layout_info, nullptr, &rhr::render::command::descriptor_set_layout) != VK_SUCCESS)
		cap::logger::fatal("failed to set descriptor layout");
}

void rhr::render::renderer::init_depth_resources()
{
	VkFormat depthFormat = rhr::render::tools::find_depth_format();
	rhr::render::tools::create_image(rhr::render::swap_chain::swap_chain_extent.width, rhr::render::swap_chain::swap_chain_extent.height, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory);
	depth_image_view = rhr::render::tools::create_image_view(depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
	rhr::render::tools::transition_image_layout(depthImage, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
}

void rhr::render::renderer::init_texture_sampler()
{
	VkPhysicalDeviceProperties properties{};
	vkGetPhysicalDeviceProperties(rhr::render::device::physical_device, &properties);

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

	if (vkCreateSampler(rhr::render::device::device_master, &sampler_info, nullptr, &texture_sampler) != VK_SUCCESS)
		cap::logger::fatal("failed to create texture sampler");
}

VkQueue rhr::render::renderer::graphics_queue;
VkQueue rhr::render::renderer::present_queue;
VkSurfaceKHR rhr::render::renderer::surface;
GLFWwindow* rhr::render::renderer::window;
VkSampler rhr::render::renderer::texture_sampler;
VkImageView rhr::render::renderer::depth_image_view;
bool rhr::render::renderer::frame_buffer_resized;
glm::mat4 rhr::render::renderer::view_matrix;
glm::mat4 rhr::render::renderer::projection_matrix;
glm::mat4 rhr::render::renderer::ui_projection_matrix;
bool rhr::render::renderer::vsync_enabled;
glm::vec<2, i32> rhr::render::renderer::window_position;
glm::vec<2, i32> rhr::render::renderer::window_size;

rhr::render::renderer::imgui_data* rhr::render::renderer::imgui_local;
vk::present_mode_khr rhr::render::renderer::present_mode;
vk::surface_format_khr rhr::render::renderer::surface_format;
bool rhr::render::renderer::reload_swap_chain_flag = false;
ImDrawData* rhr::render::renderer::imgui_draw_data;

u32 rhr::render::renderer::depth_background = 100;
u32 rhr::render::renderer::depth_plane = 95;
u32 rhr::render::renderer::depth_frame_background = 93;
u32 rhr::render::renderer::depth_collection = 90;
u32 rhr::render::renderer::depth_stack = 85;
u32 rhr::render::renderer::depth_block = 80;
u32 rhr::render::renderer::depth_argument = 75;
u32 rhr::render::renderer::depth_argument_text = 70;
u32 rhr::render::renderer::depth_ui_background = 65;
u32 rhr::render::renderer::depth_ui_text = 60;

std::vector<std::weak_ptr<rhr::render::interfaces::i_renderable>> rhr::render::renderer::m_dirty_renderable;
std::vector<std::weak_ptr<rhr::render::interfaces::i_ui>> rhr::render::renderer::m_dirty_ui;
std::shared_mutex rhr::render::renderer::m_dirty_mutex;