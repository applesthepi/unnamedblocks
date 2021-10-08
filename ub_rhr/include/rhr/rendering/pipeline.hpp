#pragma once
#include "config.h"

#include <cappuccino/utils.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

namespace rhr::render
{
///
class pipeline
{
public:
	///
	static void initialize();

	///
	static void register_(const std::string& name, const std::string& shader_color, const std::string& shader_texture, VkRenderPass* render_pass, VkCullModeFlags cull_flags = VK_CULL_MODE_NONE);

	///
	static void apply_active(const std::string& name);

	///
	static void bind_color(vk::descriptor_set* descriptor_set);

	///
	static void bind_texture(vk::descriptor_set* descriptor_set);

	///
	static VkPipeline get_color(const std::string& name);

	///
	static VkPipeline get_texture(const std::string& name);
private:
	///
	static void create_pipeline(const std::string& name, const std::string& shader, VkRenderPass* render_pass, VkCullModeFlags cull_flags);

	///
	static VkPipeline m_active_pipeline_color;

	///
	static VkPipelineLayout m_active_pipeline_color_layout;

	///
	static VkPipeline m_active_pipeline_texture;

	///
	static VkPipelineLayout m_active_pipeline_texture_layout;

	///
	static std::unordered_map<std::string, VkPipeline> m_registered_pipelines;

	///
	static std::unordered_map<std::string, VkPipelineLayout> m_registered_layouts;
};
}