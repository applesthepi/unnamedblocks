#include "argument.hpp"

i32 esp::argument::padding = 8;

esp::argument::state::state()
	: parent(nullptr)
	, plane_offset(nullptr)
	, function_collection_update(nullptr)
	, mode(mode::RAW)
	, mode_restriction(mode_restriction::NONE)
	, custom(nullptr)
{
}

void esp::argument::toggle_mode(argument::state* state)
{
	if (state->mode == argument::mode::RAW)
		state->mode = argument::mode::VAR;
	else
		state->mode = argument::mode::RAW;

	(*state->function_collection_update)();
	on_set_mode(state);
}
