#pragma once
#include "config.h"

#include <latte/utils.hpp>

namespace traffic
{

template<typename T>
void bezier_sample_points(
	glm::vec<2, T> p1,
	glm::vec<2, T> p2,
	glm::vec<2, T> p3,
	glm::vec<2, T> p4,
	u32 count,
	std::vector<glm::vec<2, f32>>& out
);

}