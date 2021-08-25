#pragma once
#include "config.h"

#if 0

#include "cap_config.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

//#if !LINUX
//	#ifdef __cplusplus
//		#define CAP_EXPORT extern "C" __declspec(dllexport)
//		#define TYPDEF_EXPORT extern "C"
//	#else
//		#define CAP_EXPORT extern __declspec(dllimport)
//		#define TYPDEF_EXPORT
//	#endif
//#else
//	#ifdef __CAP
//		#define CAP_EXPORT extern "C"
//		#define TYPDEF_EXPORT extern "C"
//	#else
//		#define CAP_EXPORT
//		#define TYPDEF_EXPORT
//	#endif
//#endif

//#define CAP_EXPORT
#define TYPDEF_EXPORT

typedef struct ModBlockPass ModBlockPass;
TYPDEF_EXPORT typedef struct ModBlockData ModBlockData;
TYPDEF_EXPORT typedef struct ModBlock ModBlock;

TYPDEF_EXPORT typedef void(*executionFunction)(ModBlockPass*);
TYPDEF_EXPORT typedef void(**executionFunctionStack)(ModBlockPass*);
TYPDEF_EXPORT typedef void(***executionFunctionStackList)(ModBlockPass*);

#ifdef __cplusplus
extern "C" {
#endif

// init
CAP_EXPORT void cpInit();

// set main entry function
CAP_EXPORT void cpSetFunctionMain(uint64_t functionMain);

// set the functionCallCount
CAP_EXPORT void cpSetFunctionCallCount(uint64_t* functionCallCount);

// set the total count of all stacks
CAP_EXPORT void cpSetFunctionTotalCount(uint64_t functionTotalCount);

// set the calls
CAP_EXPORT void cpSetCalls(executionFunctionStackList calls);

// set the data
CAP_EXPORT void cpSetData(ModBlockData** data);

// set the ModBlocks
CAP_EXPORT void cpSetBlocks(ModBlock*** blocks);

// set the build type
CAP_EXPORT void cpSetDebugBuild(bool debugBuild);

// set the super instruction; used for debugging instructions from UB exe
CAP_EXPORT void cpSetSuper(uint8_t* super, int64_t* superData, void* superMutex);

// run
CAP_EXPORT void cpRun();
#ifdef __cplusplus
}
#endif

#endif