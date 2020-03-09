#pragma once
#include "ModBlockFlags.h"

#include "ModBlockActions.h"
#include <vector>
#include <RHR/include/RHR/registries/BlockRegistry.h>

class ModBlock
{
public:
	// the path to the block definition
	virtual const char* UsePath();
	
	// does the block start a stack
	virtual bool IsTopical();
	
	// flags to check during debug build
	virtual ModBlockFlags GetFlags();
	
	// actions to run once per block type during runtime preinitialization
	virtual ModBlockActions RuntimePreInit();
	
	// actions to run on each block preprocessed during initialization
	virtual ModBlockActions RuntimeInit();
	
	// block arguments, including text
	virtual std::vector<BlockArgumentInitializer> GetArguments();
};
