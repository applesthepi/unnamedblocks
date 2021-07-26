#include "data.hpp"

#include "cappuccino/registration.hpp"

MODBLOCK_EXPORT ModBlockData::ModBlockData()
{

}

MODBLOCK_EXPORT ModBlockData::ModBlockData(const std::vector<void*>& data, const std::vector<ModBlockDataType>& types, const std::vector<ModBlockDataInterpretation>& interpretations)
	:m_data(data), m_types(types), m_interpretations(interpretations)
{

}

const std::vector<void*>& ModBlockData::GetData()
{
	return m_data;
}

const std::vector<u64>& ModBlockData::GetRuntimeData()
{
	return m_runtimeData;
}

MODBLOCK_EXPORT std::vector<void*>& ModBlockData::GetPreData()
{
	return m_preData;
}

const std::vector<ModBlockDataType>& ModBlockData::GetTypes()
{
	return m_types;
}

const std::vector<ModBlockDataInterpretation>& ModBlockData::GetInterpretations()
{
	return m_interpretations;
}

MODBLOCK_EXPORT void ModBlockData::ClearData()
{
	m_data.clear();
}

MODBLOCK_EXPORT void ModBlockData::SetRuntimeData(const std::vector<u64>& data)
{
	m_runtimeData = data;
}

MODBLOCK_EXPORT void ModBlockData::SetDataTemplates(u64 stackIdx)
{
	const std::vector<f64*>& reals = Registration::GetRealTemplate();
	const std::vector<bool*>& bools = Registration::GetBoolTemplate();
	const std::vector<std::string*>& strings = Registration::GetStringTemplate();
	std::string* str;
	for (u64 i = 0; i < m_runtimeData.size(); i++)
	{
		if (m_interpretations[i] == ModBlockDataInterpretation::REAL)
			m_data.push_back(&(reals[stackIdx][m_runtimeData[i]]));
		else if (m_interpretations[i] == ModBlockDataInterpretation::BOOL)
			m_data.push_back(&(bools[stackIdx][m_runtimeData[i]]));
		else if (m_interpretations[i] == ModBlockDataInterpretation::STRING)
			m_data.push_back(&(strings[stackIdx][m_runtimeData[i]]));
	}
}

void ModBlockData::SetTypes(const std::vector<ModBlockDataType>& types)
{
	m_types = types;
}

void ModBlockData::SetInterpretations(const std::vector<ModBlockDataInterpretation>& interpretations)
{
	m_interpretations = interpretations;
}

MODBLOCK_EXPORT void ModBlockData::SetInterpretation(const ModBlockDataInterpretation& interpretation, u64 idx)
{
	m_interpretations[idx] = interpretation;
}

MODBLOCK_EXPORT ModBlockData& ModBlockData::operator=(ModBlockData& data)
{
	m_data = data.m_data;
	m_runtimeData = data.m_runtimeData;
	m_types = data.m_types;
	m_interpretations = data.m_interpretations;

	return *this;
}
