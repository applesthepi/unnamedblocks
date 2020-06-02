#include "Cappuccino.h"

#include "Registration.h"
#include "ModBlockPass.h"

CAP_CPP_EXPORT void cpInit()
{
	Registration::Initialize();
}

CAP_CPP_EXPORT void cpSetFunctionMain(uint64_t functionMain)
{
	Registration::SetFunctionMain(functionMain);
}

CAP_CPP_EXPORT void cpSetFunctionCallCount(uint64_t* functionCallCount)
{
	Registration::SetFunctionCallCount(functionCallCount);
}

CAP_CPP_EXPORT void cpSetFunctionTotalCount(uint64_t functionTotalCount)
{
	Registration::SetFunctionTotalCount(functionTotalCount);
}

CAP_CPP_EXPORT void cpSetCalls(executionFunctionStackList calls)
{
	Registration::SetCalls(calls);
}

CAP_CPP_EXPORT void cpSetData(ModBlockData** data)
{
	Registration::SetData(data);
}

CAP_CPP_EXPORT void cpSetBlocks(ModBlock*** blocks)
{
	Registration::SetBlocks(blocks);
}

CAP_CPP_EXPORT void cpSetDebugBuild(bool debugBuild)
{
	Registration::SetDebug(debugBuild);
}

CAP_CPP_EXPORT void cpSetSuper(uint8_t* super, int64_t* superData, void* superMutex)
{
	Registration::SetSuper(super, superData, superMutex);
}

CAP_CPP_EXPORT void cpRun()
{
	Registration::Run();
}
