#include "renderer.hpp"

#include "rhr/handlers/category.hpp"
#include "rhr/handlers/input.hpp"
#include "rhr/rendering/objects/object.hpp"
#include "rhr/rendering/objects/rectangle.hpp"
#include "rhr/rendering/panel.hpp"
#include "rhr/rendering/tools.hpp"
#include "rhr/rendering/vertex.hpp"
#include "rhr/stacking/plane.hpp"

#include <cappuccino/utils.hpp>
#include <iostream>

//#define STB_IMAGE_IMPLEMENTATION
//#include <stb_image.h>

static bool run_first_render_pass = false;

static std::vector<std::shared_ptr<rhr::render::object::rectangle>> grid_objects;

static void check_vk_result(VkResult err)
{
	if (err != VK_SUCCESS)
		cap::logger::error(cap::logger::level::SYSTEM, "vulkan error code \"" + std::to_string(err) + "\"");
}

vk::image depthImage;
vk::device_memory depthImageMemory;

void rhr::render::renderer::initialize_window()
{
	glfwInit();
	m_window_primary = std::make_unique<rhr::render::component::window>(std::string("Unnamed Blocks ") + VER_CLIENT, glm::vec<2, i32>(1280, 720));
}

void rhr::render::renderer::initialize()
{
	view_matrix		  = glm::mat4(1.0);
	projection_matrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);

	m_window_primary->initialize_components();

	VmaAllocatorCreateInfo allocator_info = {};
	allocator_info.vulkanApiVersion = VK_API_VERSION_1_0;
	allocator_info.physicalDevice = *m_window_primary->get_physical_device();
	allocator_info.device = *m_window_primary->get_device();
	allocator_info.instance = *m_window_primary->get_instance();

	vmaCreateAllocator(&allocator_info, &vma_allocator);

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;	// Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
	// io.ConfigViewportsNoAutoMerge = true;
	// io.ConfigViewportsNoTaskBarIcon = true;

	// Setup Dear ImGui style
	//	ImGui::StyleColorsDark();
	// ImGui::StyleColorsClassic();

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	// ImGui::StyleColorsClassic();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding			  = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	{
		auto& colors			  = style.Colors;
		colors[ImGuiCol_WindowBg] = ImVec4 {0.1f, 0.105f, 0.11f, 1.0f};

		// Headers
		colors[ImGuiCol_Header]		   = ImVec4 {0.2f, 0.205f, 0.21f, 1.0f};
		colors[ImGuiCol_HeaderHovered] = ImVec4 {0.3f, 0.305f, 0.31f, 1.0f};
		colors[ImGuiCol_HeaderActive]  = ImVec4 {0.15f, 0.1505f, 0.151f, 1.0f};

		// Buttons
		colors[ImGuiCol_Button]		   = ImVec4 {0.2f, 0.205f, 0.21f, 1.0f};
		colors[ImGuiCol_ButtonHovered] = ImVec4 {0.3f, 0.305f, 0.31f, 1.0f};
		colors[ImGuiCol_ButtonActive]  = ImVec4 {0.15f, 0.1505f, 0.151f, 1.0f};

		// Frame BG
		colors[ImGuiCol_FrameBg]		= ImVec4 {0.2f, 0.205f, 0.21f, 1.0f};
		colors[ImGuiCol_FrameBgHovered] = ImVec4 {0.3f, 0.305f, 0.31f, 1.0f};
		colors[ImGuiCol_FrameBgActive]	= ImVec4 {0.15f, 0.1505f, 0.151f, 1.0f};

		// Tabs
		colors[ImGuiCol_Tab]				= ImVec4 {0.15f, 0.1505f, 0.151f, 1.0f};
		colors[ImGuiCol_TabHovered]			= ImVec4 {0.38f, 0.3805f, 0.381f, 1.0f};
		colors[ImGuiCol_TabActive]			= ImVec4 {0.28f, 0.2805f, 0.281f, 1.0f};
		colors[ImGuiCol_TabUnfocused]		= ImVec4 {0.15f, 0.1505f, 0.151f, 1.0f};
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4 {0.2f, 0.205f, 0.21f, 1.0f};

		// Title
		colors[ImGuiCol_TitleBg]		  = ImVec4 {0.15f, 0.1505f, 0.151f, 1.0f};
		colors[ImGuiCol_TitleBgActive]	  = ImVec4 {0.15f, 0.1505f, 0.151f, 1.0f};
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4 {0.15f, 0.1505f, 0.151f, 1.0f};
	}

	{
		ImFontAtlas* fontAtlas = io.Fonts;
		ImFontConfig fontConfig;
		fontConfig.GlyphRanges = fontAtlas->GetGlyphRangesCyrillic();
		fontConfig.MergeMode   = false;
		fontConfig.PixelSnapH  = false;

		io.FontDefault = io.Fonts->AddFontFromFileTTF("res/Roboto-Regular.ttf", 16, &fontConfig);
	}

	initialize_imgui(true);

