#pragma once
// TODO: use "using" instead of typedef
typedef char i8;
typedef short i16;
typedef int i32;
typedef long long i64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

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