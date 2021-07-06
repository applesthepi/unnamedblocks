#pragma once
#include "config.h"

#include "ModCategories.hpp"

#include <Espresso/Espresso.hpp>

#if LINUX
#define UB_EXPORT extern "C"
#else
#define UB_EXPORT extern "C" __declspec(dllexport)
#endif

UB_EXPORT void Initialization(ModData* data);
