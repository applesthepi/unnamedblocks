#include <stdio.h>
#include <Cappuccino/Cappuccino.h>

extern uint64_t functionMain;
extern uint64_t* functionCallCount;
extern void (***calls)(ModBlockPass*);
extern bool debugBuild;

int main()
{
	cpSetFunctionMain(functionMain);
	cpSetFunctionCallCount(functionCallCount);
	cpSetCalls(calls);
	cpSetDebugBuild(debugBuild);
	cpRun();

	return 0;
}
