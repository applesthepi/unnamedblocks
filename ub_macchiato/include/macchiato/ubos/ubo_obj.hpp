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
	ubo_obj(
		glm::vec<4, f32> color,
		glm::mat4 model_matrix
	) : uniform_data({ color, model_matrix }) {}

	ubo_obj()
		: uniform_data({ glm::vec<4, f32>(1.0f), glm::mat4(1.0f) }) {}

	struct uniform
	{
		glm::vec<4, f32> color;
		glm::mat4 model_matrix;
	};

	uniform uniform_data;

	u32 size() override { return sizeof(ubo_obj::uniform); }
	void* data() override { return reinterpret_cast<void*>(&uniform_data); }
};
}
