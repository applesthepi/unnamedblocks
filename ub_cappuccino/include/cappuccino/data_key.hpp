#pragma once
#include "config.h"
#include "cap_config.h"

#include "cappuccino/utils.hpp"

class DataKey
{
public:
	DataKey();
	~DataKey();

	void* GetData();
private:
	void* m_data;
};