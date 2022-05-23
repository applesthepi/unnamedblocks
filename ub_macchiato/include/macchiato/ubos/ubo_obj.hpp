#pragma once
#include "config.h"

#include "ubo.hpp"

#include <latte/utils.hpp>

namespace mac
{
///
class ubo_obj : public mac::ubo
{
public:
	glm::mat4 model_matrix;
	glm::vec<4, f32> color;

	u32 size() override { return sizeof(ubo_obj); }
};
}
