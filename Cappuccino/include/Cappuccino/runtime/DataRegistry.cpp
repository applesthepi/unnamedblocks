#include "DataRegistry.h"

DataRegistry::DataRegistry(uint64_t keyCount)
{
	m_keys = (void**)malloc(sizeof(void*) * keyCount);
}

DataRegistry::~DataRegistry()
{
	free(m_keys);
}

void DataRegistry::CreateKey(void* value, uint64_t idx)
{
	m_keys[idx] = value;
}

void* DataRegistry::GetKey(uint64_t idx)
{
	return m_keys[idx];
}
