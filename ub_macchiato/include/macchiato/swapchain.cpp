#include "swapchain.hpp"

mac::swapchain::state* mac::swapchain::create(vk::device& logical_device, vk::physical_device& physical_device, vk::surface_khr* surface, glfw::window* glfw_window)
{
	auto swapchain_state = new mac::swapchain::state {
		.surface_format = {},
		.present_mode = VK_PRESENT_MODE_FIFO_KHR,
	};

	auto swapchain_support = query_swapchain_support(physical_device, surface);

	// SURFACE FORMAT

	auto function_surface_format = [&swapchain_support, swapchain_state](vk::format target_format)
	{
		for (const auto& format : swapchain_support.formats)
		{
			if (format.format == target_format)
			{
				swapchain_state->surface_format = format;
				return true;
			}
		}

		return false;
	};

	if (!function_surface_format(VK_FORMAT_B8G8R8A8_UNORM))
	{
		if (!function_surface_format(VK_FORMAT_B8G8R8_SRGB))
		{
			latte::logger::fatal(latte::logger::level::SYSTEM, "no supported surface formats found");
			return nullptr;
		}
	}

	// PRESENT MODE

	auto function_present_mode = [&swapchain_support, swapchain_state](vk::present_mode_khr target_present_mode)
	{
		for (const auto& present_mode : swapchain_support.present_modes)
		{
			if (present_mode == target_present_mode)
			{
				swapchain_state->present_mode = present_mode;
				return true;
			}
		}

		return false;
	};

	if (!function_present_mode(/*VK_PRESENT_MODE_MAILBOX_KHR*/ VK_PRESENT_MODE_FIFO_KHR))
	{
		if (!function_present_mode(VK_PRESENT_MODE_FIFO_KHR))
		{
			latte::logger::fatal(latte::logger::level::SYSTEM, "no supported present modes found");
			return nullptr;
		}
	}

	// EXTENT

	if (swapchain_support.capabilities.currentExtent.width != UINT32_MAX)
		swapchain_state->extent = swapchain_support.capabilities.currentExtent;
	else
	{
		i32 width;
		i32 height;
		glfw::get_framebuffer_size(glfw_window, &width, &height);

		vk::extent_2d actual_extent = { static_cast<u32>(width), static_cast<u32>(height) };

		actual_extent.width = std::max(
			swapchain_support.capabilities.minImageExtent.width, std::min(swapchain_support.capabilities.maxImageExtent.width, actual_extent.width));
		actual_extent.height = std::max(
			swapchain_support.capabilities.minImageExtent.height, std::min(swapchain_support.capabilities.maxImageExtent.height, actual_extent.height));

		swapchain_state->extent = actual_extent;
	}

	// SWAPCHAIN

	// TODO: image count standardization?
	swapchain_state->image_count = swapchain_support.capabilities.minImageCount + 1;

	if (swapchain_support.capabilities.maxImageCount > 0
		&& swapchain_state->image_count > swapchain_support.capabilities.maxImageCount)
		swapchain_state->image_count = swapchain_support.capabilities.maxImageCount;

	vk::swapchain_create_info_khr swapchain_create_info_khr = {};
	swapchain_create_info_khr.sType							= VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchain_create_info_khr.surface						= *surface;

	swapchain_create_info_khr.minImageCount	   = swapchain_state->image_count;
	swapchain_create_info_khr.imageFormat	   = swapchain_state->surface_format.format;
	swapchain_create_info_khr.imageColorSpace  = swapchain_state->surface_format.colorSpace;
	swapchain_create_info_khr.imageExtent	   = swapchain_state->extent;
	swapchain_create_info_khr.imageArrayLayers = 1;
	swapchain_create_info_khr.imageUsage	   = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	auto device_queue_family_indices = mac::swapchain::find_queue_families(physical_device, surface);

	u32 queue_family_indices[] = {
		device_queue_family_indices.graphics_family.value(),
		device_queue_family_indices.present_family.value()
	};

	if (device_queue_family_indices.graphics_family != device_queue_family_indices.present_family)
	{
		swapchain_create_info_khr.imageSharingMode		= VK_SHARING_MODE_CONCURRENT;
		swapchain_create_info_khr.queueFamilyIndexCount = 2;
		swapchain_create_info_khr.pQueueFamilyIndices	= queue_family_indices;
	}
	else
	{
		swapchain_create_info_khr.imageSharingMode		= VK_SHARING_MODE_EXCLUSIVE;
		swapchain_create_info_khr.queueFamilyIndexCount = 0;
		swapchain_create_info_khr.pQueueFamilyIndices	= nullptr;
	}

	swapchain_create_info_khr.preTransform	 = swapchain_support.capabilities.currentTransform;
	swapchain_create_info_khr.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchain_create_info_khr.presentMode	 = swapchain_state->present_mode;
	swapchain_create_info_khr.clipped		 = VK_TRUE;
	swapchain_create_info_khr.oldSwapchain	 = VK_NULL_HANDLE;

	if (vk::create_swapchain_khr(logical_device, &swapchain_create_info_khr, nullptr, &swapchain_state->swapchain)
		!= VK_SUCCESS)
	{
		latte::logger::fatal(latte::logger::level::SYSTEM, "failed to create swapchain");
		return nullptr;
	}

	return swapchain_state;
}

