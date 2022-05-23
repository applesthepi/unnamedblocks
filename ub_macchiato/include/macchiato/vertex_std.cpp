#include "vertex_std.hpp"

mac::vertex_std::vertex_std(glm::vec<3, f32> _position, glm::vec<4, f32> _color)
	: position(_position)
	, color(_color)
{

}

u32 mac::vertex_std::stride()
{
	return sizeof(mac::vertex_std);
}

std::vector<u32> mac::vertex_std::offsets()
{
	return {
		offsetof(mac::vertex_std, position), // pos
		offsetof(mac::vertex_std, color)     // color
	};
}

std::vector<vk::format> mac::vertex_std::formats()
{
	return {
		VK_FORMAT_R32G32B32_SFLOAT,    // pos
		VK_FORMAT_R32G32B32A32_SFLOAT  // color
	};
}
