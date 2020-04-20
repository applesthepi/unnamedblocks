#pragma once
#include "ModBlockPass.h"
#include "ExecutionThread.h"

extern "C"
{
	typedef struct ModBlockPass ModBlockPass;

	enum LOG_TYPE
	{
		LT_INFO,
		LT_WARNING,
		LT_ERROR
	};

	// set the functionCallCount
	void cpSetFunctionCallCount(uint64_t* functionCallCount);

	// set the calls
	void cpSetCalls(executionFunctionStackList calls);

	// set the build type
	void cpSetDebugBuild(bool debugBuild);

	// run
	void cpRun();
}