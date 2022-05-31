#include "window.hpp"

#include "vertices/vertex_color.hpp"
#include "vertices/vertex_texture.hpp"
#include "ubos/ubo_cam.hpp"
#include "ubos/ubo_obj.hpp"

void mac::window::global_initialization()
{
	glfwInit();
}

void mac::window::global_shutdown()
{
	glfwTerminate();
}

mac::window::state* mac::window::create(const std::string& title, glm::vec<2, i32> size)
{
	//
	// PRIMARY COMPONENT SETUP
	//

	// WINDOW

	auto* window_state = new mac::window::state {
		.input_state = mac::input::create(),
		.command_buffer_idx = 0,
		.resize_waiting = false,
		.resize_done = false,
	};

	glfw::set_window_hint(GLFW_CLIENT_API, GLFW_NO_API);
	glfw::set_window_hint(GLFW_RESIZABLE, GLFW_TRUE);
	window_state->window = glfw::create_window(size.x, size.y, title.c_str(), nullptr, nullptr);

	if (window_state->window == nullptr)
	{
		latte::logger::fatal(latte::logger::level::SYSTEM, "failed to create window");
		return nullptr;
	}

	// DEVICE

	window_state->device_state = mac::device::create(true, title);

	if (glfw::create_window_surface(window_state->device_state->instance, window_state->window, nullptr, &window_state->surface) != VK_SUCCESS)
	{
		latte::logger::fatal(latte::logger::level::SYSTEM, "failed to create window surface");
		return nullptr;
	}

	mac::device::setup_rendering(window_state->device_state, &window_state->surface);

	// INPUT

	glfw::set_framebuffer_size_callback(window_state->window, mac::input::callback::framebuffer_resize);
	//glfw::set_key_callback(window_state->window, mac::input::callback::key);
	//glfw::set_mouse_button_callback(window_state->window, mac::input::callback::mouse_button);
	//glfw::set_scroll_callback(window_state->window, mac::input::callback::scroll);
	//glfw::set_cursor_position_callback(window_state->window, mac::input::callback::cursor_position);
	//glfw::set_window_position_callback(window_state->window, mac::input::callback::window_position);
	//glfw::set_window_focus_callback(window_state->window, mac::input::callback::window_focus_callback);

	glfw::set_window_user_pointer(window_state->window, reinterpret_cast<void*>(window_state->input_state));
	glfw::get_window_position(window_state->window, &window_state->input_state->screen_position.x, &window_state->input_state->screen_position.y);

	vma::allocator_create_info allocator_create_info = {};

	allocator_create_info.vulkanApiVersion		  = VK_API_VERSION_1_0;
	allocator_create_info.physicalDevice		  = window_state->device_state->physical_device;
	allocator_create_info.device				  = window_state->device_state->logical_device;
	allocator_create_info.instance				  = window_state->device_state->instance;

	vma::create_allocator(&allocator_create_info, &window_state->vma_allocator);

	//
	// SUBCOMPONENT SETUP
	//

	//
	// SWAPCHAIN
	// RENDERPASSES
	//

	mac::window::create_swapchain_and_renderpasses(window_state);

	//
	// DESCRIPTOR SETS
	//

	window_state->descriptor_sets.emplace_back(std::make_pair(
		"cam",
		mac::descriptor_set::create(
			window_state->device_state->logical_device,
			{ new mac::ubo_cam() },
			0
		)
	));

	window_state->descriptor_sets.emplace_back(std::make_pair(
		"obj_c",
		mac::descriptor_set::create(
			window_state->device_state->logical_device,
			{ new mac::ubo_obj() },
			0
		)
	));

	window_state->descriptor_sets.emplace_back(std::make_pair(
		"obj_t",
		mac::descriptor_set::create(
			window_state->device_state->logical_device,
			{ new mac::ubo_obj() },
			1
		)
	));

	//
	// PIPELINES
	// PIPELINE BUCKETS
	// FRAMEBUFFERS
	//

	mac::window::create_pipeline_and_framebuffers(window_state);

	//
	// COMMAND POOL
	//

	window_state->command_pool_state = mac::command_pool::create(
		window_state->device_state->logical_device,
		window_state->device_state->physical_device,
		&window_state->surface
	);

	//
	// TEXTURE SAMPLERS
	//

	vk::physical_device_properties physical_device_properties = {};
	vk::get_physical_device_properties(window_state->device_state->physical_device, &physical_device_properties);

	window_state->texture_samplers.emplace_back(std::make_pair(
		"std",
		mac::texture_sampler::create(
			window_state->device_state->logical_device,
			window_state->device_state->physical_device,
			VK_FILTER_NEAREST,
			VK_FILTER_NEAREST,
			VK_SAMPLER_ADDRESS_MODE_REPEAT,
			VK_SAMPLER_ADDRESS_MODE_REPEAT,
			VK_SAMPLER_ADDRESS_MODE_REPEAT,
			physical_device_properties.limits.maxSamplerAnisotropy
		)
	));

	//
	// DESCRIPTOR POOL
	//

	window_state->descriptor_pool = mac::descriptor_set::create_pool(window_state->device_state->logical_device);

	for (auto& descriptor_set : window_state->descriptor_sets)
		mac::descriptor_set::set_descriptor_pool(descriptor_set.second, &window_state->descriptor_pool);

	//
	// COMMAND BUFFERS
	//

	for (u8 i = 0; i < 2; i++)
	{
		window_state->command_buffers.emplace_back(mac::command_buffer::create(
			window_state->device_state->logical_device,
			window_state->command_pool_state,
			window_state->swapchain_state->image_count
		));
	}

	//
	// CAMERA
	//

	glm::vec3 position = glm::vec3(0, 0, 0);
	glm::vec3 look_at = glm::vec3(0, 0, 1);

	window_state->ubo_cam = new mac::ubo_cam();

	window_state->ubo_cam->uniform_data.projection_matrix = glm::ortho(
		0.0f,
		-1920.0f,
		0.0f,
		1080.0f,
		-1000.0f,
		1000.0f
	);

	window_state->ubo_cam->uniform_data.view_matrix = glm::lookAt(
		glm::vec3{ 0, 0, 0 },
		glm::vec3{ 0, 0, 1 },
		glm::vec3{ 0, 1, 0 }
	);

	window_state->cam_instance = mac::descriptor_set::create_instance(
		mac::window::get_descriptor_set(window_state, "cam"),
		window_state->vma_allocator,
		window_state->device_state->logical_device,
		{}
	);

	return window_state;
}

