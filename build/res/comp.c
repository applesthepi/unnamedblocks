#include <stdio.h>
#include <stdint.h>
#include <Cappuccino/ModBlockPass.h>

uint64_t functionMain;
uint64_t* functionCallCount;
void (***calls)(ModBlockPass*);
bool debugBuild;

int main()
{
	cpSetFunctionMain(functionMain);
	cpSetFunctionCallCount(functionCallCount);
	cpSetCalls(calls);
	cpSetDebugBuild(debugBuild);
	cpRun();

	return 0;
}