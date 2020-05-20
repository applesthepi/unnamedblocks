#include "UtilityGlobalReal.h"
#include <Cappuccino/Logger.h>

UtilityGlobalReal::UtilityGlobalReal()
{
	m_variables = nullptr;
}

UtilityGlobalReal::~UtilityGlobalReal()
{
	if (m_variables != nullptr)
		delete[] m_variables;
}

void UtilityGlobalReal::Bake()
{
	m_variables = new double[m_varNames.size()];
}

double& UtilityGlobalReal::Get(const uint64_t& idx)
{
	return m_variables[idx];
}

void UtilityGlobalReal::Put(const std::string& name)
{
	for (uint64_t i = 0; i < m_varNames.size(); i++)
	{
		if (m_varNames[i] == name)
			return;
	}

	m_varNames.push_back(name);
}

uint64_t UtilityGlobalReal::GetIdx(const std::string& name)
{
	for (uint64_t i = 0; i < m_varNames.size(); i++)
	{
		if (m_varNames[i] == name)
			return i;
	}

	Logger::Error("failed to get the global variable index of \"" + name + "\"; can not stop!");
	return 0;
}