void mac::swapchain::destroy(mac::swapchain::state* swapchain_state, vk::device& logical_device)
{
	vk::destroy_swapchain(logical_device, swapchain_state->swapchain, nullptr);
	delete swapchain_state;
}

mac::swapchain::swap_chain_support_details
mac::swapchain::query_swapchain_support(vk::physical_device& device, vk::surface_khr* surface)
{
	mac::swapchain::swap_chain_support_details details = {};

	vk::get_physical_device_surface_capabilities_khr(device, *surface, &details.capabilities);

	u32 format_count;
	vk::get_physical_device_surface_formats_khr(device, *surface, &format_count, nullptr);

	if (format_count == 0)
	{
		latte::logger::fatal(latte::logger::level::SYSTEM, "no supported vulkan formats found");
		return details;
	}

	details.formats.resize(format_count);
	vk::get_physical_device_surface_formats_khr(device, *surface, &format_count, details.formats.data());

	u32 present_mode_count;
	vk::get_physical_device_surface_present_modes_khr(device, *surface, &present_mode_count, nullptr);

	if (present_mode_count == 0)
	{
		latte::logger::fatal(latte::logger::level::SYSTEM, "no supported vulkan present modes found");
		return details;
	}

	details.present_modes.resize(present_mode_count);
	vk::get_physical_device_surface_present_modes_khr(device, *surface, &present_mode_count, details.present_modes.data());

	return details;
}

mac::swapchain::queue_family_indices
mac::swapchain::find_queue_families(vk::physical_device& device, vk::surface_khr* surface)
{
	mac::swapchain::queue_family_indices queue_family_indices = {};

	u32 queue_family_count = 0;
	vk::get_physical_device_queue_family_properties(device, &queue_family_count, nullptr);

	std::vector<vk::queue_family_properties> queue_families(queue_family_count);
	vk::get_physical_device_queue_family_properties(device, &queue_family_count, queue_families.data());

	for (i64 i = 0; i < queue_families.size(); i++)
	{
		vk::bool32 present_support = false;
		vk::get_physical_device_surface_support_khr(device, i, *surface, &present_support);

		if (present_support)
			queue_family_indices.present_family.emplace(i);

		if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			queue_family_indices.graphics_family.emplace(i);

		if (queue_family_indices.complete())
			return queue_family_indices;

		queue_family_indices.present_family.reset();
		queue_family_indices.graphics_family.reset();
	}

	latte::logger::fatal(latte::logger::level::SYSTEM, "no supported vulkan devices found");
	return queue_family_indices;
}