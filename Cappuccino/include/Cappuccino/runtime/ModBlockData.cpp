#include "ModBlockData.h"

MODBLOCK_EXPORT ModBlockData::ModBlockData()
	:m_setup(false)
{

}

MODBLOCK_EXPORT ModBlockData::~ModBlockData()
{
	if (m_setup)
	{
		for (uint64_t i = 0; i < m_data.size(); i++)
		{
			if (/*m_types[i] == ModBlockDataType::VAR*/false)
				delete (std::string*)(m_data[i]);
			else
			{
				if (m_interpretations[i] == ModBlockDataInterpretation::TEXT)
					delete (std::string*)(m_data[i]);
				else if (m_interpretations[i] == ModBlockDataInterpretation::STRING)
					delete (std::string*)(m_data[i]);
				else if (m_interpretations[i] == ModBlockDataInterpretation::REAL)
					delete (double*)(m_data[i]);
				else if (m_interpretations[i] == ModBlockDataInterpretation::BOOL)
					delete (bool*)(m_data[i]);
			}
		}
	}
}

MODBLOCK_EXPORT ModBlockData::ModBlockData(const std::vector<void*>& data, const std::vector<ModBlockDataType>& types, const std::vector<ModBlockDataInterpretation>& interpretations)
	:m_data(data), m_types(types), m_interpretations(interpretations), m_setup(true)
{

}

const std::vector<void*>& ModBlockData::GetData()
{
	return m_data;
}

void** ModBlockData::GetCData()
{
	if (m_data.size() == 0)
		return nullptr;
	else
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

MODBLOCK_EXPORT ModBlockData& ModBlockData::operator=(ModBlockData& data)
{
	if (data.m_setup)
	{
		if (m_setup)
			this->~ModBlockData();

		data.m_setup = false;

		m_setup = true;
		m_data = data.m_data;
		m_types = data.m_types;
		m_interpretations = data.m_interpretations;
	}

	return *this;
}