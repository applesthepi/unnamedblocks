#pragma once
#include "config.h"

#include <stdint.h>
//#include <stdbool.h>
#include <stdio.h>

#ifdef WIN32
	#ifdef __CAP
		#define CAP_CPP_EXPORT extern "C" __declspec(dllexport)
		#define TYPDEF_CPP_EXPORT extern "C"
		#define CAP_C_EXPORT __declspec(dllexport)
	#else
		#define CAP_CPP_EXPORT __declspec(dllimport)
		#define TYPDEF_CPP_EXPORT
		#define CAP_C_EXPORT __declspec(dllimport)
	#endif
#else
	#ifdef __CAP
		#define CAP_CPP_EXPORT extern "C"
		#define TYPDEF_CPP_EXPORT extern "C"
		#define CAP_C_EXPORT
	#else
		#define CAP_CPP_EXPORT 
		#define TYPDEF_CPP_EXPORT 
		#define CAP_C_EXPORT 
	#endif
#endif


TYPDEF_CPP_EXPORT typedef class ModBlockPass ModBlockPass;
TYPDEF_CPP_EXPORT typedef class ModBlockData ModBlockData;
TYPDEF_CPP_EXPORT typedef class ModBlock ModBlock;

TYPDEF_CPP_EXPORT typedef void(*executionFunction)(ModBlockPass*);
TYPDEF_CPP_EXPORT typedef void(**executionFunctionStack)(ModBlockPass*);
TYPDEF_CPP_EXPORT typedef void(***executionFunctionStackList)(ModBlockPass*);

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

