#include "pipeline.hpp"

mac::pipeline::state* mac::pipeline::create(
	vk::device& logical_device,
	const std::vector<mac::descriptor_set::state*>& descriptor_set_states,
	mac::renderpass::state* renderpass_state,
	mac::vertex* vertex,
	vk::extent_2d& extent,
	const std::string& name,
	const std::string& shader,
	vk::cull_mode_flags cull_mode_flags)
{
	auto pipeline_state = new mac::pipeline::state {
		.name = name,
	};

	std::vector<vk::descriptor_set_layout> descriptor_set_layouts;
	mac::descriptor_set::simplify(descriptor_set_states, descriptor_set_layouts);

	auto vert_shader_code = READ_FILE_BYTES_VC("res/shaders/" + shader + ".vert.spv");
	auto frag_shader_code = READ_FILE_BYTES_VC("res/shaders/" + shader + ".frag.spv");

	auto function_create_shader_module = [logical_device](const std::vector<char>& code)
	{
		vk::shader_module_create_info shader_module_create_info = {};
		vk::shader_module shader_module = {};

		shader_module_create_info.sType	 = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		shader_module_create_info.codeSize = code.size();
		shader_module_create_info.pCode	 = reinterpret_cast<const u32*>(code.data());

		if (vk::create_shader_module(logical_device, &shader_module_create_info, nullptr, &shader_module) != VK_SUCCESS)
			latte::logger::fatal(latte::logger::level::SYSTEM, "failed to create shader module");

		return shader_module;
	};

	auto vert_shader_module = function_create_shader_module(*vert_shader_code);
	auto frag_shader_module = function_create_shader_module(*frag_shader_code);

	vk::pipeline_shader_stage_create_info vert_pipeline_shader_stage_create_info		= {};
	vk::pipeline_shader_stage_create_info frag_pipeline_shader_stage_create_info		= {};
	vk::pipeline_vertex_input_state_create_info pipeline_vertex_input_state_create_info = {};
	vk::pipeline_input_assembly_state_create_info input_assembly						= {};
	vk::pipeline_viewport_state_create_info viewport_state								= {};
	vk::pipeline_rasterization_state_create_info rasterizer								= {};
	vk::pipeline_multisample_state_create_info multisampling							= {};
	vk::pipeline_color_blend_attachment_state color_blend_attachment					= {};
	vk::pipeline_color_blend_state_create_info color_blending							= {};
	vk::pipeline_dynamic_state_create_info dynamic_state								= {};
	vk::pipeline_layout_create_info pipeline_layout_info								= {};
	vk::graphics_pipeline_create_info pipeline_info										= {};

	vk::viewport viewport																= {};
	vk::rect_2d scissor																	= {};

	vk::dynamic_state dynamic_states[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_LINE_WIDTH };

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

	auto binding_description	= vertex->get_binding_description();
	auto attribute_descriptions = vertex->get_attribute_description();

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
	viewport.width	  = static_cast<f32>(extent.width);
	viewport.height	  = static_cast<f32>(extent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	scissor.offset = {0, 0};
	scissor.extent = extent;

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
	pipeline_layout_info.setLayoutCount = descriptor_set_layouts.size();
	pipeline_layout_info.pSetLayouts	= descriptor_set_layouts.data();

	if (vk::create_pipeline_layout(logical_device, &pipeline_layout_info, nullptr, &pipeline_state->pipeline_layout) != VK_SUCCESS)
		latte::logger::fatal(latte::logger::level::SYSTEM, "failed to create pipeline layout");

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

	pipeline_info.layout	 = pipeline_state->pipeline_layout;
	pipeline_info.renderPass = renderpass_state->renderpass;
	pipeline_info.subpass	 = 0;

	if (vk::create_graphics_pipelines(logical_device, VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &pipeline_state->pipeline)
		!= VK_SUCCESS)
		latte::logger::fatal(latte::logger::level::SYSTEM, "failed to create graphics pipeline");

	return pipeline_state;
}

void mac::pipeline::destroy(mac::pipeline::state* pipeline_state)
{
	delete pipeline_state;
}
