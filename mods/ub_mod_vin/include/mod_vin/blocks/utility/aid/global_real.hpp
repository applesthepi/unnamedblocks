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
	f64& Get(u64 idx);
	void Put(const std::string& name);
	u64 GetIdx(const std::string& name);
private:
	std::vector<std::string> m_varNames;
	f64* m_variables;
};