#pragma once
#include "config.h"

#include "rhr/rendering/components/swapchain.hpp"
#include "rhr/rendering/components/pipeline.hpp"
#include "rhr/rendering/components/command_pool.hpp"

#include <cappuccino/utils.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

namespace rhr::render::component
{
///
class device
{
public:
	device();

	/// Needed for most operations. The window can exist before calling this, but every other vulkan operation requires
	/// this to be called beforehand.
	void initialize(vk::surface_khr* surface);

	///
	vk::instance& get_instance();

	///
	vk::physical_device& get_physical_device();

	///
	vk::device& get_device();

	///
	vk::descriptor_set_layout& get_descriptor_set_layout();

	///
	vk::queue& get_graphics_queue();

	///
	vk::sampler& get_texture_sampler();

	///
	std::unique_ptr<rhr::render::component::swapchain>& get_swapchain();

	///
	std::unique_ptr<rhr::render::component::pipeline>& get_pipeline();

	///
	std::unique_ptr<rhr::render::component::command_pool>& get_command_pool();

	///
	static bool validation_layers_enabled;
private:
	///
	void initialize_descriptor_set();

	///
	void initialize_texture_sampler();

	///
	vk::instance m_instance;

	///
	vk::surface_khr* m_surface;

	///
	vk::physical_device m_physical_device;

	///
	vk::device m_device;

	///
	vk::debug_utils_messenger_ext m_debug_messenger;

	///
	std::vector<vk::device_queue_create_info> m_queue_create_infos;

	///
	vk::queue m_graphics_queue;

	///
	bool m_valid;

	/// Descriptor sets are like uniforms. This is the layout for it, defining its structure.
	vk::descriptor_set_layout m_descriptor_set_layout;

	///
	vk::sampler m_texture_sampler;

    ///
	std::unique_ptr<rhr::render::component::swapchain> m_swapchain;

	///
	std::unique_ptr<rhr::render::component::pipeline> m_pipeline;

	///
	std::unique_ptr<rhr::render::component::command_pool> m_command_pool;
};
}