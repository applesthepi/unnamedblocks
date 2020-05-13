#pragma once
#include "ModBlockFlagsIterator.h"

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

class ModBlockFlags
{
public:
	CAP_DLL ModBlockFlags();
	CAP_DLL ModBlockFlagsIterator UseIterator() const;
	
	CAP_DLL ModBlockFlags& CheckHeap();
	CAP_DLL ModBlockFlags& CheckStack();
private:
	ModBlockFlagsIterator m_iterator;
	uint64_t m_flag;
};
