#pragma once
#include "config.h"

#include <cappuccino/utils.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

namespace rhr::render
{
///
class render_pass
{
public:
	///
	static void init_render_pass();

	///
	static VkRenderPass render_pass_master;
};
}