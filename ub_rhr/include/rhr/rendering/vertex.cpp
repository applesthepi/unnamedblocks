#include "vertex.hpp"

rhr::render::vertex::vertex()
	: position({ 0.0f, 0.0f, 0.0f })
	, color({ 1.0f, 1.0f, 1.0f, 1.0f })
	, texture_coordinate({ 0.0f, 0.0f })
{

}

rhr::render::vertex::vertex(glm::vec<3, f32> position, glm::vec<4, f32> color)
	: position(position)
	, color(color)
	, texture_coordinate({ 0.0f, 0.0f })
{

}

rhr::render::vertex::vertex(glm::vec<3, f32> position, glm::vec<2, f32> texture_coordinate)
	: position(position)
	, color({ 1.0f, 1.0f, 1.0f, 1.0f })
	, texture_coordinate(texture_coordinate)
{

}

rhr::render::vertex::vertex(glm::vec<3, f32> position, glm::vec<4, f32> color, glm::vec<2, f32> texture_coordinate)
	: position(position)
	, color(color)
	, texture_coordinate(texture_coordinate)
{

}

VkVertexInputBindingDescription rhr::render::vertex::get_binding_description()
{
	VkVertexInputBindingDescription bindingDescription {};

	bindingDescription.binding = 0;
	bindingDescription.stride = sizeof(rhr::render::vertex);
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	return bindingDescription;
}

std::array<VkVertexInputAttributeDescription, 3> rhr::render::vertex::get_attribute_description()
{
	std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions {};

	attributeDescriptions[0].binding = 0;
	attributeDescriptions[0].location = 0;
	attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[0].offset = offsetof(vertex, position);

	attributeDescriptions[1].binding = 0;
	attributeDescriptions[1].location = 1;
	attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[1].offset = offsetof(vertex, color);

	attributeDescriptions[2].binding = 0;
	attributeDescriptions[2].location = 2;
	attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
	attributeDescriptions[2].offset = offsetof(vertex, texture_coordinate);

	return attributeDescriptions;
}