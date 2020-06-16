#pragma once

#ifdef WIN32
#include <intrin.h>
#else
#include <x86intrin.h>
#endif

#ifndef LINUX
#ifdef __CAP
#define CAP_DLL __declspec(dllexport)
#else
#define CAP_DLL __declspec(dllimport)
#endif
#else
#define CAP_DLL
#endif

class Intrinsics
{
public:
	static CAP_DLL bool AVX2();
};