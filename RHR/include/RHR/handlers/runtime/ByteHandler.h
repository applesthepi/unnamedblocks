#pragma once

#include <vector>
#include <mutex>
#include <shared_mutex>

class ByteHandler
{
public:
	static void Alloc();
	static void Dealloc();

	static unsigned long long AllocateBytes(unsigned long long size);
	static bool DeallocateBytes(unsigned long long address, unsigned long long size);

	static unsigned char* GetByte(unsigned long long address);
	static bool SetByte(unsigned long long address, unsigned char value);
private:
	static unsigned char* m_bytes;
	static bool* m_bytesBinary;
	static unsigned long long* m_bytesSize;
	static std::shared_mutex m_mutex;
};
