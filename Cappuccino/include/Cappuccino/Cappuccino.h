#ifndef H_Cap
#define H_Cap

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define CAP_EXPORT __declspec(dllexport)

#ifdef __cplusplus
extern "C"
{
#endif
	typedef struct ModBlockPass ModBlockPass;

	typedef void(*executionFunction)(ModBlockPass*);
	typedef void(**executionFunctionStack)(ModBlockPass*);
	typedef void(***executionFunctionStackList)(ModBlockPass*);

	// set main entry function
	CAP_EXPORT void cpSetFunctionMain(uint64_t functionMain);

	// set the functionCallCount
	CAP_EXPORT void cpSetFunctionCallCount(uint64_t* functionCallCount);

	// set the calls
	CAP_EXPORT void cpSetCalls(executionFunctionStackList calls);

	// set the build type
	CAP_EXPORT void cpSetDebugBuild(bool debugBuild);

	// run
	CAP_EXPORT void cpRun()
	{
		printf("re\n");
	}
#ifdef __cplusplus
}
#endif
#endif