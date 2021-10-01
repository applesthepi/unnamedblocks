#include "panel.hpp"

#include "rhr/rendering/swap_chain.hpp"
#include "rhr/rendering/device.hpp"
#include "rhr/rendering/tools.hpp"
#include "rhr/rendering/renderer.hpp"
#include "rhr/rendering/command.hpp"
#include "rhr/rendering/render_pass.hpp"
#include "rhr/stacking/plane.hpp"

void rhr::render::panel::create_panel(const std::string& id, const std::function<void(panel::data&)>& function_render, const std::function<void(panel::data&)>& function_render_master, const std::function<void(panel::data&)>& function_update_position, const std::function<void(panel::data&)>& function_update_size)
{
	data& local_data = panels.emplace_back();

	local_data.id = id;
	local_data.function_render = function_render;
	local_data.function_render_master = function_render_master;
	local_data.function_update_position = function_update_position;
	local_data.function_update_size = function_update_size;

	// create render pass

	{
//		std::array<VkSubpassDependency, 2> dependencies{};
//
//		dependencies[0].srcSubpass = 0;
//		dependencies[0].dstSubpass = VK_SUBPASS_EXTERNAL;
//		dependencies[0].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
//		dependencies[0].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
//		dependencies[0].srcAccessMask = 0 /*VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT*/;
//		dependencies[0].dstAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
//
//		dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
//
//		dependencies[1].srcSubpass = VK_SUBPASS_EXTERNAL;
//		dependencies[1].dstSubpass = 0;
//		dependencies[1].srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
//		dependencies[1].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
//		dependencies[1].srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
//		dependencies[1].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
//
//		dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

		std::array<VkSubpassDependency, 1> dependencies{};

		dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
		dependencies[0].dstSubpass = 0;
		dependencies[0].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependencies[0].srcAccessMask = 0;
		dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
//		dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		VkAttachmentDescription color_attachment{};
		color_attachment.format = rhr::render::swap_chain::swap_chain_image_format;
		color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
		color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		color_attachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

//		VkAttachmentDescription depth_attachment{};
//		depth_attachment.format = rhr::render::tools::find_depth_format();
//		depth_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
//		depth_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
//		depth_attachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
//		depth_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
//		depth_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
//		depth_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
//		depth_attachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkAttachmentReference color_attachment_ref{};
		color_attachment_ref.attachment = 0;
		color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

//		VkAttachmentReference depth_attachment_ref{};
//		depth_attachment_ref.attachment = 1;
//		depth_attachment_ref.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &color_attachment_ref;
		subpass.pDepthStencilAttachment = nullptr;
//		subpass.pDepthStencilAttachment = &depth_attachment_ref;

		std::array<VkAttachmentDescription, 1> attachments = { color_attachment };
//		std::array<VkAttachmentDescription, 2> attachments = { color_attachment, depth_attachment };

		VkRenderPassCreateInfo render_pass_info{};
		render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		render_pass_info.attachmentCount = static_cast<u32>(attachments.size());
		render_pass_info.pAttachments = attachments.data();
		render_pass_info.subpassCount = 1;
		render_pass_info.pSubpasses = &subpass;
		render_pass_info.dependencyCount = static_cast<u32>(dependencies.size());
		render_pass_info.pDependencies = dependencies.data();

		if (vkCreateRenderPass(rhr::render::device::device_master, &render_pass_info, nullptr, &local_data.render_pass) != VK_SUCCESS)
			cap::logger::fatal("failed to create render pass");
	}

	// create panel image

	rhr::render::tools::create_image(
		rhr::render::swap_chain::swap_chain_extent.width,
		rhr::render::swap_chain::swap_chain_extent.height,
		VK_FORMAT_R8G8B8A8_UNORM,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		local_data.color_image,
		local_data.color_device_memory
		);

	rhr::render::tools::create_image(
		rhr::render::swap_chain::swap_chain_extent.width,
		rhr::render::swap_chain::swap_chain_extent.height,
		rhr::render::tools::find_depth_format(),
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		local_data.depth_image,
		local_data.depth_device_memory
		);

	// create panel image views

	local_data.color_image_view = rhr::render::tools::create_image_view(
		local_data.color_image,
		rhr::render::swap_chain::swap_chain_image_format,
		VK_IMAGE_ASPECT_COLOR_BIT
		);

	local_data.depth_image_view = rhr::render::tools::create_image_view(
		local_data.depth_image,
		rhr::render::tools::find_depth_format(),
		VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT
		);

	// create panel frame buffer

	std::array<VkImageView, 1> attachments = {
		local_data.color_image_view
	};

//	std::array<VkImageView, 2> attachments = {
//		local_data.color_image_view,
//		local_data.depth_image_view
//	};

	VkFramebufferCreateInfo frame_buffer_info{};
	frame_buffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	frame_buffer_info.renderPass = local_data.render_pass;
	frame_buffer_info.attachmentCount = static_cast<u32>(attachments.size());
	frame_buffer_info.pAttachments = attachments.data();
	frame_buffer_info.width = rhr::render::swap_chain::swap_chain_extent.width;
	frame_buffer_info.height = rhr::render::swap_chain::swap_chain_extent.height;
	frame_buffer_info.layers = 1;

	if (vkCreateFramebuffer(rhr::render::device::device_master, &frame_buffer_info, nullptr, &local_data.frame_buffer) != VK_SUCCESS)
		cap::logger::fatal("failed to create frame buffers");

	// create panel texture sampler

	{
		VkPhysicalDeviceProperties properties{};
		vkGetPhysicalDeviceProperties(rhr::render::device::physical_device, &properties);

		VkSamplerCreateInfo sampler_info{};
		sampler_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		sampler_info.magFilter = VK_FILTER_NEAREST;
		sampler_info.minFilter = VK_FILTER_NEAREST;
		sampler_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		sampler_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		sampler_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

		sampler_info.anisotropyEnable = VK_TRUE;
		sampler_info.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
		sampler_info.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		sampler_info.unnormalizedCoordinates = VK_FALSE;
		sampler_info.compareEnable = VK_FALSE;
		sampler_info.compareOp = VK_COMPARE_OP_ALWAYS;

		sampler_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		sampler_info.mipLodBias = 0.0f;
		sampler_info.minLod = 0.0f;
		sampler_info.maxLod = 0.0f;

		if (vkCreateSampler(rhr::render::device::device_master, &sampler_info, nullptr, &local_data.sampler) != VK_SUCCESS)
			cap::logger::fatal("failed to create texture sampler");
	}

	// create panel descriptor set

	local_data.descriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	local_data.descriptor.imageView = local_data.color_image_view;
	local_data.descriptor.sampler = local_data.sampler;

	local_data.descriptor_set = ImGui_ImplVulkan_AddTexture(
		local_data.descriptor.sampler,
		local_data.descriptor.imageView,
		local_data.descriptor.imageLayout
	);
}

