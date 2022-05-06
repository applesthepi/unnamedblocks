#include "mod_loader.hpp"

#include <espresso/mod_data.hpp>
#include <mocha/handlers/project.hpp>

// TODO temp
//#include <RHR/config.h>
#include <filesystem>
#include <iostream>
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

	mod.Data->set_logger_linkage(
		latte::logger::get_stream_system(), latte::logger::get_stream_editor(), latte::logger::get_stream_runtime());
	mod.Data->set_registry_esp(esp::registry::get());
	mod.Data->set_registry_char_texture(rhr::registry::char_texture::get());
	mod.Data->set_renderer(rhr::render::renderer::get());
	mod.Data->set_planes(&rhr::stack::plane::primary_plane, &rhr::stack::plane::toolbar_plane);

	mods->push_back(mod);
}

ModLoaderStatus run()
{
	rhr::handler::project::mods.clear();
	typedef void (*f_initialize)(esp::mod_data*);

	mods = new std::vector<RegMod>();

	std::cout << std::filesystem::current_path() << std::endl;

	//try
	//{
	//	std::filesystem::directory_iterator pathMods("mods");
	//
	//	for (auto& file : pathMods)
	//		registerMod(file.path().stem().string(), file.path().extension().string());
	//}
	//catch (std::exception* e)
	//{
	//	latte::logger::warn(latte::logger::level::SYSTEM, "no mods folder present; starting base editor");
	//}

	std::filesystem::directory_iterator pathMods("mods");

	for (auto& file : pathMods)
		registerMod(file.path().stem().string(), file.path().extension().string());

	for (u16 i = 0; i < mods->size(); i++)
	{
#if LINUX
		if (!(*mods)[i].Supported_LINUX)
		{
			latte::logger::warn(
				latte::logger::level::SYSTEM,
				"mod \"" + (*mods)[i].FileName + "\" does not support linux and can not be loaded");
			continue;
		}

		void* so = dlopen(std::string("mods/" + (*mods)[i].FileName + ".so").c_str(), RTLD_NOW);

		if (!so)
		{
			latte::logger::error(latte::logger::level::SYSTEM, "failed to load mod \"" + (*mods)[i].FileName + "\"");
			return ModLoaderStatus::ModLoaderStatus_ERROR;
		}

		f_initialize initialize = (f_initialize)dlsym(so, "initialization");
		if (initialize == nullptr)
		{
			latte::logger::error(
				latte::logger::level::SYSTEM, "failed to load proper functions for mod \"" + (*mods)[i].FileName + "\"");
			return ModLoaderStatus::ModLoaderStatus_ERROR;
		}

		// dlclose(so);
#else
		if (!(*mods)[i].Supported_WIN)
		{
			latte::logger::warn(
				latte::logger::level::SYSTEM,
				"mod \"" + (*mods)[i].FileName + "\" does not support windows and can not be loaded");
			continue;
		}

		HINSTANCE hGetProcIDDLL = LoadLibrary(std::string("mods/" + (*mods)[i].FileName + ".dll").c_str());

		if (!hGetProcIDDLL)
		{
			latte::logger::error(latte::logger::level::SYSTEM, "failed to load mod \"" + (*mods)[i].FileName + "\"");
			return ModLoaderStatus::ModLoaderStatus_ERROR;
		}

		f_initialize initialize = (f_initialize)GetProcAddress(hGetProcIDDLL, "initialization");
		if (initialize == nullptr)
		{
			latte::logger::error(
				latte::logger::level::SYSTEM, "failed to load proper functions for mod \"" + (*mods)[i].FileName + "\"");
			return ModLoaderStatus::ModLoaderStatus_ERROR;
		}
#endif
		initialize((*mods)[i].Data);
		esp::mod_data* mod_data = (*mods)[i].Data;

		rhr::handler::project::mods.push_back(mod_data->get_mod_unlocalized_name());
		rhr::handler::input::add_alt_stream(mod_data->get_stream_input());
	}

	return ModLoaderStatus::ModLoaderStatus_OK;
}