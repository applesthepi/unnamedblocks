#pragma once

#include <string>
#include <vector>
#include <mutex>

class VariableHandler
{
public:
	static void Alloc();
	static void Dealloc();

	static bool StackReal(const char* name);
	static bool StackString(const char* name);
	static bool StackBool(const char* name);

	static void HeapReal(const char* name);
	static void HeapString(const char* name);
	static void HeapBool(const char* name);

	static bool FreeReal(const char* name);
	static bool FreeString(const char* name);
	static bool FreeBool(const char* name);

	static double* GetReal(const char* name);
	static std::string* GetString(const char* name);
	static bool* GetBool(const char* name);

	static bool SetReal(const char* name, double value);
	static bool SetString(const char* name, std::string value);
	static bool SetBool(const char* name, bool value);
private:
	static std::mutex m_mutex;
	static const char** m_stackNames;
	static std::vector<const char*>* m_heapNames;

	static double** m_stackReal;
	static std::string** m_stackString;
	static bool** m_stackBool;

	static std::vector<double*>* m_heapReal;
	static std::vector<std::string*>* m_heapString;
	static std::vector<bool*>* m_heapBool;
};