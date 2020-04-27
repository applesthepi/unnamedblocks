#include "ModBlockData.h"

__declspec(dllexport) ModBlockData::ModBlockData(const std::vector<void*>& data, const std::vector<ModBlockDataType>& types, const std::vector<ModBlockDataInterpretation>& interpretations)
	:m_data(data), m_types(types), m_interpretations(interpretations)
{

}

const std::vector<void*>& ModBlockData::GetData()
{
	return m_data;
}

void** ModBlockData::GetCData()
{
	return &m_data[0];
}

const std::vector<ModBlockDataType>& ModBlockData::GetTypes()
{
	return m_types;
}

const std::vector<ModBlockDataInterpretation>& ModBlockData::GetInterpretations()
{
	return m_interpretations;
}

void ModBlockData::HaulData(const std::vector<int64_t>& data)
{
	for (uint64_t i = 0; i < m_data.size(); i++)
	{
		if (data[i] != -1)
		{
			free(m_data[i]);
			m_data[i] = (void*)malloc(sizeof(uint64_t));
			*((uint64_t*)m_data[i]) = data[i];
		}
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

