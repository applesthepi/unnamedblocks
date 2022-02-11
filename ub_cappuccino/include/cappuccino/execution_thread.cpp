#include "execution_thread.hpp"

#include "cappuccino/registration.hpp"

static void thread_execution(cap::execution_thread* thr)
{
	const cap::execution_thread::function_stack_list calls = thr->get_calls();
	const u64* functionCallCount						   = thr->get_function_call_count();
	cap::execution_thread::function_stack local_call_stack;

	std::vector<u64> callstack_block_idx;
	std::vector<u64> callstack_stack_idx;

	bool successful					  = false;
	const std::atomic<bool>& finished = thr->get_finished();

	cap::mod::block::data** reg_data = cap::registration::get_data();
	cap::mod::block::pass* pass		 = thr->get_pass();

	pass->set_callstack_stack(&callstack_stack_idx);
	pass->set_callstack_block(&callstack_block_idx);
	pass->set_data(reg_data);
	pass->set_successful(&successful);
	pass->set_finished((std::atomic<bool>*)&finished);
	pass->set_callstack_local(&local_call_stack);
	pass->set_calls(calls);

	pass->add_callstack(thr->get_function_start(), 0, false);

	while (!finished)
	{
	loop:
		if (callstack_block_idx.back() >= functionCallCount[callstack_stack_idx.back()])
		{
			pass->pop_callstack();

			if (finished)
				break;

			callstack_block_idx.back()++;
			local_call_stack = calls[callstack_stack_idx.back()];

			continue;
		}

		local_call_stack[callstack_block_idx.back()](pass);
		callstack_block_idx.back()++;
	}

	if (thr->get_breaked())
	{
		thr->set_finished(false);

		while (!*thr->get_resume())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));

			if (thr->get_kill())
			{
				printf("killed thread during break\n");
				return;
			}

			if (thr->get_step())
			{
				thr->set_step(false);
				thr->set_finished(true);
				break;
			}
		}

		if (*thr->get_resume())
			thr->set_break(false);

		goto loop;
	}

	pass->perform_deallocation_callbacks();

	if (successful)
	{
		cap::registration::unregister_pass(pass);
		cap::registration::unregister_execution_thread(thr, successful);
	}
}

cap::execution_thread::execution_thread(
	u64 function_start,
	u64* function_call_count,
	cap::execution_thread::function_stack_list calls,
	cap::mod::block::pass* pass)
	: m_function_start(function_start)
	, m_function_call_count(function_call_count)
	, m_calls(calls)
	, m_pass(pass)
{
	m_finished = false;
	m_kill	   = false;
	m_ended	   = false;
	m_breaked  = false;
	m_resume   = nullptr;
	m_step	   = false;

	m_thread = std::thread(thread_execution, this);
}

u64 cap::execution_thread::get_function_start()
{
	return m_function_start;
}

const u64* cap::execution_thread::get_function_call_count()
{
	return m_function_call_count;
}

const cap::execution_thread::function_stack_list& cap::execution_thread::get_calls()
{
	return m_calls;
}

const std::atomic<bool>& cap::execution_thread::get_finished()
{
	return m_finished;
}

const std::atomic<bool>& cap::execution_thread::get_kill()
{
	return m_kill;
}

const std::atomic<bool>& cap::execution_thread::get_breaked()
{
	return m_breaked;
}

const std::atomic<bool>* cap::execution_thread::get_resume()
{
	return m_resume;
}

const std::atomic<bool>& cap::execution_thread::get_step()
{
	return m_step;
}

cap::mod::block::pass* cap::execution_thread::get_pass()
{
	return m_pass;
}

void cap::execution_thread::set_finished(bool finished)
{
	m_finished = finished;
}

void cap::execution_thread::set_step(bool step)
{
	m_step = step;
}

void cap::execution_thread::set_break(bool breaked)
{
	m_breaked = breaked;
}

void cap::execution_thread::end()
{
	if (m_ended)
		return;
	else
		m_ended = true;

	m_kill	   = true;
	m_finished = true;

	if (m_thread.joinable())
		m_thread.join();
	else
		printf("tried to join unjoinable thread\n");
}

void cap::execution_thread::break_thread(std::atomic<bool>* resume)
{
	m_resume   = resume;
	m_breaked  = true;
	m_finished = true;
}

void cap::execution_thread::step()
{
	m_step = true;

	while (m_step)
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
}