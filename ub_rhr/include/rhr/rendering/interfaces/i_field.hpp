#pragma once
#include "config.h"

#include "rhr/handlers/input.hpp"
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

	///
	virtual usize get_index_count();

	///
	virtual void insert_char(char charactor, usize idx);

	///
	virtual void insert_string(const std::string& string, usize idx);

	///
	virtual bool remove_char(usize idx);

	///
	virtual bool remove_string(usize idx, usize size);

	///
	virtual void mouse_button(glm::vec<2, i32> position, f32 scroll, rhr::handler::input::mouse_operation operation, rhr::handler::input::mouse_button button);
};
} // namespace rhr::render::interfaces