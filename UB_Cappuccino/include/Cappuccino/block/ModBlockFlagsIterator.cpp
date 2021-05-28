#include "ModBlockFlagsIterator.h"

ModBlockFlagsIterator::ModBlockFlagsIterator(const uint64_t* flag)
	:m_iterator(0), m_flag(flag)
{

}

void ModBlockFlagsIterator::Reset()
{
	m_iterator = 0;
}

ModBlockFlagType ModBlockFlagsIterator::PullNext()
{
	if (m_iterator < (uint32_t)ModBlockFlagType::SIZE)
		return ModBlockFlagType(*m_flag << m_iterator++);
	else
	{
		Reset();
		return ModBlockFlagType::NONE;
	}
}
