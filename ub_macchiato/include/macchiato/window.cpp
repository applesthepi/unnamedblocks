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

	// INPUT

	glfw::set_framebuffer_size_callback(window_state->window, mac::input::callback::framebuffer_resize);
	glfw::set_key_callback(window_state->window, mac::input::callback::key);
	glfw::set_mouse_button_callback(window_state->window, mac::input::callback::mouse_button);
	glfw::set_scroll_callback(window_state->window, mac::input::callback::scroll);
	glfw::set_cursor_position_callback(window_state->window, mac::input::callback::cursor_position);
	glfw::set_window_position_callback(window_state->window, mac::input::callback::window_position);

	glfw::set_window_user_pointer(window_state->window, reinterpret_cast<void*>(window_state->input_state));
	glfw::get_window_position(window_state->window, &window_state->input_state->screen_position.x, &window_state->input_state->screen_position.y);

	vma::allocator_create_info allocator_create_info = {};

	allocator_create_info.vulkanApiVersion		  = VK_API_VERSION_1_2;
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

	window_state->descriptor_sets.emplace_back(std::make_pair(
		"cam",
		mac::descriptor_set::create(
			window_state->device_state->logical_device,
			{ new mac::ubo_cam() },
			0
		)
	));

	auto descriptor_set_cam = mac::window::get_descriptor_set(window_state, "cam");

	window_state->descriptor_sets.emplace_back(std::make_pair(
		"obj_c",
		mac::descriptor_set::create(
			window_state->device_state->logical_device,
			{ new mac::ubo_obj() },
			0
		)
	));

	auto descriptor_set_obj_c = mac::window::get_descriptor_set(window_state, "obj_c");

	window_state->descriptor_sets.emplace_back(std::make_pair(
		"obj_t",
		mac::descriptor_set::create(
			window_state->device_state->logical_device,
			{ new mac::ubo_obj() },
			1
		)
	));

	auto descriptor_set_obj_t = mac::window::get_descriptor_set(window_state, "obj_t");

	// PIPELINES

	window_state->pipelines.emplace_back(std::make_pair(
		"color",
		mac::pipeline::create(
			window_state->device_state->logical_device,
			{ descriptor_set_cam, descriptor_set_obj_c },
			renderpass_master,
			new mac::vertex_std({}, {}, {}),
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
			new mac::vertex_std({}, {}, {}),
			window_state->swapchain_state->extent,
			"texture",
			"texture",
			VK_CULL_MODE_NONE
		)
	));

	// COMMAND POOL

	window_state->command_pool_state = mac::command_pool::create(
		window_state->device_state->logical_device,
		window_state->device_state->physical_device,
		&window_state->surface
	);

	// SWAPCHAIN FRAMEBUFFERS

	std::vector<vk::image> swapchain_images(window_state->swapchain_state->image_count);
	vk::get_swapchain_images_khr(window_state->device_state->logical_device, window_state->swapchain_state->swapchain, nullptr, swapchain_images.data());

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

	window_state->command_buffers.emplace_back(std::make_pair(
		"master",
		mac::command_buffer::create(
			window_state->device_state->logical_device,
			window_state->command_pool_state,
			window_state->swapchain_state->image_count
		)
	));

	// ADDITIONAL SEMAPHORES & FENCES

	// PIPELINE BUCKETS

	window_state->pipeline_buckets.emplace_back(std::make_pair(
		"",
		{

		}
	));

	return window_state;
}

void mac::window::destroy(state* window_state)
{
	delete window_state;
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

mac::command_buffer::state*
mac::window::get_command_buffer(mac::window::state* window_state, const std::string& name)
{
	for (auto& command_buffer : window_state->command_buffers)
	{
		if (command_buffer.first == name)
			return command_buffer.second;
	}

	latte::logger::error(latte::logger::level::SYSTEM, "failed to get command buffer \"" + name + "\"");
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
