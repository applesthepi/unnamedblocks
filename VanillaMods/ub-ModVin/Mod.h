#pragma once
#include <Espresso/Espresso.h>
#include <cmath>

//TODO config
//#define LINUX

#ifdef LINUX
#define UB_EXPORT extern "C"
#else
#define UB_EXPORT extern "C" __declspec(dllexport)
#endif

UB_EXPORT void Initialization(ModData* data);
