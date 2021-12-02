#include "mod_loader.hpp"

#include <rhr/handlers/project.hpp>
#include <rhr/registries/block.hpp>
#include <espresso/mod/data.hpp>

//TODO temp
//#include <RHR/config.h>
#include <iostream>
#include <filesystem>
#if LINUX
#include <dlfcn.h>
#else
#include <windows.h>
#endif

// TODO: fix case

std::vector<RegMod>* mods;

void registerMod(const std::string& fileName, const std::string& fileType)
{
	for (u32 i = 0; i < mods->size(); i++)
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

ModLoaderStatus run()
{
	rhr::handler::project::mods.clear();
	typedef void(*f_initialize)(esp::mod::data*);

	mods = new std::vector<RegMod>();

	std::cout << std::filesystem::current_path() << std::endl;

	try
	{
		std::filesystem::directory_iterator pathMods("mods");

		for (auto& file : pathMods)
			registerMod(file.path().stem().string(), file.path().extension().string());
	}
	catch (std::exception* e)
	{
		cap::logger::warn("no mods folder present; starting base editor");
	}

	for (u16 i = 0; i < mods->size(); i++)
	{
#if LINUX
		if (!(*mods)[i].Supported_LINUX)
		{
			cap::logger::warn("mod \"" + (*mods)[i].FileName + "\" does not support linux and can not be loaded");
			continue;
		}

		void* so = dlopen(std::string("mods/" + (*mods)[i].FileName + ".so").c_str(), RTLD_NOW);

		if (!so)
		{
			cap::logger::error("failed to load mod \"" + (*mods)[i].FileName + "\"");
			return ModLoaderStatus::ModLoaderStatus_ERROR;
		}

		f_initialize initialize = (f_initialize)dlsym(so, "Initialization");
		if (initialize == nullptr)
		{
			cap::logger::error("failed to load proper functions for mod \"" + (*mods)[i].FileName + "\"");
			return ModLoaderStatus::ModLoaderStatus_ERROR;
		}

		//dlclose(so);
#else
		if (!(*mods)[i].Supported_WIN)
		{
			cap::logger::warn("mod \"" + (*mods)[i].FileName + "\" does not support windows and can not be loaded");
			continue;
		}

		HINSTANCE hGetProcIDDLL = LoadLibrary(std::string("mods/" + (*mods)[i].FileName + ".dll").c_str());

		if (!hGetProcIDDLL)
		{
			cap::logger::error("failed to load mod \"" + (*mods)[i].FileName + "\"");
			return ModLoaderStatus::ModLoaderStatus_ERROR;
		}

		f_initialize initialize = (f_initialize)GetProcAddress(hGetProcIDDLL, "Initialization");
		if (initialize == nullptr)
		{
			cap::logger::error("failed to load proper functions for mod \"" + (*mods)[i].FileName + "\"");
			return ModLoaderStatus::ModLoaderStatus_ERROR;
		}
#endif
		initialize((*mods)[i].Data);
		
		rhr::handler::project::mods.push_back((*mods)[i].Data->get_mod_unlocalized_name());

		esp::mod::data* mod_data = (*mods)[i].Data;

		for (u32 j = 0; j < mod_data->get_categories().size(); j++)
			rhr::registry::block::get_registry().register_category(mod_data->get_categories()[j], mods->at(i).Data->get_mod_unlocalized_name());

		for (u32 j = 0; j < mod_data->get_blocks().size(); j++)
			rhr::registry::block::get_registry().register_block(mod_data->get_blocks()[j], mods->at(i).Data->get_mod_unlocalized_name());
	}

	return ModLoaderStatus::ModLoaderStatus_OK;
}
