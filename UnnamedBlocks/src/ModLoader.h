#pragma once
#include <string>
#include <vector>
#include <RHR/RHR.h>

enum class ModLoaderStatus
{
	ModLoaderStatus_OK, ModLoaderStatus_ERROR
};

class RegMod
{
public:
	RegMod(const std::string fileName)
		:Supported_WIN(false), Supported_LINUX(false), Data(new ModData(BlockRegistry::MainRegistry)), FileName(fileName) {}

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

void registerMod(const std::string& fileName, const std::string& fileType);
ModLoaderStatus run();