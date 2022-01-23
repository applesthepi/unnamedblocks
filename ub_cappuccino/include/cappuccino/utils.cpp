#include "utils.hpp"

namespace cap::endianness
{
std::array<char, 2> to_ne_bytes(u16 x)
{
	std::array<char, 2> bytes;
	char* value = reinterpret_cast<char*>(&x);
	bytes[0]	= value[0];
	bytes[1]	= value[1];
	return bytes;
}

std::array<char, 4> to_ne_bytes(u32 x)
{
	std::array<char, 4> bytes;
	char* value = reinterpret_cast<char*>(&x);
	bytes[0]	= value[0];
	bytes[1]	= value[1];
	bytes[2]	= value[2];
	bytes[3]	= value[3];
	return bytes;
}

std::array<char, 8> to_ne_bytes(u64 x)
{
	std::array<char, 8> bytes;
	char* value = reinterpret_cast<char*>(&x);
	bytes[0]	= value[0];
	bytes[1]	= value[1];
	bytes[2]	= value[2];
	bytes[3]	= value[3];
	bytes[4]	= value[4];
	bytes[5]	= value[5];
	bytes[6]	= value[6];
	bytes[7]	= value[7];
	return bytes;
}

std::array<char, 2> to_le_bytes(u16 x)
{
	if (is_le)
	{
		return to_ne_bytes(x);
	}
	else
	{
		std::array<char, 2> bytes;
		char* value = reinterpret_cast<char*>(&x);
		bytes[3]	= value[0];
		bytes[2]	= value[1];
		return bytes;
	}
}

std::array<char, 4> to_le_bytes(u32 x)
{
	if (is_le)
	{
		return to_ne_bytes(x);
	}
	else
	{
		std::array<char, 4> bytes;
		char* value = reinterpret_cast<char*>(&x);
		bytes[3]	= value[0];
		bytes[2]	= value[1];
		bytes[1]	= value[2];
		bytes[0]	= value[3];
		return bytes;
	}
}

std::array<char, 8> to_le_bytes(u64 x)
{
	if (is_le)
	{
		return to_ne_bytes(x);
	}
	else
	{
		std::array<char, 8> bytes;
		char* value = reinterpret_cast<char*>(&x);
		bytes[7]	= value[0];
		bytes[6]	= value[1];
		bytes[5]	= value[2];
		bytes[4]	= value[3];
		bytes[3]	= value[4];
		bytes[2]	= value[5];
		bytes[1]	= value[6];
		bytes[0]	= value[7];
		return bytes;
	}
}

std::array<char, 2> to_be_bytes(u16 x)
{
	if (!is_le)
	{
		return to_ne_bytes(x);
	}
	else
	{
		std::array<char, 2> bytes;
		char* value = reinterpret_cast<char*>(&x);
		bytes[3]	= value[0];
		bytes[2]	= value[1];
		return bytes;
	}
}

std::array<char, 4> to_be_bytes(u32 x)
{
	if (!is_le)
	{
		return to_ne_bytes(x);
	}
	else
	{
		std::array<char, 4> bytes;
		char* value = reinterpret_cast<char*>(&x);
		bytes[3]	= value[0];
		bytes[2]	= value[1];
		bytes[1]	= value[2];
		bytes[0]	= value[3];
		return bytes;
	}
}

std::array<char, 8> to_be_bytes(u64 x)
{
	if (!is_le)
	{
		return to_ne_bytes(x);
	}
	else
	{
		std::array<char, 8> bytes;
		char* value = reinterpret_cast<char*>(&x);
		bytes[7]	= value[0];
		bytes[6]	= value[1];
		bytes[5]	= value[2];
		bytes[4]	= value[3];
		bytes[3]	= value[4];
		bytes[2]	= value[5];
		bytes[1]	= value[6];
		bytes[0]	= value[7];
		return bytes;
	}
}
template<usize T>
std::array<char, T> reverse(std::array<char, T> bytes)
{
	std::array<char, T> reversed;
	for (usize i = 0; i < T; i++)
	{
		reversed[reversed.size() - i] = bytes[i];
	}
	return reversed;
}
std::array<char, 4> to_ne_bytes(f32 x) { return to_ne_bytes(*(u32*)(void*)&x); }
std::array<char, 8> to_ne_bytes(f64 x) { return to_ne_bytes(*(u64*)(void*)&x); }
std::array<char, 2> to_ne_bytes(i16 x) { return to_ne_bytes(*(u16*)(void*)&x); }
std::array<char, 4> to_ne_bytes(i32 x) { return to_ne_bytes(*(u32*)(void*)&x); }
std::array<char, 8> to_ne_bytes(i64 x) { return to_ne_bytes(*(u64*)(void*)&x); }

std::array<char, 4> to_le_bytes(f32 x) { return to_le_bytes(*(u32*)(void*)&x); }
std::array<char, 8> to_le_bytes(f64 x) { return to_le_bytes(*(u64*)(void*)&x); }
std::array<char, 2> to_le_bytes(i16 x) { return to_le_bytes(*(u16*)(void*)&x); }
std::array<char, 4> to_le_bytes(i32 x) { return to_le_bytes(*(u32*)(void*)&x); }
std::array<char, 8> to_le_bytes(i64 x) { return to_le_bytes(*(u64*)(void*)&x); }

std::array<char, 4> to_be_bytes(f32 x) { return to_be_bytes(*(u32*)(void*)&x); }
std::array<char, 8> to_be_bytes(f64 x) { return to_be_bytes(*(u64*)(void*)&x); }
std::array<char, 2> to_be_bytes(i16 x) { return to_be_bytes(*(u16*)(void*)&x); }
std::array<char, 4> to_be_bytes(i32 x) { return to_be_bytes(*(u32*)(void*)&x); }
std::array<char, 8> to_be_bytes(i64 x) { return to_be_bytes(*(u64*)(void*)&x); }

u16 u16_from_ne_bytes(std::array<char, 2> bytes) { return (u16)*bytes.data(); }
u32 u32_from_ne_bytes(std::array<char, 4> bytes) { return (u32)*bytes.data(); }
u64 u64_from_ne_bytes(std::array<char, 8> bytes) { return (u64)*bytes.data(); }

u16 u16_from_le_bytes(std::array<char, 2> bytes)
{
	if (is_le)
		return (u16)*bytes.data();
	else
		return (u16) * (reverse<2>(bytes)).data();
}
u32 u32_from_le_bytes(std::array<char, 4> bytes)
{
	if (is_le)
		return (u32)*bytes.data();
	else
		return (u32) * (reverse<4>(bytes)).data();
}
u64 u64_from_le_bytes(std::array<char, 8> bytes)
{
	if (is_le)
		return (u64)*bytes.data();
	else
		return (u64) * (reverse<8>(bytes)).data();
}

u16 u16_from_be_bytes(std::array<char, 2> bytes)
{
	if (!is_le)
		return (u16)*bytes.data();
	else
		return (u16) * (reverse<2>(bytes)).data();
}
u32 u32_from_be_bytes(std::array<char, 4> bytes)
{
	if (!is_le)
		return (u32)*bytes.data();
	else
		return (u32) * (reverse<4>(bytes)).data();
}
u64 u64_from_be_bytes(std::array<char, 8> bytes)
{
	if (!is_le)
		return (u64)*bytes.data();
	else
		return (u64) * (reverse<8>(bytes)).data();
}

i16 i16_from_ne_bytes(std::array<char, 2> bytes) { return (i16)*bytes.data(); }
i32 i32_from_ne_bytes(std::array<char, 4> bytes) { return (i32)*bytes.data(); }
i64 i64_from_ne_bytes(std::array<char, 8> bytes) { return (i64)*bytes.data(); }
f32 f32_from_ne_bytes(std::array<char, 4> bytes) { return (f32)*bytes.data(); }
f64 f64_from_ne_bytes(std::array<char, 8> bytes) { return (f64)*bytes.data(); }

i16 i16_from_le_bytes(std::array<char, 2> bytes)
{
	if (is_le)
		return (i16)*bytes.data();
	else
		return (i16) * (reverse<2>(bytes)).data();
}
i32 i32_from_le_bytes(std::array<char, 4> bytes)
{
	if (is_le)
		return (i32)*bytes.data();
	else
		return (i32) * (reverse<4>(bytes)).data();
}
i64 i64_from_le_bytes(std::array<char, 8> bytes)
{
	if (is_le)
		return (i64)*bytes.data();
	else
		return (i64) * (reverse<8>(bytes)).data();
}
f32 f32_from_le_bytes(std::array<char, 4> bytes)
{
	if (is_le)
		return (f32)*bytes.data();
	else
		return (f32) * (reverse<4>(bytes)).data();
}
f64 f64_from_le_bytes(std::array<char, 8> bytes)
{
	if (is_le)
		return (f64)*bytes.data();
	else
		return (f64) * (reverse<8>(bytes)).data();
}

i16 i16_from_be_bytes(std::array<char, 2> bytes)
{
	if (!is_le)
		return (i16)*bytes.data();
	else
		return (i16) * (reverse<2>(bytes)).data();
}
i32 i32_from_be_bytes(std::array<char, 4> bytes)
{
	if (!is_le)
		return (i32)*bytes.data();
	else
		return (i32) * (reverse<4>(bytes)).data();
}
i64 i64_from_be_bytes(std::array<char, 8> bytes)
{
	if (!is_le)
		return (i64)*bytes.data();
	else
		return (i64) * (reverse<8>(bytes)).data();
}
f32 f32_from_be_bytes(std::array<char, 4> bytes)
{
	if (!is_le)
		return (f32)*bytes.data();
	else
		return (f32) * (reverse<4>(bytes)).data();
}
f64 f64_from_be_bytes(std::array<char, 8> bytes)
{
	if (!is_le)
		return (f64)*bytes.data();
	else
		return (f64) * (reverse<8>(bytes)).data();
}
} // namespace cap::endianness
