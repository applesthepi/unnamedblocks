#pragma once
#include "config.h"

#include "cappuccino/utils.hpp"

namespace cap
{
///
class preprocessor_data
{
public:
	preprocessor_data(std::vector<void*>& v_custom);

	/// Should be destroyed after use.
	void add_structure(const std::string& name, void* structure);

	/// Should be destroyed after use.
	void* get_structure(const std::string& name);

	/// these are NOT a blocks' arguments! These are the variables' storage inside Cappuccino.
	/// only use these if your know what your doing!
	/// if your trying to use the  blocks' arguments during the initializations, then use the cap::mod::block::data, not
	/// this.
	u64 custom_put(void* mem);

	// f64& GetReal(u64 idx);
	// bool& GetBool(u64 idx);
	// std::string& GetString(u64 idx);

	///
	u64 block_idx;

	///
	u64 stack_idx;

private:
	///
	std::vector<std::string> m_names;

	///
	std::vector<void*> m_structures;

	///
	const std::vector<f64*>& m_v_real;

	///
	const std::vector<bool*>& m_v_bool;

	///
	const std::vector<std::string*>& m_v_string;

	///
	std::vector<void*>& m_v_custom;
};
} // namespace cap