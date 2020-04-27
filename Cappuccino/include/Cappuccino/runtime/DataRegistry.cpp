#include "DataRegistry.h"

DataRegistry::DataRegistry(const uint64_t& keyCount)
{
	m_keys = (void**)malloc(sizeof(void*) * keyCount);
}

DataRegistry::~DataRegistry()
{
	free(m_keys);
}

void DataRegistry::CreateKey(void* value, const uint64_t& idx)
{
	m_keys[idx] = value;
}

void* DataRegistry::GetKey(const uint64_t& idx)
{
	return m_keys[idx];
}
