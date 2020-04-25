#include "Cappuccino.h"

#include "Registration.h"
#include "ModBlockPass.h"

CAP_EXPORT void cpSetFunctionMain(uint64_t functionMain)
{
	Registration::SetFunctionMain(functionMain);
}

CAP_EXPORT void cpSetFunctionCallCount(uint64_t* functionCallCount)
{
	Registration::SetFunctionCallCount(functionCallCount);
}

CAP_EXPORT void cpSetCalls(executionFunctionStackList calls)
{
	Registration::SetCalls(calls);
}

CAP_EXPORT void cpSetDebugBuild(bool debugBuild)
{
	Registration::SetDebug(debugBuild);
}

CAP_EXPORT void cpRun()
{
	Registration::Initialize();
	Registration::Run();


}
