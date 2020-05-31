#pragma once
#include <string>
#include <vector>
#include <RHR/RHR.h>
#include <Espresso/mod/ModData.h>

enum class ModLoaderStatus
{
	ModLoaderStatus_OK, ModLoaderStatus_ERROR
};

class RegMod
{
public:
	RegMod(const std::string fileName)
		: FileName(fileName), Data(new ModData()), Supported_WIN(false), Supported_LINUX(false) {}

	void SupportWIN()
	{
		Supported_WIN = true;
	}

	void SupportLINUX()
	{
		Supported_LINUX = true;
	}

	std::string FileName;
	ModData* Data;

	bool Supported_WIN;
	bool Supported_LINUX;
};

void registerMod(std::string& fileName, std::string& fileType);
ModLoaderStatus run();
