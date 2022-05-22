#include "command_buffer.hpp"

mac::command_buffer::state* mac::command_buffer::create(vk::device& logical_device, mac::command_pool::state* command_pool_state, u32 frame_buffer_count)
{
	auto command_buffer_state = new mac::command_buffer::state {

	};

	vk::command_buffer_allocate_info command_buffer_allocate_info = {};

	command_buffer_allocate_info.sType				= VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	command_buffer_allocate_info.commandPool		= command_pool_state->command_pool;
	command_buffer_allocate_info.level				= VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	command_buffer_allocate_info.commandBufferCount = frame_buffer_count;

	if (vk::allocate_command_buffers(logical_device, &command_buffer_allocate_info, &command_buffer_state->command_buffer)
		!= VK_SUCCESS)
	{
		latte::logger::fatal(latte::logger::level::SYSTEM, "failed to create command buffers");
		return nullptr;
	}

	vk::command_buffer_begin_info command_buffer_begin_info	= {};
	//vk::render_pass_begin_info render_pass_info = {};
	//std::array<vk::clear_value, 1> clear_values = {};

	command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	if (vk::begin_command_buffer(command_buffer_state->command_buffer, &command_buffer_begin_info) != VK_SUCCESS)
	{
		latte::logger::fatal(latte::logger::level::SYSTEM, "failed to begin recording the command buffer during setup");
		return nullptr;
	}

	if (vk::end_command_buffer(command_buffer_state->command_buffer) != VK_SUCCESS)
	{
		latte::logger::fatal(latte::logger::level::SYSTEM, "failed to stop recording the command buffer during setup");
		return nullptr;
	}
}

void mac::command_buffer::destroy(mac::command_buffer::state* command_buffer_state)
{
	delete command_buffer_state;
}