void rhr::render::panel::run_imgui()
{
	ImGui_ImplVulkanH_Frame* fd = &rhr::render::renderer::imgui_local->data.Frames[rhr::render::renderer::imgui_local->data.FrameIndex];

	for (auto& data : panels)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin(data.id.c_str());

		glm::vec<2, i32> panel_size = { ImGui::GetWindowSize().x, ImGui::GetWindowSize().y };
		glm::vec<2, i32> panel_position = { ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMin().x, ImGui::GetWindowPos().y + ImGui::GetWindowContentRegionMin().y };

		if (panel_size != data.panel_last_size)
		{
			data.panel_last_size = panel_size;
			data.function_update_size(data);
		}

		if (panel_position != data.panel_last_position)
		{
			data.panel_last_position = panel_position;
			data.function_update_position(data);
		}

		// render pass

		std::array<VkClearValue, 1> clear_values{};
		clear_values[0].color = { { 0.0f, 0.0f, 0.0f, 1.0f } };
//		clear_values[1].depthStencil = { 1.0f, 0 };

		{
			VkRenderPassBeginInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			info.renderPass = data.render_pass;
			info.framebuffer = data.frame_buffer;
			info.renderArea.extent.width = rhr::render::renderer::window_size.x;
			info.renderArea.extent.height = rhr::render::renderer::window_size.y;
			info.clearValueCount = clear_values.size();
			info.pClearValues = clear_values.data();

			vkCmdBeginRenderPass(*rhr::render::command::active_command_buffer, &info, VK_SUBPASS_CONTENTS_INLINE);
			data.function_render(data);
			vkCmdEndRenderPass(*rhr::render::command::active_command_buffer);
		}

		ImGui::Image(
			data.descriptor_set,
			{
				static_cast<f32>(rhr::render::renderer::window_size.x),
				static_cast<f32>(rhr::render::renderer::window_size.y)
			}
		);

		ImGui::End();
		ImGui::PopStyleVar();
	}
}

void rhr::render::panel::run_master_render_pass()
{
	for (auto& data : panels)
		data.function_render_master(data);
}

void rhr::render::panel::initialize_panels()
{
	create_panel("plane_primary",
		[](panel::data& data)
		{
			rhr::stack::plane::primary_plane->render();
		},
		[](panel::data& data)
		{
			rhr::stack::plane::primary_plane->render_master_pass();
		},
		[](panel::data& data)
		{
			rhr::stack::plane::primary_plane->set_position_parent_virtual_offset(data.panel_last_position - rhr::render::renderer::window_position);
		},
		[](panel::data& data)
		{
			rhr::stack::plane::primary_plane->set_size_parent(data.panel_last_size);
			rhr::stack::plane::primary_plane->set_size_max();
		}
	);

	create_panel("plane_toolbar",
		[](panel::data& data)
		{
			rhr::stack::plane::toolbar_plane->render();
		},
		[](panel::data& data)
		{
			rhr::stack::plane::toolbar_plane->render_master_pass();
		},
		[](panel::data& data)
		{
			rhr::stack::plane::toolbar_plane->set_position_parent_virtual_offset(data.panel_last_position - rhr::render::renderer::window_position);
		},
		[](panel::data& data)
		{
			rhr::stack::plane::toolbar_plane->set_size_parent(data.panel_last_size);
			rhr::stack::plane::toolbar_plane->set_size_max();
		}
	);
}

std::vector<rhr::render::panel::data> rhr::render::panel::panels;