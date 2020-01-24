#pragma once

#include <string>
#include <vector>
#include <shared_mutex>
#include <mutex>

class VariableHandler
{
public:
	VariableHandler();
	~VariableHandler();
	void Reset();

	bool StackReal(const char* name);
	bool StackString(const char* name);
	bool StackBool(const char* name);

	void HeapReal(const char* name);
	void HeapString(const char* name);
	void HeapBool(const char* name);

	bool FreeReal(const char* name);
	bool FreeString(const char* name);
	bool FreeBool(const char* name);

	double* GetReal(const char* name);
	std::string* GetString(const char* name);
	bool* GetBool(const char* name);

	bool SetReal(const char* name, double value);
	bool SetString(const char* name, std::string value);
	bool SetBool(const char* name, bool value);

	VariableHandler& operator=(const VariableHandler& other);
private:
	std::shared_mutex* m_mutex;
	const char** m_stackNames;
	std::vector<const char*>* m_heapNames;

	double** m_stackReal;
	std::string** m_stackString;
	bool** m_stackBool;

	std::vector<double*>* m_heapReal;
	std::vector<std::string*>* m_heapString;
	std::vector<bool*>* m_heapBool;
};