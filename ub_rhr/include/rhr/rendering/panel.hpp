#pragma once
#include "config.h"

#include <cappuccino/utils.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

namespace rhr::render
{
///
class panel
{
public:
	///
	struct data
	{
		///
		bool basic_imgui;

		///
		std::string id;

		///
		std::function<void(panel::data&)> function_render;

		///
		std::function<void(panel::data&)> function_render_master;

		///
		std::function<void(panel::data&)> function_update_size;

		///
		std::function<void(panel::data&)> function_update_position;

		///
		std::function<void(panel::data&)> function_imgui;

		///
		VkFramebuffer frame_buffer;

		///
		VkRenderPass render_pass;

		///
		VkSampler sampler;

		///
		VkDescriptorImageInfo descriptor;

		///
		VkImage color_image;

		///
		VkImageView color_image_view;

		///
		VkDeviceMemory color_device_memory;

		///
		VkImage depth_image;

		///
		VkImageView depth_image_view;

		///
		VkDeviceMemory depth_device_memory;

		///
		ImTextureID descriptor_set;

		///
		glm::vec<2, i32> panel_last_position;

		///
		glm::vec<2, i32> panel_last_size;
	};

	///
	static void create_panel(
		const std::string& id,
		const std::function<void(panel::data&)>& function_render,
		const std::function<void(panel::data&)>& function_render_master,
		const std::function<void(panel::data&)>& function_update_position,
		const std::function<void(panel::data&)>& function_update_size);

	///
	static void create_panel(const std::string& id, const std::function<void(panel::data&)>& function_imgui);

	///
	static void run_imgui();

	///
	static void run_master_render_pass();

	///
	static void initialize_panels();

	///
	static std::vector<data> panels;
};
} // namespace rhr::render