void mac::window::destroy(state* window_state)
{
	delete window_state;
}

void mac::window::run(mac::window::state* window_state)
{
	window_state->thread_rendering = std::thread(thread_rendering, window_state);
	window_state->thread_rendering.detach();

	while (!glfw::window_should_close(window_state->window))
	{
		glfwWaitEvents();

		if (window_state->input_state->framebuffer_resize)
		{
			while (!window_state->resize_waiting && window_state->input_state->framebuffer_resize)
				std::this_thread::sleep_for(std::chrono::milliseconds(10));

			window_state->resize_done = false;
			window_state->resize_waiting = false;
			window_state->input_state->framebuffer_resize = false;

			while (window_state->input_state->size_zero)
				glfwWaitEvents();

			while (!window_state->resize_done)
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}

	if (window_state->thread_rendering.joinable())
		window_state->thread_rendering.join();

	glfwDestroyWindow(window_state->window);
}

void mac::window::thread_rendering(mac::window::state* window_state)
{
	mac::renderpass::state* renderpass_master;
	vk::result result = VK_SUCCESS;

	auto function_setup_references = [&renderpass_master, window_state]()
	{
		renderpass_master = mac::window::get_renderpass(window_state, "master");
	};

	function_setup_references();

	auto function_validate_result = [&result](const std::string& error)
	{
#ifndef NDEBUG
		if (result != VK_SUCCESS)
			latte::logger::fatal(latte::logger::level::SYSTEM, error);
#endif
	};

	while (!glfw::window_should_close(window_state->window))
	{
		// FRAME DELAY

		if (!window_state->input_state->focused)
			std::this_thread::sleep_for(std::chrono::milliseconds(16));

		//std::this_thread::sleep_for(std::chrono::milliseconds(50));

		//
		// FRAME UPDATE
		//

		for (auto& callback : window_state->frame_updates)
			callback(1.0);

		mac::descriptor_set::update_descriptor_buffer(
			window_state->cam_instance->descriptor_buffers[0],
			window_state->vma_allocator,
			window_state->ubo_cam
		);

		//
		// IMGUI
		//
#if 0
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		{
			static bool opt_fullscreen_persistant	  = true;
			bool opt_fullscreen						  = opt_fullscreen_persistant;
			static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

			// We are using the ImGuiWindowFlags_NoDocking flag to make the
			// parent window not dockable into, because it would be confusing to
			// have two docking targets within each others.
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

			if (opt_fullscreen)
			{
				ImGuiViewport* viewport = ImGui::GetMainViewport();

				ImGui::SetNextWindowPos(viewport->Pos);
				ImGui::SetNextWindowSize(viewport->Size);
				ImGui::SetNextWindowViewport(viewport->ID);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

				window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
				window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
			}

			// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace()
			// will render our background and handle the pass-thru hole, so we
			// ask Begin() to not render a background.
			if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
				window_flags |= ImGuiWindowFlags_NoBackground;

			// Important: note that we proceed even if Begin() returns false
			// (aka window is collapsed). This is because we want to keep our
			// DockSpace() active. If a DockSpace() is inactive, all active
			// windows docked into it will lose their parent and become
			// undocked. We cannot preserve the docking relationship between an
			// active window and an inactive docking, otherwise any change of
			// dockspace/settings would lead to windows being stuck in limbo and
			// never being visible.
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			static bool p_open = true;
			ImGui::Begin("###DockSpace", &p_open, window_flags);
			ImGui::PopStyleVar();

			if (ImGui::Button("run"))
				latte::logger::info(latte::logger::level::SYSTEM, "button");

			ImGui::SameLine();

			if (ImGui::Button("save"))
				latte::logger::info(latte::logger::level::SYSTEM, "button");

			ImGui::SameLine();

			if (ImGui::Button("load"))
				latte::logger::info(latte::logger::level::SYSTEM, "button");

			if (opt_fullscreen)
				ImGui::PopStyleVar(2);

			// DockSpace
			ImGuiIO& io = ImGui::GetIO();
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}

			ImGui::End();
		}
#endif

		//
		// WAIT TO RECORD
		//

		auto command_buffer                = window_state->command_buffers[window_state->command_buffer_idx];
		auto& ready_write_fence            = command_buffer->ready_write_fence;
		auto& ready_execution_semaphore    = command_buffer->ready_execution_semaphore;
		auto& ready_presentation_semaphore = command_buffer->ready_presentation_semaphore;

		result = vk::wait_for_fences(
			window_state->device_state->logical_device,
			1,
			&ready_write_fence,
			VK_TRUE,
			UINT64_MAX
		);
		function_validate_result("failed to wait for ready_write_fence");

		// EVENT OPERATIONS

		// TODO: delete zombie buffers

		u32 swapchain_image_idx;

		result = vk::acquire_next_image(
			window_state->device_state->logical_device,
			window_state->swapchain_state->swapchain,
			UINT64_MAX,
			ready_execution_semaphore,
			VK_NULL_HANDLE,
			&swapchain_image_idx
		);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window_state->input_state->framebuffer_resize)
		{
			while (!window_state->input_state->framebuffer_resize)
				std::this_thread::sleep_for(std::chrono::milliseconds(10));

			window_state->resize_waiting = true;

			while (window_state->input_state->framebuffer_resize)
				std::this_thread::sleep_for(std::chrono::milliseconds(10));

			while (window_state->input_state->size_zero)
				std::this_thread::sleep_for(std::chrono::milliseconds(10));

			// UNSIGNAL SEMAPHORE

			const VkPipelineStageFlags psw = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;

			VkSubmitInfo submit_info = {};
			submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			submit_info.waitSemaphoreCount = 1;
			submit_info.pWaitSemaphores = &ready_execution_semaphore;
			submit_info.pWaitDstStageMask = &psw;

			vkQueueSubmit(window_state->device_state->graphics_queue, 1, &submit_info, nullptr);

			// RELOAD SWAPCHAIN

			mac::window::reload_swapchain(window_state);
			function_setup_references();

			window_state->resize_done = true;

			continue;
		}
		function_validate_result("failed to acquire next image");

		auto framebuffer = window_state->framebuffers[swapchain_image_idx];

		result = vk::reset_fences(
			window_state->device_state->logical_device,
			1,
			&ready_write_fence
		);
		function_validate_result("failed to reset ready_write_fence");

		//
		// RECORD COMMAND BUFFER
		//

		vk::command_buffer_begin_info command_buffer_begin_info = {};
		command_buffer_begin_info.sType	= VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		command_buffer_begin_info.flags	= 0;

		result = vk::begin_command_buffer(command_buffer->command_buffer, &command_buffer_begin_info);
		function_validate_result("failed to begin command buffer");

		{
			std::unique_lock lock(window_state->spawn_objects_mutex);
			for (auto object : window_state->spawn_objects)
				object->initial_update_buffers(window_state->vma_allocator, command_buffer->command_buffer);
			window_state->spawn_objects.clear();
		}

		{
			std::unique_lock lock(window_state->dirty_objects_mutex);
			for (auto object : window_state->dirty_objects)
				object->update_buffers(window_state->vma_allocator, command_buffer->command_buffer);
			window_state->dirty_objects.clear();
		}

		vk::cmd::pipeline_barrier(
			command_buffer->command_buffer,
			VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
			VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
			0,
			0, nullptr,
			0, nullptr,
			0, nullptr
		);

		//lungo::handler::panel::get()->run_imgui();

		//
		// RECORD RENDER PASS
		//

		std::array<vk::clear_value, 1> clear_values {
			{ 0.0f, 0.0f, 0.0f, 1.0f }
		};

		vk::render_pass_begin_info render_pass_begin_info = {};

		render_pass_begin_info.sType					= VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		render_pass_begin_info.renderPass				= renderpass_master->renderpass;
		render_pass_begin_info.framebuffer			    = framebuffer->frame_buffer;
		render_pass_begin_info.renderArea.extent.width  = window_state->swapchain_state->extent.width;
		render_pass_begin_info.renderArea.extent.height = window_state->swapchain_state->extent.height;
		render_pass_begin_info.clearValueCount		    = clear_values.size();
		render_pass_begin_info.pClearValues			    = clear_values.data();

		vk::cmd::begin_render_pass(
			command_buffer->command_buffer,
			&render_pass_begin_info,
			VK_SUBPASS_CONTENTS_INLINE
		);

		for (auto& pipeline_bucket : window_state->pipeline_buckets)
		{
			std::shared_lock lock(pipeline_bucket.second->shared_mutex);

			vk::cmd::bind_pipeline(
				command_buffer->command_buffer,
				VK_PIPELINE_BIND_POINT_GRAPHICS,
				pipeline_bucket.second->pipeline->pipeline
			);

			for (auto object : pipeline_bucket.second->objects)
			{
				object->render(command_buffer->command_buffer,
					window_state->vma_allocator,
					pipeline_bucket.second->pipeline->pipeline_layout
				);
			}
		}

		vk::cmd::end_render_pass(command_buffer->command_buffer);

		result = vk::end_command_buffer(command_buffer->command_buffer);
		function_validate_result("failed to end command buffer");

		//
		// SUBMIT TO GRAPHICS QUEUE
		//

		std::vector<vk::command_buffer> used_command_buffers;
		used_command_buffers.emplace_back(command_buffer->command_buffer);

		vk::pipeline_stage_flags pipeline_stage_flags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		vk::submit_info submit_info					  = {};

		submit_info.sType			     = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submit_info.waitSemaphoreCount   = 1;
		submit_info.pWaitSemaphores	     = &ready_execution_semaphore;
		submit_info.pWaitDstStageMask    = &pipeline_stage_flags;
		submit_info.commandBufferCount   = used_command_buffers.size();
		submit_info.pCommandBuffers		 = used_command_buffers.data();
		submit_info.signalSemaphoreCount = 1;
		submit_info.pSignalSemaphores	 = &ready_presentation_semaphore;

		result = vk::queue_submit(window_state->device_state->graphics_queue, 1, &submit_info, ready_write_fence);
		function_validate_result("failed submit to graphics queue");

		//
		// SUBMIT TO PRESENT QUEUE
		//

		vk::present_info present_info	= {};
		present_info.sType				= VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		present_info.waitSemaphoreCount = 1;
		present_info.pWaitSemaphores	= &ready_presentation_semaphore;
		present_info.swapchainCount		= 1;
		present_info.pSwapchains		= &window_state->swapchain_state->swapchain;
		present_info.pImageIndices		= &swapchain_image_idx;

		result = vk::queue_present(window_state->device_state->graphics_queue, &present_info);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window_state->input_state->framebuffer_resize)
		{
			while (!window_state->input_state->framebuffer_resize)
				std::this_thread::sleep_for(std::chrono::milliseconds(10));

			window_state->resize_waiting = true;

			while (window_state->input_state->framebuffer_resize)
				std::this_thread::sleep_for(std::chrono::milliseconds(10));

			while (window_state->input_state->size_zero)
				std::this_thread::sleep_for(std::chrono::milliseconds(10));

			// RELOAD SWAPCHAIN

			mac::window::reload_swapchain(window_state);

			function_setup_references();
			window_state->resize_done = true;

			result = VK_SUCCESS;
		}
		else
			function_validate_result("failed present graphics queue");

		window_state->command_buffer_idx = (window_state->command_buffer_idx + 1) % window_state->command_buffers.size();
	}
}

