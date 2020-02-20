#include "ByteHandler.h"
#include "handlers/Logger.h"

#include <cstring>
#include <cstdlib>
#include <string>

ByteHandler::ByteHandler()
{
	m_bytesSize = new unsigned long long(1000);
	m_bytes = (unsigned char*)calloc(*m_bytesSize, sizeof(unsigned char));
	m_bytesBinary = (bool*)calloc(*m_bytesSize, sizeof(bool));
}

ByteHandler::~ByteHandler()
{
	delete m_bytesSize;

	free(m_bytes);
	free(m_bytesBinary);
}

void ByteHandler::Reset()
{
	*m_bytesSize = 1000;

	memset(m_bytes, 0, *m_bytesSize);
	memset(m_bytesBinary, false, *m_bytesSize);
}

unsigned long long ByteHandler::AllocateBytes(unsigned long long size)
{
	std::unique_lock<std::shared_timed_mutex> lock(m_mutex);

	unsigned long long sprint = 0;
	unsigned long long count = 0;

	while (sprint < *m_bytesSize)
	{
		if (m_bytesBinary[sprint])
			count = 0;
		else
			count++;

		if (count == size)
		{
			memset(m_bytesBinary + (sprint - count) + 1, true, size);
			return (sprint - count) + 1;
		}

		sprint++;
	}

	while (*m_bytesSize - sprint < size)
	{
		*m_bytesSize *= 2;
	}

	m_bytes = (unsigned char*)realloc(m_bytes, *m_bytesSize);
	m_bytesBinary = (bool*)realloc(m_bytesBinary, *m_bytesSize);

	memset(m_bytes + sprint, 0, size);
	memset(m_bytesBinary + sprint, true, size);

	return sprint;
}

bool ByteHandler::DeallocateBytes(unsigned long long address, unsigned long long size)
{
	std::unique_lock<std::shared_timed_mutex> lock(m_mutex);

	if (address + size > * m_bytesSize)
	{
		Logger::Error("failed to deallocate " + std::to_string(size) + " bytes at " + std::to_string(address));
		return false;
	}

	memset(m_bytes + address, 0, size);
	memset(m_bytesBinary + address, false, size);

	return true;
}

unsigned char* ByteHandler::GetByte(unsigned long long address)
{
	std::shared_lock<std::shared_timed_mutex> lock(m_mutex);

	if (address >= *m_bytesSize)
		return nullptr;

	return &(m_bytes[address]);
}

bool ByteHandler::SetByte(unsigned long long address, unsigned char value)
{
	std::unique_lock<std::shared_timed_mutex> lock(m_mutex);

	if (address >= *m_bytesSize)
	{
		Logger::Error("failed to set byte " + std::to_string(value) + " at address " + std::to_string(address));
		return false;
	}

	m_bytes[address] = value;
	return true;
}

ByteHandler& ByteHandler::operator=(const ByteHandler& other)
{
	return *this;
}
