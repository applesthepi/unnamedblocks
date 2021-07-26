#include "flags_iterator.hpp"

ModBlockFlagsIterator::ModBlockFlagsIterator(const u64* flag)
	:m_iterator(0), m_flag(flag)
{

}

void ModBlockFlagsIterator::Reset()
{
	m_iterator = 0;
}

ModBlockFlagType ModBlockFlagsIterator::PullNext()
{
	if (m_iterator < (u32)ModBlockFlagType::SIZE)
		return ModBlockFlagType(*m_flag << m_iterator++);
	else
	{
		Reset();
		return ModBlockFlagType::NONE;
	}
}
