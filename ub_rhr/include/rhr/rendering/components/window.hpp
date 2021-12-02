#pragma once
#include "config.h"

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
	glm::vec<2, i32> m_window_size;

	///
	glm::vec<2, i32> m_window_position;
};
}