#pragma once
#include "config.h"

#include <latte/utils.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

namespace rhr::render::component
{
///
class frame
{
public:
	frame();
	~frame();

	///
	vk::frame_buffer frame_buffer;

	///
	vk::image frame_image;

	///
	vk::image_view frame_view;

	///
	vk::fence fence_in_flight;

	///
	vk::fence fence_image_in_flight;

	///
	vk::semaphore semaphore_image;

	///
	vk::semaphore semaphore_finished;
};
} // namespace rhr::render::component