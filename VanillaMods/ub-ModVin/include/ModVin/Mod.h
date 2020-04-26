#pragma once
#include "config.h"
#include "ModCategories.h"

#include "blocks/BlockTest.h"
#include "blocks/BlockMain.h"

#include <Espresso/Espresso.h>
#include <cmath>

#ifdef LINUX
#define UB_EXPORT extern "C"
#else
#define UB_EXPORT extern "C" __declspec(dllexport)
#endif

UB_EXPORT void Initialization(ModData* data);