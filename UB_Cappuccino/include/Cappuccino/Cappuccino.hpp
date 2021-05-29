#pragma once
#include "config.h"

#include "Utils.hpp"
#include "Export.hpp"

typedef class ModBlockPass ModBlockPass;
typedef class ModBlockData ModBlockData;
typedef class ModBlock ModBlock;

typedef void(*executionFunction)(ModBlockPass*);
typedef void(**executionFunctionStack)(ModBlockPass*);
typedef void(***executionFunctionStackList)(ModBlockPass*);

// init
CAP_CPP_EXPORT void cpInit();

// set main entry function
CAP_CPP_EXPORT void cpSetFunctionMain(uint64_t functionMain);

// set the functionCallCount
CAP_CPP_EXPORT void cpSetFunctionCallCount(uint64_t* functionCallCount);

// set the total count of all stacks
CAP_CPP_EXPORT void cpSetFunctionTotalCount(uint64_t functionTotalCount);

// set the calls
CAP_CPP_EXPORT void cpSetCalls(executionFunctionStackList calls);

// set the data
CAP_CPP_EXPORT void cpSetData(ModBlockData** data);

// set the ModBlocks
CAP_CPP_EXPORT void cpSetBlocks(ModBlock*** blocks);

// set the build type
CAP_CPP_EXPORT void cpSetDebugBuild(bool debugBuild);

// set the super instruction; used for debugging instructions from UB exe
CAP_CPP_EXPORT void cpSetSuper(uint8_t* super, int64_t* superData, void* superMutex);

// run
CAP_CPP_EXPORT void cpRun();

