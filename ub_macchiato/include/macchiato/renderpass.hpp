#pragma once
#include "config.h"

#include <latte/utils.hpp>

namespace mac::renderpass
{
///
struct state
{
	///
	vk::render_pass renderpass;
};

///
mac::renderpass::state* create(vk::device& logical_device, vk::surface_format_khr& surface_format);

///
void destroy(mac::renderpass::state* renderpass_state, vk::device& logical_device);
}