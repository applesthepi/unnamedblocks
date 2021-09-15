#include "data.hpp"

#include "cappuccino/registration.hpp"

MODBLOCK_EXPORT cap::mod::block::data::data()
{

}

MODBLOCK_EXPORT cap::mod::block::data::data(const std::vector<void*>& data, const std::vector<cap::mod::block::data::type>& types, const std::vector<cap::mod::block::data::interpretation>& interpretations)
	:m_data(data), m_types(types), m_interpretations(interpretations)
{

}

const std::vector<void*>& cap::mod::block::data::get_data()
{
	return m_data;
}

const std::vector<u64>& cap::mod::block::data::get_runtime_data()
{
	return m_runtime_data;
}

MODBLOCK_EXPORT std::vector<void*>& cap::mod::block::data::get_pre_data()
{
	return m_pre_data;
}

const std::vector<cap::mod::block::data::type>& cap::mod::block::data::get_types()
{
	return m_types;
}

const std::vector<cap::mod::block::data::interpretation>& cap::mod::block::data::get_interpretations()
{
	return m_interpretations;
}

MODBLOCK_EXPORT void cap::mod::block::data::clear_data()
{
	m_data.clear();
}

MODBLOCK_EXPORT void cap::mod::block::data::set_runtime_data(const std::vector<u64>& data)
{
	m_runtime_data = data;
}

MODBLOCK_EXPORT void cap::mod::block::data::set_data_templates(u64 stack_idx)
{
	const std::vector<f64*>& reals = cap::registration::get_real_template();
	const std::vector<bool*>& bools = cap::registration::get_bool_template();
	const std::vector<std::string*>& strings = cap::registration::get_string_template();
	std::string* str;
	for (u64 i = 0; i < m_runtime_data.size(); i++)
	{
		if (m_interpretations[i] == cap::mod::block::data::interpretation::REAL)
			m_data.push_back(&(reals[stack_idx][m_runtime_data[i]]));
		else if (m_interpretations[i] == cap::mod::block::data::interpretation::BOOL)
			m_data.push_back(&(bools[stack_idx][m_runtime_data[i]]));
		else if (m_interpretations[i] == cap::mod::block::data::interpretation::STRING)
			m_data.push_back(&(strings[stack_idx][m_runtime_data[i]]));
	}
}

void cap::mod::block::data::set_types(const std::vector<cap::mod::block::data::type>& types)
{
	m_types = types;
}

void cap::mod::block::data::set_interpretations(const std::vector<cap::mod::block::data::interpretation>& interpretations)
{
	m_interpretations = interpretations;
}

MODBLOCK_EXPORT void cap::mod::block::data::set_interpretation(const cap::mod::block::data::interpretation& interpretation, u64 idx)
{
	m_interpretations[idx] = interpretation;
}

MODBLOCK_EXPORT cap::mod::block::data& cap::mod::block::data::operator=(cap::mod::block::data& data)
{
	m_data = data.m_data;
	m_runtime_data = data.m_runtime_data;
	m_types = data.m_types;
	m_interpretations = data.m_interpretations;

	return *this;
}
