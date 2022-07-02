#include "rt_argument.hpp"

i32 esp::rt_argument::padding = 8;

esp::rt_argument::state::state()
	: parent(nullptr)
	, plane_offset(nullptr)
	, function_collection_update(nullptr)
	, mode(mode::RAW)
	, mode_restriction(mode_restriction::NONE)
	, custom(nullptr)
{
}

void esp::rt_argument::toggle_mode(rt_argument::state* state)
{
	if (state->mode == rt_argument::mode::RAW)
		state->mode = rt_argument::mode::VAR;
	else
		state->mode = rt_argument::mode::RAW;

	(*state->function_collection_update)();
	on_set_mode(state);
}

bool esp::rt_argument::check_bounds(esp::rt_argument::state* state, glm::vec<2, i32> position)
{
	auto& virtual_absolute = state->parent->get_position_virtual_absolute();

	return position.x >= virtual_absolute.x && position.x < virtual_absolute.x + get_width(state) &&
		position.y >= virtual_absolute.y && position.y < virtual_absolute.y + BLOCK_HEIGHT_CONTENT;
}
