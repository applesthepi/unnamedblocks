#pragma once
#include "config.h"

#include <cappuccino/utils.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

namespace rhr::render
{
///
class swap_chain
{
public:
	///
	static void init_swap_chain();

	///
	static void init_image_views();

	///
	static void init_frame_buffers();

	///
	static void init_sync_objects();

	///
	static std::vector<VkSemaphore> image_available_semaphores;

	///
	static std::vector<VkSemaphore> render_finished_semaphores;

	///
	static std::vector<VkFence> in_flight_fences;

	///
	static std::vector<VkFence> images_in_flight;

	///
	static std::vector<VkImage> swap_chain_images;

	///
	static std::vector<VkImageView> swap_chain_image_views;

	///
	static VkFormat swap_chain_image_format;

	///
	static VkExtent2D swap_chain_extent;

	///
	static std::vector<VkFramebuffer> swap_chain_frame_buffers;

	///
	static VkSwapchainKHR swapchain_khr;
};
}