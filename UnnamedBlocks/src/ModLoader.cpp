#include <windows.h>
#include <iostream>

#include "ModLoader.h"

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