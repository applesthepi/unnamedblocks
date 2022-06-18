#pragma once
#include "config.h"

#include <latte/utils.hpp>

namespace traffic
{
void bezier_sample_points(
	glm::vec<2, f32> p1,
	glm::vec<2, f32> p2,
	glm::vec<2, f32> p3,
	glm::vec<2, f32> p4,
	u32 count,
	std::vector<glm::vec<2, f32>>& out
);
}
