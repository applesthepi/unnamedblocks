#include "pass.hpp"

#include "rhr/rendering/renderer.hpp"

rhr::render::component::pass::pass()
	: m_render_pass(nullptr)
{
	std::unique_ptr<rhr::render::component::window>& window = rhr::render::renderer::get_window_primary();

	std::array<vk::subpass_dependency, 1> subpass_dependencies = {};
	vk::attachment_description color_attachment_description	   = {};
	vk::attachment_reference color_attachment_reference		   = {};
	vk::subpass_description color_subpass_description		   = {};
	vk::render_pass_create_info render_pass_create_info		   = {};

	subpass_dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
	subpass_dependencies[0].dstSubpass = 0;
	subpass_dependencies[0].srcStageMask =
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	subpass_dependencies[0].dstStageMask =
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	subpass_dependencies[0].srcAccessMask = 0;
	subpass_dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	color_attachment_description.format			= *window->get_swapchain_format();
	color_attachment_description.samples		= VK_SAMPLE_COUNT_1_BIT;
	color_attachment_description.loadOp			= VK_ATTACHMENT_LOAD_OP_CLEAR;
	color_attachment_description.storeOp		= VK_ATTACHMENT_STORE_OP_STORE;
	color_attachment_description.stencilLoadOp	= VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	color_attachment_description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	color_attachment_description.initialLayout	= VK_IMAGE_LAYOUT_UNDEFINED;
	color_attachment_description.finalLayout	= VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	color_attachment_reference.attachment = 0;
	color_attachment_reference.layout	  = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	color_subpass_description.pipelineBindPoint	   = VK_PIPELINE_BIND_POINT_GRAPHICS;
	color_subpass_description.colorAttachmentCount = 1;
	color_subpass_description.pColorAttachments	   = &color_attachment_reference;

	std::array<vk::attachment_description, 1> attachment_descriptions = {color_attachment_description};

	render_pass_create_info.sType			= VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	render_pass_create_info.attachmentCount = static_cast<u32>(attachment_descriptions.size());
	render_pass_create_info.pAttachments	= attachment_descriptions.data();
	render_pass_create_info.subpassCount	= 1;
	render_pass_create_info.pSubpasses		= &color_subpass_description;
	render_pass_create_info.dependencyCount = static_cast<u32>(subpass_dependencies.size());
	render_pass_create_info.pDependencies	= subpass_dependencies.data();

	if (vk::create_render_pass(*window->get_device(), &render_pass_create_info, nullptr, &m_render_pass) != VK_SUCCESS)
		cap::logger::fatal(cap::logger::level::SYSTEM, "failed to create render pass");
}

rhr::render::component::pass::~pass()
{
	vkDestroyRenderPass(*rhr::render::renderer::get_window_primary()->get_device(), m_render_pass, nullptr);
}

vk::render_pass& rhr::render::component::pass::get_render_pass()
{
	return m_render_pass;
}
