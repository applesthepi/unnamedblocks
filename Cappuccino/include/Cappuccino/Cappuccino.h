#pragma once
#include "config.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#ifndef LINUX
	#ifdef __CAP
		#define CAP_EXPORT extern "C" __declspec(dllexport)
		#define TYPDEF_EXPORT extern "C"
	#else
		#define CAP_EXPORT __declspec(dllimport)
		#define TYPDEF_EXPORT
	#endif
#else
	#ifdef __CAP
		#define CAP_EXPORT extern "C"
		#define TYPDEF_EXPORT extern "C"
	#else
		#define CAP_EXPORT 
		#define TYPDEF_EXPORT 
	#endif
#endif

TYPDEF_EXPORT typedef struct ModBlockPass ModBlockPass;
TYPDEF_EXPORT typedef struct ModBlockData ModBlockData;

TYPDEF_EXPORT typedef void(*executionFunction)(ModBlockPass*);
TYPDEF_EXPORT typedef void(**executionFunctionStack)(ModBlockPass*);
TYPDEF_EXPORT typedef void(***executionFunctionStackList)(ModBlockPass*);

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

// set the build type
CAP_EXPORT void cpSetDebugBuild(bool debugBuild);

// run
CAP_EXPORT void cpRun();

