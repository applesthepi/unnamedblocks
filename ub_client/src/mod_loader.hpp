#pragma once
#include "config.h"

#include <espresso/mod_data.hpp>

namespace loader
{
///
enum class status
{
	OK, ERROR
};

///
enum class support
{
	WINDOWS, UNIX
};

///
struct mod
{
	///
	support mod_support;

	///
};

///
status load_mods();
}

enum class ModLoaderStatus
{
	ModLoaderStatus_OK,
	ModLoaderStatus_ERROR
};

class RegMod
{
public:
	RegMod(const std::string fileName)
		: FileName(fileName)
		, Data(new esp::mod_data())
		, Supported_WIN(false)
		, Supported_LINUX(false)
	{}

	void SupportWIN()
	{
		Supported_WIN = true;
	}

	void SupportLINUX()
	{
		Supported_LINUX = true;
	}

	std::string FileName;
	esp::mod_data* Data;

	bool Supported_WIN;
	bool Supported_LINUX;
};

void registerMod(std::string& fileName, std::string& fileType);
ModLoaderStatus run();
