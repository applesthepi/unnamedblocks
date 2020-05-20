#pragma once
#include <cinttypes>
#include <string>
#include <vector>

class UtilityGlobal
{
public:
	UtilityGlobal();

	void Bake();
	

	double* GetReal(const uint64_t& idx);
	bool* GetBool(const uint64_t& idx);
	std::string* GetString(const uint64_t& idx);
private:
	std::vector<std::string> m_varNames;
	void** m_variables;
};