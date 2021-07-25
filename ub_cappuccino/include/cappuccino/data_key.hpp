#pragma once
#include "config.h"

#include "Utils.hpp"

class DataKey
{
public:
	DataKey();
	~DataKey();

	void* GetData();
private:
	void* m_data;
};