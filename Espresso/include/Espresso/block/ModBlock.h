#pragma once
#include "ModBlockFlags.h"
#include "ModBlockActions.h"

#include <Cappuccino/ModBlockPass.h>
#include <vector>
#include <string>

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

class BlockArgumentInitializer
{
public:
	BlockArgumentInitializer(const BlockArgumentType type, const BlockArgumentVariableModeRestriction restriction, const BlockArgumentVariableMode mode, const std::string defaultValue)
		:Type(type), Restriction(restriction), Mode(mode), DefaultValue(defaultValue) {}

	BlockArgumentType Type;
	BlockArgumentVariableModeRestriction Restriction;
	BlockArgumentVariableMode Mode;
	std::string DefaultValue;
};

typedef void(*blockExecution)(ModBlockPass*);

class ModBlock
{
public:
	// how the block will be identified as. Convention example: "mymod_some_block_name"
	virtual const char* GetUnlocalizedName() const;

	// what will be executed in a release build
	virtual blockExecution PullExecuteRelease();

	// what will be executed in a debug build
	virtual blockExecution PullExecuteDebug();
	
	// category of the block
	virtual const char* GetCategory() const;
	
	// does the block start a stack
	virtual bool IsTopical() const;
	
	// flags to check during debug build
	virtual const ModBlockFlags GetFlags() const;
	
	// actions to run once per block type; during runtime preInitialization
	virtual const ModBlockActions RuntimePreInit() const;
	
	// actions to run once per block type on every stack; during runtime initialization
	virtual const ModBlockActions RuntimeInit() const;

	// actions to run on each block preprocessed; during postInitialization
	virtual const ModBlockActions RuntimePostInit() const;
	
	// block arguments, including text
	virtual const std::vector<BlockArgumentInitializer> GetArguments() const;
};
