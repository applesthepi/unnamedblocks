#pragma once
#include <vector>
#include <string>

class UtiltiyListCell
{
public:
	UtiltiyListCell(void* memory, bool copy);
	~UtiltiyListCell();

	UtiltiyListCell& operator=(const UtiltiyListCell& list);

	void* Memory;
	bool Copy;
};

class UtilityList
{
public:
	void AddValue(void* memory, bool copy);
	void InsertValue(void* memory, uint64_t idx, bool copy);

	void RemoveValue(uint64_t idx);
	void ReplaceValue(void* memory, uint64_t idx, bool copy);

	void Reserve(uint64_t size);
	const uint64_t Size();

	double* IndexReal(uint64_t idx);
	bool* IndexBool(uint64_t idx);
	std::string* IndexString(uint64_t idx);
private:
	std::vector<UtiltiyListCell> m_memory;
};