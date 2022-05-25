#pragma once
#include "config.h"

#include "vertex.hpp"

#include <latte/utils.hpp>

namespace mac::vertex_texture
{
///
struct vertex
{
	glm::vec<3, f32> position;
	glm::vec<2, f32> texture_coordinates;
};

class info : public mac::vertex::info
{
public:
	static const u32 stride_s = sizeof(mac::vertex_texture::vertex);
	u32 stride() override { return stride_s; }

	std::vector<u32> offsets() override { return {
			offsetof(mac::vertex_texture::vertex, position),
			offsetof(mac::vertex_texture::vertex, texture_coordinates)
	}; }

	std::vector<vk::format> formats() override { return {
			VK_FORMAT_R32G32B32_SFLOAT,
			VK_FORMAT_R32G32_SFLOAT
	}; }
};
}
