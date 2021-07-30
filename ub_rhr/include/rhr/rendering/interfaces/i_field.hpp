#pragma once
#include "config.h"

#include <cappuccino/utils.hpp>

namespace rhr::render::interfaces
{
/// Interface for fields that the field handler can invoke common functions.
class i_field
{
public:
	///
	virtual std::optional<usize> pick_index(glm::vec<2, i32> position, bool ignore_y);

	///
	virtual std::optional<glm::vec<2, i32>> get_index_position(usize idx);
};
}