#include "pipeline.hpp"

#include "lungo/renderer.hpp"
#include "lungo/tools.hpp"

rhr::render::component::pipeline::pipeline()
	: m_active_pipeline_color {}
	, m_active_pipeline_color_layout {}
	, m_active_pipeline_texture {}
	, m_active_pipeline_texture_layout {}
{
	register_paired_pipeline("master", "ui", "ui_texture");
}

rhr::render::component::pipeline::~pipeline()
{
	for (auto& registered_pipeline : m_registered_pipelines)
		vkDestroyPipeline(
			*rhr::render::renderer::get_window_primary()->get_device(), registered_pipeline.second, nullptr);

	for (auto& registered_layout : m_registered_layouts)
		vkDestroyPipelineLayout(
			*rhr::render::renderer::get_window_primary()->get_device(), registered_layout.second, nullptr);
}

void rhr::render::component::pipeline::apply_active_pipeline(const std::string& name)
{
	m_active_pipeline_color		   = m_registered_pipelines["c_" + name];
	m_active_pipeline_color_layout = m_registered_layouts["c_" + name];

	m_active_pipeline_texture		 = m_registered_pipelines["t_" + name];
	m_active_pipeline_texture_layout = m_registered_layouts["t_" + name];
}

void rhr::render::component::pipeline::bind_color_pipeline(vk::descriptor_set* descriptor_set)
{
	std::unique_ptr<rhr::render::component::window>& window = rhr::render::renderer::get_window_primary();

	vkCmdBindDescriptorSets(
		*window->get_active_command_buffer(),
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		m_active_pipeline_color_layout,
		0,
		1,
		descriptor_set,
		0,
		nullptr);

	vkCmdBindPipeline(*window->get_active_command_buffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_active_pipeline_color);
}

void rhr::render::component::pipeline::bind_texture_pipeline(vk::descriptor_set* descriptor_set)
{
	std::unique_ptr<rhr::render::component::window>& window = rhr::render::renderer::get_window_primary();

	vkCmdBindDescriptorSets(
		*window->get_active_command_buffer(),
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		m_active_pipeline_texture_layout,
		0,
		1,
		descriptor_set,
		0,
		nullptr);

	vkCmdBindPipeline(*window->get_active_command_buffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_active_pipeline_texture);
}

vk::pipeline& rhr::render::component::pipeline::get_color_pipeline(const std::string& name)
{
	return m_registered_pipelines["c_" + name];
}

vk::pipeline& rhr::render::component::pipeline::get_texture_pipeline(const std::string& name)
{
	return m_registered_pipelines["t_" + name];
}

void rhr::render::component::pipeline::register_paired_pipeline(
	const std::string& name,
	const std::string& shader_color,
	const std::string& shader_texture,
	vk::cull_mode_flags cull_mode_flags)
{
	create_pipeline("c_" + name, shader_color, cull_mode_flags);
	create_pipeline("t_" + name, shader_texture, cull_mode_flags);
}

