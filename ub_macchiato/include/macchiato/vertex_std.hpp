#pragma once
#include "config.h"

#include "vertex.hpp"

#include <latte/utils.hpp>

namespace mac
{
///
class vertex_std : public mac::vertex
{
public:
	vertex_std(glm::vec<3, f32> _position, glm::vec<4, f32> _color, glm::vec<2, f32> _texture_coordinates);

	glm::vec<3, f32> position;
	glm::vec<4, f32> color;
	glm::vec<2, f32> texture_coordinates;
private:
	u32 stride() override;
	std::vector<u32> offsets() override;
	std::vector<vk::format> formats() override;
};
}
