#include "pass.hpp"

#include "cappuccino/registration.hpp"

#include <iostream>

static f64 gReal  = 0.0;
static bool gBool = false;
static std::string gString;

cap::mod::block::pass::pass(const initializer& init)
{
	if (init.debug_mode)
	{
		m_get_real	 = &pass::get_real_debug;
		m_get_bool	 = &pass::get_bool_debug;
		m_get_string = &pass::get_string_debug;

		m_get_pre_data = &pass::get_pre_data_debug;
	}
	else
	{
		m_get_real	 = &pass::get_real_release;
		m_get_bool	 = &pass::get_bool_release;
		m_get_string = &pass::get_string_release;

		m_get_pre_data = &pass::get_pre_data_release;
	}

	m_variables_real_count	 = init.variables_real_count;
	m_variables_bool_count	 = init.variables_bool_count;
	m_variables_string_count = init.variables_string_count;
	m_custom_registrer_mutex = init.custom_register_mutex;
	m_custom_register		 = init.custom_register;
	m_stop					 = init.stop;
	m_variable_registry		 = init.variable_registry;
	m_begin_time			 = init.begin_time;
	m_random.seed(std::time(0));
	m_messages_info.reserve(1024);
	m_messages_warn.reserve(1024);
	m_messages_error.reserve(1024);
	m_active_idx = nullptr;

	m_data = cap::registration::get_data();

	// ======================================================================================================
	// ================== setup functions
	// ======================================================================================================

	// pre

	for (u64 i = 0; i < cap::registration::get_function_total_count(); i++)
	{
		void*** stack = new void**[cap::registration::get_function_call_count()[i]];
		std::vector<u64> sizes;

		for (u64 a = 0; a < cap::registration::get_function_call_count()[i]; a++)
		{
			stack[a] = m_data[i][a].get_pre_data().data();
			sizes.push_back(m_data[i][a].get_pre_data().size());
		}

		u64* h_sizes = new u64[sizes.size()];

		for (u64 a = 0; a < sizes.size(); a++)
			h_sizes[a] = sizes[a];

		m_data_stack_pre.push_back(stack);
		m_pre_data_count.push_back(h_sizes);
	}

	// idx

	for (u64 i = 0; i < cap::registration::get_function_total_count(); i++)
	{
		u64** stack = new u64*[cap::registration::get_function_call_count()[i]];

		for (u64 a = 0; a < cap::registration::get_function_call_count()[i]; a++)
			stack[a] = (u64*)m_data[i][a].get_runtime_data().data();

		m_data_stack_idx.push_back((const u64**)stack);
	}

	// ======================================================================================================
	// ================== setup types
	// ======================================================================================================

	const std::vector<f64*>& real_template			 = cap::registration::get_real_template();
	const std::vector<bool*>& bool_template			 = cap::registration::get_bool_template();
	const std::vector<std::string*>& string_template = cap::registration::get_string_template();

	for (u64 i = 0; i < cap::registration::get_function_total_count(); i++)
	{
		m_data_stack_real.push_back(new f64[m_variables_real_count->at(i)]);

		for (u64 a = 0; a < m_variables_real_count->at(i); a++)
			m_data_stack_real[i][a] = real_template[i][a];
	}

	for (u64 i = 0; i < cap::registration::get_function_total_count(); i++)
	{
		m_data_stack_bool.push_back(new bool[m_variables_bool_count->at(i)]);

		for (u64 a = 0; a < m_variables_bool_count->at(i); a++)
			m_data_stack_bool[i][a] = bool_template[i][a];
	}

	for (u64 i = 0; i < cap::registration::get_function_total_count(); i++)
	{
		m_data_stack_string.push_back(new std::string[m_variables_string_count->at(i)]);

		for (u64 a = 0; a < m_variables_string_count->at(i); a++)
			m_data_stack_string[i][a] = string_template[i][a];
	}
}

void cap::mod::block::pass::set_data(cap::mod::block::data** data) { m_data = data; }

void cap::mod::block::pass::set_callstack_stack(std::vector<u64>* callstack) { m_callstack_stack_idx = callstack; }

void cap::mod::block::pass::set_callstack_block(std::vector<u64>* callstack) { m_callstack_block_idx = callstack; }

void cap::mod::block::pass::set_successful(bool* successful) { m_successful = successful; }

void cap::mod::block::pass::set_finished(std::atomic<bool>* finished) { m_finished = finished; }

void cap::mod::block::pass::set_callstack_local(void (***local_callstack)(pass*)) { m_local_callstack = local_callstack; }

void cap::mod::block::pass::set_calls(void (***calls)(pass*)) { m_calls = calls; }

