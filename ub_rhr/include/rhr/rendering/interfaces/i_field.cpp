#include "i_field.hpp"

std::optional<usize> rhr::render::interfaces::i_field::pick_index(glm::vec<2, i32> position, bool ignore_y)
{
	return std::nullopt;
}

std::optional<glm::vec<2, i32>> rhr::render::interfaces::i_field::get_index_position(usize idx)
{
	return std::nullopt;
}

usize rhr::render::interfaces::i_field::get_index_count()
{
	return 0;
}

void rhr::render::interfaces::i_field::insert_char(char charactor, usize idx)
{

}

void rhr::render::interfaces::i_field::insert_string(const std::string& string, usize idx)
{

}

bool rhr::render::interfaces::i_field::remove_char(usize idx)
{
	return false;
}

bool rhr::render::interfaces::i_field::remove_string(usize idx, usize size)
{
	return false;
}

void rhr::render::interfaces::i_field::mouse_button(glm::vec<2, i32> position, f32 scroll, MouseOperation operation,
													MouseButton button)
{

}
