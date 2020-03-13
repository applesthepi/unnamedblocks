//#include "ModLoader.h"
#include <RHR/config.h>
#include <RHR/RHR.h>
#include <SFML/Graphics.hpp>
#include <Espresso/Logger.h>
#include <chrono>

//#include "content/ContentLoader.h"
#include <GL/glew.h>
//#include <stdio.h> 
//typedef int(__cdecl* MYPROC)(LPWSTR);
#include <RHR/RHR.h>

#ifdef LINUX
#include <X11/Xlib.h>
#endif

#include <iostream>
#include <cstring>
#include <vector>

//#include <dlfcn.h>
#include <windows.h>

#define UB_VERSION "Unnamed Blocks v0.0b2"
#define UB_BETA_BUILD true

#define CONTEXT_COLOR 180, 180, 180, 200

std::vector<Button*> contextButtons;
std::vector<Button*> toolbarButtons;
std::vector<Button*> catButtons;

ContextSystem sys;

static Plane* toolbarPlane;
static unsigned char toolbarCatagory = 0;
static unsigned short toolbarStackCount = 0;

static void ReloadCatagory(uint16_t index, BlockRegistry* registry)
{
	toolbarPlane->DeleteAllBlocks();

	unsigned int idx = 0;
	unsigned int widest = 0;

	for (unsigned int a = 0; a < registry->GetBlocks().size(); a++)
	{
		if (registry->GetBlocks()[a].GetCategory() == registry->GetCategories()[index].GetUnlocalizedName())
		{
			Stack* stack = new Stack(sf::Vector2i(5, 5 + (idx * (Global::BlockHeight + 5))), registry);
			Block* block = new Block(registry->GetBlocks()[a].GetUnlocalizedName(), registry, stack->GetFunctionUpdate(), stack->GetFunctionSelect());

			toolbarPlane->AddStack(stack);
			stack->AddBlock(block);

			if (block->GetWidth() > widest)
			{
				widest = block->GetWidth();
			}

			idx++;
		}
	}

	if (widest == 0)
		Global::ToolbarWidth = 100;
	else
		Global::ToolbarWidth = widest + 10;

	toolbarCatagory = index;
	toolbarStackCount = toolbarPlane->GetStackCount();
}

int main()
{
	/*
	typedef void(*f_initialize)(int*);
	system("tcc comp.c -shared -o comp.so");
	
	int* re = new int();
	*re = 5;

	void* so = dlopen("./comp.so", RTLD_NOW);
	if(so == nullptr) {
		printf("%s\n", dlerror());
		static_assert("killme");
	}
	f_initialize initialize = (f_initialize)dlsym(so, "re");
	initialize(re);
	
	Logger::Debug(std::to_string(*re));

	system("pause");

	return 0;
	*/


	typedef void(*f_initialize)(int*);
	system("call compile.bat");

	int* re = new int();
	*re = 5;

	HINSTANCE dll = LoadLibrary("./comp.dll");
	if (dll == nullptr) {
		Logger::Error("REEE 1");
		system("pause");
		return 0;
	}
	f_initialize initialize = (f_initialize)GetProcAddress(dll, "re");
	if (initialize == nullptr) {
		Logger::Error("REEE 2");
		system("pause");
		return 0;
	}

	initialize(re);

	Logger::Debug(std::to_string(*re));

	system("pause");
	return 0;
}
