#pragma once
#include "config.h"

#include <latte/utils.hpp>

namespace lungo::handler
{
// TODO: make panels override methods instead of passing functionals.
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
		std::function<void(panel::data&, double delta_time)> function_frame_update;

		///
		std::function<void(panel::data&)> function_render;

		///
		std::function<void(panel::data&)> function_reload_swap_chain;

		///
		std::function<void(panel::data&)> function_update_buffers;

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
	static panel* get();

	///
	static void set(panel* pan);

	///
	void initialize();

	///
	void create_panel(
		const std::string& id,
		const std::function<void(panel::data&, double delta_time)>& function_frame_update,
		const std::function<void(panel::data&)>& function_render,
		const std::function<void(panel::data&)>& function_reload_swap_chain,
		const std::function<void(panel::data&)>& function_update_buffers,
		const std::function<void(panel::data&)>& function_render_master,
		const std::function<void(panel::data&)>& function_update_position,
		const std::function<void(panel::data&)>& function_update_size);

	///
	void create_panel(const std::string& id, const std::function<void(panel::data&)>& function_imgui);

	///
	void run_frame_update(double delta_time);

	/// Either uses imgui for the master render pass or starts a custom render pass.
	void run_imgui();

	///
	void run_reload_swap_chain();

	///
	void run_update_buffers();

	///
	void run_master_render_pass();

	///
	std::vector<data> panels;
private:
	///
	void create_panel_render_pass(data& local_data);

	///
	static panel* m_panel;
};
}