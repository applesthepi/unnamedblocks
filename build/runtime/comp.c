// ==================[ WINDOWS

/*
#include <stdio.h>
#include <windows.h>

__declspec(dllexport) void re(int* num)
{
	*num += 10;
}
*/

// ==================[ LINUX

#include <stdio.h>

extern void re(int* number)
{
	*number += 10;
}
