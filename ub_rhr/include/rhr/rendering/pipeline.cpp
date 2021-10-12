#include "pipeline.hpp"

#include "rhr/rendering/swap_chain.hpp"
#include "rhr/rendering/render_pass.hpp"
#include "rhr/rendering/tools.hpp"
#include "rhr/rendering/renderer.hpp"
#include "rhr/rendering/command.hpp"
#include "rhr/rendering/device.hpp"

void rhr::render::pipeline::initialize()
{
	m_registered_pipelines.clear();
	m_registered_layouts.clear();

	register_("master", "ui", "ui_texture", &rhr::render::render_pass::render_pass_master);
}

void rhr::render::pipeline::register_(const std::string& name, const std::string& shader_color, const std::string& shader_texture, VkRenderPass* render_pass, VkCullModeFlags cull_flags)
{
	create_pipeline("c_" + name, shader_color, render_pass, cull_flags);
	create_pipeline("t_" + name, shader_texture, render_pass, cull_flags);
}

void rhr::render::pipeline::apply_active(const std::string& name)
{
	 m_active_pipeline_color = m_registered_pipelines["c_" + name];
	 m_active_pipeline_color_layout = m_registered_layouts["c_" + name];

	 m_active_pipeline_texture = m_registered_pipelines["t_" + name];
	 m_active_pipeline_texture_layout = m_registered_layouts["t_" + name];
}

void rhr::render::pipeline::bind_color(vk::descriptor_set* descriptor_set)
{
	vkCmdBindDescriptorSets(
		*rhr::render::command::active_command_buffer,
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		m_active_pipeline_color_layout,
		0,
		1,
		descriptor_set,
		0,
		nullptr
	);

	vkCmdBindPipeline(
		*rhr::render::command::active_command_buffer,
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		m_active_pipeline_color
	);
}

void rhr::render::pipeline::bind_texture(vk::descriptor_set* descriptor_set)
{
	vkCmdBindDescriptorSets(
		*rhr::render::command::active_command_buffer,
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		m_active_pipeline_texture_layout,
		0,
		1,
		descriptor_set,
		0,
		nullptr
	);

	vkCmdBindPipeline(
		*rhr::render::command::active_command_buffer,
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		m_active_pipeline_texture
	);
}

