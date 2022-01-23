#pragma once
#include "config.h"

#include <espresso/mod/data.hpp>

// TODO: fix case

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
		, Data(new esp::mod::data())
		, Supported_WIN(false)
		, Supported_LINUX(false)
	{}

	void SupportWIN() { Supported_WIN = true; }

	void SupportLINUX() { Supported_LINUX = true; }

	std::string FileName;
	esp::mod::data* Data;

	bool Supported_WIN;
	bool Supported_LINUX;
};

void registerMod(std::string& fileName, std::string& fileType);
ModLoaderStatus run();
