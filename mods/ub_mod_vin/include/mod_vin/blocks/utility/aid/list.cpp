#include "UtilityList.hpp"

UtiltiyListCell::UtiltiyListCell(void* memory, bool copy)
	:Memory(memory), Copy(copy) {}

UtiltiyListCell::~UtiltiyListCell()
{
	if (Copy && Memory != nullptr)
		delete Memory;
}

UtiltiyListCell& UtiltiyListCell::operator=(const UtiltiyListCell& list)
{
	if (Copy && Memory != nullptr)
		delete Memory;

	Memory = list.Memory;
	Copy = list.Copy;

	return *this;
}

void UtilityList::AddValue(void* memory, bool copy)
{
	m_memory.push_back({ memory, copy });
}

void UtilityList::InsertValue(void* memory, u64 idx, bool copy)
{
	m_memory.insert(m_memory.begin() + idx, { memory, copy });
}

void UtilityList::RemoveValue(u64 idx)
{
	m_memory.erase(m_memory.begin() + idx);
}

void UtilityList::ReplaceValue(void* memory, u64 idx, bool copy)
{
	m_memory[idx] = { memory, copy };
}

void UtilityList::Reserve(u64 size)
{
	m_memory.reserve(size);
}

u64 UtilityList::Size()
{
	return m_memory.size();
}

f64* UtilityList::IndexReal(u64 idx)
{
	return (f64*)m_memory[idx].Memory;
}

bool* UtilityList::IndexBool(u64 idx)
{
	return (bool*)m_memory[idx].Memory;
}

std::string* UtilityList::IndexString(u64 idx)
{
	return (std::string*)m_memory[idx].Memory;
}
