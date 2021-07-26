#include "cappuccino.hpp"

#include "cappuccino/registration.hpp"
#include "cappuccino/mod_block/pass.hpp"

CAP_CPP_EXPORT void cpInit()
{
	Registration::Initialize();
}

CAP_CPP_EXPORT void cpSetFunctionMain(u64 functionMain)
{
	Registration::SetFunctionMain(functionMain);
}

CAP_CPP_EXPORT void cpSetFunctionCallCount(u64* functionCallCount)
{
	Registration::SetFunctionCallCount(functionCallCount);
}

CAP_CPP_EXPORT void cpSetFunctionTotalCount(u64 functionTotalCount)
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

CAP_CPP_EXPORT void cpSetSuper(u8* super, i64* superData, void* superMutex)
{
	Registration::SetSuper(super, superData, superMutex);
}

CAP_CPP_EXPORT void cpRun()
{
	Registration::Run();
}
