#include <iostream>
#include "ModLoader.h"

#define LINUX
#ifdef LINUX
#include <dlfcn.h>
void run()
{
	typedef int(*f_test)();
	{
		void* so = dlopen("mods/libVinMod.so", RTLD_NOW);

		if (!so) {
			std::cout << "could not load the dynamic library" << std::endl;
			return;
		}

		// resolve function address here
		f_test test = (f_test) dlsym(so, "test");
		if (test == nullptr) {
			std::cout << "could not locate the function" << std::endl;
			return;
		}

		std::cout << "test() returned " << test() << std::endl;
	}
}
#else

#include <windows.h>

void run()
{
	typedef int(__stdcall* f_test)();

	{
		HINSTANCE hGetProcIDDLL = LoadLibrary("mods/ModVin.dll");

		if (!hGetProcIDDLL) {
			std::cout << "could not load the dynamic library" << std::endl;
			return;
		}

		// resolve function address here
		f_test test = (f_test)GetProcAddress(hGetProcIDDLL, "test");
		if (test == nullptr) {
			std::cout << "could not locate the function" << std::endl;
			return;
		}

		std::cout << "test() returned " << test() << std::endl;
	}
}
#endif
