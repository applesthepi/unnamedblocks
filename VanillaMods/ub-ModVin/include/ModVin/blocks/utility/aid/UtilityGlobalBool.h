#pragma once
#include <cinttypes>
#include <string>
#include <vector>

class UtilityGlobal
{
public:
	UtilityGlobal();

	void Bake();
	

	double* GetReal(uint64_t idx);
	bool* GetBool(uint64_t idx);
	std::string* GetString(uint64_t idx);
private:
	std::vector<std::string> m_varNames;
	void** m_variables;
};