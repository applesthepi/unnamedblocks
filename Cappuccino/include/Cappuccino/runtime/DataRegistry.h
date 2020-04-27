#pragma once
#include <string>

class DataRegistry
{
public:
	DataRegistry(const uint64_t& keyCount);
	~DataRegistry();

	void CreateKey(void* value, const uint64_t& idx);
	void* GetKey(const uint64_t& idx);
private:
	void** m_keys;
};