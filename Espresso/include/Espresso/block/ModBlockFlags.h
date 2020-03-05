#pragma once
#include "ModBlockFlagsIterator.h"

#include <stdint.h>

class ModBlockFlags
{
public:
	ModBlockFlags();
	ModBlockFlagsIterator& UseIterator();
	
	ModBlockFlags& CheckHeap();
	ModBlockFlags& CheckStack();
private:
	ModBlockFlagsIterator m_iterator;
	uint64_t m_flag;
};
