#include "window.hpp"

#include "vertex_std.hpp"

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

	// DESCRIPTOR SETS

	window_state->descriptor_sets.emplace_back(std::make_pair(
		"std",
		mac::descriptor_set::create(
			window_state->device_state->logical_device,
			{
				mac::descriptor_set::binding_type::UNIFORM_BUFFER,
				mac::descriptor_set::binding_type::IMAGE_SAMPLER
			}
		)
	));

	auto renderpass_master = mac::window::get_renderpass(window_state, "master");
	auto descriptor_set_std = mac::window::get_descriptor_set(window_state, "std");

	// PIPELINES

	window_state->pipelines.emplace_back(std::make_pair(
		"std",
		mac::pipeline::create(
			window_state->device_state->logical_device,
			{ descriptor_set_std },
			renderpass_master,
			new mac::vertex_std({}, {}, {}),
			window_state->swapchain_state->extent,
			"master",
			"std",
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

	// command pool command buffers

	

	// swapchain syncronization (semaphores and fences)






	//m_swapchain = std::make_unique<rhr::render::component::swapchain>();
	//m_swapchain->create_render_passes();
	//
	//initialize_descriptor_set();
	//
	//m_pipeline	   = std::make_unique<rhr::render::component::pipeline>();
	//m_command_pool = std::make_unique<rhr::render::component::command_pool>();
	//m_swapchain->create_framebuffers();
	//
	//initialize_texture_sampler();
	//
	//m_command_pool->initialize_descriptor_pool();
	//m_command_pool->initialize_command_buffers();
	//m_swapchain->create_synchronization();
	//
	//m_pipeline->apply_active_pipeline("master");

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

	latte::logger::error(latte::logger::level::SYSTEM, "failed to get texture sampler set \"" + name + "\"");
	return nullptr;
}
