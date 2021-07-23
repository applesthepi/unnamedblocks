#pragma once
#include "config.h"

#include "Logger.hpp"

#include <bit>
#include <cinttypes>
#include <cmath>
#include <array>
#include <vector>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <functional>
#include <atomic>
#include <unordered_map>
#include <fstream>
#include <string_view>
#include <string>
#include <cstring>
#include <cassert>
#include <cstdint>
#include <random>
#include <set>
#include <memory>
#include <optional>
#include <future>
#include <thread>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <zstd.h>

#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#if LINUX
#define TIME_POINT std::chrono::system_clock::time_point
#else
#define TIME_POINT std::chrono::steady_clock::time_point
#endif

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
#if 1
// (keep char)
#define short
#define int
#define long
#define unsigned

#define int8_t;
#define int16_t;
#define int32_t;
#define int64_t;
#define uint8_t;
#define uint16_t;
#define uint32_t;
#define uint64_t;
#define size_t;
#endif

// TODO: add vulkan functions
// Maintain code style standards.
namespace vk
{
typedef VkBuffer buffer;
typedef VkDeviceMemory device_memory;
typedef VkImage image;
typedef VkImageView image_view;
typedef VkDescriptorSet descriptor_set;
typedef VkDeviceSize device_size;
}

// TODO: add FT functions
// Maintain code style standards.
namespace ft
{
typedef FT_Library library;
typedef FT_Face face;
typedef FT_Error error;
}

namespace cap::endianness
{
	constexpr bool is_le = std::endian::native == std::endian::little;
	template <usize T>
	std::array<char, T> reverse(std::array<char, T> bytes);
	std::array<char, 2> to_ne_bytes(u16 x);
	std::array<char, 4> to_ne_bytes(u32 x);
	std::array<char, 8> to_ne_bytes(u64 x);

	std::array<char, 2> to_le_bytes(u16 x);
	std::array<char, 4> to_le_bytes(u32 x);
	std::array<char, 8> to_le_bytes(u64 x);

	std::array<char, 2> to_be_bytes(u16 x);
	std::array<char, 4> to_be_bytes(u32 x);
	std::array<char, 8> to_be_bytes(u64 x);

	std::array<char, 4> to_ne_bytes(f32 x);
	std::array<char, 8> to_ne_bytes(f64 x);
	std::array<char, 2> to_ne_bytes(i16 x);
	std::array<char, 4> to_ne_bytes(i32 x);
	std::array<char, 8> to_ne_bytes(i64 x);

	std::array<char, 4> to_le_bytes(f32 x);
	std::array<char, 8> to_le_bytes(f64 x);
	std::array<char, 2> to_le_bytes(i16 x);
	std::array<char, 4> to_le_bytes(i32 x);
	std::array<char, 8> to_le_bytes(i64 x);

	std::array<char, 4> to_be_bytes(f32 x);
	std::array<char, 8> to_be_bytes(f64 x);
	std::array<char, 2> to_be_bytes(i16 x);
	std::array<char, 4> to_be_bytes(i32 x);
	std::array<char, 8> to_be_bytes(i64 x);

	u16 u16_from_ne_bytes(std::array<char, 2> bytes);
	u32 u32_from_ne_bytes(std::array<char, 4> bytes);
	u64 u64_from_ne_bytes(std::array<char, 8> bytes);

	u16 u16_from_le_bytes(std::array<char, 2> bytes);
	u32 u32_from_le_bytes(std::array<char, 4> bytes);
	u64 u64_from_le_bytes(std::array<char, 8> bytes);

	u16 u16_from_be_bytes(std::array<char, 2> bytes);
	u32 u32_from_be_bytes(std::array<char, 4> bytes);
	u64 u64_from_be_bytes(std::array<char, 8> bytes);

	i16 i16_from_ne_bytes(std::array<char, 2> bytes);
	i32 i32_from_ne_bytes(std::array<char, 4> bytes);
	i64 i64_from_ne_bytes(std::array<char, 8> bytes);
	f32 float_from_ne_bytes(std::array<char, 4> bytes);
	f64 double_from_ne_bytes(std::array<char, 8> bytes);

	i16 i16_from_le_bytes(std::array<char, 2> bytes);
	i32 i32_from_le_bytes(std::array<char, 4> bytes);
	i64 i64_from_le_bytes(std::array<char, 8> bytes);
	f32 float_from_le_bytes(std::array<char, 4> bytes);
	f64 double_from_le_bytes(std::array<char, 8> bytes);

	i16 i16_from_be_bytes(std::array<char, 2> bytes);
	i32 i32_from_be_bytes(std::array<char, 4> bytes);
	i64 i64_from_be_bytes(std::array<char, 8> bytes);
	f32 float_from_be_bytes(std::array<char, 4> bytes);
	f64 double_from_be_bytes(std::array<char, 8> bytes);
}