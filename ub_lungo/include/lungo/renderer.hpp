#pragma once
#include "config.h"

#include "lungo/components/window.hpp"
#include "lungo/interfaces/i_renderable.hpp"
#include "lungo/interfaces/i_ui.hpp"
#include "lungo/objects/object.hpp"

#include <latte/utils.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>
#include <vk_mem_alloc.h>

namespace rhr::render
{
///
class renderer
{
public:
	///
	struct frame_buffer_attachment
	{
		VkImage image;
		VkDeviceMemory mem;
		VkImageView view;
	};

	///
	struct offscreen_pass
	{
		VkFramebuffer frame_buffer;
		frame_buffer_attachment color, depth;
		VkRenderPass render_pass;
		VkSampler sampler;
		VkDescriptorImageInfo descriptor;
	};

	///
	struct imgui_data
	{
		imgui_data(ImGuiIO& a_io, ImGuiStyle& a_style)
			: io(a_io)
			, style(a_style)
		{}

		ImGuiIO& io;
		ImGuiStyle& style;
		ImGui_ImplVulkanH_Window data;
		ImGui_ImplVulkanH_Frame* frames;
		ImGui_ImplVulkanH_FrameSemaphores* semaphores;
	};

	///
	static renderer* get();

	///
	static void set(renderer* reg);

	///
	void initialize_window();

	///
	void initialize();

	///
	std::unique_ptr<rhr::render::component::window>& get_window_primary();

	///
	void reload_swapchain();

	///
	void initialize_imgui(bool first_time);

	///
	void add_dirty(std::weak_ptr<rhr::render::interfaces::i_renderable> renderable);

	///
	void add_dirty(std::weak_ptr<rhr::render::interfaces::i_ui> ui);

	///
	void process_dirty();

	///
	void render_pass_setup();

	///
	void render_pass_master();

	///
	void frame_present();

	///
	glm::mat4 view_matrix;

	///
	glm::mat4 projection_matrix;

	///
	glm::mat4 ui_projection_matrix;

	///
	static u32 depth_background;
	static u32 depth_plane;
	static u32 depth_frame_background;
	static u32 depth_collection;
	static u32 depth_stack;
	static u32 depth_block;
	static u32 depth_argument;
	static u32 depth_argument_text;
	static u32 depth_cursor;
	static u32 depth_ui_background;
	static u32 depth_ui_text;

	///
	imgui_data* imgui_local;

	///
	ImDrawData* imgui_draw_data;

	///
	VmaAllocator vma_allocator;

	///
#ifdef NDEBUG
	static const bool enable_validation_layers = false;
#else
	static const bool enable_validation_layers = true;
#endif
private:
	///
	static renderer* m_renderer;

	///
	std::unique_ptr<rhr::render::component::window> m_window_primary;

	///
	std::vector<std::weak_ptr<rhr::render::interfaces::i_renderable>> m_dirty_renderable;

	///
	std::vector<std::weak_ptr<rhr::render::interfaces::i_ui>> m_dirty_ui;

	///
	std::shared_mutex m_dirty_mutex;
};
} // namespace rhr::render