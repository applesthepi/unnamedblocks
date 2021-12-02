#include "swap_chain.hpp"

#include "rhr/rendering/tools.hpp"
#include "rhr/rendering/renderer.hpp"
#include "rhr/rendering/render_pass.hpp"
#include "rhr/rendering/device.hpp"

void rhr::render::swap_chain::init_swap_chain()
{

}

void rhr::render::swap_chain::init_image_views()
{
	rhr::render::swap_chain::swap_chain_image_views.resize(rhr::render::swap_chain::swap_chain_images.size());

	for (u32 i = 0; i < rhr::render::swap_chain::swap_chain_images.size(); i++)
		rhr::render::swap_chain::swap_chain_image_views[i] = rhr::render::tools::create_image_view(
			rhr::render::swap_chain::swap_chain_images[i],
			rhr::render::swap_chain::swap_chain_image_format,
			VK_IMAGE_ASPECT_COLOR_BIT
		);
}

void rhr::render::swap_chain::init_frame_buffers()
{
	swap_chain_frame_buffers.resize(swap_chain_image_views.size());

	for (usize i = 0; i < swap_chain_image_views.size(); i++)
	{
		std::array<VkImageView, 1> attachments = {
			swap_chain_image_views[i]
		};

//		std::array<VkImageView, 2> attachments = {
//			swap_chain_image_views[i],
//			rhr::render::renderer::depth_image_view
//		};

		VkFramebufferCreateInfo frame_buffer_info{};
		frame_buffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		frame_buffer_info.renderPass = rhr::render::render_pass::render_pass_master;
		frame_buffer_info.attachmentCount = static_cast<u32>(attachments.size());
		frame_buffer_info.pAttachments = attachments.data();
		frame_buffer_info.width = swap_chain_extent.width;
		frame_buffer_info.height = swap_chain_extent.height;
		frame_buffer_info.layers = 1;

		if (vkCreateFramebuffer(rhr::render::device::device_master, &frame_buffer_info, nullptr, &swap_chain_frame_buffers[i]) != VK_SUCCESS)
			cap::logger::fatal("failed to create frame buffers");
	}
}

void rhr::render::swap_chain::init_sync_objects()
{
	rhr::render::swap_chain::render_finished_semaphores.clear();

	/*for (usize i = 0; i < rhr::render::swap_chain::render_finished_semaphores.size(); i++)
	{
		vkDestroySemaphore(rhr::render::device::device_master, render_finished_semaphores[i], nullptr);
		vkDestroySemaphore(rhr::render::device::device_master, image_available_semaphores[i], nullptr);
		vkDestroyFence(rhr::render::device::device_master, in_flight_fences[i], nullptr);
	}*/

	image_available_semaphores.clear();
	render_finished_semaphores.clear();
	in_flight_fences.clear();
	images_in_flight.clear();

	image_available_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
	render_finished_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
	in_flight_fences.resize(MAX_FRAMES_IN_FLIGHT);
	images_in_flight.resize(swap_chain_images.size(), VK_NULL_HANDLE);

	VkSemaphoreCreateInfo semaphore_info{};
	semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fence_info{};
	fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (usize i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		if (vkCreateSemaphore(rhr::render::device::device_master, &semaphore_info, nullptr, &image_available_semaphores[i]) != VK_SUCCESS ||
			vkCreateSemaphore(rhr::render::device::device_master, &semaphore_info, nullptr, &render_finished_semaphores[i]) != VK_SUCCESS ||
			vkCreateFence(rhr::render::device::device_master, &fence_info, nullptr, &in_flight_fences[i]) != VK_SUCCESS)
			cap::logger::fatal("failed to create semaphores");
	}
}

std::vector<VkSemaphore> rhr::render::swap_chain::image_available_semaphores;
std::vector<VkSemaphore> rhr::render::swap_chain::render_finished_semaphores;
std::vector<VkFence> rhr::render::swap_chain::in_flight_fences;
std::vector<VkFence> rhr::render::swap_chain::images_in_flight;
std::vector<VkImage> rhr::render::swap_chain::swap_chain_images;
std::vector<VkImageView> rhr::render::swap_chain::swap_chain_image_views;
VkFormat rhr::render::swap_chain::swap_chain_image_format;
VkExtent2D rhr::render::swap_chain::swap_chain_extent;
std::vector<VkFramebuffer> rhr::render::swap_chain::swap_chain_frame_buffers;
VkSwapchainKHR rhr::render::swap_chain::swapchain_khr;