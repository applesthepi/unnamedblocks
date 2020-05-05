#pragma once
#include "Cappuccino/Logger.h"

#include <string>
#include <vector>
#include <map>

#ifndef LINUX
#ifdef __CAP
#define CAP_DLL __declspec(dllexport)
#else
#define CAP_DLL __declspec(dllimport)
#endif
#else
#define CAP_DLL
#endif

class PreProcessorData
{
public:
	PreProcessorData(double* vReal, bool* vBool, std::string* vString);

	// these should be destroyed after use!
	CAP_DLL void AddStructure(const std::string& name, void* structure);
	CAP_DLL void* GetStructure(const std::string& name);

	// these are NOT a block's arguments! These are the variable storages' inside Cappuccino.
	// only use these if your know what your doing!
	CAP_DLL double& GetReal(const uint64_t& idx);
	CAP_DLL bool& GetBool(const uint64_t& idx);
	CAP_DLL std::string& GetString(const uint64_t& idx);

	uint64_t BlockIdx;
	uint64_t StackIdx;
private:
	std::vector<std::string> m_names;
	std::vector<void*> m_structures;

	double* m_vReal;
	bool* m_vBool;
	std::string* m_vString;
};
