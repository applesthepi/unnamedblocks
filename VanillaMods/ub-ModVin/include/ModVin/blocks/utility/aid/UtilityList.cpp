#include "UtilityList.h"

UtiltiyListCell::UtiltiyListCell(void* memory, const bool& copy)
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
}

void UtilityList::AddValue(void* memory, const bool& copy)
{
	m_memory.push_back({ memory, copy });
}

void UtilityList::InsertValue(void* memory, const uint64_t& idx, const bool& copy)
{
	m_memory.insert(m_memory.begin() + idx, { memory, copy });
}

void UtilityList::RemoveValue(const uint64_t& idx)
{
	m_memory.erase(m_memory.begin() + idx);
}

void UtilityList::ReplaceValue(void* memory, const uint64_t& idx, const bool& copy)
{
	m_memory[idx] = { memory, copy };
}

void UtilityList::Reserve(const uint64_t& size)
{
	m_memory.reserve(size);
}

const uint64_t UtilityList::Size()
{
	return m_memory.size();
}

double* UtilityList::IndexReal(const uint64_t& idx)
{
	return (double*)m_memory[idx].Memory;
}

bool* UtilityList::IndexBool(const uint64_t& idx)
{
	return (bool*)m_memory[idx].Memory;
}

std::string* UtilityList::IndexString(const uint64_t& idx)
{
	return (std::string*)m_memory[idx].Memory;
}
