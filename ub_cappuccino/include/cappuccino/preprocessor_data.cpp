#include "preprocessor_data.hpp"

#include "cappuccino/registration.hpp"

PreProcessorData::PreProcessorData(std::vector<void*>& vCustom)
	:m_vReal(Registration::GetRealTemplate()), m_vBool(Registration::GetBoolTemplate()), m_vString(Registration::GetStringTemplate()), m_vCustom(vCustom)
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
	for (u64 i = 0; i < m_names.size(); i++)
	{
		if (m_names[i] == name)
			return m_structures[i];
	}

	Logger::Error("failed to locate structure \"" + name + "\"");
	return nullptr;
}
/*
CAP_DLL f64& PreProcessorData::GetReal(u64 idx)
{
	return m_vReal[idx];
}

CAP_DLL bool& PreProcessorData::GetBool(u64 idx)
{
	return m_vBool[idx];
}

CAP_DLL std::string& PreProcessorData::GetString(u64 idx)
{
	return m_vString[idx];
}
*/
CAP_DLL u64 PreProcessorData::CustomPut(void* mem)
{
	m_vCustom.push_back(mem);
	return m_vCustom.size() - 1;
}
