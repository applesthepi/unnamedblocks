#pragma once
#include "config.h"

#include <latte/utils.hpp>

namespace mac::command_pool
{
///
struct state
{
	///
	vk::command_pool command_pool;
};

///
mac::command_pool::state* create(vk::device& logical_device, vk::physical_device& physical_device, vk::surface_khr* surface);

///
void destroy(mac::command_pool::state* command_pool_state);
}