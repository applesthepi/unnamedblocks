#include <stdio.h>
#include <cappuccino/cappuccino.h>

#if !LINUX
#define COMP_EXPORT extern __declspec(dllimport)
#else
#define COMP_EXPORT extern
#endif

COMP_EXPORT uint64_t functionMain;
COMP_EXPORT uint64_t* functionCallCount;
COMP_EXPORT uint64_t functionTotalCount;
COMP_EXPORT void (***calls)(ModBlockPass*);
COMP_EXPORT cap::mod::block::data** functionData;
COMP_EXPORT ModBlock*** modBlocks;
COMP_EXPORT bool debugBuild;
COMP_EXPORT uint8_t* superInstruction;
COMP_EXPORT int64_t* superData;
COMP_EXPORT void* superMutex;

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
	cpSetSuper(superInstruction, superData, superMutex);

	cpRun();

	return 0;
}
