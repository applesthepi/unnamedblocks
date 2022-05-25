#pragma once
#include "config.h"

#include <latte/utils.hpp>

namespace mac::vertex
{
///
class info
{
public:
	///
	vk::vertex_input_binding_description get_binding_description();

	///
	std::vector<vk::vertex_input_attribute_description> get_attribute_description();

	///
	virtual u32 stride() = 0;

	///
	virtual std::vector<u32> offsets() = 0;

	///
	virtual std::vector<vk::format> formats() = 0;
};
}
