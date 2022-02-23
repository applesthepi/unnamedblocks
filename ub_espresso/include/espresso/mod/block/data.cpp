#include "data.hpp"

#include "cappuccino/registration.hpp"

espresso::mod::block::data::data()
	: m_t_real(nullptr)
	, m_t_bool(nullptr)
	, m_t_string(nullptr)
{}

espresso::mod::block::data::data(
	const std::vector<void*>& data,
	const std::vector<espresso::mod::block::data::type>& types,
	const std::vector<espresso::mod::block::data::interpretation>& interpretations,
	std::vector<f64*>* t_real,
	std::vector<bool*>* t_bool,
	std::vector<std::string*>* t_string
	)
	: m_data(data)
	, m_types(types)
	, m_interpretations(interpretations)
	, m_t_real(t_real)
	, m_t_bool(t_bool)
	, m_t_string(t_string)
{}

const std::vector<void*>& espresso::mod::block::data::get_data()
{
	return m_data;
}

const std::vector<u64>& espresso::mod::block::data::get_runtime_data()
{
	return m_runtime_data;
}

std::vector<void*>& espresso::mod::block::data::get_pre_data()
{
	return m_pre_data;
}

const std::vector<espresso::mod::block::data::type>& espresso::mod::block::data::get_types()
{
	return m_types;
}

const std::vector<espresso::mod::block::data::interpretation>& espresso::mod::block::data::get_interpretations()
{
	return m_interpretations;
}

void espresso::mod::block::data::clear_data()
{
	m_data.clear();
}

void espresso::mod::block::data::set_runtime_data(const std::vector<u64>& data)
{
	m_runtime_data = data;
}

void espresso::mod::block::data::set_data_templates(u64 stack_idx)
{
	std::vector<f64*>* reals			 = m_t_real;
	std::vector<bool*>* bools			 = m_t_bool;
	std::vector<std::string*>* strings = m_t_string;
	std::string* str;
	for (u64 i = 0; i < m_runtime_data.size(); i++)
	{
		if (m_interpretations[i] == espresso::mod::block::data::interpretation::REAL)
			m_data.push_back(&(reals[stack_idx][m_runtime_data[i]]));
		else if (m_interpretations[i] == espresso::mod::block::data::interpretation::BOOL)
			m_data.push_back(&(bools[stack_idx][m_runtime_data[i]]));
		else if (m_interpretations[i] == espresso::mod::block::data::interpretation::STRING)
			m_data.push_back(&(strings[stack_idx][m_runtime_data[i]]));
	}
}

void espresso::mod::block::data::set_types(const std::vector<espresso::mod::block::data::type>& types)
{
	m_types = types;
}

void espresso::mod::block::data::set_interpretations(
	const std::vector<espresso::mod::block::data::interpretation>& interpretations)
{
	m_interpretations = interpretations;
}

void espresso::mod::block::data::set_interpretation(const espresso::mod::block::data::interpretation& interpretation, u64 idx)
{
	m_interpretations[idx] = interpretation;
}

espresso::mod::block::data& espresso::mod::block::data::operator=(espresso::mod::block::data& data)
{
	m_data			  = data.m_data;
	m_runtime_data	  = data.m_runtime_data;
	m_types			  = data.m_types;
	m_interpretations = data.m_interpretations;
	m_t_real          = data.m_t_real;
	m_t_bool          = data.m_t_bool;
	m_t_string        = data.m_t_string;

	return *this;
}
