#pragma once
#include <cinttypes>
#include <string>
#include <vector>

#define GLOBAL_REAL_NAME "vin_global_real"
#define GLOBAL_REAL_CUSTOM "vin_global_real_custom"

class UtilityGlobalReal
{
public:
	UtilityGlobalReal();
	~UtilityGlobalReal();

	void Bake();
	double& Get(const uint64_t& idx);
	void Put(const std::string& name);
	uint64_t GetIdx(const std::string& name);
private:
	std::vector<std::string> m_varNames;
	double* m_variables;
};