void mac::window::add_dirty_object(mac::window::state* window_state, mac::object* object)
{
	window_state->dirty_objects.emplace_back(object);
}

mac::renderpass::state* mac::window::get_renderpass(mac::window::state* window_state, const std::string& name)
{
	for (auto& renderpass : window_state->renderpasses)
	{
		if (renderpass.first == name)
			return renderpass.second;
	}

	latte::logger::error(latte::logger::level::SYSTEM, "failed to get renderpass \"" + name + "\"");
	return nullptr;
}

mac::pipeline::state* mac::window::get_pipeline(mac::window::state* window_state, const std::string& name)
{
	for (auto& pipeline : window_state->pipelines)
	{
		if (pipeline.first == name)
			return pipeline.second;
	}

	latte::logger::error(latte::logger::level::SYSTEM, "failed to get pipeline \"" + name + "\"");
	return nullptr;
}

mac::descriptor_set::state* mac::window::get_descriptor_set(mac::window::state* window_state, const std::string& name)
{
	for (auto& descriptor_set : window_state->descriptor_sets)
	{
		if (descriptor_set.first == name)
			return descriptor_set.second;
	}

	latte::logger::error(latte::logger::level::SYSTEM, "failed to get descriptor set \"" + name + "\"");
	return nullptr;
}

