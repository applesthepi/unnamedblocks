#pragma once
#include <vector>
#include <map>

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
	bool AddContainer(const std::string& container);
	bool AddValue(const std::string& container, const std::string& name, const PreprocessorCell& cell);
;
private:
	std::map<std::string, std::map<std::string, PreprocessorCell>> m_localContainers;
};

class Preprocessor
{
public:
	bool AddContainer(const std::string& container);
	bool AddValue(const std::string& container, const std::string& name, const PreprocessorCell& cell);
private:
	std::map<std::string, std::map<std::string, PreprocessorCell>> m_globalContainers;
	std::vector<PreprocessorStack> m_stacks;
};
