#pragma once
#include "config.h"

#include "input.hpp"
#include "device.hpp"
#include "swapchain.hpp"
#include "pipeline.hpp"
#include "command_pool.hpp"
#include "renderpass.hpp"
#include "descriptor_set.hpp"
#include "framebuffer.hpp"
#include "texture_sampler.hpp"

#include <latte/utils.hpp>

namespace mac::window
{
///
struct state
{
	///
	glfw::window* window;

	///
	vk::surface_khr surface;

	///
	mac::input::state* input_state;

	///
	mac::device::state* device_state;

	///
	mac::swapchain::state* swapchain_state;

	///
	mac::command_pool::state* command_pool_state;

	///
	vk::descriptor_pool descriptor_pool;

	///
	std::vector<std::pair<std::string, mac::renderpass::state*>> renderpasses;

	///
	std::vector<std::pair<std::string, mac::pipeline::state*>> pipelines;

	///
	std::vector<std::pair<std::string, mac::descriptor_set::state*>> descriptor_sets;

	///
	std::vector<std::pair<std::string, mac::texture_sampler::state*>> texture_samplers;

	///
	std::vector<mac::framebuffer::state*> framebuffers;
};

///
void global_initialization();

///
mac::window::state* create(const std::string& title, glm::vec<2, i32> size);

///
void destroy(mac::window::state* window_state);

///
mac::renderpass::state* get_renderpass(mac::window::state* window_state, const std::string& name);

///
mac::pipeline::state* get_pipeline(mac::window::state* window_state, const std::string& name);

///
mac::descriptor_set::state* get_descriptor_set(mac::window::state* window_state, const std::string& name);

///
mac::texture_sampler::state* get_texture_sampler(mac::window::state* window_state, const std::string& name);
}
