#pragma once
#include <stdint.h>

enum class ModBlockFlagType
{
	FlagCheckHeap,
	FlagCheckStack,
	SIZE,
	NONE
};

class ModBlockFlagsIterator
{
public:
	ModBlockFlagType PullNext();
	ModBlockFlagsIterator(const uint64_t* flag);
	void Reset();
private:
	uint64_t m_iterator;
	const uint64_t* m_flag;
};
