#include "swapchain.hpp"

#include "rhr/rendering/tools.hpp"
#include "rhr/rendering/renderer.hpp"

rhr::render::component::swapchain::swapchain()
	: m_image_format{}
	, m_extent{}
	, m_swapchain{}
	, m_surface_format{}
	, m_present_mode{}
{
	// Initialize swapchain.

	std::unique_ptr<rhr::render::component::window>& window = rhr::render::renderer::get_window_primary();

	rhr::render::tools::swap_chain_support_details swap_chain_support = rhr::render::tools::query_swap_chain_support(
		window->get_physical_device(),
		window->get_surface()
	);

	m_surface_format = rhr::render::tools::choose_swap_surface_format(swap_chain_support.formats);
	m_present_mode = rhr::render::tools::choose_swap_present_mode(swap_chain_support.present_modes);
	vk::extent_2d extent = rhr::render::tools::choose_swap_extent(swap_chain_support.capabilities);

	u32 image_count = swap_chain_support.capabilities.minImageCount + 1;

	if (swap_chain_support.capabilities.maxImageCount > 0 && image_count > swap_chain_support.capabilities.maxImageCount)
		image_count = swap_chain_support.capabilities.maxImageCount;

	vk::swapchain_create_info_khr swapchain_create_info_khr = {};
	swapchain_create_info_khr.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchain_create_info_khr.surface = *window->get_surface();

	swapchain_create_info_khr.minImageCount = image_count;
	swapchain_create_info_khr.imageFormat = m_surface_format.format;
	swapchain_create_info_khr.imageColorSpace = m_surface_format.colorSpace;
	swapchain_create_info_khr.imageExtent = extent;
	swapchain_create_info_khr.imageArrayLayers = 1;
	swapchain_create_info_khr.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	rhr::render::tools::queue_family_indices indices = rhr::render::tools::find_queue_families(window->get_physical_device(), window->get_surface());
	u32 queue_family_indices[] = { indices.graphics_family.value(), indices.present_family.value() };

	if (indices.graphics_family != indices.present_family)
	{
		swapchain_create_info_khr.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		swapchain_create_info_khr.queueFamilyIndexCount = 2;
		swapchain_create_info_khr.pQueueFamilyIndices = queue_family_indices;
	}
	else
	{
		swapchain_create_info_khr.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapchain_create_info_khr.queueFamilyIndexCount = 0; // Optional
		swapchain_create_info_khr.pQueueFamilyIndices = nullptr; // Optional
	}

	swapchain_create_info_khr.preTransform = swap_chain_support.capabilities.currentTransform;
	swapchain_create_info_khr.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchain_create_info_khr.presentMode = m_present_mode;
	swapchain_create_info_khr.clipped = VK_TRUE;
	swapchain_create_info_khr.oldSwapchain = VK_NULL_HANDLE;

	if (vk::create_swapchain_khr(*window->get_device(), &swapchain_create_info_khr, nullptr, &m_swapchain) != VK_SUCCESS)
		cap::logger::fatal("failed to create swap chain");

	m_frames.resize(image_count);

	std::vector<vk::image> swapchain_images(image_count);
	vk::get_swapchain_images_khr(*window->get_device(), m_swapchain, &image_count, swapchain_images.data());

	m_image_format = m_surface_format.format;
	m_extent = extent;

	// Initialize image views.

	for (u8 i = 0; i < m_frames.size(); i++)
	{
		m_frames[i].frame_image = swapchain_images[i];

		m_frames[i].frame_view = rhr::render::tools::create_image_view(
			m_frames[i].frame_image,
			m_image_format,
			VK_IMAGE_ASPECT_COLOR_BIT
		);
	}
}

rhr::render::component::swapchain::~swapchain()
{
	for (auto& frame : m_frames)
	{
		vkDestroyFramebuffer(*rhr::render::renderer::get_window_primary()->get_device(), frame.frame_buffer, nullptr);
		vkDestroyImageView(*rhr::render::renderer::get_window_primary()->get_device(), frame.frame_view, nullptr);
	}

	vkDestroySwapchainKHR(*rhr::render::renderer::get_window_primary()->get_device(), m_swapchain, nullptr);
}

