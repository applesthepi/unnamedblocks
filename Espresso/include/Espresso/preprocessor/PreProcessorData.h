#pragma once
#include <string>
#include <vector>
#include <map>
#include "Espresso/Logger.h"

enum PreprocessorStorageType
{
	Unsigned,
	Signed,
	Double,
	String,
	Boolean
};

class PreprocessorCell
{
public:
	uint64_t* GetUInt();
	int64_t* GetSInt();
	double* GetDouble();
	const char* GetString();
	bool* GetBoolean();
private:
	PreprocessorStorageType m_type;
	void* m_value;
};

class PreprocessorStack
{
public:
	void AddContainer(const std::string& container);
	bool AddValue(const std::string& container, const std::string& name, const PreprocessorCell& cell);
	const PreprocessorCell& GetValue(const std::string& container, const std::string& name);
private:
	std::map<std::string, std::map<std::string, PreprocessorCell>> m_localContainers;
};

class PreProcessorData
{
public:
	uint64_t RegisterPreProcessorStack();

	// global containers
	
	void AddGlobalContainer(const std::string& container);
	void AddGlobalValue(const std::string& container, const std::string& name, const PreprocessorCell& cell);
	const PreprocessorCell& GetGlobalValue(const std::string& container, const std::string& name);

	// local containers

	void AddLocalContainer(const uint64_t& idx, const std::string& container);
	void AddLocalValue(const uint64_t& idx, const std::string& container, const std::string& name, const PreprocessorCell& cell);
	const PreprocessorCell& GetLocalValue(const uint64_t& idx, const std::string& container, const std::string& name);
private:
	PreprocessorStack m_globalStack;
	std::vector<PreprocessorStack> m_stacks;
};
