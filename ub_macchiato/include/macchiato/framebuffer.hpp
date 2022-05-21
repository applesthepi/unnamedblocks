#pragma once
#include "config.h"

#include <latte/utils.hpp>

namespace mac::framebuffer
{
///
struct state
{
	///
	vk::frame_buffer frame_buffer;

	///
	vk::image image;

	///
	vk::image_view image_view;
};

///
void destroy(mac::framebuffer::state* framebuffer_state);

///
mac::framebuffer::state* create(vk::device& logical_device, vk::render_pass& renderpass, vk::extent_2d& extent, vk::surface_format_khr& surface_format, vk::image& image);

///
vk::image_view create_image_view(vk::device& logical_device, vk::image& image, vk::format format, vk::image_aspect_flags aspect_flags);
}