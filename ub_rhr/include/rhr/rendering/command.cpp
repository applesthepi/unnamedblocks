#include "command.hpp"

#include "rhr/rendering/swap_chain.hpp"
#include "rhr/rendering/render_pass.hpp"
#include "rhr/rendering/tools.hpp"
#include "rhr/rendering/renderer.hpp"
#include "rhr/rendering/pipeline.hpp"
#include "rhr/rendering/device.hpp"

void rhr::render::command::init_command_pool()
{
	rhr::render::tools::queue_family_indices queue_family_indices = rhr::render::tools::find_queue_families(&rhr::render::device::physical_device, &rhr::render::renderer::surface);

	VkCommandPoolCreateInfo pool_info{};
	pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	pool_info.queueFamilyIndex = queue_family_indices.graphics_family.value();
	pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

	if (vkCreateCommandPool(rhr::render::device::device_master, &pool_info, nullptr, &command_pool) != VK_SUCCESS)
		cap::logger::fatal("failed to create command pool");
}

void rhr::render::command::init_descriptor_pool()
{
	std::array<VkDescriptorPoolSize, 2> pool_sizes{};

	pool_sizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	pool_sizes[0].descriptorCount = static_cast<u32>(10000);

	pool_sizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	pool_sizes[1].descriptorCount = static_cast<u32>(10000);

	VkDescriptorPoolCreateInfo pool_info{};
	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_info.poolSizeCount = static_cast<u32>(pool_sizes.size());
	pool_info.pPoolSizes = pool_sizes.data();
	pool_info.maxSets = 10000;
	pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

	if (vkCreateDescriptorPool(rhr::render::device::device_master, &pool_info, nullptr, &descriptor_pool) != VK_SUCCESS)
		cap::logger::fatal("failed to create descriptor pool");
}

void rhr::render::command::init_command_buffers()
{
	command_buffer_panels.resize(rhr::render::swap_chain::swap_chain_frame_buffers.size());
	command_buffer_master.resize(rhr::render::swap_chain::swap_chain_frame_buffers.size());

	generate_command_buffer(command_buffer_panels.size(), command_buffer_panels.data());
	generate_command_buffer(command_buffer_master.size(), command_buffer_master.data());

	for (usize i = 0; i < command_buffer_panels.size(); i++)
		setup_command_buffer(&command_buffer_panels[i], &rhr::render::render_pass::render_pass_master, &rhr::render::swap_chain::swap_chain_frame_buffers[i]);

	for (usize i = 0; i < command_buffer_master.size(); i++)
		setup_command_buffer(&command_buffer_master[i], &rhr::render::render_pass::render_pass_master, &rhr::render::swap_chain::swap_chain_frame_buffers[i]);
}

void rhr::render::command::generate_command_buffer(u32 count, VkCommandBuffer* command_buffer)
{
	VkCommandBufferAllocateInfo alloc_info{};
	alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	alloc_info.commandPool = command_pool;
	alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	alloc_info.commandBufferCount = count;

	if (vkAllocateCommandBuffers(rhr::render::device::device_master, &alloc_info, command_buffer) != VK_SUCCESS)
		cap::logger::fatal("failed to create command buffers");
}

void rhr::render::command::setup_command_buffer(VkCommandBuffer* command_buffer, VkRenderPass* render_pass, VkFramebuffer* frame_buffer)
{
	vk::command_buffer_begin_info begin_info{};
	begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	begin_info.flags = 0; // Optional
	begin_info.pInheritanceInfo = nullptr; // Optional

	if (vkBeginCommandBuffer(*command_buffer, &begin_info) != VK_SUCCESS)
		cap::logger::fatal("failed to start the command buffer");

	vk::render_pass_begin_info render_pass_info{};
	render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	render_pass_info.renderPass = *render_pass;
	render_pass_info.framebuffer = *frame_buffer;

	render_pass_info.renderArea.offset = { 0, 0 };
	render_pass_info.renderArea.extent = rhr::render::swap_chain::swap_chain_extent;

	std::array<vk::clear_value, 1> clear_values{};
	clear_values[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
//	clear_values[1].depthStencil = { 1.0f, 0 };

	render_pass_info.clearValueCount = static_cast<u32>(clear_values.size());
	render_pass_info.pClearValues = clear_values.data();

	vkCmdBeginRenderPass(*command_buffer, &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);
	vkCmdBindPipeline(*command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, rhr::render::pipeline::get_color("master"));
	vkCmdEndRenderPass(*command_buffer);

	if (vkEndCommandBuffer(*command_buffer) != VK_SUCCESS)
		cap::logger::fatal("failed to close the command buffer");
}

std::vector<VkCommandBuffer> rhr::render::command::command_buffer_panels;
std::vector<VkCommandBuffer> rhr::render::command::command_buffer_master;
VkCommandBuffer* rhr::render::command::active_command_buffer;
std::vector<VkDeviceQueueCreateInfo> rhr::render::command::queue_create_infos;
VkDescriptorSetLayout rhr::render::command::descriptor_set_layout;
VkCommandPool rhr::render::command::command_pool;
VkDescriptorPool rhr::render::command::descriptor_pool;