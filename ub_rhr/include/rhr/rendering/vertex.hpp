#pragma once
#include "config.h"

#include <Cappuccino/Utils.hpp>

/*
typedef glm::vec<3, float> VertexPosition;
typedef glm::vec<2, float> VertexTextureCoords;

class Vertex
{
public:
	Vertex();
	Vertex(VertexPosition position, VertexTextureCoords textureCoords, uint8_t texturePage);

	VertexPosition Position;
	VertexTextureCoords TextureCoords;
	uint8_t TexturePage;
};
*/

typedef glm::vec<3, float> VertexPosition;
typedef glm::vec<2, float> VertexTextureCoords;

class Vertex
{
public:
	Vertex();
	Vertex(VertexPosition position, VertexTextureCoords textureCoords, uint8_t texturePage);
	Vertex(VertexPosition position, glm::vec3 color, VertexTextureCoords textureCoords);

	VertexPosition Position;
	glm::vec3 Color;
	VertexTextureCoords TextureCoord;

	static VkVertexInputBindingDescription GetBindingDescription();
	static std::array<VkVertexInputAttributeDescription, 3> GetAttributeDescriptions();
};