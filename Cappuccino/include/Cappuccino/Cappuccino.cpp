#include "Cappuccino.h"
#include "Registration.h"

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
	Registration::Run();
}