mac::texture_sampler::state* mac::window::get_texture_sampler(mac::window::state* window_state, const std::string& name)
{
	for (auto& texture_sampler : window_state->texture_samplers)
	{
		if (texture_sampler.first == name)
			return texture_sampler.second;
	}

	latte::logger::error(latte::logger::level::SYSTEM, "failed to get texture sampler \"" + name + "\"");
	return nullptr;
}

mac::window::pipeline_bucket*
mac::window::get_pipeline_bucket(mac::window::state* window_state, const std::string& name)
{
	for (auto& pipeline_bucket : window_state->pipeline_buckets)
	{
		if (pipeline_bucket.first == name)
			return pipeline_bucket.second;
	}

	latte::logger::error(latte::logger::level::SYSTEM, "failed to get pipeline bucket \"" + name + "\"");
	return nullptr;
}

void mac::window::load_image(mac::window::state* window_state, const std::string& path, const std::string& registry_name)
{
	//
	// LOAD IMAGE DATA
	//

	auto reg_image = new mac::window::reg_image{};
	window_state->registered_images[registry_name] = reg_image;

	i32 texture_width;
	i32 texture_height;
	i32 texture_channels;
	u8* pixels = nullptr;

	pixels = stbi_load(path.c_str(), &texture_width, &texture_height, &texture_channels, 4);

	if (!pixels)
		latte::logger::fatal(latte::logger::level::SYSTEM, "failed to load texture \"" + path + "\"");

	vk::device_size image_size = texture_width * texture_height * texture_channels;

	//
	// CREATE CPU & GPU BUFFERS
	//

	vk::buffer_create_info buffer_create_info = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
	vma::allocation_create_info allocation_create_info = {};
	vk::image_create_info image_create_info = {};
	vk::image_view_create_info image_view_create_info {};

	buffer_create_info.size = image_size;
	buffer_create_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

	allocation_create_info.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;

	vk::buffer staging_buffer;
	vma::allocation staging_buffer_allocation;

	if (vma::create_buffer(
		window_state->vma_allocator,
		&buffer_create_info,
		&allocation_create_info,
		&staging_buffer,
		&staging_buffer_allocation,
		nullptr) != VK_SUCCESS)
		latte::logger::fatal(latte::logger::level::SYSTEM, __FILE__, __LINE__, "failed to allocate staging buffer for image");

	void* data;
	vma::map_memory(window_state->vma_allocator, staging_buffer_allocation, &data);
	memcpy(data, pixels, image_size);
	vma::unmap_memory(window_state->vma_allocator, staging_buffer_allocation);

	image_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	image_create_info.imageType = VK_IMAGE_TYPE_2D;
	image_create_info.format = VK_FORMAT_R8G8B8A8_UNORM;
	image_create_info.extent = { static_cast<u32>(texture_width), static_cast<u32>(texture_height), 1 };
	image_create_info.mipLevels	 = 1;
	image_create_info.arrayLayers	 = 1;
	image_create_info.samples = VK_SAMPLE_COUNT_1_BIT;
	image_create_info.tiling = VK_IMAGE_TILING_LINEAR;
	image_create_info.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
	image_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	allocation_create_info.usage = VMA_MEMORY_USAGE_GPU_ONLY;

	if (vma::create_image(window_state->vma_allocator, &image_create_info, &allocation_create_info, &reg_image->image, &reg_image->allocation, nullptr)
		!= VK_SUCCESS)
		latte::logger::fatal(latte::logger::level::SYSTEM, __FILE__, __LINE__, "failed to image");

	//
	// CPU ---> GPU
	//

	vk::image_memory_barrier image_memory_barrier = {};
	vk::buffer_image_copy buffer_image_copy = {};

	auto command_buffer = mac::window::begin_single_time_command(window_state->device_state->logical_device, window_state->command_pool_state->command_pool);

	image_memory_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	image_memory_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	image_memory_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	image_memory_barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	image_memory_barrier.subresourceRange.baseMipLevel = 0;
	image_memory_barrier.subresourceRange.levelCount = 1;
	image_memory_barrier.subresourceRange.baseArrayLayer = 0;
	image_memory_barrier.subresourceRange.layerCount = 1;
	image_memory_barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	image_memory_barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	image_memory_barrier.image = reg_image->image;
	image_memory_barrier.srcAccessMask = 0;
	image_memory_barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

	vk::cmd::pipeline_barrier(
		command_buffer,
		VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
		VK_PIPELINE_STAGE_TRANSFER_BIT,
		0,
		0, nullptr,
		0, nullptr,
		1, &image_memory_barrier);

	buffer_image_copy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	buffer_image_copy.imageSubresource.layerCount = 1;
	buffer_image_copy.imageExtent.width = texture_width;
	buffer_image_copy.imageExtent.height = texture_height;
	buffer_image_copy.imageExtent.depth = 1;

	vk::cmd::copy_buffer_to_image(command_buffer, staging_buffer, reg_image->image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &buffer_image_copy);

	image_memory_barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	image_memory_barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	image_memory_barrier.image = reg_image->image;
	image_memory_barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	image_memory_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

	vk::cmd::pipeline_barrier(
		command_buffer,
		VK_PIPELINE_STAGE_TRANSFER_BIT,
		VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
		0,
		0, nullptr,
		0, nullptr,
		1, &image_memory_barrier);

	mac::window::end_single_time_command(command_buffer, window_state->device_state->graphics_queue);
	vma::destroy_buffer(window_state->vma_allocator, staging_buffer, staging_buffer_allocation);

	//
	// CREATE IMAGE VIEW
	//

	image_view_create_info.sType							  = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	image_view_create_info.image							  = reg_image->image;
	image_view_create_info.viewType						  = VK_IMAGE_VIEW_TYPE_2D;
	image_view_create_info.format						  = VK_FORMAT_R8G8B8A8_UNORM;
	image_view_create_info.subresourceRange.aspectMask	  = VK_IMAGE_ASPECT_COLOR_BIT;
	image_view_create_info.subresourceRange.baseMipLevel	  = 0;
	image_view_create_info.subresourceRange.levelCount	  = 1;
	image_view_create_info.subresourceRange.baseArrayLayer = 0;
	image_view_create_info.subresourceRange.layerCount	  = 1;

	if (vk::create_image_view(window_state->device_state->logical_device, &image_view_create_info, nullptr, &reg_image->image_view)
		!= VK_SUCCESS)
		latte::logger::fatal(latte::logger::level::SYSTEM, "failed to create image view");
}

