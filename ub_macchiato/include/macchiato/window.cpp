#include "window.hpp"

#include "vertex_std.hpp"
#include "ubos/ubo_cam.hpp"
#include "ubos/ubo_obj.hpp"

void mac::window::global_initialization()
{
	glfwInit();
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

	//glfw::set_framebuffer_size_callback(window_state->window, mac::input::callback::framebuffer_resize);
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

	auto renderpass_master = mac::window::get_renderpass(window_state, "master");

	// DESCRIPTOR SETS

	u32 binding_idx = 0;

	window_state->descriptor_sets.emplace_back(std::make_pair(
		"cam",
		mac::descriptor_set::create(
			window_state->device_state->logical_device,
			{ new mac::ubo_cam() },
			0,
			&binding_idx
		)
	));

	auto descriptor_set_cam = mac::window::get_descriptor_set(window_state, "cam");

	window_state->descriptor_sets.emplace_back(std::make_pair(
		"obj_c",
		mac::descriptor_set::create(
			window_state->device_state->logical_device,
			{ new mac::ubo_obj() },
			0,
			&binding_idx
		)
	));

	auto descriptor_set_obj_c = mac::window::get_descriptor_set(window_state, "obj_c");

	//window_state->descriptor_sets.emplace_back(std::make_pair(
	//	"obj_t",
	//	mac::descriptor_set::create(
	//		window_state->device_state->logical_device,
	//		{ new mac::ubo_obj() },
	//		1,
	//		&binding_idx
	//	)
	//));
	//
	//auto descriptor_set_obj_t = mac::window::get_descriptor_set(window_state, "obj_t");

	// PIPELINES

	window_state->pipelines.emplace_back(std::make_pair(
		"color",
		mac::pipeline::create(
			window_state->device_state->logical_device,
			{ descriptor_set_cam },
			renderpass_master,
			new mac::vertex_std({}, {}),
			window_state->swapchain_state->extent,
			"color",
			"color",
			VK_CULL_MODE_NONE
		)
	));

	auto pipeline_color = mac::window::get_pipeline(window_state, "color");

	//window_state->pipelines.emplace_back(std::make_pair(
	//	"texture",
	//	mac::pipeline::create(
	//		window_state->device_state->logical_device,
	//		{ descriptor_set_cam, descriptor_set_obj_t },
	//		renderpass_master,
	//		new mac::vertex_std({}, {}, {}),
	//		window_state->swapchain_state->extent,
	//		"texture",
	//		"texture",
	//		VK_CULL_MODE_NONE
	//	)
	//));
	//
	//auto pipeline_texture = mac::window::get_pipeline(window_state, "texture");

	// COMMAND POOL

	window_state->command_pool_state = mac::command_pool::create(
		window_state->device_state->logical_device,
		window_state->device_state->physical_device,
		&window_state->surface
	);

	// SWAPCHAIN FRAMEBUFFERS

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

	// TEXTURE SAMPLERS

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

	// DESCRIPTOR POOL

	window_state->descriptor_pool = mac::descriptor_set::create_pool(window_state->device_state->logical_device);

	for (auto& descriptor_set : window_state->descriptor_sets)
		mac::descriptor_set::set_descriptor_pool(descriptor_set.second, &window_state->descriptor_pool);

	// COMMAND BUFFERS

	for (u8 i = 0; i < 2; i++)
	{
		window_state->command_buffers.emplace_back(mac::command_buffer::create(
			window_state->device_state->logical_device,
			window_state->command_pool_state,
			window_state->swapchain_state->image_count
		));
	}

	// PIPELINE BUCKETS

	window_state->pipeline_buckets.emplace_back(std::make_pair(
		"color",
		new mac::window::pipeline_bucket {
			.pipeline = pipeline_color
		}
	));

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
		glfw::poll_events();

	if (window_state->thread_rendering.joinable())
		window_state->thread_rendering.join();
}

void mac::window::thread_rendering(mac::window::state* window_state)
{
	auto renderpass_master = get_renderpass(window_state, "master");
	vk::result result = VK_SUCCESS;

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

		// FRAME UPDATE

		// ZOMBIE OPERATIONS

		// TODO: delete zombie buffers

		// WAIT TO RECORD

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

		result = vk::reset_fences(
			window_state->device_state->logical_device,
			1,
			&ready_write_fence
		);

		function_validate_result("failed to reset ready_write_fence");

		u32 swapchain_image_idx;

		result = vk::acquire_next_image(
			window_state->device_state->logical_device,
			window_state->swapchain_state->swapchain,
			UINT64_MAX,
			ready_execution_semaphore,
			VK_NULL_HANDLE,
			&swapchain_image_idx
		);

		auto framebuffer = window_state->framebuffers[swapchain_image_idx];

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
		{
			// RELOAD SWAPCHAIN
		}

		function_validate_result("failed to acquire next image");

		// RECORD COMMAND BUFFER

		vk::command_buffer_begin_info command_buffer_begin_info = {};
		command_buffer_begin_info.sType	= VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		command_buffer_begin_info.flags	= 0;

		result = vk::begin_command_buffer(command_buffer->command_buffer, &command_buffer_begin_info);
		function_validate_result("failed to begin command buffer");

		for (auto object : window_state->dirty_objects)
			object->update_buffers(window_state->vma_allocator, command_buffer->command_buffer);

		vk::cmd::pipeline_barrier(
			command_buffer->command_buffer,
			VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
			VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
			0,
			0, nullptr,
			0, nullptr,
			0, nullptr
		);

		// RECORD RENDER PASS

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

		// SUBMIT TO GRAPHICS QUEUE

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

		// SUBMIT TO PRESENT QUEUE

		vk::present_info present_info	= {};
		present_info.sType				= VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		present_info.waitSemaphoreCount = 1;
		present_info.pWaitSemaphores	= &ready_presentation_semaphore;
		present_info.swapchainCount		= 1;
		present_info.pSwapchains		= &window_state->swapchain_state->swapchain;
		present_info.pImageIndices		= &swapchain_image_idx;

		result = vk::queue_present(window_state->device_state->graphics_queue, &present_info);
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
