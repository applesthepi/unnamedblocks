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
	ubo_cam(
		glm::mat4 view_matrix,
		glm::mat4 projection_matrix
	) : uniform_data({ view_matrix, projection_matrix }) {}

	ubo_cam()
		: uniform_data({ glm::mat4(1.0f), glm::mat4(1.0f) }) {}

	struct uniform
	{
		glm::mat4 view_matrix;
		glm::mat4 projection_matrix;
	};

	uniform uniform_data;

	u32 size() override { return sizeof(ubo_cam::uniform); }
	void* data() override { return reinterpret_cast<void*>(&uniform_data); }
};
}
