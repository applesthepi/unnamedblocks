#include "PreProcessorData.h"
#include "Espresso/Logger.h"

uint64_t* PreprocessorCell::GetUInt()
{
	if (m_type != PreprocessorStorageType::Unsigned)
	{
		Logger::Error("attempted to get uint, invalid type found");
		return nullptr;
	}
	else
		return (uint64_t*)m_value;
}

int64_t* PreprocessorCell::GetSInt()
{
	if (m_type != PreprocessorStorageType::Signed)
	{
		Logger::Error("attempted to get sint, invalid type found");
		return nullptr;
	}
	else
		return (int64_t*)m_value;
}

double* PreprocessorCell::GetDouble()
{
	if (m_type != PreprocessorStorageType::Double)
	{
		Logger::Error("attempted to get double, invalid type found");
		return nullptr;
	}
	else
		return (double*)m_value;
}

const char* PreprocessorCell::GetString()
{
	if (m_type != PreprocessorStorageType::String)
	{
		Logger::Error("attempted to get string, invalid type found");
		return nullptr;
	}
	else
		return (const char*)m_value;
}


bool* PreprocessorCell::GetBoolean()
{
	if (m_type != PreprocessorStorageType::Boolean)
	{
		Logger::Error("attempted to get boolean, invalid type found");
		return nullptr;
	}
	else
		return (bool*)m_value;
}

void PreprocessorStack::AddContainer(const std::string& container)
{
	m_localContainers.insert({ container, std::map<std::string, PreprocessorCell>() });
}

bool PreprocessorStack::AddValue(const std::string& container, const std::string& name, const PreprocessorCell& cell)
{
	auto search = m_localContainers.find(container);
	if (search == m_localContainers.end())
	{
		Logger::Error("failed to add value \"" + name + "\" to container \"" + container + "\"");
		return false;
	}
	else
	{
		search->second.insert({ name, cell });
		return true;
	}

	return false;
}

const PreprocessorCell& PreprocessorStack::GetValue(const std::string& container, const std::string& name)
{
	auto search = m_localContainers.find(container);
	if (search == m_localContainers.end())
	{
		Logger::Error("failed to get value \"" + name + "\" from container \"" + container + "\"");
		return PreprocessorCell();
	}
	else
	{
		auto innerSearch = search->second.find(name);
		if (innerSearch == search->second.end())
		{
			Logger::Error("failed to get value \"" + name + "\" from container \"" + container + "\"");
			return PreprocessorCell();
		}
		else
			return innerSearch->second;
	}

	return PreprocessorCell();
}

uint64_t PreProcessorData::RegisterPreProcessorStack()
{
	m_stacks.push_back(PreprocessorStack());
	return m_stacks.size() - 1;
}

void PreProcessorData::AddGlobalContainer(const std::string& container)
{
	m_globalStack.AddContainer(container);
}

void PreProcessorData::AddGlobalValue(const std::string& container, const std::string& name, const PreprocessorCell& cell)
{
	m_globalStack.AddValue(container, name, cell);
}

const PreprocessorCell& PreProcessorData::GetGlobalValue(const std::string& container, const std::string& name)
{
	return m_globalStack.GetValue(container, name);
}

void PreProcessorData::AddLocalContainer(const uint64_t& idx, const std::string& container)
{
	if (idx >= m_stacks.size())
	{
		Logger::Error("adding container to stack that does not exist \"" + std::to_string(idx) + "\"");
		return;
	}

	m_stacks[idx].AddContainer(container);
}

void PreProcessorData::AddLocalValue(const uint64_t& idx, const std::string& container, const std::string& name, const PreprocessorCell& cell)
{
	if (idx >= m_stacks.size())
	{
		Logger::Error("adding value to stack that does not exist \"" + std::to_string(idx) + "\"");
		return;
	}

	m_stacks[idx].AddValue(container, name, cell);
}

const PreprocessorCell& PreProcessorData::GetLocalValue(const uint64_t& idx, const std::string& container, const std::string& name)
{
	if (idx >= m_stacks.size())
	{
		Logger::Error("getting value of stack that does not exist \"" + std::to_string(idx) + "\"");
		return PreprocessorCell();
	}

	return m_stacks[idx].GetValue(container, name);
}