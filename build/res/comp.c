#include <stdio.h>
#include <stdint.h>
#include <Espresso/mod/ModBlockPass.h>

uint64_t functionCount;
uint64_t* functionCallCount;

void (***calls)(ModBlockPass*);

int main()
{
	printf("testing");
	return 0;
}