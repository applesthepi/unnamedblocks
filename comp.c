#include <stdio.h>
#include <windows.h>

__declspec(dllexport) void re(int* num)
{
	*num += 10;
}