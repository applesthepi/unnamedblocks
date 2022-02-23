#pragma once
// TODO: use "using" instead of typedef
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

typedef unsigned long long usize;

// Change to 0 if you want to keep
// access to the internal types.
#if 0
// (keep char)
#define short
#define int
#define long
#define unsigned

#define float
#define double

#define int8_t
#define int16_t
#define int32_t
#define int64_t
#define uint8_t
#define uint16_t
#define uint32_t
#define uint64_t
#define size_t
#endif