#pragma once

#include <vector>
#include <mutex>
#include <shared_mutex>

class ByteHandler
{
public:
	ByteHandler();
	~ByteHandler();

	void Reset();
	unsigned long long AllocateBytes(unsigned long long size);
	bool DeallocateBytes(unsigned long long address, unsigned long long size);

	unsigned char* GetByte(unsigned long long address);
	bool SetByte(unsigned long long address, unsigned char value);

	ByteHandler& operator=(const ByteHandler& other);
private:
	unsigned char* m_bytes;
	bool* m_bytesBinary;
	unsigned long long* m_bytesSize;
	std::shared_timed_mutex m_mutex;
};
