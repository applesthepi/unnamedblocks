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
	m_flag |= (uint64_t)ModBlockFlagType::FlagCheckHeap;
	return *this;
}

ModBlockFlags& ModBlockFlags::CheckStack()
{
	m_flag |= (uint64_t)ModBlockFlagType::FlagCheckStack;
	return *this;
}

