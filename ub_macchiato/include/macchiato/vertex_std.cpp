#include "vertex_std.hpp"

mac::vertex_std::vertex_std(glm::vec<3, f32> _position, glm::vec<4, f32> _color, glm::vec<2, f32> _texture_coordinates)
	: position(_position)
	, color(_color)
	, texture_coordinates(_texture_coordinates)
{

}

u32 mac::vertex_std::stride()
{
	return sizeof(mac::vertex_std);
}

std::vector<u32> mac::vertex_std::offsets()
{
	return {
		sizeof(f32) * 3, // pos
		sizeof(f32) * 4, // color
		sizeof(f32) * 2  // tex coord
	};
}

std::vector<vk::format> mac::vertex_std::formats()
{
	return {
		VK_FORMAT_R32G32B32_SFLOAT,    // pos
		VK_FORMAT_R32G32B32A32_SFLOAT, // color
		VK_FORMAT_R32G32_SFLOAT        // tex coord
	};
}
