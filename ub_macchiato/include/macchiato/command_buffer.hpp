#pragma once
#include "config.h"

#include "command_pool.hpp"

#include <latte/utils.hpp>

namespace mac::command_buffer
{
///
struct state
{
	/// Holds commands to be sent to the gpu during submission.
	vk::command_buffer command_buffer;

	/// After execution of the command buffers, this is signaled so
	/// command buffer recording can begin again. Signaled same as
	/// the ready_presentation_semaphore.
	vk::fence ready_write_fence;

	/// When the presentation of the frame is done, the gpu can begin
	/// execution of the command buffers to overwrite the framebuffer.
	vk::semaphore ready_execution_semaphore;

	/// After execution of the command buffers, this is signaled so
	/// the presentation can begin.
	vk::semaphore ready_presentation_semaphore;
};

///
mac::command_buffer::state* create(vk::device& logical_device, mac::command_pool::state* command_pool_state, u32 frame_buffer_count);

///
void destroy(mac::command_buffer::state* command_buffer_state);
}
