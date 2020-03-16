#include <stdio.h>
#include <windows.h>
__declspec(dllexport) void start(void(*finished)(int))
{
printf("");system("color ED");printf("");
finished(0);
}