void cap::mod::block::pass::perform_deallocation_callbacks()
{
	for (u64 i = 0; i < m_deallocation_calls.size(); i++)
	{
		(*(m_deallocation_calls[i]))(this);
		delete m_deallocation_calls[i];
	}

	m_deallocation_calls.clear();
}

std::chrono::steady_clock::time_point* cap::mod::block::pass::get_begin_time() { return m_begin_time; }

void cap::mod::block::pass::stop()
{
	m_stop();

	while (!*m_finished)
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void cap::mod::block::pass::add_callstack(u64 stack, u64 block, bool special)
{
	m_callstack_stack_idx->push_back(stack);
	m_callstack_block_idx->push_back(block);

	if (special)
	{
		m_stacking_special.push_back(true);

		f64* reals			 = new f64[m_variables_real_count->at(m_callstack_stack_idx->back())];
		bool* bools			 = new bool[m_variables_bool_count->at(m_callstack_stack_idx->back())];
		std::string* strings = new std::string[m_variables_string_count->at(m_callstack_stack_idx->back())];

		for (u64 i = 0; i < m_variables_real_count->at(m_callstack_stack_idx->back()); i++)
			reals[i] = m_data_stack_real[m_callstack_stack_idx->back()][i];

		for (u64 i = 0; i < m_variables_bool_count->at(m_callstack_stack_idx->back()); i++)
			bools[i] = m_data_stack_bool[m_callstack_stack_idx->back()][i];

		for (u64 i = 0; i < m_variables_string_count->at(m_callstack_stack_idx->back()); i++)
			strings[i] = m_data_stack_string[m_callstack_stack_idx->back()][i];

		m_stacking_real.push_back(reals);
		m_stacking_bool.push_back(bools);
		m_stacking_string.push_back(strings);
	}
	else
	{
		m_stacking_special.push_back(false);

		m_stacking_real.push_back(m_data_stack_real[m_callstack_stack_idx->back()]);
		m_stacking_bool.push_back(m_data_stack_bool[m_callstack_stack_idx->back()]);
		m_stacking_string.push_back(m_data_stack_string[m_callstack_stack_idx->back()]);
	}

	update_locations();
}

void cap::mod::block::pass::pop_callstack()
{
	m_callstack_stack_idx->pop_back();
	m_callstack_block_idx->pop_back();

	if (m_stacking_special.back())
	{
		delete[] m_stacking_real.back();
		delete[] m_stacking_bool.back();
		delete[] m_stacking_string.back();
	}

	m_stacking_real.pop_back();
	m_stacking_bool.pop_back();
	m_stacking_string.pop_back();

	if (m_callstack_stack_idx->size() > 0)
		update_locations();
	else
	{
		*m_finished	  = true;
		*m_successful = true;
	}
}

void cap::mod::block::pass::set_stack_idx(u64 idx)
{
	if (m_callstack_stack_idx->size() > 0)
		m_callstack_stack_idx->back() = idx;

	update_locations();
}

void cap::mod::block::pass::set_block_idx(u64 idx)
{
	if (m_callstack_block_idx->size() > 0)
		m_callstack_block_idx->back() = idx;
}

std::mt19937_64& cap::mod::block::pass::get_random_generator() { return m_random; }

void cap::mod::block::pass::add_deallocation(std::function<void(cap::mod::block::pass*)>* dealloc) { m_deallocation_calls.push_back(dealloc); }

void cap::mod::block::pass::remove_deallocation(std::function<void(cap::mod::block::pass*)>* dealloc)
{
	for (u64 i = 0; i < m_deallocation_calls.size(); i++)
	{
		if (m_deallocation_calls[i] == dealloc)
		{
			m_deallocation_calls.erase(m_deallocation_calls.begin() + i);
			delete dealloc;
			return;
		}
	}
}

u64 cap::mod::block::pass::custom_put(void* mem)
{
	// TODO change to shared
	std::unique_lock<std::mutex> lock(*m_custom_registrer_mutex);

	u64 customIdx = m_custom_register->size();
	m_custom_register->push_back(mem);

	return customIdx;
}

void* cap::mod::block::pass::custom_get(u64 idx)
{
	// TODO change to shared
	std::unique_lock<std::mutex> lock(*m_custom_registrer_mutex);

	return m_custom_register->at(idx);
}

void cap::mod::block::pass::custom_free(u64 idx, bool deallocate)
{
	std::unique_lock<std::mutex> lock(*m_custom_registrer_mutex);

	if (deallocate)
		delete m_custom_register->at(idx);

	m_custom_register->at(idx) = nullptr;
}

void cap::mod::block::pass::log_info(const std::string& message)
{
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

	// Should be enough
	char prefix[100];

	std::unique_lock<std::mutex> lock = std::unique_lock<std::mutex>(m_messages_mutex);

	snprintf(
		prefix,
		100,
		"[%02lu:%02lu] [%03lu%03luu%03luu] ",
		((u64)std::chrono::duration_cast<std::chrono::minutes>(now - *m_begin_time).count()),
		((u64)std::chrono::duration_cast<std::chrono::seconds>(now - *m_begin_time).count()),
		((u64)std::chrono::duration_cast<std::chrono::milliseconds>(now - *m_begin_time).count()) % 1000,
		((u64)std::chrono::duration_cast<std::chrono::microseconds>(now - *m_begin_time).count()) % 1000,
		((u64)std::chrono::duration_cast<std::chrono::nanoseconds>(now - *m_begin_time).count()) % 1000);

	std::string& str = m_messages_info.emplace_back();
	str.reserve(strlen(prefix) + message.length());
	str += prefix;
	str += message;
}

void cap::mod::block::pass::log_warning(const std::string& message)
{
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

	// Should be enough
	char prefix[100];

	std::unique_lock<std::mutex> lock = std::unique_lock<std::mutex>(m_messages_mutex);

	snprintf(
		prefix,
		100,
		"[%02lu:%02lu] [%03lu%03luu%03luu] ",
		((u64)std::chrono::duration_cast<std::chrono::minutes>(now - *m_begin_time).count()),
		((u64)std::chrono::duration_cast<std::chrono::seconds>(now - *m_begin_time).count()),
		((u64)std::chrono::duration_cast<std::chrono::milliseconds>(now - *m_begin_time).count()) % 1000,
		((u64)std::chrono::duration_cast<std::chrono::microseconds>(now - *m_begin_time).count()) % 1000,
		((u64)std::chrono::duration_cast<std::chrono::nanoseconds>(now - *m_begin_time).count()) % 1000);

	// TODO: overhaul cap logging

	std::string& str = m_messages_warn.emplace_back();
	str.reserve(strlen(prefix) + message.length());
	str += prefix;
	str += message;
}

void cap::mod::block::pass::log_error(const std::string& message, cap::mod::block::pass::logger_fatality fatality)
{
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

	// Should be enough
	char prefix[100];

	std::unique_lock<std::mutex> lock = std::unique_lock<std::mutex>(m_messages_mutex);

	if (fatality == logger_fatality::OK)
		snprintf(
			prefix,
			100,
			"[E-OK] [%02lu:%02lu] [%03lu:%03lu:%03lu] ",
			((u64)std::chrono::duration_cast<std::chrono::minutes>(now - *m_begin_time).count()),
			((u64)std::chrono::duration_cast<std::chrono::seconds>(now - *m_begin_time).count()),
			((u64)std::chrono::duration_cast<std::chrono::milliseconds>(now - *m_begin_time).count()) % 1000,
			((u64)std::chrono::duration_cast<std::chrono::microseconds>(now - *m_begin_time).count()) % 1000,
			((u64)std::chrono::duration_cast<std::chrono::nanoseconds>(now - *m_begin_time).count()) % 1000);
	else if (fatality == logger_fatality::BREAK)
		snprintf(
			prefix,
			100,
			"[E-BREAK] [%02lu:%02lu] [%03lu:%03lu:%03lu] ",
			((u64)std::chrono::duration_cast<std::chrono::minutes>(now - *m_begin_time).count()),
			((u64)std::chrono::duration_cast<std::chrono::seconds>(now - *m_begin_time).count()),
			((u64)std::chrono::duration_cast<std::chrono::milliseconds>(now - *m_begin_time).count()) % 1000,
			((u64)std::chrono::duration_cast<std::chrono::microseconds>(now - *m_begin_time).count()) % 1000,
			((u64)std::chrono::duration_cast<std::chrono::nanoseconds>(now - *m_begin_time).count()) % 1000);
	else if (fatality == logger_fatality::ABORT)
		snprintf(
			prefix,
			100,
			"[E-ABORT] [%02lu:%02lu] [%03lu:%03lu:%03lu] ",
			((u64)std::chrono::duration_cast<std::chrono::minutes>(now - *m_begin_time).count()),
			((u64)std::chrono::duration_cast<std::chrono::seconds>(now - *m_begin_time).count()),
			((u64)std::chrono::duration_cast<std::chrono::milliseconds>(now - *m_begin_time).count()) % 1000,
			((u64)std::chrono::duration_cast<std::chrono::microseconds>(now - *m_begin_time).count()) % 1000,
			((u64)std::chrono::duration_cast<std::chrono::nanoseconds>(now - *m_begin_time).count()) % 1000);

	// TODO break or abort

	std::string& str = m_messages_error.emplace_back();
	str.reserve(strlen(prefix) + message.length());
	str += prefix;
	str += message;
}

f64& cap::mod::block::pass::get_real(u64 idx) { return (this->*(m_get_real))(idx); }

bool& cap::mod::block::pass::get_bool(u64 idx) { return (this->*(m_get_bool))(idx); }

std::string& cap::mod::block::pass::get_string(u64&& idx) { return (this->*(m_get_string))(idx); }

void* cap::mod::block::pass::get_pre_data(u64 idx) { return (this->*(m_get_pre_data))(idx); }

const std::vector<std::string>& cap::mod::block::pass::pull_messages_info() { return m_messages_info; }

const std::vector<std::string>& cap::mod::block::pass::pull_messages_warn() { return m_messages_warn; }

const std::vector<std::string>& cap::mod::block::pass::pull_messages_error() { return m_messages_error; }

void cap::mod::block::pass::lock_messages() { m_messages_mutex.lock(); }

void cap::mod::block::pass::unlock_messages() { m_messages_mutex.unlock(); }

void cap::mod::block::pass::update_locations()
{
	*m_local_callstack = m_calls[m_callstack_stack_idx->back()];

	m_active_real	= m_stacking_real.back();
	m_active_bool	= m_stacking_bool.back();
	m_active_string = m_stacking_string.back();
	m_active_idx	= m_data_stack_idx[m_callstack_stack_idx->back()];
	m_active_pre	= m_data_stack_pre[m_callstack_stack_idx->back()];
}

f64& cap::mod::block::pass::get_real_debug(u64 idx)
{
	if (idx >= m_variables_real_count->at(m_callstack_stack_idx->back()))
	{
		log_error(
			"attempted to get real out of range \"" + std::to_string(idx) + "\". registry size is \"" + std::to_string(m_variables_real_count->at(m_callstack_stack_idx->back()))
				+ "\"",
			logger_fatality::ABORT);
		return gReal;
	}

	u64 v_idx  = m_active_idx[m_callstack_block_idx->back()][idx];
	f64& value = m_active_real[v_idx];
	return value;
}

f64& cap::mod::block::pass::get_real_release(u64 idx) { return m_active_real[m_active_idx[m_callstack_block_idx->back()][idx]]; }

bool& cap::mod::block::pass::get_bool_debug(u64 idx)
{
	if (idx >= m_variables_bool_count->at(m_callstack_stack_idx->back()))
	{
		log_error(
			"attempted to get bool out of range \"" + std::to_string(idx) + "\". registry size is \"" + std::to_string(m_variables_bool_count->at(m_callstack_stack_idx->back()))
				+ "\"",
			logger_fatality::ABORT);
		return gBool;
	}

	u64 v_idx	= m_active_idx[m_callstack_block_idx->back()][idx];
	bool& value = m_active_bool[v_idx];
	return value;
}

bool& cap::mod::block::pass::get_bool_release(u64 idx) { return m_active_bool[m_active_idx[m_callstack_block_idx->back()][idx]]; }

std::string& cap::mod::block::pass::get_string_debug(u64 idx)
{
	if (idx >= m_variables_string_count->at(m_callstack_stack_idx->back()))
	{
		log_error(
			"attempted to get string out of range \"" + std::to_string(idx) + "\". registry size is \""
				+ std::to_string(m_variables_string_count->at(m_callstack_stack_idx->back())) + "\"",
			logger_fatality::ABORT);
		return gString;
	}

	u64 v_idx		   = m_active_idx[m_callstack_block_idx->back()][idx];
	std::string& value = m_active_string[v_idx];
	return value;
}

std::string& cap::mod::block::pass::get_string_release(u64 idx) { return m_active_string[m_active_idx[m_callstack_block_idx->back()][idx]]; }

void* cap::mod::block::pass::get_pre_data_debug(u64 idx)
{
	if (idx >= m_pre_data_count[m_callstack_stack_idx->back()][m_callstack_block_idx->back()])
	{
		log_error(
			"attempted to get string out of range \"" + std::to_string(idx) + "\". registry size is \""
				+ std::to_string(m_variables_string_count->at(m_callstack_stack_idx->back())) + "\"",
			logger_fatality::ABORT);
		return nullptr;
	}

	void* value = m_active_pre[m_callstack_block_idx->back()][idx];
	return value;
}

void* cap::mod::block::pass::get_pre_data_release(u64 idx) { return m_active_pre[m_callstack_block_idx->back()][idx]; }

cap::mod::block::pass::initializer::initializer()
{
	debug_mode = true;

	data				   = nullptr;
	variables_real_count   = nullptr;
	variables_bool_count   = nullptr;
	variables_string_count = nullptr;

	custom_register_mutex = nullptr;
	custom_register		  = nullptr;
	stop				  = nullptr;

	// debug only
	data_size		  = 0;
	variable_registry = nullptr;
	begin_time		  = nullptr;
}
