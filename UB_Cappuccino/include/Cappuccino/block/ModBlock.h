#pragma once
#include "ModBlockFlags.h"
#include "ModBlockActions.h"
#include "../ModBlockPass.h"
#include "../runtime/ModBlockData.h"

#include <vector>
#include <string>

#ifndef LINUX
#ifdef __CAP
#define CAP_DLL __declspec(dllexport)
#else
#define CAP_DLL __declspec(dllimport)
#endif
#else
#define CAP_DLL
#endif

/*
Important Modding Information

- Do NOT use the "Logger" class during runtime! Use the logger methods inside of the ModBlockPass.

*/

enum class BlockArgumentType
{
	/*
	This is the argument type, an argument can only allow this variable type to be used.

	"ANY" is indicated for any *variable* input. The type will unspecified in Cappuccino and will be presented as a "void*"
	*/
	TEXT, REAL, BOOL, STRING, ANY
};

enum class BlockArgumentVariableModeRestriction
{
	/*
	This controls how the input is given to the mod

	NONE          - excepts either raw or var, raw will be raw, var will be the value
	RESTRICTED    - does not change from initial "BlockArgumentVariableMode"
	*/
	NONE, RESTRICTED
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
typedef bool(*blockInitialization)(PreProcessorData&);
typedef bool(*blockDataInitialization)(PreProcessorData&, ModBlockData&);

class ModBlock
{
public:
	// how the block will be identified as. Convention example: "mymod_some_block_name"
	CAP_DLL virtual const char* GetUnlocalizedName() const;

	// category of the block
	CAP_DLL virtual const char* GetCategory() const;

	// does the block start a stack
	CAP_DLL virtual bool IsTopical() const;

	// what will be executed in a release build
	CAP_DLL virtual blockExecution PullExecuteRelease() const;

	// what will be executed in a debug build
	CAP_DLL virtual blockExecution PullExecuteDebug() const;
	
	// actions to run once per block type; during runtime before Initialization
	CAP_DLL virtual blockInitialization GetRuntimeGlobalPreInit() const;

	// actions to run once per block type; during runtime after Initialization
	CAP_DLL virtual blockInitialization GetRuntimeGlobalPostInit() const;
	
	// actions to run once per block type per stack; during runtime before Initialization
	CAP_DLL virtual blockInitialization GetRuntimeLocalPreInit() const;

	// actions to run once per block type per stack; during runtime after Initialization
	CAP_DLL virtual blockInitialization GetRuntimeLocalPostInit() const;

	// actions to run on each block preprocessed; during Initialization
	CAP_DLL virtual std::vector<std::pair<blockDataInitialization, uint16_t>> GetRuntimeStages() const;
	
	// block arguments, including text
	CAP_DLL virtual const std::vector<BlockArgumentInitializer> GetArguments() const;
};
