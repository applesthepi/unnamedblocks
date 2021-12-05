#pragma once
#include "config.h"

#include "rhr/rendering/objects/object.hpp"
#include "rhr/rendering/interfaces/i_renderable.hpp"
#include "rhr/rendering/interfaces/i_ui.hpp"
#include "rhr/rendering/components/window.hpp"

#include <cappuccino/utils.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

namespace rhr::render
{
///
class renderer
{
public:
	///
	struct frame_buffer_attachment {
		VkImage image;
		VkDeviceMemory mem;
		VkImageView view;
	};

	///
	struct offscreen_pass {
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
		, style(a_style) {}

		ImGuiIO& io;
		ImGuiStyle& style;
		ImGui_ImplVulkanH_Window data;
		ImGui_ImplVulkanH_Frame* frames;
		ImGui_ImplVulkanH_FrameSemaphores* semaphores;
	};

	///
	static void initialize_window();

	///
	static void initialize();

	///
	static std::unique_ptr<rhr::render::component::window>& get_window_primary();

	///
	static void reload_swapchain();

	///
	static void initialize_imgui(bool first_time);

	///
	static void add_dirty(std::weak_ptr<rhr::render::interfaces::i_renderable> renderable);

	///
	static void add_dirty(std::weak_ptr<rhr::render::interfaces::i_ui> ui);

	///
	static void process_dirty();

	///
	static void render_pass_setup();

	///
	static void render_pass_master();

	///
	static void frame_present();

	///
	static glm::mat4 view_matrix;

	///
	static glm::mat4 projection_matrix;

	///
	static glm::mat4 ui_projection_matrix;

	///
	static u32 depth_background;
	static u32 depth_plane;
	static u32 depth_frame_background;
	static u32 depth_collection;
	static u32 depth_stack;
	static u32 depth_block;
	static u32 depth_argument;
	static u32 depth_argument_text;
	static u32 depth_ui_background;
	static u32 depth_ui_text;

	///
	static imgui_data* imgui_local;

	///
	static ImDrawData* imgui_draw_data;

	///
#ifdef NDEBUG
	static const bool enable_validation_layers = false;
#else
	static const bool enable_validation_layers = true;
#endif
private:
	///
	static std::unique_ptr<rhr::render::component::window> m_window_primary;

	///
	static std::vector<std::weak_ptr<rhr::render::interfaces::i_renderable>> m_dirty_renderable;

	///
	static std::vector<std::weak_ptr<rhr::render::interfaces::i_ui>> m_dirty_ui;

	///
	static std::shared_mutex m_dirty_mutex;
};
}