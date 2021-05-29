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
#include <glm/glm.hpp>

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