void rhr::render::pipeline::create_pipeline(const std::string& name, const std::string& shader, VkRenderPass* render_pass, VkCullModeFlags cull_flags)
{
	auto vert_shader_code = rhr::render::tools::read_file_bytes("res/shaders/" + shader + ".vert.spv");
	auto frag_shader_code = rhr::render::tools::read_file_bytes("res/shaders/" + shader + ".frag.spv");

	VkShaderModule vert_shader_module = rhr::render::tools::create_shader_module(*vert_shader_code, &rhr::render::device::device_master);
	VkShaderModule frag_shader_module = rhr::render::tools::create_shader_module(*frag_shader_code, &rhr::render::device::device_master);

	VkPipelineShaderStageCreateInfo vert_shader_stage_info{};
	vert_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vert_shader_stage_info.stage = VK_SHADER_STAGE_VERTEX_BIT;

	vert_shader_stage_info.module = vert_shader_module;
	vert_shader_stage_info.pName = "main";

	VkPipelineShaderStageCreateInfo frag_shader_stage_info{};
	frag_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	frag_shader_stage_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	frag_shader_stage_info.module = frag_shader_module;
	frag_shader_stage_info.pName = "main";

	VkPipelineShaderStageCreateInfo shader_stages[] = { vert_shader_stage_info, frag_shader_stage_info };

	auto binding_description = rhr::render::vertex::get_binding_description();
	auto attribute_descriptions = rhr::render::vertex::get_attribute_description();

	VkPipelineVertexInputStateCreateInfo vertex_input_info{};
	vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertex_input_info.vertexBindingDescriptionCount = 1;
	vertex_input_info.vertexAttributeDescriptionCount = static_cast<u32>(attribute_descriptions.size());
	vertex_input_info.pVertexBindingDescriptions = &binding_description;
	vertex_input_info.pVertexAttributeDescriptions = attribute_descriptions.data();

	VkPipelineInputAssemblyStateCreateInfo input_assembly{};
	input_assembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	input_assembly.primitiveRestartEnable = VK_FALSE;

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<f32>(rhr::render::swap_chain::swap_chain_extent.width);
	viewport.height = static_cast<f32>(rhr::render::swap_chain::swap_chain_extent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = rhr::render::swap_chain::swap_chain_extent;

	VkPipelineViewportStateCreateInfo viewport_state{};
	viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewport_state.viewportCount = 1;
	viewport_state.pViewports = &viewport;
	viewport_state.scissorCount = 1;
	viewport_state.pScissors = &scissor;

	VkPipelineRasterizationStateCreateInfo rasterizer{};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = cull_flags;
	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;

	rasterizer.depthBiasEnable = VK_FALSE;
	rasterizer.depthBiasConstantFactor = 0.0f; // Optional
	rasterizer.depthBiasClamp = 0.0f; // Optional
	rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

	VkPipelineMultisampleStateCreateInfo multisampling = {};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	VkPipelineColorBlendAttachmentState color_blend_attachment{};
	color_blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	color_blend_attachment.blendEnable = VK_TRUE;

	color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	color_blend_attachment.colorBlendOp = VK_BLEND_OP_ADD;

	color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	color_blend_attachment.alphaBlendOp = VK_BLEND_OP_ADD;

	VkPipelineColorBlendStateCreateInfo color_blending{};
	color_blending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
//	color_blending.logicOpEnable = VK_FALSE;
//	color_blending.logicOp = VK_LOGIC_OP_COPY; // Optional
	color_blending.attachmentCount = 1;
	color_blending.pAttachments = &color_blend_attachment;
//	color_blending.blendConstants[0] = 0.0f; // Optional
//	color_blending.blendConstants[1] = 0.0f; // Optional
//	color_blending.blendConstants[2] = 0.0f; // Optional
//	color_blending.blendConstants[3] = 0.0f; // Optional

	VkDynamicState dynamic_states[] = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_LINE_WIDTH
	};

	VkPipelineDynamicStateCreateInfo dynamic_state{};
	dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamic_state.dynamicStateCount = 2;
	dynamic_state.pDynamicStates = dynamic_states;

	VkPipelineLayoutCreateInfo pipeline_layout_info{};
	pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipeline_layout_info.setLayoutCount = 1;
	pipeline_layout_info.pSetLayouts = &rhr::render::command::descriptor_set_layout;
	pipeline_layout_info.pushConstantRangeCount = 0; // Optional
	pipeline_layout_info.pPushConstantRanges = nullptr; // Optional

	VkPipelineLayout pipeline_layout;

	if (vkCreatePipelineLayout(rhr::render::device::device_master, &pipeline_layout_info, nullptr, &pipeline_layout) != VK_SUCCESS)
		cap::logger::fatal("failed to create pipeline layout");
	//	VkPipelineDepthStencilStateCreateInfo depth_stencil{};
	//	depth_stencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	//	depth_stencil.depthTestEnable = VK_TRUE;
	//	depth_stencil.depthWriteEnable = VK_TRUE;
	//	depth_stencil.depthCompareOp = VK_COMPARE_OP_LESS;
	//	depth_stencil.depthBoundsTestEnable = VK_FALSE;
	//	depth_stencil.minDepthBounds = 0.0f; // Optional
	//	depth_stencil.maxDepthBounds = 1.0f; // Optional
	//	depth_stencil.stencilTestEnable = VK_FALSE;
	//	depth_stencil.front = {}; // Optional
	//	depth_stencil.back = {}; // Optional

	VkGraphicsPipelineCreateInfo pipeline_info{};
	pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipeline_info.stageCount = 2;
	pipeline_info.pStages = shader_stages;
	pipeline_info.pVertexInputState = &vertex_input_info;
	pipeline_info.pInputAssemblyState = &input_assembly;
	pipeline_info.pViewportState = &viewport_state;
	pipeline_info.pRasterizationState = &rasterizer;
	pipeline_info.pMultisampleState = &multisampling;
	pipeline_info.pDepthStencilState = nullptr;
	//	pipeline_info.pDepthStencilState = &depth_stencil;
	pipeline_info.pColorBlendState = &color_blending;
	pipeline_info.pDynamicState = nullptr; // Optional

	pipeline_info.layout = pipeline_layout;
	pipeline_info.renderPass = *render_pass;
	pipeline_info.subpass = 0;
	pipeline_info.basePipelineHandle = VK_NULL_HANDLE; // Optional
	pipeline_info.basePipelineIndex = -1; // Optional

	VkPipeline pipeline;

	if (vkCreateGraphicsPipelines(rhr::render::device::device_master, VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &pipeline) != VK_SUCCESS)
		cap::logger::fatal("failed to create graphics pipeline");

	m_registered_pipelines[name] = pipeline;
	m_registered_layouts[name] = pipeline_layout;
}

VkPipeline rhr::render::pipeline::get_color(const std::string& name)
{
	return m_registered_pipelines["c_" + name];
}

VkPipeline rhr::render::pipeline::get_texture(const std::string& name)
{
	return m_registered_pipelines["t_" + name];
}

VkPipeline rhr::render::pipeline::m_active_pipeline_color;
VkPipelineLayout rhr::render::pipeline::m_active_pipeline_color_layout;

VkPipeline rhr::render::pipeline::m_active_pipeline_texture;
VkPipelineLayout rhr::render::pipeline::m_active_pipeline_texture_layout;

std::unordered_map<std::string, VkPipeline> rhr::render::pipeline::m_registered_pipelines;
std::unordered_map<std::string, VkPipelineLayout> rhr::render::pipeline::m_registered_layouts;
