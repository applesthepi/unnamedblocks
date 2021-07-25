#pragma once
#include "config.h"

#include <cappuccino/utils.hpp>

namespace rhr::render::cardinal
{
enum class local
{
	UP, DOWN, LEFT, RIGHT
};

enum class global
{
	NORTH, SOUTH, WEST, EAST
};

enum class plane
{
	HORIZONTAL, VERTICAL
};
}