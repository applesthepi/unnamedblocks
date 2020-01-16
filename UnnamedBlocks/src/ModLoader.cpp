#include "ModLoader.h"

#include <iostream>
#include <RHR/RHR.h>
#include <windows.h>

//#define LINUX
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

void run()
{
	ModData* data = new ModData();

	typedef void(*f_test)(ModData*);

	{
		HINSTANCE hGetProcIDDLL = LoadLibrary("mods/ModVin.dll");

		if (!hGetProcIDDLL)
		{
			//failed to load mod
			return;//changeme
		}

		// resolve function address here
		f_test test = (f_test)GetProcAddress(hGetProcIDDLL, "Initialization");
		if (test == nullptr)
		{
			//can not locate function
			return;//changeme
		}

		test(data);
		//result
	}

	BlockRegistry::RegisterCatagory(&(*data->GetCatagories())[0]);
	BlockRegistry::RegisterBlock(&(*data->GetBlocks())[0]);
}
#endif
