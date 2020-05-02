#include <stdio.h>
#include <Cappuccino/Cappuccino.h>

#ifndef LINUX
#define COMP_EXPORT extern __declspec(dllimport)
#else
#define COMP_EXPORT extern
#endif

COMP_EXPORT uint64_t functionMain;
COMP_EXPORT uint64_t* functionCallCount;
COMP_EXPORT uint64_t functionTotalCount;
COMP_EXPORT void (***calls)(ModBlockPass*);
COMP_EXPORT ModBlockData** functionData;
COMP_EXPORT ModBlock*** modBlocks;
COMP_EXPORT bool debugBuild;

int main()
{
	cpInit();

	cpSetFunctionMain(functionMain);
	cpSetFunctionCallCount(functionCallCount);
	cpSetFunctionTotalCount(functionTotalCount);
	cpSetCalls(calls);
	cpSetData(functionData);
	cpSetBlocks(modBlocks);
	cpSetDebugBuild(debugBuild);

	cpRun();

	return 0;
}
