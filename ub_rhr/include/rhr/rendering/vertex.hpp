#pragma once
#include "config.h"

#include <cappuccino/utils.hpp>

namespace rhr::render
{
///
class vertex
{
public:
	///
	vertex();

	///
	vertex(glm::vec<3, f32> position, glm::vec<4, f32> color);

	///
	vertex(glm::vec<3, f32> position, glm::vec<2, f32> texture_coordinate);

	///
	vertex(glm::vec<3, f32> position, glm::vec<4, f32> color, glm::vec<2, f32> texture_coordinate);

	///
	glm::vec<3, f32> position;

	///
	glm::vec<4, f32> color;

	///
	glm::vec<2, f32> texture_coordinate;

	///
	static VkVertexInputBindingDescription get_binding_description();

	///
	static std::array<VkVertexInputAttributeDescription, 3> get_attribute_description();
};
}