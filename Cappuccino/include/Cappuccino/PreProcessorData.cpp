#include "PreProcessorData.h"
#include "Cappuccino/Logger.h"

PreProcessorData::PreProcessorData(double* vReal, bool* vBool, std::string* vString, std::vector<void*>& vCustom)
	:m_vReal(vReal), m_vBool(vBool), m_vString(vString), m_vCustom(vCustom)
{
	BlockIdx = 0;
	StackIdx = 0;
}

void PreProcessorData::AddStructure(const std::string& name, void* structure)
{
	m_names.push_back(name);
	m_structures.push_back(structure);
}

void* PreProcessorData::GetStructure(const std::string& name)
{
	for (uint64_t i = 0; i < m_names.size(); i++)
	{
		if (m_names[i] == name)
			return m_structures[i];
	}

	Logger::Error("failed to locate structure \"" + name + "\"");
	return nullptr;
}
/*
CAP_DLL double& PreProcessorData::GetReal(const uint64_t& idx)
{
	return m_vReal[idx];
}

CAP_DLL bool& PreProcessorData::GetBool(const uint64_t& idx)
{
	return m_vBool[idx];
}

CAP_DLL std::string& PreProcessorData::GetString(const uint64_t& idx)
{
	return m_vString[idx];
}
*/
CAP_DLL const uint64_t PreProcessorData::CustomPut(void* mem)
{
	m_vCustom.push_back(mem);
	return m_vCustom.size() - 1;
}
