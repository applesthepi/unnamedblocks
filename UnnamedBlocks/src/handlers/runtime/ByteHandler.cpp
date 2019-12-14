#include "ByteHandler.h"

void ByteHandler::Alloc()
{
	m_bytesSize = new unsigned long long(1000);
	m_bytes = (unsigned char*)calloc(*m_bytesSize, sizeof(unsigned char));
	m_bytesBinary = (bool*)malloc(*m_bytesSize * sizeof(unsigned char));
	memset(m_bytesBinary, false, *m_bytesSize);
}

void ByteHandler::Dealloc()
{
	delete m_bytesSize;
	free(m_bytes);
	free(m_bytesBinary);
}

unsigned long long ByteHandler::AllocateBytes(unsigned long long size)
{
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
	if (address + size > *m_bytesSize)
		return false;

	memset(m_bytes + address, 0, size);
	memset(m_bytesBinary + address, false, size);
}

unsigned char* ByteHandler::GetByte(unsigned long long address)
{
	if (address >= *m_bytesSize)
		return nullptr;

	return &(m_bytes[address]);
}

bool ByteHandler::SetByte(unsigned long long address, unsigned char value)
{
	if (address >= *m_bytesSize)
		return false;

	m_bytes[address] = value;
	return true;
}

unsigned char* ByteHandler::m_bytes;

bool* ByteHandler::m_bytesBinary;

unsigned long long* ByteHandler::m_bytesSize;
