#pragma once
#include "config.h"

#include <cappuccino/utils.hpp>

namespace rhr::render
{
///
class render_pass
{
public:
	///
	static void init_render_pass();

	///
	static VkRenderPass render_pass_master;
};
}