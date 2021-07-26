#pragma once
#include "config.h"

#include "cappuccino/utils.hpp"
#include "cappuccino/mod_block/flags_iterator.hpp"

#if !LINUX
#ifdef __CAP
#define CAP_DLL __declspec(dllexport)
#else
#define CAP_DLL __declspec(dllimport)
#endif
#else
#define CAP_DLL
#endif

class ModBlockFlags
{
public:
	CAP_DLL ModBlockFlags();
	CAP_DLL ModBlockFlagsIterator UseIterator() const;
	
	CAP_DLL ModBlockFlags& CheckHeap();
	CAP_DLL ModBlockFlags& CheckStack();
private:
	ModBlockFlagsIterator m_iterator;
	u64 m_flag;
};
