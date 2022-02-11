#pragma once
#include "config.h"

#include "cappuccino/data_registry.hpp"
#include "cappuccino/utils.hpp"

namespace cap::mod::block
{
///
class data
{
public:
	///
	enum class type
	{
		VAR,
		RAW
	};

	///
	enum class interpretation
	{
		TEXT,
		STRING,
		REAL,
		BOOL,
		ANY
	};

	data();
	data(
		const std::vector<void*>& data,
		const std::vector<type>& types,
		const std::vector<interpretation>& interpretations);

	///
	const std::vector<void*>& get_data();

	///
	const std::vector<u64>& get_runtime_data();

	///
	std::vector<void*>& get_pre_data();

	///
	const std::vector<type>& get_types();

	///
	const std::vector<interpretation>& get_interpretations();

	///
	void clear_data();

	///
	void set_runtime_data(const std::vector<u64>& data);

	///
	void set_data_templates(u64 stack_idx);

	///
	void set_types(const std::vector<type>& types);

	///
	void set_interpretations(const std::vector<interpretation>& interpretations);

	///
	void set_interpretation(const interpretation& interpretation, u64 idx);

	///
	data& operator=(data& data);

private:
	///
	std::vector<void*> m_data;

	///
	std::vector<u64> m_runtime_data;

	///
	std::vector<void*> m_pre_data;

	///
	std::vector<type> m_types;

	///
	std::vector<interpretation> m_interpretations;
};
} // namespace cap::mod::block