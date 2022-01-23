#pragma once
#include "config.h"

#include <cappuccino/utils.hpp>

namespace rhr::render::component
{
///
class pass
{
public:
	pass();
	~pass();

	///
	vk::render_pass& get_render_pass();

private:
	///
	vk::render_pass m_render_pass;
};
} // namespace rhr::render::component