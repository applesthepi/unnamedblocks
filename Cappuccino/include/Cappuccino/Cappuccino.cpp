#include "Cappuccino.h"
#include "Registration.h"
#include "ModBlockPass.h"

#ifdef __cplusplus
extern "C"
{
#endif

	void cpSetFunctionCallCount(uint64_t* functionCallCount)
	{
		Registration::SetFunctionCallCount(functionCallCount);
	}

	void cpSetCalls(executionFunctionStackList calls)
	{
		Registration::SetCalls(calls);
	}

	void cpSetDebugBuild(bool debugBuild)
	{
		Registration::SetDebug(debugBuild);
	}

	void cpRun()
	{
		Registration::Initialize();
		Registration::Run();
	}

	void cpSetFunctionMain(uint64_t functionMain)
	{
		Registration::SetFunctionMain(functionMain);
	}

#ifdef __cplusplus
}
#endif