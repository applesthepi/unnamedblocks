#include "Vertex.hpp"

Vertex::Vertex()
    : Position(VertexPosition(0.0, 0.0, 0.0))
    , Color({ 1.0, 1.0, 1.0 })
    , TextureCoord(VertexTextureCoords(0.0, 0.0))
{
}

Vertex::Vertex(VertexPosition position, VertexTextureCoords textureCoords, uint8_t texturePage)
    : Position(position)
    , Color({ 1.0, 1.0, 1.0 })
    , TextureCoord(textureCoords)
{
}

Vertex::Vertex(VertexPosition position, glm::vec3 color, VertexTextureCoords textureCoords)
    : Position(position)
    , Color(color)
    , TextureCoord(textureCoords)
{
}

VkVertexInputBindingDescription Vertex::GetBindingDescription()
{
	VkVertexInputBindingDescription bindingDescription {};

	bindingDescription.binding = 0;
	bindingDescription.stride = sizeof(Vertex);
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	return bindingDescription;
}

std::array<VkVertexInputAttributeDescription, 3> Vertex::GetAttributeDescriptions()
{
	std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions {};

	attributeDescriptions[0].binding = 0;
	attributeDescriptions[0].location = 0;
	attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[0].offset = offsetof(Vertex, Position);

	attributeDescriptions[1].binding = 0;
	attributeDescriptions[1].location = 1;
	attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[1].offset = offsetof(Vertex, Color);

	attributeDescriptions[2].binding = 0;
	attributeDescriptions[2].location = 2;
	attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
	attributeDescriptions[2].offset = offsetof(Vertex, TextureCoord);

	return attributeDescriptions;
}