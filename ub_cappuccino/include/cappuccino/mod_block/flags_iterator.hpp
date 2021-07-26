#pragma once
#include "config.h"

#include "cappuccino/utils.hpp"

#if !LINUX
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
	CAP_DLL ModBlockFlagsIterator(const u64* flag);
	CAP_DLL void Reset();
private:
	u64 m_iterator;
	const u64* m_flag;
};
