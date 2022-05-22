#pragma once
#include "config.h"

#include "ubo.hpp"

#include <latte/utils.hpp>

namespace mac
{
///
class ubo_cam : public mac::ubo
{
public:
	glm::mat4 view_matrix;
	glm::mat4 projection_matrix;

	u32 size() override { return sizeof(ubo_cam); }
};
}
