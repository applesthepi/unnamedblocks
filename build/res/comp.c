#include <Cappuccino/Cappuccino.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

uint64_t functionMain;
uint64_t* functionCallCount;
void (***calls)(ModBlockPass*);
bool debugBuild;

int main()
{
	printf("running");
	
	cpSetFunctionMain(functionMain);
	cpSetFunctionCallCount(functionCallCount);
	cpSetCalls(calls);
	cpSetDebugBuild(debugBuild);
	cpRun();
	
	return 0;
}