void rhr::render::component::pipeline::create_pipeline(
	const std::string& name, const std::string& shader, vk::cull_mode_flags cull_mode_flags)
{
	std::unique_ptr<rhr::render::component::window>& window = rhr::render::renderer::get_window_primary();

	auto vert_shader_code = rhr::render::tools::read_file_bytes("res/shaders/" + shader + ".vert.spv");
	auto frag_shader_code = rhr::render::tools::read_file_bytes("res/shaders/" + shader + ".frag.spv");

	vk::shader_module vert_shader_module =
		rhr::render::tools::create_shader_module(*vert_shader_code, window->get_device());
	vk::shader_module frag_shader_module =
		rhr::render::tools::create_shader_module(*frag_shader_code, window->get_device());

	vk::pipeline_shader_stage_create_info vert_pipeline_shader_stage_create_info		= {};
	vk::pipeline_shader_stage_create_info frag_pipeline_shader_stage_create_info		= {};
	vk::pipeline_vertex_input_state_create_info pipeline_vertex_input_state_create_info = {};
	vk::pipeline_input_assembly_state_create_info input_assembly						= {};
	vk::viewport viewport																= {};
	vk::rect_2d scissor																	= {};
	vk::pipeline_viewport_state_create_info viewport_state								= {};
	vk::pipeline_rasterization_state_create_info rasterizer								= {};
	vk::pipeline_multisample_state_create_info multisampling							= {};
	vk::pipeline_color_blend_attachment_state color_blend_attachment					= {};
	vk::pipeline_color_blend_state_create_info color_blending							= {};
	vk::pipeline_dynamic_state_create_info dynamic_state								= {};
	vk::pipeline_layout_create_info pipeline_layout_info								= {};
	vk::pipeline_layout pipeline_layout													= {};
	vk::graphics_pipeline_create_info pipeline_info										= {};

	vk::dynamic_state dynamic_states[] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_LINE_WIDTH};

	vert_pipeline_shader_stage_create_info.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vert_pipeline_shader_stage_create_info.stage  = VK_SHADER_STAGE_VERTEX_BIT;
	vert_pipeline_shader_stage_create_info.module = vert_shader_module;
	vert_pipeline_shader_stage_create_info.pName  = "main";

	frag_pipeline_shader_stage_create_info.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	frag_pipeline_shader_stage_create_info.stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
	frag_pipeline_shader_stage_create_info.module = frag_shader_module;
	frag_pipeline_shader_stage_create_info.pName  = "main";

	std::array<vk::pipeline_shader_stage_create_info, 2> shader_stages = {
		vert_pipeline_shader_stage_create_info, frag_pipeline_shader_stage_create_info};

	auto binding_description	= rhr::render::vertex::get_binding_description();
	auto attribute_descriptions = rhr::render::vertex::get_attribute_description();

	pipeline_vertex_input_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	pipeline_vertex_input_state_create_info.vertexBindingDescriptionCount = 1;
	pipeline_vertex_input_state_create_info.vertexAttributeDescriptionCount =
		static_cast<u32>(attribute_descriptions.size());
	pipeline_vertex_input_state_create_info.pVertexBindingDescriptions	 = &binding_description;
	pipeline_vertex_input_state_create_info.pVertexAttributeDescriptions = attribute_descriptions.data();

	input_assembly.sType				  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	input_assembly.topology				  = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	input_assembly.primitiveRestartEnable = VK_FALSE;

	viewport.x		  = 0.0f;
	viewport.y		  = 0.0f;
	viewport.width	  = static_cast<f32>(window->get_swapchain_extent()->width);
	viewport.height	  = static_cast<f32>(window->get_swapchain_extent()->height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	scissor.offset = {0, 0};
	scissor.extent = *window->get_swapchain_extent();

	viewport_state.sType		 = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewport_state.viewportCount = 1;
	viewport_state.pViewports	 = &viewport;
	viewport_state.scissorCount	 = 1;
	viewport_state.pScissors	 = &scissor;

	rasterizer.sType				   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable		   = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode			   = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth			   = 1.0f;
	rasterizer.cullMode				   = cull_mode_flags;
	rasterizer.frontFace			   = VK_FRONT_FACE_COUNTER_CLOCKWISE;

	rasterizer.depthBiasEnable		   = VK_FALSE;
	rasterizer.depthBiasConstantFactor = 0.0f; // Optional
	rasterizer.depthBiasClamp		   = 0.0f; // Optional
	rasterizer.depthBiasSlopeFactor	   = 0.0f; // Optional

	multisampling.sType				   = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable  = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	color_blend_attachment.colorWriteMask =
		VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	color_blend_attachment.blendEnable = VK_TRUE;

	color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	color_blend_attachment.colorBlendOp		   = VK_BLEND_OP_ADD;

	color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	color_blend_attachment.alphaBlendOp		   = VK_BLEND_OP_ADD;

	color_blending.sType		   = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	color_blending.attachmentCount = 1;
	color_blending.pAttachments	   = &color_blend_attachment;
	// color_blending.logicOpEnable = VK_FALSE;

	dynamic_state.sType				= VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamic_state.dynamicStateCount = 2;
	dynamic_state.pDynamicStates	= dynamic_states;

	pipeline_layout_info.sType			= VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipeline_layout_info.setLayoutCount = 1;
	pipeline_layout_info.pSetLayouts	= window->get_descriptor_set_layout();

	if (vkCreatePipelineLayout(*window->get_device(), &pipeline_layout_info, nullptr, &pipeline_layout) != VK_SUCCESS)
		cap::logger::fatal(cap::logger::level::SYSTEM, "failed to create pipeline layout");

	pipeline_info.sType				  = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipeline_info.stageCount		  = 2;
	pipeline_info.pStages			  = shader_stages.data();
	pipeline_info.pVertexInputState	  = &pipeline_vertex_input_state_create_info;
	pipeline_info.pInputAssemblyState = &input_assembly;
	pipeline_info.pViewportState	  = &viewport_state;
	pipeline_info.pRasterizationState = &rasterizer;
	pipeline_info.pMultisampleState	  = &multisampling;
	pipeline_info.pDepthStencilState  = nullptr;
	pipeline_info.pColorBlendState	  = &color_blending;

	pipeline_info.layout	 = pipeline_layout;
	pipeline_info.renderPass = *window->get_render_pass(0);
	pipeline_info.subpass	 = 0;

	vk::pipeline pipeline;

	if (vkCreateGraphicsPipelines(*window->get_device(), VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &pipeline)
		!= VK_SUCCESS)
		cap::logger::fatal(cap::logger::level::SYSTEM, "failed to create graphics pipeline");

	m_registered_pipelines[name] = pipeline;
	m_registered_layouts[name]	 = pipeline_layout;
}
