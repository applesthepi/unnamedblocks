#pragma once
#include "config.h"

#include "Utils.hpp"

class DataRegistry
{
public:
	DataRegistry(uint64_t keyCount);
	~DataRegistry();

	void CreateKey(void* value, uint64_t idx);
	void* GetKey(uint64_t idx);
private:
	void** m_keys;
};