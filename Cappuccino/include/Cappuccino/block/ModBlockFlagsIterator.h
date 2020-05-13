#pragma once
#include <stdint.h>
#include "config.h"

#ifndef LINUX
#ifdef __CAP
#define CAP_DLL __declspec(dllexport)
#else
#define CAP_DLL __declspec(dllimport)
#endif
#else
#define CAP_DLL
#endif

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
	CAP_DLL ModBlockFlagType PullNext();
	CAP_DLL ModBlockFlagsIterator(const uint64_t* flag);
	CAP_DLL void Reset();
private:
	uint64_t m_iterator;
	const uint64_t* m_flag;
};