vk::image_view& mac::window::get_image(mac::window::state* window_state, const std::string& registry_name)
{
	auto& image_view = window_state->registered_images[registry_name]->image_view;
#ifndef NDEBUG
	if (image_view == nullptr)
		latte::logger::error(latte::logger::level::SYSTEM, __FILE__, __LINE__, "image \"" + registry_name + "\" does not exist in the registry");
#endif
	return image_view;
}

vk::command_buffer mac::window::begin_single_time_command(vk::device& logical_device, vk::command_pool& command_pool)
{
	vk::command_buffer command_buffer = {};
	vk::command_buffer_begin_info command_buffer_begin_info = {};
	vk::command_buffer_allocate_info command_buffer_allocate_info {};

	command_buffer_allocate_info.sType				 = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	command_buffer_allocate_info.level				 = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	command_buffer_allocate_info.commandPool		 = command_pool;
	command_buffer_allocate_info.commandBufferCount = 1;

	vk::allocate_command_buffers(logical_device, &command_buffer_allocate_info, &command_buffer);

	command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	command_buffer_begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	if (vk::begin_command_buffer(command_buffer, &command_buffer_begin_info) != VK_SUCCESS)
		latte::logger::fatal(latte::logger::level::SYSTEM, "failed to begin single time command buffer");

	return command_buffer;
}

