#pragma once
#include "config.h"

#include "lungo/components/frame.hpp"
#include "lungo/components/pass.hpp"

#include <latte/utils.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

namespace rhr::render::component
{
///
class swapchain
{
public:
	swapchain();
	~swapchain();

	///
	void create_render_passes();

	///
	void create_framebuffers();

	///
	void create_synchronization();

	///
	vk::format& get_format();

	///
	vk::surface_format_khr& get_surface_format();

	///
	vk::present_mode_khr& get_present_mode();

	///
	vk::extent_2d& get_extent();

	///
	vk::render_pass& get_render_pass(u8 idx);

	///
	u8 get_framebuffer_count();

	///
	vk::frame_buffer& get_framebuffer(u8 idx);

	///
	vk::swapchain_khr& get_swapchain();

	///
	rhr::render::component::frame& get_frame(u8 idx);

private:
	///
	std::vector<rhr::render::component::frame> m_frames;

	///
	vk::format m_image_format;

	///
	vk::extent_2d m_extent;

	///
	vk::swapchain_khr m_swapchain;

	///
	vk::surface_format_khr m_surface_format;

	///
	vk::present_mode_khr m_present_mode;

	///
	std::vector<rhr::render::component::pass> m_render_passes;
};
} // namespace rhr::render::component