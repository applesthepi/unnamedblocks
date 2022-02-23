#pragma once
#include "config.h"

#include <latte/utils.hpp>

namespace rhr::render::cardinal
{
enum class local
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

enum class local_horizontal
{
	LEFT,
	RIGHT
};

enum class local_vertical
{
	UP,
	DOWN
};

enum class global
{
	NORTH,
	SOUTH,
	WEST,
	EAST
};

enum class plane
{
	HORIZONTAL,
	VERTICAL
};
} // namespace rhr::render::cardinal