void mac::window::end_single_time_command(vk::command_buffer& command_buffer, vk::queue& graphics_queue)
{
	vk::end_command_buffer(command_buffer);
	vk::submit_info submit_info = {};

	submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit_info.commandBufferCount = 1;
	submit_info.pCommandBuffers = &command_buffer;

	if (vk::queue_submit(graphics_queue, 1, &submit_info, nullptr) != VK_SUCCESS)
		latte::logger::fatal(latte::logger::level::SYSTEM, "failed to submit single time command buffer");

	if (vk::queue_wait_idle(graphics_queue) != VK_SUCCESS)
		latte::logger::fatal(latte::logger::level::SYSTEM, "failed to wait on single time command buffer");
}

void mac::window::reload_swapchain(mac::window::state* window_state)
{
	// MINIMIZE

	vk::device_wait_idle(window_state->device_state->logical_device);
	latte::logger::info(latte::logger::level::SYSTEM, "recreating swapchain...");

	// DESTROY

	for (auto framebuffer : window_state->framebuffers)
		mac::framebuffer::destroy(framebuffer, window_state->device_state->logical_device);

	for (auto& pipeline : window_state->pipelines)
		mac::pipeline::destroy(pipeline.second, window_state->device_state->logical_device);

	auto pipeline_buckets_copy = window_state->pipeline_buckets;

	for (auto& renderpass : window_state->renderpasses)
		mac::renderpass::destroy(renderpass.second, window_state->device_state->logical_device);

	mac::swapchain::destroy(window_state->swapchain_state, window_state->device_state->logical_device);

	// CLEAR

	window_state->framebuffers.clear();
	window_state->pipelines.clear();
	window_state->pipeline_buckets.clear();
	window_state->renderpasses.clear();

	// CREATE

	mac::window::create_swapchain_and_renderpasses(window_state);
	mac::window::create_pipeline_and_framebuffers(window_state);

	// TRANSFER OBJECTS

	for (auto& pipeline_bucket : pipeline_buckets_copy)
	{
		auto real_pb = mac::window::get_pipeline_bucket(window_state, pipeline_bucket.first);

		for (auto pb_obj : pipeline_bucket.second->objects)
			real_pb->objects.emplace_back(pb_obj);
	}

	for (auto& pipeline_bucket : pipeline_buckets_copy)
		delete pipeline_bucket.second;

	pipeline_buckets_copy.clear();

	latte::logger::info(latte::logger::level::SYSTEM, "...recreated swapchain");
	vk::device_wait_idle(window_state->device_state->logical_device);
}

