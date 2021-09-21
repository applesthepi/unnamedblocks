#pragma once
#include <cinttypes>
#include <string>
#include <vector>

class UtilityGlobal
{
public:
	UtilityGlobal();

	void Bake();
	

	double* get_real(uint64_t idx);
	bool* get_bool(uint64_t idx);
	std::string* get_string(uint64_t idx);
private:
	std::vector<std::string> m_varNames;
	void** m_variables;
};