#pragma once
#include <vector>
#include <string>

class UtiltiyListCell
{
public:
	UtiltiyListCell(void* memory, const bool& copy);
	~UtiltiyListCell();

	UtiltiyListCell& operator=(const UtiltiyListCell& list);

	void* Memory;
	bool Copy;
};

class UtilityList
{
public:
	void AddValue(void* memory, const bool& copy);
	void InsertValue(void* memory, const uint64_t& idx, const bool& copy);

	void RemoveValue(const uint64_t& idx);
	void ReplaceValue(void* memory, const uint64_t& idx, const bool& copy);

	void Reserve(const uint64_t& size);
	const uint64_t Size();

	double* IndexReal(const uint64_t& idx);
	bool* IndexBool(const uint64_t& idx);
	std::string* IndexString(const uint64_t& idx);
private:
	std::vector<UtiltiyListCell> m_memory;
};