#pragma once

class DataKey
{
public:
	DataKey();
	~DataKey();

	void* GetData();
private:
	void* m_data;
};