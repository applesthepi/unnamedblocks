#pragma once
#include "ModBlockFlags.h"
#include "ModBlockActions.h"

#include <Cappuccino/ModBlockPass.h>
#include <Cappuccino/runtime/ModBlockData.h>
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
	*/
	NONE, ONLY_RAW, ONLY_VAR
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
	
	// flags to check during debug build
	//CAP_DLL virtual const ModBlockFlags GetFlags() const;
	
	// actions to run once per block type; during runtime before Initialization
	CAP_DLL virtual blockInitialization GetRuntimeGlobalPreInit() const;

	// actions to run once per block type; during runtime after Initialization
	CAP_DLL virtual blockInitialization GetRuntimeGlobalPostInit() const;
	
	// actions to run once per block type; during runtime before Initialization
	CAP_DLL virtual blockInitialization GetRuntimeLocalPreInit() const;

	// actions to run once per block type; during runtime after Initialization
	CAP_DLL virtual blockInitialization GetRuntimeLocalPostInit() const;

	// actions to run on each block preprocessed; during Initialization
	CAP_DLL virtual blockDataInitialization GetRuntimeInit() const;
	
	// block arguments, including text
	CAP_DLL virtual const std::vector<BlockArgumentInitializer> GetArguments() const;
};