#if 0
	i32 x = 0;
	i32 y = 0;

	i32 width  = m_window_primary->get_window_size().x;
	i32 height = m_window_primary->get_window_size().y;

	while (x < width)
	{
		auto& rect = grid_objects.emplace_back(std::make_shared<rhr::render::object::rectangle>());
		rect->set_depth(depth_ui_background);

		rect->set_size_local({ 1, height }, true);
		rect->set_position_local_physical({ x, 0 }, true);

		x += 100;
	}

	while (y < height)
	{
		auto& rect = grid_objects.emplace_back(std::make_shared<rhr::render::object::rectangle>());
		rect->set_depth(depth_ui_background);

		rect->set_size_local({ width, 1 }, true);
		rect->set_position_local_physical({ 0, y }, true);

		y += 100;
	}
#endif
}

std::unique_ptr<rhr::render::component::window>& rhr::render::renderer::get_window_primary() { return m_window_primary; }

void rhr::render::renderer::reload_swapchain()
{
	int width = 0, height = 0;
	glfwGetFramebufferSize(m_window_primary->get_window(), &width, &height);

	while (width == 0 || height == 0)
	{
		glfwGetFramebufferSize(m_window_primary->get_window(), &width, &height);
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(*m_window_primary->get_device());

	cap::logger::info(cap::logger::level::SYSTEM, "recreating swapchain...");
	cap::logger::info(cap::logger::level::SYSTEM, "framebuffer new size: " + std::to_string(width) + ", " + std::to_string(height));

	m_window_primary->recreate_swapchain();
	initialize_imgui(false);
	rhr::render::panel::initialize_panels();

	cap::logger::info(cap::logger::level::SYSTEM, "...recreated swapchain");
	cap::logger::info(cap::logger::level::SYSTEM, "reloading swapchain dependent objects...");

	rhr::render::renderer::imgui_local->data.FrameIndex = 0;
	rhr::render::renderer::get_window_primary()->flag_clear_swapchain_recreation();

	VkResult err = vkDeviceWaitIdle(*rhr::render::renderer::get_window_primary()->get_device());

	if (err != VK_SUCCESS)
		cap::logger::fatal(cap::logger::level::SYSTEM, "failed to idle device during swapchain reload; vulkan error code: " + std::to_string(static_cast<i32>(err)));

	rhr::stack::plane::primary_plane->reload_swap_chain();
	rhr::stack::plane::toolbar_plane->reload_swap_chain();

	for (auto& grid_object : grid_objects)
		grid_object->reload_swap_chain();

	cap::logger::info(cap::logger::level::SYSTEM, "...reloaded swapchain dependent objects");
}

void rhr::render::renderer::initialize_imgui(bool first_time)
{
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	ImGuiStyle& style = ImGui::GetStyle();

	rhr::render::tools::swap_chain_support_details swap_chain_support =
		rhr::render::tools::query_swap_chain_support(m_window_primary->get_physical_device(), m_window_primary->get_surface());
	u8 image_count = m_window_primary->get_framebuffer_count();

	rhr::render::tools::queue_family_indices indices = rhr::render::tools::find_queue_families(m_window_primary->get_physical_device(), m_window_primary->get_surface());
	std::set<u32> unique_queue_families				 = {indices.graphics_family.value(), indices.present_family.value()};

	if (first_time)
	{
		imgui_local = new imgui_data(io, style);

		imgui_local->frames		= new ImGui_ImplVulkanH_Frame[static_cast<usize>(image_count)];
		imgui_local->semaphores = new ImGui_ImplVulkanH_FrameSemaphores[static_cast<usize>(image_count)];
	}

	for (u8 i = 0; i < image_count; i++)
	{
		imgui_local->frames[i].CommandPool		  = *m_window_primary->get_command_pool();
		imgui_local->frames[i].CommandBuffer	  = *m_window_primary->get_master_command_buffer(i);
		imgui_local->frames[i].CommandBufferPanel = *m_window_primary->get_panel_command_buffer(i);
		imgui_local->frames[i].Fence			  = m_window_primary->get_frame(i)->fence_in_flight;
		imgui_local->frames[i].Backbuffer		  = m_window_primary->get_frame(i)->frame_image;
		imgui_local->frames[i].BackbufferView	  = m_window_primary->get_frame(i)->frame_view;
		imgui_local->frames[i].Framebuffer		  = m_window_primary->get_frame(i)->frame_buffer;
	}

	for (u32 i = 0; i < image_count; i++)
	{
		imgui_local->semaphores[i].ImageAcquiredSemaphore  = m_window_primary->get_frame(i)->semaphore_image;
		imgui_local->semaphores[i].RenderCompleteSemaphore = m_window_primary->get_frame(i)->semaphore_finished;
	}

	imgui_local->data.Width			= m_window_primary->get_window_size().x;
	imgui_local->data.Height		= m_window_primary->get_window_size().y;
	imgui_local->data.Swapchain		= *m_window_primary->get_swapchain();
	imgui_local->data.Surface		= *m_window_primary->get_surface();
	imgui_local->data.SurfaceFormat = *m_window_primary->get_surface_format();
	imgui_local->data.PresentMode	= *m_window_primary->get_present_mode();
	imgui_local->data.RenderPass	= *m_window_primary->get_render_pass(0);
	imgui_local->data.Pipeline		= *m_window_primary->get_color_pipeline("master");
	//	imgui_local->data.ClearEnable = false;
	//	imgui_local->data.ClearValue = VkClearValue(0.0f, 0.0f, 0.0f, 1.0f);
	imgui_local->data.FrameIndex	  = 0;
	imgui_local->data.ImageCount	  = m_window_primary->get_framebuffer_count();
	imgui_local->data.SemaphoreIndex  = 0;
	imgui_local->data.Frames		  = imgui_local->frames;
	imgui_local->data.FrameSemaphores = imgui_local->semaphores;

	ImGui_ImplVulkan_InitInfo imgui_info {
		.Instance		 = *m_window_primary->get_instance(),
		.PhysicalDevice	 = *m_window_primary->get_physical_device(),
		.Device			 = *m_window_primary->get_device(),
		.QueueFamily	 = static_cast<uint32_t>(indices.graphics_family.value()),
		.Queue			 = *m_window_primary->get_graphics_queue(),
		.PipelineCache	 = nullptr,
		.DescriptorPool	 = *m_window_primary->get_descriptor_pool(),
		.MinImageCount	 = swap_chain_support.capabilities.minImageCount,
		.ImageCount		 = static_cast<uint32_t>(m_window_primary->get_framebuffer_count()),
		.MSAASamples	 = VK_SAMPLE_COUNT_1_BIT,
		.Allocator		 = nullptr,
		.CheckVkResultFn = [](VkResult result)
		{
			if (result != VK_SUCCESS)
				cap::logger::fatal(cap::logger::level::SYSTEM, "imgui fatal error: " + std::to_string(static_cast<i32>(result)));
		}};

	if (first_time)
		ImGui_ImplGlfw_InitForVulkan(m_window_primary->get_window(), true);

	ImGui_ImplVulkan_Init(&imgui_info, *m_window_primary->get_render_pass(0));

	{
		VkResult err;

		err = vkResetCommandPool(*m_window_primary->get_device(), *m_window_primary->get_command_pool(), 0);
		check_vk_result(err);

		// Use any command queue
		VkCommandPool command_pool	   = imgui_local->data.Frames[imgui_local->data.FrameIndex].CommandPool;
		VkCommandBuffer command_buffer = imgui_local->data.Frames[imgui_local->data.FrameIndex].CommandBuffer;

		err = vkResetCommandPool(*m_window_primary->get_device(), command_pool, 0);
		check_vk_result(err);

		VkCommandBufferBeginInfo begin_info = {};
		begin_info.sType					= VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		begin_info.flags					= 0;

		err = vkBeginCommandBuffer(command_buffer, &begin_info);
		check_vk_result(err);

		ImGui_ImplVulkan_CreateFontsTexture(command_buffer);

		VkSubmitInfo end_info		= {};
		end_info.sType				= VK_STRUCTURE_TYPE_SUBMIT_INFO;
		end_info.commandBufferCount = 1;
		end_info.pCommandBuffers	= &command_buffer;

		err = vkEndCommandBuffer(command_buffer);
		check_vk_result(err);

		err = vkQueueSubmit(*m_window_primary->get_graphics_queue(), 1, &end_info, VK_NULL_HANDLE);
		check_vk_result(err);

		err = vkDeviceWaitIdle(*m_window_primary->get_device());
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

	//for (const auto& renderable : m_dirty_renderable)
	//{
	//	if (auto object = renderable.lock())
	//		object->update_buffers();
	//}

	//for (const auto& ui : m_dirty_ui)
	//{
	//	if (auto object = ui.lock())
	//		object->update_buffers();
	//}

	m_dirty_renderable.clear();
	m_dirty_ui.clear();
}

void rhr::render::renderer::render_pass_setup()
{
	rhr::render::renderer::ui_projection_matrix =
		glm::ortho(0.0f, static_cast<f32>(m_window_primary->get_window_size().x), 0.0f, static_cast<f32>(m_window_primary->get_window_size().y), -10000.0f, 10000.0f);

	VkSemaphore image_acquired_semaphore  = imgui_local->data.FrameSemaphores[imgui_local->data.SemaphoreIndex].ImageAcquiredSemaphore;
	VkSemaphore render_complete_semaphore = imgui_local->data.FrameSemaphores[imgui_local->data.SemaphoreIndex].RenderCompleteSemaphore;
	// cap::logger::info("vkAcquireNextImageKHR");
	VkResult err =
		vkAcquireNextImageKHR(*m_window_primary->get_device(), imgui_local->data.Swapchain, UINT64_MAX, image_acquired_semaphore, VK_NULL_HANDLE, &imgui_local->data.FrameIndex);

	if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
	{
		reload_swapchain();
		return;
	}

	check_vk_result(err);

	ImGui_ImplVulkanH_Frame* fd = &imgui_local->data.Frames[imgui_local->data.FrameIndex];

	// frame sync

	err = vkWaitForFences(*m_window_primary->get_device(), 1, &fd->Fence, VK_TRUE, UINT64_MAX); // wait indefinitely instead of periodically checking
	check_vk_result(err);

	err = vkResetFences(*m_window_primary->get_device(), 1, &fd->Fence);
	check_vk_result(err);

	// initialize command buffer for render

	//	VkResult err;

	std::array<VkClearValue, 1> clear_values {};
	clear_values[0].color = {
		{0.0f, 0.0f, 0.0f, 0.0f}
	};
	//	clear_values[1].depthStencil = { 1.0f, 0 };

	//	VkSemaphore image_acquired_semaphore  = imgui_local->data.FrameSemaphores[imgui_local->data.SemaphoreIndex].ImageAcquiredSemaphore;
	//	VkSemaphore render_complete_semaphore = imgui_local->data.FrameSemaphores[imgui_local->data.SemaphoreIndex].RenderCompleteSemaphore;

	//	ImGui_ImplVulkanH_Frame* fd = &imgui_local->data.Frames[imgui_local->data.FrameIndex];

	{
		VkCommandBufferBeginInfo info = {};
		info.sType					  = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		info.flags					  = 0;

		err = vkBeginCommandBuffer(fd->CommandBuffer, &info);
		m_window_primary->set_active_command_buffer(imgui_local->data.FrameIndex);
		check_vk_result(err);
	}

	rhr::stack::plane::primary_plane->frame_update(1.0);
	rhr::stack::plane::toolbar_plane->frame_update(1.0);

	for (auto& grid_object : grid_objects)
		grid_object->frame_update(1.0);

	rhr::stack::plane::primary_plane->update_buffers();
	rhr::stack::plane::toolbar_plane->update_buffers();

	for (auto& grid_object : grid_objects)
		grid_object->update_buffers();

	//process_dirty();
}

void rhr::render::renderer::render_pass_master()
{
	VkResult err;

	std::array<VkClearValue, 1> clear_values {};
	clear_values[0].color = {
		{0.0f, 0.0f, 0.0f, 0.0f}
	};
	//	clear_values[1].depthStencil = { 1.0f, 0 };

	ImGui_ImplVulkanH_Frame* fd = &imgui_local->data.Frames[imgui_local->data.FrameIndex];

	VkSemaphore image_acquired_semaphore  = imgui_local->data.FrameSemaphores[imgui_local->data.SemaphoreIndex].ImageAcquiredSemaphore;
	VkSemaphore render_complete_semaphore = imgui_local->data.FrameSemaphores[imgui_local->data.SemaphoreIndex].RenderCompleteSemaphore;
#if 0
	{
		// close out panel frame buffer

		err = vkEndCommandBuffer(fd->CommandBufferPanel);
		check_vk_result(err);

		// setup master command buffer

		VkCommandBufferBeginInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		info.flags = 0;

		err = vkBeginCommandBuffer(fd->CommandBuffer, &info);
		m_window_primary->get_active_command_buffer() = &(fd->CommandBuffer);
		check_vk_result(err);
	}
#endif
	// seperate panel render passes from final render pass

	//	vkCmdPipelineBarrier(
	//		*m_window_primary->get_active_command_buffer(),
	//		VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
	//		VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
	//		0,
	//		0,
	//		nullptr,
	//		0,
	//		nullptr,
	//		0,
	//		nullptr
	//	);

	m_window_primary->apply_active_pipeline("master");

	// final render pass

	{
		VkRenderPassBeginInfo info	  = {};
		info.sType					  = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		info.renderPass				  = imgui_local->data.RenderPass;
		info.framebuffer			  = fd->Framebuffer;
		info.renderArea.extent.width  = imgui_local->data.Width;
		info.renderArea.extent.height = imgui_local->data.Height;
		info.clearValueCount		  = clear_values.size();
		info.pClearValues			  = clear_values.data();

		//		vkCmdBindPipeline(*m_window_primary->get_active_command_buffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, rhr::render::pipeline::ui_texture_pipeline);

		vkCmdBeginRenderPass(*m_window_primary->get_active_command_buffer(), &info, VK_SUBPASS_CONTENTS_INLINE);
		ImGui_ImplVulkan_RenderDrawData(imgui_draw_data, *m_window_primary->get_active_command_buffer());

		//		vkCmdBindPipeline(*m_window_primary->get_active_command_buffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, rhr::render::pipeline::ui_texture_pipeline);

		rhr::render::panel::run_master_render_pass();

		for (auto& grid_object : grid_objects)
			grid_object->render();

		//		rhr::stack::plane::primary_plane->render();
		vkCmdEndRenderPass(*m_window_primary->get_active_command_buffer());
	}

	// submit command buffer and queue to gpu

	{
		std::array<VkCommandBuffer, 1> used_command_buffers = {//			fd->CommandBufferPanel,
															   fd->CommandBuffer};

		VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		VkSubmitInfo info				= {};
		info.sType						= VK_STRUCTURE_TYPE_SUBMIT_INFO;
		info.waitSemaphoreCount			= 1;
		info.pWaitSemaphores			= &image_acquired_semaphore;
		info.pWaitDstStageMask			= &wait_stage;
		info.commandBufferCount			= used_command_buffers.size();
		info.pCommandBuffers			= used_command_buffers.data();
		info.signalSemaphoreCount		= 1;
		info.pSignalSemaphores			= &render_complete_semaphore;

		err = vkEndCommandBuffer(fd->CommandBuffer);
		check_vk_result(err);

		err = vkQueueSubmit(*m_window_primary->get_graphics_queue(), 1, &info, fd->Fence);
		check_vk_result(err);
	}
}

void rhr::render::renderer::frame_present()
{
	VkSemaphore render_complete_semaphore = imgui_local->data.FrameSemaphores[imgui_local->data.SemaphoreIndex].RenderCompleteSemaphore;

	VkPresentInfoKHR info	= {};
	info.sType				= VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	info.waitSemaphoreCount = 1;
	info.pWaitSemaphores	= &render_complete_semaphore;
	info.swapchainCount		= 1;
	info.pSwapchains		= &imgui_local->data.Swapchain;
	info.pImageIndices		= &imgui_local->data.FrameIndex;

	VkResult err = vkQueuePresentKHR(*m_window_primary->get_graphics_queue(), &info);

	if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
		reload_swapchain();

	err = vkQueueWaitIdle(*m_window_primary->get_graphics_queue());
	check_vk_result(err);

	err = vkResetCommandPool(*m_window_primary->get_device(), *m_window_primary->get_command_pool(), 0);
	check_vk_result(err);

	imgui_local->data.SemaphoreIndex = (imgui_local->data.SemaphoreIndex + 1) % imgui_local->data.ImageCount;
}

glm::mat4 rhr::render::renderer::view_matrix;
glm::mat4 rhr::render::renderer::projection_matrix;
glm::mat4 rhr::render::renderer::ui_projection_matrix;

rhr::render::renderer::imgui_data* rhr::render::renderer::imgui_local;
ImDrawData* rhr::render::renderer::imgui_draw_data;
VmaAllocator rhr::render::renderer::vma_allocator;

u32 rhr::render::renderer::depth_background		  = 10;
u32 rhr::render::renderer::depth_plane			  = 15;
u32 rhr::render::renderer::depth_frame_background = 20;
u32 rhr::render::renderer::depth_collection		  = 25;
u32 rhr::render::renderer::depth_stack			  = 30;
u32 rhr::render::renderer::depth_block			  = 35;
u32 rhr::render::renderer::depth_argument		  = 40;
u32 rhr::render::renderer::depth_argument_text	  = 45;
u32 rhr::render::renderer::depth_cursor			  = 46;
u32 rhr::render::renderer::depth_ui_background	  = 50;
u32 rhr::render::renderer::depth_ui_text		  = 55;

////////////////////////////////////////////////////////////////////////////

std::unique_ptr<rhr::render::component::window> rhr::render::renderer::m_window_primary;

////////////////////////////////////////////////////////////////////////////

std::vector<std::weak_ptr<rhr::render::interfaces::i_renderable>> rhr::render::renderer::m_dirty_renderable;
std::vector<std::weak_ptr<rhr::render::interfaces::i_ui>> rhr::render::renderer::m_dirty_ui;
std::shared_mutex rhr::render::renderer::m_dirty_mutex;