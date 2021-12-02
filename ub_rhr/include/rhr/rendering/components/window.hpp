#pragma once
#include "config.h"

#include "device.hpp"

#include <cappuccino/utils.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

namespace rhr::render::components
{
///
class window
{
public:
	window(const std::string& title = "", const glm::vec<2, i32>& window_size = { 1280, 720 });

	///
	void flag_swapchain_recreation(const glm::vec<2, i32>& window_size);

	///
	void update_window_position(const glm::vec<2, i32>& window_position);
private:
	///
	glfw::window* m_window;

	///
	bool m_recreate_swapchain;

	///
	bool m_valid;

	///
	glm::vec<2, i32> m_window_size;

	///
	glm::vec<2, i32> m_window_position;

	///
	std::unique_ptr<rhr::render::components::device> m_device;

	///
	vk::surface_khr m_surface;
};
}