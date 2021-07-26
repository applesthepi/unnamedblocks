#include "UtilityGlobalReal.hpp"
#include <Cappuccino/Logger.hpp>

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
	m_variables = new f64[m_varNames.size()];
}

f64& UtilityGlobalReal::Get(u64 idx)
{
	return m_variables[idx];
}

void UtilityGlobalReal::Put(const std::string& name)
{
	for (u64 i = 0; i < m_varNames.size(); i++)
	{
		if (m_varNames[i] == name)
			return;
	}

	m_varNames.push_back(name);
}

u64 UtilityGlobalReal::GetIdx(const std::string& name)
{
	for (u64 i = 0; i < m_varNames.size(); i++)
	{
		if (m_varNames[i] == name)
			return i;
	}

	Logger::Error("failed to get the global variable index of \"" + name + "\"; can not stop!");
	return 0;
}
