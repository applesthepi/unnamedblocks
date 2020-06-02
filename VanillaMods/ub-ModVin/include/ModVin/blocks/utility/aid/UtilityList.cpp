#include "UtilityList.h"

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

void UtilityList::InsertValue(void* memory, uint64_t idx, bool copy)
{
	m_memory.insert(m_memory.begin() + idx, { memory, copy });
}

void UtilityList::RemoveValue(uint64_t idx)
{
	m_memory.erase(m_memory.begin() + idx);
}

void UtilityList::ReplaceValue(void* memory, uint64_t idx, bool copy)
{
	m_memory[idx] = { memory, copy };
}

void UtilityList::Reserve(uint64_t size)
{
	m_memory.reserve(size);
}

const uint64_t UtilityList::Size()
{
	return m_memory.size();
}

double* UtilityList::IndexReal(uint64_t idx)
{
	return (double*)m_memory[idx].Memory;
}

bool* UtilityList::IndexBool(uint64_t idx)
{
	return (bool*)m_memory[idx].Memory;
}

std::string* UtilityList::IndexString(uint64_t idx)
{
	return (std::string*)m_memory[idx].Memory;
}
