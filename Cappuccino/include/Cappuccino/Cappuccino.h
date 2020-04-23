#ifndef H_Cap
#define H_Cap

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C"
{
#endif
	/*
	enum LOG_TYPE
	{
		LT_INFO,
		LT_WARNING,
		LT_ERROR
	};*/

	typedef struct ModBlockPass ModBlockPass;

	typedef void(*executionFunction)(ModBlockPass*);
	typedef void(**executionFunctionStack)(ModBlockPass*);
	typedef void(***executionFunctionStackList)(ModBlockPass*);

	// set main entry function
	void cpSetFunctionMain(uint64_t functionMain);

	// set the functionCallCount
	void cpSetFunctionCallCount(uint64_t* functionCallCount);

	// set the calls
	void cpSetCalls(executionFunctionStackList calls);

	// set the build type
	void cpSetDebugBuild(bool debugBuild);

	// run
	void cpRun()
	{
		printf("re\n");
	}

#ifdef __cplusplus
}
#endif
#endif