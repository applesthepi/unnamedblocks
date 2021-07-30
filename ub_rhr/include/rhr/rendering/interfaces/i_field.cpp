#include "i_field.hpp"

std::optional<usize> rhr::render::interfaces::i_field::pick_index(glm::vec<2, i32> position, bool ignore_y)
{
	return std::nullopt;
}

std::optional<glm::vec<2, i32>> rhr::render::interfaces::i_field::get_index_position(usize idx)
{
	return std::nullopt;
}
