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
#include "command_buffer.hpp"
#include "objects/object.hpp"
#include "ubos/ubo_cam.hpp"

#include <latte/utils.hpp>

namespace mac::window
{
///
struct pipeline_bucket
{
	///
	mac::pipeline::state* pipeline;

	///
	std::vector<mac::object*> objects;

	///
	std::shared_mutex shared_mutex;
};

///
struct reg_image
{
	///
	vk::image image;

	///
	vk::image_view image_view;

	///
	vma::allocation allocation;
};

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
	std::vector<mac::command_buffer::state*> command_buffers;

	///
	std::vector<std::pair<std::string, mac::renderpass::state*>> renderpasses;

	///
	std::vector<std::pair<std::string, mac::pipeline::state*>> pipelines;

	///
	std::vector<std::pair<std::string, mac::descriptor_set::state*>> descriptor_sets;

	///
	std::vector<std::pair<std::string, mac::texture_sampler::state*>> texture_samplers;

	///
	std::vector<std::pair<std::string, mac::window::pipeline_bucket*>> pipeline_buckets;

	///
	std::vector<mac::framebuffer::state*> framebuffers;

	///
	u8 command_buffer_idx;

	///
	vma::allocator vma_allocator;

	///
	std::vector<mac::object*> dirty_objects;
	std::shared_mutex dirty_objects_mutex;

	///
	std::vector<mac::object*> spawn_objects;
	std::shared_mutex spawn_objects_mutex;

	///
	std::thread thread_rendering;

	///
	std::unordered_map<std::string, reg_image*> registered_images;

	///
	mac::descriptor_set::instance* cam_instance;

	///
	mac::ubo_cam* ubo_cam;

	///
	std::atomic_bool resize_waiting;

	///
	std::atomic_bool resize_done;

	///
	glm::vec<2, i32> size;

	///
	std::vector<std::function<void(double)>> frame_updates;
};

///
void global_initialization();

///
void global_shutdown();

///
mac::window::state* create(const std::string& title, glm::vec<2, i32> size);

///
void destroy(mac::window::state* window_state);

///
void run(mac::window::state* window_state);

///
void thread_rendering(mac::window::state* window_state);

///
void add_dirty_object(mac::window::state* window_state, mac::object* object);

///
mac::renderpass::state* get_renderpass(mac::window::state* window_state, const std::string& name);

///
mac::pipeline::state* get_pipeline(mac::window::state* window_state, const std::string& name);

///
mac::descriptor_set::state* get_descriptor_set(mac::window::state* window_state, const std::string& name);

///
mac::texture_sampler::state* get_texture_sampler(mac::window::state* window_state, const std::string& name);

///
mac::window::pipeline_bucket* get_pipeline_bucket(mac::window::state* window_state, const std::string& name);

///
void load_image(mac::window::state* window_state, const std::string& path, const std::string& registry_name);

///
vk::image_view& get_image(mac::window::state* window_state, const std::string& registry_name);

/// Only use before the main render loop begins, and after window initialization.
vk::command_buffer begin_single_time_command(vk::device& logical_device, vk::command_pool& command_pool);

/// Only use before the main render loop begins, and after window initialization.
void end_single_time_command(vk::command_buffer& command_buffer, vk::queue& graphics_queue);

///
void reload_swapchain(mac::window::state* window_state);

///
void create_swapchain_and_renderpasses(mac::window::state* window_state);

///
void create_pipeline_and_framebuffers(mac::window::state* window_state);

///
void add_frame_update(mac::window::state* window_state, const std::function<void(double)>& callback);
}
