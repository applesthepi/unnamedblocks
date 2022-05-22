#include "framebuffer.hpp"

mac::framebuffer::state* mac::framebuffer::create(vk::device& logical_device, vk::render_pass& renderpass, vk::extent_2d& extent, vk::surface_format_khr& surface_format, vk::image& image)
{
	auto framebuffer_state = new mac::framebuffer::state {

	};

	std::array<vk::image_view, 1> attachments = {
		mac::framebuffer::create_image_view(logical_device, image, surface_format.format, VK_IMAGE_ASPECT_COLOR_BIT)
	};

	vk::framebuffer_create_info framebuffer_create_info {};
	framebuffer_create_info.sType			= VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	framebuffer_create_info.renderPass		= renderpass;
	framebuffer_create_info.attachmentCount = static_cast<u32>(attachments.size());
	framebuffer_create_info.pAttachments	= attachments.data();
	framebuffer_create_info.width			= extent.width;
	framebuffer_create_info.height			= extent.height;
	framebuffer_create_info.layers			= 1;

	if (vk::create_framebuffer(logical_device, &framebuffer_create_info, nullptr, &framebuffer_state->frame_buffer)
		!= VK_SUCCESS)
		latte::logger::fatal(latte::logger::level::SYSTEM, "failed to create frame buffers");

	mac::framebuffer::create_semaphore(framebuffer_state, logical_device, "acquired");
	mac::framebuffer::create_semaphore(framebuffer_state, logical_device, "finished");
	mac::framebuffer::create_fence(framebuffer_state, logical_device, "flight");

	return framebuffer_state;
}

void mac::framebuffer::destroy(mac::framebuffer::state* framebuffer_state)
{
	delete framebuffer_state;
}

vk::image_view mac::framebuffer::create_image_view(vk::device& logical_device, vk::image& image, vk::format format, vk::image_aspect_flags aspect_flags)
{
	vk::image_view_create_info image_view_create_info = {};
	image_view_create_info.sType							  = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	image_view_create_info.image							  = image;
	image_view_create_info.viewType						  = VK_IMAGE_VIEW_TYPE_2D;
	image_view_create_info.format						  = format;
	image_view_create_info.subresourceRange.aspectMask	  = aspect_flags;
	image_view_create_info.subresourceRange.baseMipLevel	  = 0;
	image_view_create_info.subresourceRange.levelCount	  = 1;
	image_view_create_info.subresourceRange.baseArrayLayer = 0;
	image_view_create_info.subresourceRange.layerCount	  = 1;

	vk::image_view image_view;

	if (vkCreateImageView(logical_device, &image_view_create_info, nullptr, &image_view)
		!= VK_SUCCESS)
	{
		latte::logger::fatal(latte::logger::level::SYSTEM, "failed to create image view");
		return {};
	}

	return image_view;
}

void mac::framebuffer::create_semaphore(mac::framebuffer::state* framebuffer_state, vk::device& logical_device, const std::string& name)
{
	vk::semaphore_create_info semaphore_create_info = {};
	semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	vk::semaphore semaphore = {};

	if (vk::create_semaphore(logical_device, &semaphore_create_info, nullptr, &semaphore) != VK_SUCCESS)
	{
		latte::logger::fatal(latte::logger::level::SYSTEM, "failed to create semaphore \"" + name + "\"");
		return;
	}

	framebuffer_state->semaphores.emplace_back(std::make_pair(
		name,
		semaphore
	));
}

void mac::framebuffer::create_fence(mac::framebuffer::state* framebuffer_state, vk::device& logical_device, const std::string& name)
{
	vk::fence_create_info fence_create_info = {};
	fence_create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fence_create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	vk::fence fence = {};

	if (vk::create_fence(logical_device, &fence_create_info, nullptr, &fence) != VK_SUCCESS)
	{
		latte::logger::fatal(latte::logger::level::SYSTEM, "failed to create fence \"" + name + "\"");
		return;
	}

	framebuffer_state->fences.emplace_back(std::make_pair(
		name,
		fence
	));
}
