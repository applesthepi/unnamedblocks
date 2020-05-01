#include "ModLoader.h"
//TODO temp
//#include <RHR/config.h>
#include <iostream>
#include <filesystem>
#ifdef LINUX
#include <dlfcn.h>
#else
#include <windows.h>
#endif

std::vector<RegMod>* mods;

void registerMod(const std::string& fileName, const std::string& fileType)
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

ModLoaderStatus run(BlockRegistry* registry)
{
	ProjectHandler::Mods.clear();
	typedef void(*f_initialize)(ModData*);

	mods = new std::vector<RegMod>();

	std::filesystem::directory_iterator pathMods("mods");
	for(auto& file : pathMods)
		registerMod(file.path().stem().string(), file.path().extension().string());

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
		initialize((*mods)[i].Data);
		
		ProjectHandler::Mods.push_back((*mods)[i].Data->ModUnlocalizedName);

		ModDataBaked baked = (*mods)[i].Data->Bake();

		for (uint32_t j = 0; j < baked.CategoriesLength; j++)
			registry->RegisterCatagory(baked.Categories[j]);

		for (uint32_t j = 0; j < baked.BlocksLength; j++)
			registry->RegisterBlock(baked.Blocks[j]);

		for (uint32_t j = 0; j < baked.BlocksLength; j++)
			registry->RegisterExeDebug(baked.Blocks[j]->PullExecuteDebug());

		for (uint32_t j = 0; j < baked.BlocksLength; j++)
			registry->RegisterExeRelease(baked.Blocks[j]->PullExecuteRelease());
	}

	//delete mods;
	return ModLoaderStatus::ModLoaderStatus_OK;
}
