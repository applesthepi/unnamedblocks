#include "flags.hpp"

ModBlockFlags::ModBlockFlags()
	:m_iterator(ModBlockFlagsIterator(&m_flag))
{
	m_flag = 0;
}

ModBlockFlagsIterator ModBlockFlags::UseIterator() const
{
	return m_iterator;
}

ModBlockFlags& ModBlockFlags::CheckHeap()
{
	m_flag |= (u64)ModBlockFlagType::FlagCheckHeap;
	return *this;
}

ModBlockFlags& ModBlockFlags::CheckStack()
{
	m_flag |= (u64)ModBlockFlagType::FlagCheckStack;
	return *this;
}

