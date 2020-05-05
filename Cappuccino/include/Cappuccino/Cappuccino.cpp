#include "Cappuccino.h"

#include "Registration.h"
#include "ModBlockPass.h"

CAP_EXPORT void cpInit()
{
	Registration::Initialize();
}

CAP_EXPORT void cpSetFunctionMain(uint64_t functionMain)
{
	Registration::SetFunctionMain(functionMain);
}

CAP_EXPORT void cpSetFunctionCallCount(uint64_t* functionCallCount)
{
	Registration::SetFunctionCallCount(functionCallCount);
}

CAP_EXPORT void cpSetFunctionTotalCount(uint64_t functionTotalCount)
{
	Registration::SetFunctionTotalCount(functionTotalCount);
}

CAP_EXPORT void cpSetCalls(executionFunctionStackList calls)
{
	Registration::SetCalls(calls);
}

CAP_EXPORT void cpSetData(ModBlockData** data)
{
	Registration::SetData(data);
}

CAP_EXPORT void cpSetBlocks(ModBlock*** blocks)
{
	Registration::SetBlocks(blocks);
}

CAP_EXPORT void cpSetDebugBuild(bool debugBuild)
{
	Registration::SetDebug(debugBuild);
}

CAP_EXPORT void cpSetSuper(uint8_t* super, void* superMutex)
{
	Registration::SetSuper(super, superMutex);
}

CAP_EXPORT void cpRun()
{
	Registration::Run();
}
