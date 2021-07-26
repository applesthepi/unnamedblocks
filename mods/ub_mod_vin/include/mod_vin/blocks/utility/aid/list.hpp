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
	void InsertValue(void* memory, u64 idx, bool copy);

	void RemoveValue(u64 idx);
	void ReplaceValue(void* memory, u64 idx, bool copy);

	void Reserve(u64 size);
	u64 Size();

	f64* IndexReal(u64 idx);
	bool* IndexBool(u64 idx);
	std::string* IndexString(u64 idx);
private:
	std::vector<UtiltiyListCell> m_memory;
};
