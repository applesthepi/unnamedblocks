#pragma once
#include "config.h"

#include "cappuccino/utils.hpp"

#if !LINUX
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
	PreProcessorData(std::vector<void*>& vCustom);

	// these should be destroyed after use!
	CAP_DLL void AddStructure(const std::string& name, void* structure);
	CAP_DLL void* GetStructure(const std::string& name);

	// these are NOT a blocks' arguments! These are the variables' storage inside Cappuccino.
	// only use these if your know what your doing!
	// if your trying to use the  blocks' arguments during the initializations, then use the ModBlockData, not this.
	//CAP_DLL f64& GetReal(u64 idx);
	//CAP_DLL bool& GetBool(u64 idx);
	//CAP_DLL std::string& GetString(u64 idx);

	CAP_DLL u64 CustomPut(void* mem);

	u64 BlockIdx;
	u64 StackIdx;
private:
	std::vector<std::string> m_names;
	std::vector<void*> m_structures;

	const std::vector<f64*>& m_vReal;
	const std::vector<bool*>& m_vBool;
	const std::vector<std::string*>& m_vString;

	std::vector<void*>& m_vCustom;
};
