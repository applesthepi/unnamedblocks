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
private:
	///
	vk::render_pass m_render_pass;
};
}