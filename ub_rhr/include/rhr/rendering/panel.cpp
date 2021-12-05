#include "panel.hpp"

#include "rhr/rendering/tools.hpp"
#include "rhr/rendering/renderer.hpp"
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
		// Color attachment
		VkImageCreateInfo image = {};
		image.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		image.imageType = VK_IMAGE_TYPE_2D;
		image.format = VK_FORMAT_B8G8R8A8_UNORM;
		image.extent.width = rhr::render::renderer::get_window_primary()->get_swapchain_extent()->width;
		image.extent.height = rhr::render::renderer::get_window_primary()->get_swapchain_extent()->height;
		image.extent.depth = 1;
		image.mipLevels = 1;
		image.arrayLayers = 1;
		image.samples = VK_SAMPLE_COUNT_1_BIT;
		image.tiling = VK_IMAGE_TILING_OPTIMAL;
		// We will sample directly from the color attachment
		image.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

		VkMemoryAllocateInfo memAlloc = {};
		memAlloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;

		VkMemoryRequirements memReqs;

		vkCreateImage(*rhr::render::renderer::get_window_primary()->get_device(), &image, nullptr, &local_data.color_image);
		vkGetImageMemoryRequirements(*rhr::render::renderer::get_window_primary()->get_device(), local_data.color_image, &memReqs);
		memAlloc.allocationSize = memReqs.size;
		memAlloc.memoryTypeIndex = rhr::render::tools::find_memory_type(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		vkAllocateMemory(*rhr::render::renderer::get_window_primary()->get_device(), &memAlloc, nullptr, &local_data.color_device_memory);
		vkBindImageMemory(*rhr::render::renderer::get_window_primary()->get_device(), local_data.color_image, local_data.color_device_memory, 0);

		VkImageViewCreateInfo colorImageView = {};
		colorImageView.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		colorImageView.viewType = VK_IMAGE_VIEW_TYPE_2D;
		colorImageView.format = VK_FORMAT_B8G8R8A8_UNORM;
		colorImageView.subresourceRange = {};
		colorImageView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		colorImageView.subresourceRange.baseMipLevel = 0;
		colorImageView.subresourceRange.levelCount = 1;
		colorImageView.subresourceRange.baseArrayLayer = 0;
		colorImageView.subresourceRange.layerCount = 1;
		colorImageView.image = local_data.color_image;
		vkCreateImageView(*rhr::render::renderer::get_window_primary()->get_device(), &colorImageView, nullptr, &local_data.color_image_view);

		// Create sampler to sample from the attachment in the fragment shader
		VkSamplerCreateInfo samplerInfo = {};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		samplerInfo.addressModeV = samplerInfo.addressModeU;
		samplerInfo.addressModeW = samplerInfo.addressModeU;
		samplerInfo.mipLodBias = 0.0f;
		samplerInfo.maxAnisotropy = 1.0f;
		samplerInfo.minLod = 0.0f;
		samplerInfo.maxLod = 1.0f;
		samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
		vkCreateSampler(*rhr::render::renderer::get_window_primary()->get_device(), &samplerInfo, nullptr, &local_data.sampler);

		// Depth stencil attachment
//		image.format = fbDepthFormat;
//		image.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

//		VK_CHECK_RESULT(vkCreateImage(device, &image, nullptr, &offscreenPass.depth.image));
//		vkGetImageMemoryRequirements(device, offscreenPass.depth.image, &memReqs);
//		memAlloc.allocationSize = memReqs.size;
//		memAlloc.memoryTypeIndex = vulkanDevice->getMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
//		VK_CHECK_RESULT(vkAllocateMemory(device, &memAlloc, nullptr, &offscreenPass.depth.mem));
//		VK_CHECK_RESULT(vkBindImageMemory(device, offscreenPass.depth.image, offscreenPass.depth.mem, 0));

//		VkImageViewCreateInfo depthStencilView = vks::initializers::imageViewCreateInfo();
//		depthStencilView.viewType = VK_IMAGE_VIEW_TYPE_2D;
//		depthStencilView.format = fbDepthFormat;
//		depthStencilView.flags = 0;
//		depthStencilView.subresourceRange = {};
//		depthStencilView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
//		depthStencilView.subresourceRange.baseMipLevel = 0;
//		depthStencilView.subresourceRange.levelCount = 1;
//		depthStencilView.subresourceRange.baseArrayLayer = 0;
//		depthStencilView.subresourceRange.layerCount = 1;
//		depthStencilView.image = offscreenPass.depth.image;
//		VK_CHECK_RESULT(vkCreateImageView(device, &depthStencilView, nullptr, &offscreenPass.depth.view));

		// Create a separate render pass for the offscreen rendering as it may differ from the one used for scene rendering

		std::array<VkAttachmentDescription, 1> attchmentDescriptions = {};
		// Color attachment
		attchmentDescriptions[0].format = VK_FORMAT_B8G8R8A8_UNORM;
		attchmentDescriptions[0].samples = VK_SAMPLE_COUNT_1_BIT;
		attchmentDescriptions[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attchmentDescriptions[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attchmentDescriptions[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attchmentDescriptions[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attchmentDescriptions[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attchmentDescriptions[0].finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		// Depth attachment
//		attchmentDescriptions[1].format = fbDepthFormat;
//		attchmentDescriptions[1].samples = VK_SAMPLE_COUNT_1_BIT;
//		attchmentDescriptions[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
//		attchmentDescriptions[1].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
//		attchmentDescriptions[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
//		attchmentDescriptions[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
//		attchmentDescriptions[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
//		attchmentDescriptions[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkAttachmentReference colorReference = { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
//		VkAttachmentReference depthReference = { 1, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };

		VkSubpassDescription subpassDescription = {};
		subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpassDescription.colorAttachmentCount = 1;
		subpassDescription.pColorAttachments = &colorReference;
//		subpassDescription.pDepthStencilAttachment = &depthReference;

		// Use subpass dependencies for layout transitions
//		std::array<VkSubpassDependency, 2> dependencies;
//
//		dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
//		dependencies[0].dstSubpass = 0;
//		dependencies[0].srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
//		dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
//		dependencies[0].srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
//		dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
//		dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
//
//		dependencies[1].srcSubpass = 0;
//		dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
//		dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
//		dependencies[1].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
//		dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
//		dependencies[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
//		dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;


		std::array<VkSubpassDependency, 1> dependencies{};

		dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
		dependencies[0].dstSubpass = 0;
		dependencies[0].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependencies[0].srcAccessMask = 0;
		dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo render_pass_info{};
		render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		render_pass_info.attachmentCount = static_cast<u32>(attchmentDescriptions.size());
		render_pass_info.pAttachments = attchmentDescriptions.data();
		render_pass_info.subpassCount = 1;
		render_pass_info.pSubpasses = &subpassDescription;
		render_pass_info.dependencyCount = static_cast<u32>(dependencies.size());
		render_pass_info.pDependencies = dependencies.data();

		if (vkCreateRenderPass(*rhr::render::renderer::get_window_primary()->get_device(), &render_pass_info, nullptr, &local_data.render_pass) != VK_SUCCESS)
			cap::logger::fatal("failed to create render pass");
	}

	// create panel image

//	rhr::render::tools::create_image(
//		rhr::render::swap_chain::swap_chain_extent.width,
//		rhr::render::swap_chain::swap_chain_extent.height,
//		rhr::render::swap_chain::swap_chain_image_format,
//		VK_IMAGE_TILING_OPTIMAL,
//		VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT,
//		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
//		local_data.color_image,
//		local_data.color_device_memory
//		);
//
//	rhr::render::tools::create_image(
//		rhr::render::swap_chain::swap_chain_extent.width,
//		rhr::render::swap_chain::swap_chain_extent.height,
//		rhr::render::tools::find_depth_format(),
//		VK_IMAGE_TILING_OPTIMAL,
//		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
//		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
//		local_data.depth_image,
//		local_data.depth_device_memory
//		);

	// create panel image views

//	local_data.color_image_view = rhr::render::tools::create_image_view(
//		local_data.color_image,
//		rhr::render::swap_chain::swap_chain_image_format,
//		VK_IMAGE_ASPECT_COLOR_BIT
//		);
//
//	local_data.depth_image_view = rhr::render::tools::create_image_view(
//		local_data.depth_image,
//		rhr::render::tools::find_depth_format(),
//		VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT
//		);

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
	frame_buffer_info.width = rhr::render::renderer::get_window_primary()->get_swapchain_extent()->width;
	frame_buffer_info.height = rhr::render::renderer::get_window_primary()->get_swapchain_extent()->height;
	frame_buffer_info.layers = 1;

	if (vkCreateFramebuffer(*rhr::render::renderer::get_window_primary()->get_device(), &frame_buffer_info, nullptr, &local_data.frame_buffer) != VK_SUCCESS)
		cap::logger::fatal("failed to create frame buffers");

	// create panel texture sampler

	{
		VkPhysicalDeviceProperties properties{};
		vkGetPhysicalDeviceProperties(*rhr::render::renderer::get_window_primary()->get_physical_device(), &properties);

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

		if (vkCreateSampler(*rhr::render::renderer::get_window_primary()->get_device(), &sampler_info, nullptr, &local_data.sampler) != VK_SUCCESS)
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

	rhr::render::renderer::get_window_primary()->register_paired_pipeline("panel_" + id, "ui", "ui_texture");
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
		clear_values[0].color = { { 0.0f, 0.0f, 0.0f, 0.0f } };
//		clear_values[1].depthStencil = { 1.0f, 0 };

		rhr::render::renderer::get_window_primary()->apply_active_pipeline("panel_" + data.id);

		{
			VkRenderPassBeginInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			info.renderPass = data.render_pass;
			info.framebuffer = data.frame_buffer;
			info.renderArea.extent.width = rhr::render::renderer::get_window_primary()->get_window_size().x;
			info.renderArea.extent.height = rhr::render::renderer::get_window_primary()->get_window_size().y;
			info.clearValueCount = clear_values.size();
			info.pClearValues = clear_values.data();

			vkCmdBeginRenderPass(*rhr::render::renderer::get_window_primary()->get_active_command_buffer(), &info, VK_SUBPASS_CONTENTS_INLINE);
			data.function_render(data);
			vkCmdEndRenderPass(*rhr::render::renderer::get_window_primary()->get_active_command_buffer());
		}

		ImGui::Image(
			data.descriptor_set,
			{
				static_cast<f32>(rhr::render::renderer::get_window_primary()->get_window_size().x),
				static_cast<f32>(rhr::render::renderer::get_window_primary()->get_window_size().y)
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
	panels.clear();

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
			rhr::stack::plane::primary_plane->set_position_parent_virtual_offset(data.panel_last_position - rhr::render::renderer::get_window_primary()->get_window_position());
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
			rhr::stack::plane::toolbar_plane->set_position_parent_virtual_offset(data.panel_last_position - rhr::render::renderer::get_window_primary()->get_window_position());
		},
		[](panel::data& data)
		{
			rhr::stack::plane::toolbar_plane->set_size_parent(data.panel_last_size);
			rhr::stack::plane::toolbar_plane->set_size_max();
		}
	);
}

std::vector<rhr::render::panel::data> rhr::render::panel::panels;