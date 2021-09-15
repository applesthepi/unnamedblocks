#pragma once
#include "config.h"

#include "cappuccino/utils.hpp"
#include "cappuccino/mod/block/block.hpp"
#include "cappuccino/mod/block/pass.hpp"
#include "cappuccino/execution_thread.hpp"
#include "cappuccino/mod/block/data.hpp"

namespace cap
{
///
class registration
{
public:
	///
	static void initialize();

	///
	static void register_pass(cap::mod::block::pass* pass);

	///
	static void unregister_pass(cap::mod::block::pass* pass);

	///
	static void register_execution_thread(cap::execution_thread* thr);

	///
	static void unregister_execution_thread(cap::execution_thread* thr, bool join);

	///
	static void set_function_main(u64 main);

	///
	static void set_function_call_count(u64* function_call_count);

	///
	static void set_function_total_count(u64 function_total_count);

	///
	static void set_calls(cap::execution_thread::function_stack_list calls);

	///
	static void set_data(cap::mod::block::data** data);

	///
	static void set_blocks(cap::mod::block::block*** blocks);

	///
	static void set_debug(bool debug_build);

	///
	static void set_super(u8* super, i64* super_data, void* super_mutex);

	///
	static void end_all(cap::mod::block::pass* whitelist = nullptr);

	///
	static void stop();

	///
	static std::atomic<bool>& get_util_finished();

	///
	static std::atomic<bool>& get_stop();

	///
	static u64* get_function_call_count();

	///
	static u64 get_function_total_count();

	///
	static cap::execution_thread::function_stack_list get_calls();

	///
	static void set_util_return_finished(bool finished);

	///
	static void run_utility_tick();

	///
	static void run();

	///
	static bool is_all_done();

	///
	static const std::vector<f64*>& get_real_template();

	///
	static const std::vector<bool*>& get_bool_template();

	///
	static const std::vector<std::string*>& get_string_template();

	///
	static const std::vector<u64>* get_real_count();

	///
	static const std::vector<u64>* get_bool_count();

	///
	static const std::vector<u64>* get_string_count();

	///
	static std::mutex* get_custom_mutex();

	///
	static std::vector<void*>* get_custom_registry();

	///
	static cap::mod::block::data** get_data();
private:
	///
	static bool global_pre(cap::preprocessor_data& data);

	///
	static bool global_post(cap::preprocessor_data& data);

	///
	static bool local_pre(cap::preprocessor_data& data);

	///
	static bool local_post(cap::preprocessor_data& data);

	///
	static bool init(cap::preprocessor_data& pre_data, cap::mod::block::data** block_data);

	/*
		1 - [ R/D ] stop; kill all

		2 - [ R/D ] break all
		3 - [ R/D ] resume all

		4 - [ __D ] break single thread (idx)
		5 - [ __D ] step single thread
		6 - [ __D ] resume single thread
	*/

	///
	static bool test_super_base();

	///
	static bool test_super_debug();

	///
	static void compile_data_debug();

	///
	static void compile_data_release();

	///
	static void run_context();

	///
	static std::mutex m_passes_mutex;

	///
	static std::vector<cap::mod::block::pass*> m_passes;

	///
	static std::vector<bool> m_passes_flagged;

	///
	static std::mutex m_execution_mutex;

	///
	static std::vector<cap::execution_thread*> m_execution;

	///
	static std::vector<bool> m_execution_flagged;

	///
	static std::vector<bool> m_execution_join;

	///
	static u64 m_function_main;

	///
	static u64* m_function_call_count;

	///
	static u64 m_function_total_count;

	///
	static cap::execution_thread::function_stack_list m_calls;

	///
	static cap::mod::block::data** m_data;

	///
	static cap::mod::block::block*** m_blocks;

	///
	static std::vector<u64> m_variable_real_count;

	///
	static std::vector<u64> m_variable_bool_count;

	///
	static std::vector<u64> m_variable_string_count;

	///
	static std::vector<f64*> m_variable_real_template;

	///
	static std::vector<bool*> m_variable_bool_template;

	///
	static std::vector<std::string*> m_variable_string_template;

	///
	static std::mutex m_custom_register_mutex;

	///
	static std::vector<void*> m_custom_register;

	///
	static std::atomic<bool> m_util_finished;

	///
	static std::atomic<bool> m_util_return_finished;

	///
	static std::atomic<bool> m_all_done;

	///
	static std::atomic<bool> m_stop;

	///
	static std::thread m_util_thread;

	///
	static bool m_debug_build;

	///
	static u8* m_super;

	///
	static i64* m_super_data;

	///
	static std::mutex* m_super_mutex;

	///
	static std::chrono::steady_clock::time_point m_time_begin;

	///
	static bool m_break_full;

	///
	static bool m_break_single;

	///
	static i64 m_break_single_data;

	///
	static std::atomic<bool> m_break_full_resume;

	///
	static std::atomic<bool> m_break_single_resume;

	///
	static std::vector<std::vector<std::string>> m_variable_registry;

	///
	static std::vector<u64> m_variable_registry_offsets;
};
}