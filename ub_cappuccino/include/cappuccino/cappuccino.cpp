#if 0

#include "cappuccino.h"
#include "cappuccino/registration.hpp"
#include "cappuccino/mod_block/pass.hpp"

extern "C"
{
	void cpInit()
	{
		Registration::Initialize();
	}

	void cpSetFunctionMain(u64 functionMain)
	{
		Registration::SetFunctionMain(functionMain);
	}

	void cpSetFunctionCallCount(u64* functionCallCount)
	{
		Registration::SetFunctionCallCount(functionCallCount);
	}

	void cpSetFunctionTotalCount(u64 functionTotalCount)
	{
		Registration::SetFunctionTotalCount(functionTotalCount);
	}

	void cpSetCalls(executionFunctionStackList calls)
	{
		Registration::SetCalls(calls);
	}

	void cpSetData(ModBlockData** data)
	{
		Registration::SetData(data);
	}

	void cpSetBlocks(ModBlock*** blocks)
	{
		Registration::SetBlocks(blocks);
	}

	void cpSetDebugBuild(bool debugBuild)
	{
		Registration::SetDebug(debugBuild);
	}

	void cpSetSuper(u8* super, i64* superData, void* superMutex)
	{
		Registration::SetSuper(super, superData, superMutex);
	}

	void cpRun()
	{
		Registration::Run();
	}
}
#endif