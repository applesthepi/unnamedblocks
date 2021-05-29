#pragma once
#include "config.h"

#define CAP_CPP_EXPORT extern "C"
#define CAP_C_EXPORT

// #ifdef WIN32
// 	#ifdef __CAP
// 		#define CAP_CPP_EXPORT extern "C" __declspec(dllexport)
// 		#define TYPDEF_CPP_EXPORT extern "C"
// 		#define CAP_C_EXPORT __declspec(dllexport)
// 	#else
// 		#define CAP_CPP_EXPORT __declspec(dllimport)
// 		#define TYPDEF_CPP_EXPORT
// 		#define CAP_C_EXPORT __declspec(dllimport)
// 	#endif
// #else
// 	#ifdef __CAP
// 		#define CAP_CPP_EXPORT extern "C"
// 		#define TYPDEF_CPP_EXPORT extern "C"
// 		#define CAP_C_EXPORT
// 	#else
// 		#define CAP_CPP_EXPORT
// 		#define TYPDEF_CPP_EXPORT
// 		#define CAP_C_EXPORT
// 	#endif
// #endif