#pragma once
#include "config.h"

#include "cappuccino/utils.hpp"
#include "cappuccino/data_registry.hpp"

#if LINUX
#define MODBLOCK_EXPORT
#else
#define MODBLOCK_EXPORT __declspec(dllexport)
#endif

namespace cap::mod::block
{
///
class data
{
public:
	///
	enum class type
	{
		VAR, RAW
	};

	///
	enum class interpretation
	{
		TEXT, STRING, REAL, BOOL, ANY
	};

	MODBLOCK_EXPORT data();
	MODBLOCK_EXPORT data(const std::vector<void*>& data, const std::vector<type>& types, const std::vector<interpretation>& interpretations);

	///
	MODBLOCK_EXPORT const std::vector<void*>& get_data();

	///
	MODBLOCK_EXPORT const std::vector<u64>& get_runtime_data();

	///
	MODBLOCK_EXPORT std::vector<void*>& get_pre_data();

	///
	MODBLOCK_EXPORT const std::vector<type>& get_types();

	///
	MODBLOCK_EXPORT const std::vector<interpretation>& get_interpretations();

	///
	MODBLOCK_EXPORT void clear_data();

	///
	MODBLOCK_EXPORT void set_runtime_data(const std::vector<u64>& data);

	///
	MODBLOCK_EXPORT void set_data_templates(u64 stack_idx);

	///
	MODBLOCK_EXPORT void set_types(const std::vector<type>& types);

	///
	MODBLOCK_EXPORT void set_interpretations(const std::vector<interpretation>& interpretations);

	///
	MODBLOCK_EXPORT void set_interpretation(const interpretation& interpretation, u64 idx);

	///
	MODBLOCK_EXPORT data& operator=(data& data);
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
}