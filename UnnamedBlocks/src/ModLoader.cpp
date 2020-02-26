#include "ModLoader.h"
#include "config.h"
#include <iostream>
#include <boost/filesystem.hpp>

#ifdef LINUX
#include <dlfcn.h>
#else
#include <windows.h>
#endif

std::vector<RegMod>* mods;

void registerMod(std::string& fileName, std::string& fileType)
{
	for (uint32_t i = 0; i < mods->size(); i++)
	{
		if ((*mods)[i].FileName == fileName)
		{
			if (fileType == ".dll")
				(*mods)[i].SupportWIN();
			else if (fileType == ".so")
				(*mods)[i].SupportLINUX();

			return;
		}
	}
	
	if (fileType != ".dll" && fileType != ".so")
		return;

	RegMod mod(fileName);

	if (fileType == ".dll")
		mod.SupportWIN();
	else if (fileType == ".so")
		mod.SupportLINUX();

	mods->push_back(mod);
}

ModLoaderStatus run(ByteHandler* byte, ObjectHandler* object, RuntimeHandler* runtime, ThreadHandler* thread, VariableHandler* variable, BlockRegistry* registry)
{
	typedef void(*f_initialize)(ModData*);

	mods = new std::vector<RegMod>();
	boost::filesystem::path pathMods("mods/");

	for (auto& entry : boost::filesystem::directory_iterator(pathMods))//https://stackoverflow.com/questions/4430780/how-can-i-extract-the-file-name-and-extension-from-a-path-in-c
	{
		std::string na = entry.path().stem().string();
		std::string ex = entry.path().extension().string();
		registerMod(na, ex);
	}

	for (uint16_t i = 0; i < mods->size(); i++)
	{
#ifdef LINUX
		if (!(*mods)[i].Supported_LINUX)
		{
			Logger::Warn("mod \"" + (*mods)[i].FileName + "\" does not support linux and can not be loaded");
			continue;
		}

		void* so = dlopen(std::string("mods/" + (*mods)[i].FileName + ".so").c_str(), RTLD_NOW);

		if (!so)
		{
			Logger::Error("failed to load mod \"" + (*mods)[i].FileName + "\"");
			return ModLoaderStatus::ModLoaderStatus_ERROR;
		}

		f_initialize initialize = (f_initialize)dlsym(so, "Initialization");
		if (initialize == nullptr)
		{
			Logger::Error("failed to load proper functions for mod \"" + (*mods)[i].FileName + "\"");
			return ModLoaderStatus::ModLoaderStatus_ERROR;
		}

		//dlclose(so);
#else
		if (!(*mods)[i].Supported_WIN)
		{
			Logger::Warn("mod \"" + (*mods)[i].FileName + "\" does not support windows and can not be loaded");
			continue;
		}

		HINSTANCE hGetProcIDDLL = LoadLibrary(std::string("mods/" + (*mods)[i].FileName + ".dll").c_str());

		if (!hGetProcIDDLL)
		{
			Logger::Error("failed to load mod \"" + (*mods)[i].FileName + "\"");
			return ModLoaderStatus::ModLoaderStatus_ERROR;
		}

		f_initialize initialize = (f_initialize)GetProcAddress(hGetProcIDDLL, "Initialization");
		if (initialize == nullptr)
		{
			Logger::Error("failed to load proper functions for mod \"" + (*mods)[i].FileName + "\"");
			return ModLoaderStatus::ModLoaderStatus_ERROR;
		}
#endif
		(*mods)[i].Data->Object = object;
		(*mods)[i].Data->Byte = byte;
		(*mods)[i].Data->Runtime = runtime;
		(*mods)[i].Data->Thread = thread;
		(*mods)[i].Data->Variable = variable;
		(*mods)[i].Data->Registry = registry;

		initialize((*mods)[i].Data);

		std::vector<RegCatagory>* catagories = (*mods)[i].Data->GetCatagories();
		std::vector<RegBlock>* blocks = (*mods)[i].Data->GetBlocks();

		for (uint32_t i = 0; i < catagories->size(); i++)
			registry->RegisterCatagory(&(*catagories)[i]);

		for (uint32_t i = 0; i < blocks->size(); i++)
			registry->RegisterBlock(&(*blocks)[i], variable);
	}

	delete mods;
	return ModLoaderStatus::ModLoaderStatus_OK;
}
