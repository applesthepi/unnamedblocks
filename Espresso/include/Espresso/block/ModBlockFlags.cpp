#include "Espresso/include/Espresso/block/ModBlockFlags.h"

ModBlockFlags::ModBlockFlags()
	:m_iterator(ModBlockFlagsIterator(&m_flag))
{
	m_flag = 0;
}

ModBlockFlagsIterator& ModBlockFlags::UseIterator()
{
	return m_iterator;
}

ModBlockFlags& ModBlockFlags::CheckHeap()
{
	m_flag |= (uint64_t)ModBlockFlagType::FLAG_CHECK_HEAP;
	return *this;
}

ModBlockFlags& ModBlockFlags::CheckStack()
{
	m_flag |= (uint64_t)ModBlockFlagType::FLAG_CHECK_STACK;
	return *this;
}

