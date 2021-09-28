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
	static void init_pipelines();

	///
	static void create_pipeline(const std::string& shader, VkPipeline* pipeline, VkPipelineLayout* layout);

	///
	static VkPipeline ui_pipeline;

	///
	static VkPipeline ui_texture_pipeline;

	///
	static VkPipelineLayout ui_pipeline_layout;

	///
	static VkPipelineLayout ui_texture_pipeline_layout;
};
}