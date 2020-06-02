#pragma once
#include <bit>
#include <cinttypes>
#include <array>
namespace endianness {
	constexpr bool is_le = std::endian::native == std::endian::little;
	std::array<char, 2> to_ne_bytes(uint16_t x);
	std::array<char, 4> to_ne_bytes(uint32_t x);
	std::array<char, 8> to_ne_bytes(uint64_t x);

	std::array<char, 2> to_le_bytes(uint16_t x);
	std::array<char, 4> to_le_bytes(uint32_t x);
	std::array<char, 8> to_le_bytes(uint64_t x);

	std::array<char, 2> to_be_bytes(uint16_t x);
	std::array<char, 4> to_be_bytes(uint32_t x);
	std::array<char, 8> to_be_bytes(uint64_t x);

	std::array<char, 4> to_ne_bytes(float x) {return to_ne_bytes(x);}
	std::array<char, 8> to_ne_bytes(double x) {return to_ne_bytes(x);}
	std::array<char, 2> to_ne_bytes(int16_t x) {return to_ne_bytes(x);}
	std::array<char, 4> to_ne_bytes(int32_t x) {return to_ne_bytes(x);}
	std::array<char, 8> to_ne_bytes(int64_t x) {return to_ne_bytes(x);}

	std::array<char, 4> to_le_bytes(float x) {return to_le_bytes(x);}
	std::array<char, 8> to_le_bytes(double x) {return to_le_bytes(x);}
	std::array<char, 2> to_le_bytes(int16_t x) {return to_le_bytes(x);}
	std::array<char, 4> to_le_bytes(int32_t x) {return to_le_bytes(x);}
	std::array<char, 8> to_le_bytes(int64_t x) {return to_le_bytes(x);}

	std::array<char, 4> to_be_bytes(float x) {return to_be_bytes(x);}
	std::array<char, 8> to_be_bytes(double x) {return to_be_bytes(x);}
	std::array<char, 2> to_be_bytes(int16_t x) {return to_be_bytes(x);}
	std::array<char, 4> to_be_bytes(int32_t x) {return to_be_bytes(x);}
	std::array<char, 8> to_be_bytes(int64_t x) {return to_be_bytes(x);}

	uint16_t u16_from_ne_bytes(std::array<char, 2> bytes) {return (uint16_t)*bytes.data();}
	uint32_t u32_from_ne_bytes(std::array<char, 4> bytes) {return (uint32_t)*bytes.data();}
	uint64_t u64_from_ne_bytes(std::array<char, 8> bytes) {return (uint64_t)*bytes.data();}

	uint16_t u16_from_le_bytes(std::array<char, 2> bytes) {return (uint16_t)*bytes.data();}
	uint32_t u32_from_le_bytes(std::array<char, 4> bytes) {return (uint32_t)*bytes.data();}
	uint64_t u64_from_le_bytes(std::array<char, 8> bytes) {return (uint64_t)*bytes.data();}

	uint16_t u16_from_be_bytes(std::array<char, 2> bytes) {return (uint16_t)*bytes.data();}
	uint32_t u32_from_be_bytes(std::array<char, 4> bytes) {return (uint32_t)*bytes.data();}
	uint64_t u64_from_be_bytes(std::array<char, 8> bytes) {return (uint64_t)*bytes.data();}

	float float_from_ne_bytes(std::array<char, 4> bytes) {return (float)*bytes.data();}
	double double_from_ne_bytes(std::array<char, 8> bytes) {return (double)*bytes.data();}
	int16_t i16_from_ne_bytes(std::array<char, 2> bytes) {return (int16_t)*bytes.data();}
	int32_t i32_from_ne_bytes(std::array<char, 4> bytes) {return (int32_t)*bytes.data();}
	int64_t i64_from_ne_bytes(std::array<char, 8> bytes) {return (int64_t)*bytes.data();}

	float float_from_le_bytes(std::array<char, 4> bytes) {return (float)*bytes.data();}
	double double_from_le_bytes(std::array<char, 8> bytes) {return (double)*bytes.data();}
	int16_t i16_from_le_bytes(std::array<char, 2> bytes) {return (int16_t)*bytes.data();}
	int32_t i32_from_le_bytes(std::array<char, 4> bytes) {return (int32_t)*bytes.data();}
	int64_t i64_from_le_bytes(std::array<char, 8> bytes) {return (int64_t)*bytes.data();}

	float float_from_be_bytes(std::array<char, 4> bytes) {return (float)*bytes.data();}
	double double_from_be_bytes(std::array<char, 8> bytes) {return (double)*bytes.data();}
	int16_t i16_from_be_bytes(std::array<char, 2> bytes) {return (int16_t)*bytes.data();}
	int32_t i32_from_be_bytes(std::array<char, 4> bytes) {return (int32_t)*bytes.data();}
	int64_t i64_from_be_bytes(std::array<char, 8> bytes) {return (int64_t)*bytes.data();}

}
