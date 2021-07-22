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

// TODO: does this work?
#define float float #warning "please use f32 for standards"
#define double double #warning "please use f64 for standards"

namespace vk
{
typedef VkImage image;
typedef VkDeviceMemory device_memory;
}

namespace ft
{
typedef FT_Library library;
typedef FT_Face face;
}

namespace endianness
{
	constexpr bool is_le = std::endian::native == std::endian::little;
	template <size_t T>
	std::array<char, T> reverse(std::array<char, T> bytes);
	std::array<char, 2> to_ne_bytes(uint16_t x);
	std::array<char, 4> to_ne_bytes(uint32_t x);
	std::array<char, 8> to_ne_bytes(uint64_t x);

	std::array<char, 2> to_le_bytes(uint16_t x);
	std::array<char, 4> to_le_bytes(uint32_t x);
	std::array<char, 8> to_le_bytes(uint64_t x);

	std::array<char, 2> to_be_bytes(uint16_t x);
	std::array<char, 4> to_be_bytes(uint32_t x);
	std::array<char, 8> to_be_bytes(uint64_t x);

	std::array<char, 4> to_ne_bytes(float x);
	std::array<char, 8> to_ne_bytes(double x);
	std::array<char, 2> to_ne_bytes(int16_t x);
	std::array<char, 4> to_ne_bytes(int32_t x);
	std::array<char, 8> to_ne_bytes(int64_t x);

	std::array<char, 4> to_le_bytes(float x);
	std::array<char, 8> to_le_bytes(double x);
	std::array<char, 2> to_le_bytes(int16_t x);
	std::array<char, 4> to_le_bytes(int32_t x);
	std::array<char, 8> to_le_bytes(int64_t x);

	std::array<char, 4> to_be_bytes(float x);
	std::array<char, 8> to_be_bytes(double x);
	std::array<char, 2> to_be_bytes(int16_t x);
	std::array<char, 4> to_be_bytes(int32_t x);
	std::array<char, 8> to_be_bytes(int64_t x);
	uint16_t u16_from_ne_bytes(std::array<char, 2> bytes);
	uint32_t u32_from_ne_bytes(std::array<char, 4> bytes);
	uint64_t u64_from_ne_bytes(std::array<char, 8> bytes);

	uint16_t u16_from_le_bytes(std::array<char, 2> bytes);
	uint32_t u32_from_le_bytes(std::array<char, 4> bytes);
	uint64_t u64_from_le_bytes(std::array<char, 8> bytes);

	uint16_t u16_from_be_bytes(std::array<char, 2> bytes);
	uint32_t u32_from_be_bytes(std::array<char, 4> bytes);
	uint64_t u64_from_be_bytes(std::array<char, 8> bytes);

	int16_t i16_from_ne_bytes(std::array<char, 2> bytes);
	int32_t i32_from_ne_bytes(std::array<char, 4> bytes);
	int64_t i64_from_ne_bytes(std::array<char, 8> bytes);
	float float_from_ne_bytes(std::array<char, 4> bytes);
	double double_from_ne_bytes(std::array<char, 8> bytes);

	int16_t i16_from_le_bytes(std::array<char, 2> bytes);
	int32_t i32_from_le_bytes(std::array<char, 4> bytes);
	int64_t i64_from_le_bytes(std::array<char, 8> bytes);
	float float_from_le_bytes(std::array<char, 4> bytes);
	double double_from_le_bytes(std::array<char, 8> bytes);

	int16_t i16_from_be_bytes(std::array<char, 2> bytes);
	int32_t i32_from_be_bytes(std::array<char, 4> bytes);
	int64_t i64_from_be_bytes(std::array<char, 8> bytes);
	float float_from_be_bytes(std::array<char, 4> bytes);
	double double_from_be_bytes(std::array<char, 8> bytes);
}