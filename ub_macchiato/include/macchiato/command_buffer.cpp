#include "command_buffer.hpp"

mac::command_buffer::state* mac::command_buffer::create(vk::device& logical_device, mac::command_pool::state* command_pool_state, u32 frame_buffer_count)
{
	auto command_buffer_state = new mac::command_buffer::state {
		.ready_write_fence = nullptr,
		.ready_execution_semaphore = nullptr,
		.ready_presentation_semaphore = nullptr,
	};

	// SEMAPHORES

	auto function_create_semaphore = [&logical_device](vk::semaphore* semaphore)
	{
		vk::semaphore_create_info semaphore_create_info = {};
		semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		if (vk::create_semaphore(logical_device, &semaphore_create_info, nullptr, semaphore) != VK_SUCCESS)
		{
			latte::logger::fatal(latte::logger::level::SYSTEM, "failed to create semaphore");
			return;
		}
	};

	function_create_semaphore(&command_buffer_state->ready_execution_semaphore);
	function_create_semaphore(&command_buffer_state->ready_presentation_semaphore);

	// FENCES

	auto function_create_fence = [&logical_device](vk::fence* fence)
	{
		vk::fence_create_info fence_create_info = {};
		fence_create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fence_create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		if (vk::create_fence(logical_device, &fence_create_info, nullptr, fence) != VK_SUCCESS)
		{
			latte::logger::fatal(latte::logger::level::SYSTEM, "failed to create fence");
			return;
		}
	};

	function_create_fence(&command_buffer_state->ready_write_fence);

	// COMMAND BUFFER

	vk::command_buffer_allocate_info command_buffer_allocate_info = {};

	command_buffer_allocate_info.sType				= VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	command_buffer_allocate_info.commandPool		= command_pool_state->command_pool;
	command_buffer_allocate_info.level				= VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	command_buffer_allocate_info.commandBufferCount = 1;

	if (vk::allocate_command_buffers(logical_device, &command_buffer_allocate_info, &command_buffer_state->command_buffer)
		!= VK_SUCCESS)
	{
		latte::logger::fatal(latte::logger::level::SYSTEM, "failed to create command buffers");
		return nullptr;
	}

	// BLANK COMMAND BUFFER

	vk::command_buffer_begin_info command_buffer_begin_info	= {};
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

	return command_buffer_state;
}

void mac::command_buffer::destroy(mac::command_buffer::state* command_buffer_state)
{
	delete command_buffer_state;
}