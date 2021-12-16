#pragma once
#include "config.h"

#include "cappuccino/utils.hpp"
#include "data.hpp"

namespace cap::mod::block
{
/// When used as an argument of the execution functions for blocks, the variable must not be moved as it will be tracked
/// by the full build classes. In this context, it must be a pointer and must be called using the "->"
/// operator. This is because the full build classes is looking for this to process it correctly.
class pass
{
public:
	///
	class initializer
	{
	public:
		initializer();

		///
		bool debug_mode;

		/// User data.
		void** data;

		///
		const std::vector<u64>* variables_real_count;

		///
		const std::vector<u64>* variables_bool_count;

		///
		const std::vector<u64>* variables_string_count;

		///
		std::mutex* custom_register_mutex;

		///
		std::vector<void*>* custom_register;

		///
		void(*stop)();

		///
		std::chrono::steady_clock::time_point* begin_time;

		/// Debug Only.
		u64 data_size;

		/// Debug Only.
		std::vector<std::string>* variable_registry;
	};

	///
	enum class logger_fatality
	{
		/// Proceeds as normal.
		OK,

		/// Breaks in the editor; the user can resume.
		BREAK,

		/// Terminates the runtime environment.
		ABORT
	};

	// ====================================================
	// engine calls
	// ====================================================

	///
	pass(const initializer& init);

	///
	const std::vector<std::string>& pull_messages_info();

	///
	const std::vector<std::string>& pull_messages_warn();

	///
	const std::vector<std::string>& pull_messages_error();

	///
	void lock_messages();

	///
	void unlock_messages();

	///
	void set_data(cap::mod::block::data** data);

	///
	void set_callstack_stack(std::vector<u64>* callstack);

	///
	void set_callstack_block(std::vector<u64>* callstack);

	///
	void set_successful(bool* successful);

	///
	void set_finished(std::atomic<bool>* finished);

	///
	void set_callstack_local(void(***local_callstack)(pass*));

	///
	void set_calls(void(***calls)(pass*));

	///
	void perform_deallocation_callbacks();


	//void PerformLocationUpdate();

	// ====================================================
	// user calls
	// ====================================================

	///
	void log_info(const std::string& message);

	///
	void log_warning(const std::string& message);

	///
	void log_error(const std::string& message, logger_fatality fatality);

	///
	f64& get_real(u64 idx);

	///
	bool& get_bool(u64 idx);

	///
	std::string& get_string(u64&& idx);

	///
	void* get_pre_data(u64 idx);

	///
	std::chrono::steady_clock::time_point* get_begin_time();

	///
	void stop();

	///
	void add_callstack(u64 stack, u64 block, bool special = true);

	///
	void pop_callstack();

	///
	void set_stack_idx(u64 idx);

	///
	void set_block_idx(u64 idx);

	///
	std::mt19937_64& get_random_generator();

	// deallocation callbacks MUST be on the heap.
	// do not deallocate the callback yourself, Cappuccino will do that.

	///
	void add_deallocation(std::function<void(pass*)>* dealloc);

	///
	void remove_deallocation(std::function<void(pass*)>* dealloc);

	// ====================================================
	// custom allocations
	// ====================================================

	// do NOT use "malloc" or any other "C" style allocation; you MUST use "new" to allocate!
	// do NOT allocate an array; you MUST allocate a single object.
	// minimize the number of custom allocations you do.
	// calling "CustomFree" deletes the object, but its index persists!

	// after "CustomPut", it must be either persist forever, or you must
	//		call "CustomFree", to deallocate it. If The object is freed externally before
	//		Cappuccino ends, then you must call "CustomFree" with the parameter "deallocate" set to false.
	//		This will ensure it does not get freed at the end.

	///
	u64 custom_put(void* mem);

	///
	void* custom_get(u64 idx);

	///
	void custom_free(u64 idx, bool deallocate = true);
private:
	///
	void update_locations();
	// user data interactions
	///
	f64& (pass::* m_get_real)(u64 idx);

	///
	f64& get_real_debug(u64 idx);

	///
	f64& get_real_release(u64 idx);

	///
	bool& (pass::* m_get_bool)(u64 idx);

	///
	bool& get_bool_debug(u64 idx);

	///
	bool& get_bool_release(u64 idx);

	///
	std::string& (pass::* m_get_string)(u64 idx);

	///
	std::string& get_string_debug(u64 idx);

	///
	std::string& get_string_release(u64 idx);

	///
	void* (pass::* m_get_pre_data)(u64 idx);

	///
	void* get_pre_data_debug(u64 idx);

	///
	void* get_pre_data_release(u64 idx);

	///
	std::mutex m_messages_mutex;

	///
	std::vector<std::string> m_messages_info;

	///
	std::vector<std::string> m_messages_warn;

	///
	std::vector<std::string> m_messages_error;

	///
	void(*m_stop)();

	///
	std::chrono::steady_clock::time_point* m_begin_time;

	///
	std::mt19937_64 m_random;
	// debug only
	///
	std::vector<std::string>* m_variable_registry;
	// user data
	///
	cap::mod::block::data** m_data;

	///
	std::vector<const u64**> m_data_stack_idx;

	///
	std::vector<void***> m_data_stack_pre;

	///
	std::vector<f64*> m_data_stack_real;

	///
	std::vector<bool*> m_data_stack_bool;

	///
	std::vector<std::string*> m_data_stack_string;

	///
	const u64** m_active_idx;

	///
	void*** m_active_pre;

	///
	f64* m_active_real;

	///
	bool* m_active_bool;

	///
	std::string* m_active_string;

	///
	std::vector<bool> m_stacking_special;

	///
	std::vector<f64*> m_stacking_real;

	///
	std::vector<bool*> m_stacking_bool;

	///
	std::vector<std::string*> m_stacking_string;

	///
	std::vector<u64*> m_pre_data_count;

	///
	const std::vector<u64>* m_variables_real_count;

	///
	const std::vector<u64>* m_variables_bool_count;

	///
	const std::vector<u64>* m_variables_string_count;

	///
	std::mutex* m_custom_registrer_mutex;

	///
	std::vector<void*>* m_custom_register;

	///
	std::vector<u64>* m_callstack_stack_idx;

	///
	std::vector<u64>* m_callstack_block_idx;

	///
	void(***m_local_callstack)(pass*);

	///
	void(***m_calls)(pass*);

	///
	std::vector<std::function<void(pass*)>*> m_deallocation_calls;

	///
	bool* m_successful;

	///
	std::atomic<bool>* m_finished;
};
}