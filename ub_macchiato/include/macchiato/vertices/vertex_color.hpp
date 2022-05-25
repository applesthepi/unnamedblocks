#pragma once
#include "config.h"

#include "vertex.hpp"

#include <latte/utils.hpp>

namespace mac::vertex_color
{
///
struct vertex
{
	glm::vec<3, f32> position;
	glm::vec<4, f32> color;
};

class info : public mac::vertex::info
{
public:
	static const u32 stride_s = sizeof(mac::vertex_color::vertex);
	u32 stride() override { return stride_s; }

	std::vector<u32> offsets() override { return {
			offsetof(mac::vertex_color::vertex, position),
			offsetof(mac::vertex_color::vertex, color)
	}; }

	std::vector<vk::format> formats() override { return {
			VK_FORMAT_R32G32B32_SFLOAT,
			VK_FORMAT_R32G32B32A32_SFLOAT
	}; }
};
}
