#include "preprocessor_data.hpp"

#include "cappuccino/registration.hpp"

cap::preprocessor_data::preprocessor_data(std::vector<void*>& v_custom)
	: m_v_real(cap::registration::get_real_template()), m_v_bool(cap::registration::get_bool_template()), m_v_string(
cap::registration::get_string_template()), m_v_custom(v_custom)
{
	block_idx = 0;
	stack_idx = 0;
}

void cap::preprocessor_data::add_structure(const std::string& name, void* structure)
{
	m_names.push_back(name);
	m_structures.push_back(structure);
}

void* cap::preprocessor_data::get_structure(const std::string& name)
{
	for (u64 i = 0; i < m_names.size(); i++)
	{
		if (m_names[i] == name)
			return m_structures[i];
	}

	cap::logger::error("failed to locate structure \"" + name + "\"");
	return nullptr;
}
/*
f64& cap::preprocessor_data::GetReal(u64 idx)
{
	return m_vReal[idx];
}

bool& cap::preprocessor_data::GetBool(u64 idx)
{
	return m_vBool[idx];
}

std::string& cap::preprocessor_data::GetString(u64 idx)
{
	return m_vString[idx];
}
*/
u64 cap::preprocessor_data::custom_put(void* mem)
{
	m_v_custom.push_back(mem);
	return m_v_custom.size() - 1;
}
