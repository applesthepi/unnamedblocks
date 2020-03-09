#include "Preprocessor.h"
#include "handlers/Logger.h"

uint64_t* PreprocessorCell::GetUInt()
{
	if (m_type != PreprocessorStorageType::Unsigned)
	{
		Logger::Error("attempted to get uint, invalid type found");
		return nullptr;
	}
	else
		return (uint64_t*)m_value;
}

int64_t* PreprocessorCell::GetSInt()
{
	if (m_type != PreprocessorStorageType::Signed)
	{
		Logger::Error("attempted to get sint, invalid type found");
		return nullptr;
	}
	else
		return (int64_t*)m_value;
}

double* PreprocessorCell::GetDouble()
{
	if (m_type != PreprocessorStorageType::Double)
	{
		Logger::Error("attempted to get double, invalid type found");
		return nullptr;
	}
	else
		return (double*)m_value;
}

const char* PreprocessorCell::GetString()
{
	if (m_type != PreprocessorStorageType::String)
	{
		Logger::Error("attempted to get string, invalid type found");
		return nullptr;
	}
	else
		return (const char*)m_value;
}


bool* PreprocessorCell::GetBoolean()
{
	if (m_type != PreprocessorStorageType::Boolean)
	{
		Logger::Error("attempted to get boolean, invalid type found");
		return nullptr;
	}
	else
		return (bool*)m_value;
}