void rhr::render::component::swapchain::create_render_passes()
{
	m_render_passes.clear();
	m_render_passes.reserve(1);
	rhr::render::component::pass& render_pass_master = m_render_passes.emplace_back();
}

void rhr::render::component::swapchain::create_framebuffers()
{
	std::unique_ptr<rhr::render::component::window>& window = rhr::render::renderer::get_window_primary();

	for (auto& frame : m_frames)
	{
		std::array<vk::image_view, 1> attachments = {
			frame.frame_view
		};

		vk::framebuffer_create_info framebuffer_create_info{};
		framebuffer_create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebuffer_create_info.renderPass = *window->get_render_pass(0);
		framebuffer_create_info.attachmentCount = static_cast<u32>(attachments.size());
		framebuffer_create_info.pAttachments = attachments.data();
		framebuffer_create_info.width = m_extent.width;
		framebuffer_create_info.height = m_extent.height;
		framebuffer_create_info.layers = 1;

		if (vk::create_framebuffer(*window->get_device(), &framebuffer_create_info, nullptr, &frame.frame_buffer) != VK_SUCCESS)
			cap::logger::fatal("failed to create frame buffers");
	}
}

void rhr::render::component::swapchain::create_synchronization()
{
	std::unique_ptr<rhr::render::component::window>& window = rhr::render::renderer::get_window_primary();

	vk::semaphore_create_info semaphore_create_info{};
	semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	vk::fence_create_info fence_create_info{};
	fence_create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fence_create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (auto& frame : m_frames)
	{
		if (vk::create_semaphore(*window->get_device(), &semaphore_create_info, nullptr, &frame.semaphore_image) != VK_SUCCESS ||
			vk::create_semaphore(*window->get_device(), &semaphore_create_info, nullptr, &frame.semaphore_finished) != VK_SUCCESS ||
			vk::create_fence(*window->get_device(), &fence_create_info, nullptr, &frame.fence_in_flight) != VK_SUCCESS)
			cap::logger::fatal("failed to create semaphores");
	}
}

vk::format& rhr::render::component::swapchain::get_format()
{
	return m_image_format;
}

vk::surface_format_khr& rhr::render::component::swapchain::get_surface_format()
{
	return m_surface_format;
}

vk::present_mode_khr& rhr::render::component::swapchain::get_present_mode()
{
	return m_present_mode;
}

vk::extent_2d& rhr::render::component::swapchain::get_extent()
{
	return m_extent;
}

vk::render_pass& rhr::render::component::swapchain::get_render_pass(u8 idx)
{
	if (idx >= static_cast<u8>(m_render_passes.size()))
	{
		if (!m_render_passes.empty())
		{
			cap::logger::error("failed to fetch render pass using idx, using first one instead");
			return m_render_passes.front().get_render_pass();
		}
		else
			cap::logger::fatal("failed to fetch render pass, none registered");
	}

	return m_render_passes[static_cast<usize>(idx)].get_render_pass();
}

u8 rhr::render::component::swapchain::get_framebuffer_count()
{
	return static_cast<u8>(m_frames.size());
}

vk::frame_buffer& rhr::render::component::swapchain::get_framebuffer(u8 idx)
{
	if (idx >= static_cast<u8>(m_frames.size()))
	{
		if (!m_frames.empty())
		{
			cap::logger::error("failed to fetch framebuffer using idx, using first one instead");
			return m_frames.front().frame_buffer;
		}
		else
			cap::logger::fatal("failed to fetch framebuffer, none registered");
	}

	return m_frames[static_cast<usize>(idx)].frame_buffer;
}

vk::swapchain_khr& rhr::render::component::swapchain::get_swapchain()
{
	return m_swapchain;
}

rhr::render::component::frame& rhr::render::component::swapchain::get_frame(u8 idx)
{
	if (idx >= static_cast<u8>(m_frames.size()))
	{
		if (!m_frames.empty())
		{
			cap::logger::error("failed to fetch frame using idx, using first one instead");
			return m_frames.front();
		}
		else
			cap::logger::fatal("failed to fetch frame, none registered");
	}

	return m_frames[static_cast<usize>(idx)];
}
