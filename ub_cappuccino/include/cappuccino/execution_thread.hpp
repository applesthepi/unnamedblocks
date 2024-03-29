#pragma once
#include "config.h"

#include <espresso/mod/block/pass.hpp>

#include <latte/utils.hpp>

namespace cap
{
///
class execution_thread
{
public:
	///
	typedef void (***function_stack_list)(espresso::mod::block::pass*);

	///
	typedef void (*function)(espresso::mod::block::pass*);

	///
	typedef void (**function_stack)(espresso::mod::block::pass*);

	///
	execution_thread(
		u64 function_start, u64* function_call_count, function_stack_list calls, espresso::mod::block::pass* pass);

	///
	u64 get_function_start();

	///
	const u64* get_function_call_count();

	///
	const function_stack_list& get_calls();

	///
	const std::atomic<bool>& get_finished();

	///
	const std::atomic<bool>& get_kill();

	///
	const std::atomic<bool>& get_breaked();

	///
	const std::atomic<bool>* get_resume();

	///
	const std::atomic<bool>& get_step();

	///
	espresso::mod::block::pass* get_pass();

	///
	void set_finished(bool finished);

	///
	void set_step(bool step);

	///
	void set_break(bool breaked);

	///
	void end();

	///
	void break_thread(std::atomic<bool>* resume);

	///
	void step();

private:
	///
	std::atomic<bool> m_finished;

	///
	std::atomic<bool> m_kill;

	///
	std::atomic<bool> m_ended;

	///
	std::atomic<bool> m_breaked;

	///
	std::atomic<bool>* m_resume;

	///
	std::atomic<bool> m_step;

	//
	const u64 m_function_start;

	///
	const u64* m_function_call_count;

	///
	function_stack_list m_calls;

	///
	std::thread m_thread;

	///
	espresso::mod::block::pass* m_pass;
};
} // namespace cap