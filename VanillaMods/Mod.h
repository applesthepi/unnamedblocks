#pragma once
#ifdef LINUX
extern "C" int test() {
	return 5;
}
#else
int __declspec(dllexport) __stdcall test()
{
	return 5;
}
#endif
