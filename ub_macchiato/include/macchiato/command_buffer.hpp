#pragma once
#include "config.h"

#include "command_pool.hpp"

#include <latte/utils.hpp>

namespace mac::command_buffer
{
///
struct state
{
	///
	vk::command_buffer command_buffer;
};

///
mac::command_buffer::state* create(vk::device& logical_device, mac::command_pool::state* command_pool_state, u32 frame_buffer_count);

///
void destroy(mac::command_buffer::state* command_buffer_state);
}
