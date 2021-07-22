#include "Utils.hpp"

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
	template<size_t T> std::array<char, T> reverse(std::array<char, T> bytes) {
		std::array<char, T> reversed;
		for(size_t i = 0; i < T; i++) {
			reversed[reversed.size() - i] = bytes[i];
		}
		return reversed;
	}
	std::array<char, 4> to_ne_bytes(float x) {return to_ne_bytes(*(uint32_t*)(void*)&x);}
	std::array<char, 8> to_ne_bytes(double x) {return to_ne_bytes(*(uint64_t*)(void*)&x);}
	std::array<char, 2> to_ne_bytes(int16_t x) {return to_ne_bytes(*(uint16_t*)(void*)&x);}
	std::array<char, 4> to_ne_bytes(int32_t x) {return to_ne_bytes(*(uint32_t*)(void*)&x);}
	std::array<char, 8> to_ne_bytes(int64_t x) {return to_ne_bytes(*(uint64_t*)(void*)&x);}

	std::array<char, 4> to_le_bytes(float x) {return to_le_bytes(*(uint32_t*)(void*)&x);}
	std::array<char, 8> to_le_bytes(double x) {return to_le_bytes(*(uint64_t*)(void*)&x);}
	std::array<char, 2> to_le_bytes(int16_t x) {return to_le_bytes(*(uint16_t*)(void*)&x);}
	std::array<char, 4> to_le_bytes(int32_t x) {return to_le_bytes(*(uint32_t*)(void*)&x);}
	std::array<char, 8> to_le_bytes(int64_t x) {return to_le_bytes(*(uint64_t*)(void*)&x);}

	std::array<char, 4> to_be_bytes(float x) {return to_be_bytes(*(uint32_t*)(void*)&x);}
	std::array<char, 8> to_be_bytes(double x) {return to_be_bytes(*(uint64_t*)(void*)&x);}
	std::array<char, 2> to_be_bytes(int16_t x) {return to_be_bytes(*(uint16_t*)(void*)&x);}
	std::array<char, 4> to_be_bytes(int32_t x) {return to_be_bytes(*(uint32_t*)(void*)&x);}
	std::array<char, 8> to_be_bytes(int64_t x) {return to_be_bytes(*(uint64_t*)(void*)&x);}
	uint16_t u16_from_ne_bytes(std::array<char, 2> bytes) {return (uint16_t)*bytes.data();}
	uint32_t u32_from_ne_bytes(std::array<char, 4> bytes) {return (uint32_t)*bytes.data();}
	uint64_t u64_from_ne_bytes(std::array<char, 8> bytes) {return (uint64_t)*bytes.data();}

	uint16_t u16_from_le_bytes(std::array<char, 2> bytes) {if(is_le) return (uint16_t)*bytes.data(); else return (uint16_t)*(reverse(bytes)).data();}
	uint32_t u32_from_le_bytes(std::array<char, 4> bytes) {if(is_le) return (uint32_t)*bytes.data(); else return (uint32_t)*(reverse(bytes)).data();}
	uint64_t u64_from_le_bytes(std::array<char, 8> bytes) {if(is_le) return (uint64_t)*bytes.data(); else return (uint64_t)*(reverse(bytes)).data();}

	uint16_t u16_from_be_bytes(std::array<char, 2> bytes) {if(!is_le) return (uint16_t)*bytes.data(); else return (uint16_t)*(reverse(bytes)).data();}
	uint32_t u32_from_be_bytes(std::array<char, 4> bytes) {if(!is_le) return (uint32_t)*bytes.data(); else return (uint32_t)*(reverse(bytes)).data();}
	uint64_t u64_from_be_bytes(std::array<char, 8> bytes) {if(!is_le) return (uint64_t)*bytes.data(); else return (uint64_t)*(reverse(bytes)).data();}

	int16_t i16_from_ne_bytes(std::array<char, 2> bytes) {return (int16_t)*bytes.data();}
	int32_t i32_from_ne_bytes(std::array<char, 4> bytes) {return (int32_t)*bytes.data();}
	int64_t i64_from_ne_bytes(std::array<char, 8> bytes) {return (int64_t)*bytes.data();}
	float float_from_ne_bytes(std::array<char, 4> bytes) {return (float)*bytes.data();}
	double double_from_ne_bytes(std::array<char, 8> bytes) {return (double)*bytes.data();}

	int16_t i16_from_le_bytes(std::array<char, 2> bytes) {if(is_le) return (int16_t)*bytes.data(); else return (int16_t)*(reverse(bytes)).data();}
	int32_t i32_from_le_bytes(std::array<char, 4> bytes) {if(is_le) return (int32_t)*bytes.data(); else return (int32_t)*(reverse(bytes)).data();}
	int64_t i64_from_le_bytes(std::array<char, 8> bytes) {if(is_le) return (int64_t)*bytes.data(); else return (int64_t)*(reverse(bytes)).data();}
	float float_from_le_bytes(std::array<char, 4> bytes) {if(is_le) return (float)*bytes.data(); else return (float)*(reverse(bytes)).data();}
	double double_from_le_bytes(std::array<char, 8> bytes) {if(is_le) return (double)*bytes.data(); else return (double)*(reverse(bytes)).data();}

	int16_t i16_from_be_bytes(std::array<char, 2> bytes) {if(!is_le) return (int16_t)*bytes.data(); else return (int16_t)*(reverse(bytes)).data();}
	int32_t i32_from_be_bytes(std::array<char, 4> bytes) {if(!is_le) return (int32_t)*bytes.data(); else return (int32_t)*(reverse(bytes)).data();}
	int64_t i64_from_be_bytes(std::array<char, 8> bytes) {if(!is_le) return (int64_t)*bytes.data(); else return (int64_t)*(reverse(bytes)).data();}
	float float_from_be_bytes(std::array<char, 4> bytes) {if(!is_le) return (float)*bytes.data(); else return (float)*(reverse(bytes)).data();}
	double double_from_be_bytes(std::array<char, 8> bytes) {if(!is_le) return (double)*bytes.data(); else return (double)*(reverse(bytes)).data();}
}
