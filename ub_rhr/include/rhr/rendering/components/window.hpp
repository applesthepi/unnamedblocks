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
	window(std::string title, glm::vec<2, i32> window_size);

	///
	void create_systems();

	///
	void delete_systems();

	///
	vk::surface_khr* get_surface();

	///
	vk::physical_device* get_physical_device();

	///
	vk::device* get_device();

	///
	void flag_swapchain_recreation(glm::vec<2, i32> window_size);

	///
	void update_window_position(glm::vec<2, i32> window_position);
private:
    ///
    void setup();

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
    std::string m_window_title;

	///
	std::unique_ptr<rhr::render::components::device> m_device;

	///
	vk::surface_khr m_surface;
};
}