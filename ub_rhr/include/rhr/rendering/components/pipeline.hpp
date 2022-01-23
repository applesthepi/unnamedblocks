#pragma once
#include "config.h"

#include <cappuccino/utils.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

namespace rhr::render::component
{
///
class pipeline
{
public:
	pipeline();
	~pipeline();

	///
	void apply_active_pipeline(const std::string& name);

	///
	void bind_color_pipeline(vk::descriptor_set* descriptor_set);

	///
	void bind_texture_pipeline(vk::descriptor_set* descriptor_set);

	///
	vk::pipeline& get_color_pipeline(const std::string& name);

	///
	vk::pipeline& get_texture_pipeline(const std::string& name);

	///
	void
	register_paired_pipeline(const std::string& name, const std::string& shader_color, const std::string& shader_texture, vk::cull_mode_flags cull_mode_flags = VK_CULL_MODE_NONE);

private:
	///
	void create_pipeline(const std::string& name, const std::string& shader, vk::cull_mode_flags cull_mode_flags);

	///
	vk::pipeline m_active_pipeline_color;

	///
	vk::pipeline_layout m_active_pipeline_color_layout;

	///
	vk::pipeline m_active_pipeline_texture;

	///
	vk::pipeline_layout m_active_pipeline_texture_layout;

	///
	std::unordered_map<std::string, vk::pipeline> m_registered_pipelines;

	///
	std::unordered_map<std::string, vk::pipeline_layout> m_registered_layouts;
};
} // namespace rhr::render::component