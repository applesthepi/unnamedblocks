#pragma once
#include "config.h"

#include "categories.hpp"

#include <espresso/mod/data.hpp>

#if LINUX
#define UB_EXPORT extern "C"
#else
#define UB_EXPORT extern "C" __declspec(dllexport)
#endif

UB_EXPORT void Initialization(esp::mod::data* data);
