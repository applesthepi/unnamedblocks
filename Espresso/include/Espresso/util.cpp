#include "util.h"
#include <array>
#include <cinttypes>
namespace endianness {
	std::array<char, 2> to_ne_bytes(uint16_t x) {
		std::array<char, 2> bytes;
		char* value = reinterpret_cast<char*>(&x);
		bytes[0] = value[0];
		bytes[1] = value[1];
		return bytes;
	}

	std::array<char, 4> to_ne_bytes(uint32_t x) {
		std::array<char, 4> bytes;
		char* value = reinterpret_cast<char*>(&x);
		bytes[0] = value[0];
		bytes[1] = value[1];
		bytes[2] = value[2];
		bytes[3] = value[3];
		return bytes;
	}

	std::array<char, 8> to_ne_bytes(uint64_t x) {
		std::array<char, 8> bytes;
		char* value = reinterpret_cast<char*>(&x);
		bytes[0] = value[0];
		bytes[1] = value[1];
		bytes[2] = value[2];
		bytes[3] = value[3];
		bytes[4] = value[4];
		bytes[5] = value[5];
		bytes[6] = value[6];
		bytes[7] = value[7];
		return bytes;
	}

	std::array<char,2> to_le_bytes(uint16_t x) {
		if (is_le) {
			return to_ne_bytes(x);
		}
		else {
			std::array<char, 2> bytes;
			char* value = reinterpret_cast<char*>(&x);
			bytes[3] = value[0];
			bytes[2] = value[1];
			return bytes;
		}
	}

	std::array<char,4> to_le_bytes(uint32_t x) {
		if (is_le) {
			return to_ne_bytes(x);
		}
		else {
			std::array<char, 4> bytes;
			char* value = reinterpret_cast<char*>(&x);
			bytes[3] = value[0];
			bytes[2] = value[1];
			bytes[1] = value[2];
			bytes[0] = value[3];
			return bytes;
		}
	}

	std::array<char,8> to_le_bytes(uint64_t x) {
		if (is_le) {
			return to_ne_bytes(x);
		}
		else {
			std::array<char, 8> bytes;
			char* value = reinterpret_cast<char*>(&x);
			bytes[7] = value[0];
			bytes[6] = value[1];
			bytes[5] = value[2];
			bytes[4] = value[3];
			bytes[3] = value[4];
			bytes[2] = value[5];
			bytes[1] = value[6];
			bytes[0] = value[7];
			return bytes;
		}
	}

	std::array<char,2> to_be_bytes(uint16_t x) {
		if (!is_le) {
			return to_ne_bytes(x);
		}
		else {
			std::array<char, 2> bytes;
			char* value = reinterpret_cast<char*>(&x);
			bytes[3] = value[0];
			bytes[2] = value[1];
			return bytes;
		}
	}

	std::array<char,4> to_be_bytes(uint32_t x) {
		if (!is_le) {
			return to_ne_bytes(x);
		}
		else {
			std::array<char, 4> bytes;
			char* value = reinterpret_cast<char*>(&x);
			bytes[3] = value[0];
			bytes[2] = value[1];
			bytes[1] = value[2];
			bytes[0] = value[3];
			return bytes;
		}
	}

	std::array<char,8> to_be_bytes(uint64_t x) {
		if (!is_le) {
			return to_ne_bytes(x);
		}
		else {
			std::array<char, 8> bytes;
			char* value = reinterpret_cast<char*>(&x);
			bytes[7] = value[0];
			bytes[6] = value[1];
			bytes[5] = value[2];
			bytes[4] = value[3];
			bytes[3] = value[4];
			bytes[2] = value[5];
			bytes[1] = value[6];
			bytes[0] = value[7];
			return bytes;
		}
	}
}
