#pragma once

enum class BlockArgumentType
{
	/*
	This is the argument type, an argument can only allow this variable type to be used.
	*/
	TEXT, REAL, BOOL, STRING
};

enum class BlockArgumentVariableModeRestriction
{
	/*
	This controls how the input is given to the mod

	NONE          - excepts either raw or var, raw will be raw, var will be the value
	ONLY_RAW      - excepts only raw         , raw will be raw
	ONLY_VAR      - excepts only var         , var will be the value
	ONLY_VAR_KEEP - excepts only var         , var will be the variable name (inputed) - this is for if you want to "show"
																						 to the user that you expect a variable.
	*/
	NONE, ONLY_RAW, ONLY_VAR, ONLY_VAR_KEEP
};

enum class BlockArgumentVariableMode
{
	/*
	This is the default assignment of whether it starts as var or raw input. The user can change what they input.
	If you want to restrict how data is inputed see the "BlockArgumentVariableModeRestriction" class.
	*/
	VAR, RAW
};