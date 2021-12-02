#pragma once
#include "config.h"

#include "rhr/rendering/components/frame.hpp"

#include <cappuccino/utils.hpp>
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
	void create_swapchain();

	///
	void create_image_views();

	///
	void create_framebuffers();

	///
	void create_synchronization();
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
};
}