void mac::window::create_swapchain_and_renderpasses(mac::window::state* window_state)
{
	// SWAPCHAIN

	window_state->swapchain_state = mac::swapchain::create(
		window_state->device_state->logical_device,
		window_state->device_state->physical_device,
		&window_state->surface,
		window_state->window
	);

	// RENDERPASSES

	window_state->renderpasses.emplace_back(std::make_pair(
		"master",
		mac::renderpass::create(
			window_state->device_state->logical_device,
			window_state->swapchain_state->surface_format
		)
	));
}

void mac::window::create_pipeline_and_framebuffers(mac::window::state* window_state)
{
	auto descriptor_set_cam = mac::window::get_descriptor_set(window_state, "cam");
	auto descriptor_set_obj_c = mac::window::get_descriptor_set(window_state, "obj_c");
	auto descriptor_set_obj_t = mac::window::get_descriptor_set(window_state, "obj_t");

	auto renderpass_master = mac::window::get_renderpass(window_state, "master");

	//
	// PIPELINES
	//

	window_state->pipelines.emplace_back(std::make_pair(
		"color",
		mac::pipeline::create(
			window_state->device_state->logical_device,
			{ descriptor_set_cam, descriptor_set_obj_c },
			renderpass_master,
			new mac::vertex_color::info{},
			window_state->swapchain_state->extent,
			"color",
			"color",
			VK_CULL_MODE_NONE
		)
	));

	window_state->pipelines.emplace_back(std::make_pair(
		"texture",
		mac::pipeline::create(
			window_state->device_state->logical_device,
			{ descriptor_set_cam, descriptor_set_obj_t },
			renderpass_master,
			new mac::vertex_texture::info{},
			window_state->swapchain_state->extent,
			"texture",
			"texture",
			VK_CULL_MODE_NONE
		)
	));

	auto pipeline_color = mac::window::get_pipeline(window_state, "color");
	auto pipeline_texture = mac::window::get_pipeline(window_state, "texture");

	//
	// PIPELINE BUCKETS
	//

	window_state->pipeline_buckets.emplace_back(std::make_pair(
		"color",
		new mac::window::pipeline_bucket {
			.pipeline = pipeline_color
		}
	));

	window_state->pipeline_buckets.emplace_back(std::make_pair(
		"texture",
		new mac::window::pipeline_bucket {
			.pipeline = pipeline_texture
		}
	));

	//
	// FRAMEBUFFERS
	//

	std::vector<vk::image> swapchain_images(window_state->swapchain_state->image_count);
	vk::get_swapchain_images_khr(
		window_state->device_state->logical_device,
		window_state->swapchain_state->swapchain,
		&window_state->swapchain_state->image_count,
		swapchain_images.data()
	);

	for (auto& swapchain_image : swapchain_images)
	{
		window_state->framebuffers.emplace_back(
			mac::framebuffer::create(
				window_state->device_state->logical_device,
				renderpass_master->renderpass,
				window_state->swapchain_state->extent,
				window_state->swapchain_state->surface_format,
				swapchain_image
			)
		);
	}
}

void mac::window::add_frame_update(mac::window::state* window_state, const std::function<void(double)>& callback)
{
	window_state->frame_updates.emplace_back(callback);
}
