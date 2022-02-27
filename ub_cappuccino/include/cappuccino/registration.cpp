#include "registration.hpp"

#include "cappuccino/preprocessor_data.hpp"

#include <iostream>

#if LINUX

#else
#include <windows.h>
#endif

static void thread_util_release()
{
	while (!cap::registration::get_util_finished())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		cap::registration::run_utility_tick();
	}

	// for sync
	cap::registration::run_utility_tick();
	cap::registration::set_util_return_finished(true);
}

static void thread_util_debug()
{
	while (!cap::registration::get_util_finished())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		cap::registration::run_utility_tick();
	}

	// for sync
	cap::registration::run_utility_tick();
	cap::registration::set_util_return_finished(true);
}

void cap::registration::initialize()
{
	m_util_finished		   = false;
	m_util_return_finished = false;
	m_all_done			   = false;
	m_stop				   = false;
	m_debug_build		   = true;
	m_super				   = nullptr;

	m_break_full		= false;
	m_break_single		= false;
	m_break_single_data = 0;

	m_break_full_resume	  = false;
	m_break_single_resume = false;

	m_passes.clear();
	m_passes_flagged.clear();
	m_execution.clear();
	m_execution_flagged.clear();
	m_execution_join.clear();

	m_custom_register.clear();
	m_variable_registry.clear();

	m_variable_real_count.clear();
	m_variable_bool_count.clear();
	m_variable_string_count.clear();

	m_variable_real_template.clear();
	m_variable_bool_template.clear();
	m_variable_string_template.clear();

	m_time_begin = std::chrono::steady_clock::now();
}

void cap::registration::register_pass(espresso::mod::block::pass* pass)
{
	std::unique_lock<std::mutex> lock(m_passes_mutex);

	m_passes.push_back(pass);
	m_passes_flagged.push_back(false);
}

void cap::registration::unregister_pass(espresso::mod::block::pass* pass)
{
	std::unique_lock<std::mutex> lock(m_passes_mutex);

	for (u64 i = 0; i < m_passes.size(); i++)
	{
		if (m_passes[i] == pass)
		{
			m_passes_flagged[i] = true;
			return;
		}
	}
}

void cap::registration::register_execution_thread(cap::execution_thread* thr)
{
	std::unique_lock<std::mutex> lock(m_execution_mutex);

	m_execution.push_back(thr);
	m_execution_flagged.push_back(false);
	m_execution_join.push_back(false);
}

void cap::registration::unregister_execution_thread(cap::execution_thread* thr, bool join)
{
	std::unique_lock<std::mutex> lock(m_execution_mutex);

	for (u64 i = 0; i < m_execution.size(); i++)
	{
		if (m_execution[i] == thr)
		{
			m_execution_flagged[i] = true;
			m_execution_join[i]	   = join;
			return;
		}
	}
}

void cap::registration::set_function_main(u64 main)
{
	m_function_main = main;
}

void cap::registration::set_function_call_count(u64* function_call_count)
{
	m_function_call_count = function_call_count;
}

void cap::registration::set_function_total_count(u64 function_total_count)
{
	m_function_total_count = function_total_count;
}

void cap::registration::set_calls(cap::execution_thread::function_stack_list calls)
{
	m_calls = calls;
}

void cap::registration::set_data(espresso::mod::block::data** data)
{
	m_data = data;
}

void cap::registration::set_blocks(espresso::mod::block::block*** blocks)
{
	m_blocks = blocks;
}

void cap::registration::set_debug(bool debug_build)
{
	m_debug_build = debug_build;
}

void cap::registration::set_super(u8* super, i64* super_data, void* super_mutex)
{
	m_super		  = super;
	m_super_data  = super_data;
	m_super_mutex = (std::mutex*)super_mutex;
}

void cap::registration::end_all(espresso::mod::block::pass* whitelist)
{
	// dont lock execution mutex because it is already locked

	if (whitelist == nullptr)
	{
		for (u64 i = 0; i < m_execution.size(); i++)
		{
			m_execution[i]->end();
			m_execution_flagged[i] = true;
		}

		for (u64 i = 0; i < m_passes.size(); i++)
			m_passes_flagged[i] = true;
	}
	else
	{
		for (u64 i = 0; i < m_execution.size(); i++)
		{
			if (m_passes[i] != whitelist)
			{
				m_execution[i]->end();
				m_execution_flagged[i] = true;
			}
		}

		for (u64 i = 0; i < m_passes.size(); i++)
			m_passes_flagged[i] = true;
	}
}

void cap::registration::stop()
{
	m_stop = true;
}

std::atomic<bool>& cap::registration::get_util_finished()
{
	return m_util_finished;
}

std::atomic<bool>& cap::registration::get_stop()
{
	return m_stop;
}

u64* cap::registration::get_function_call_count()
{
	return m_function_call_count;
}

u64 cap::registration::get_function_total_count()
{
	return m_function_total_count;
}

cap::execution_thread::function_stack_list cap::registration::get_calls()
{
	return m_calls;
}

void cap::registration::set_util_return_finished(bool finished)
{
	m_util_return_finished = finished;
}

void cap::registration::run_utility_tick()
{
	std::unique_lock<std::mutex> lock1(m_passes_mutex);
	std::unique_lock<std::mutex> lock2(m_execution_mutex);

	static latte::logger::buffer* logging_buffer = new latte::logger::buffer(false);
	logging_buffer->reserve(256 * 3 + 10);

	for (u64 i = 0; i < m_passes.size(); i++)
	{
		m_passes[i]->lock_messages();

		// Info.

		{
			const std::vector<std::string>& messages = m_passes[i]->pull_messages_info();

			u64 amount = messages.size();
			if (amount > 256)
			{
				amount = 256;
				logging_buffer->warn(latte::logger::level::RUNTIME, "restricting to 256 messages per tick...");
			}

			for (u64 a = 0; a < amount; a++)
				logging_buffer->info(latte::logger::level::RUNTIME, messages[a]);
		}

		// Warn.

		{
			const std::vector<std::string>& messages = m_passes[i]->pull_messages_warn();

			u64 amount = messages.size();
			if (amount > 256)
			{
				amount = 256;
				logging_buffer->warn(latte::logger::level::RUNTIME, "restricting to 256 messages per tick...");
			}

			for (u64 a = 0; a < amount; a++)
				logging_buffer->warn(latte::logger::level::RUNTIME, messages[a]);
		}

		// Error.

		{
			const std::vector<std::string>& messages = m_passes[i]->pull_messages_error();

			u64 amount = messages.size();
			if (amount > 256)
			{
				amount = 256;
				logging_buffer->warn(latte::logger::level::RUNTIME, "restricting to 256 messages per tick...");
			}

			for (u64 a = 0; a < amount; a++)
				logging_buffer->error(latte::logger::level::RUNTIME, messages[a]);
		}

		m_passes[i]->unlock_messages();
	}

	logging_buffer->flush();

	// cleanup passes

	for (u64 i = 0; i < m_passes.size(); i++)
	{
		if (m_passes_flagged[i])
		{
			espresso::mod::block::pass* pass = m_passes[i];

			m_passes.erase(m_passes.begin() + i);
			m_passes_flagged.erase(m_passes_flagged.begin() + i);

			delete pass;
		}
	}

	// cleanup threads

	for (u64 i = 0; i < m_execution.size(); i++)
	{
		if (m_execution_flagged[i])
		{
			cap::execution_thread* exe = m_execution[i];

			if (m_execution_join[i])
				exe->end();

			m_execution.erase(m_execution.begin() + i);
			m_execution_flagged.erase(m_execution_flagged.begin() + i);
			m_execution_join.erase(m_execution_join.begin() + i);

			delete exe;
		}
	}

	if (cap::registration::get_stop())
		cap::registration::end_all();

	m_all_done = is_all_done();

	if (m_all_done)
		m_util_finished = true;
}

void cap::registration::run()
{
#if LINUX

#else

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	if (!CreateProcess( NULL,   // No module name (use command line)
		"cargo.exe build --release",
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		"toolchain\\ubbs\\",           // Use parent's starting directory
		&si,            // Pointer to STARTUPINFO structure
		&pi )           // Pointer to PROCESS_INFORMATION structure
		)
	{
		std::ostringstream stream;
		stream << GetLastError();
		latte::logger::error(latte::logger::level::EDITOR, std::string("failed to create a windows process: ") + stream.str());
		return;
	}

	WaitForSingleObject(pi.hProcess, INFINITE);

	DWORD exit_code;
	GetExitCodeProcess(pi.hProcess, &exit_code);

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	if (exit_code != 0)
	{
		latte::logger::error(latte::logger::level::EDITOR, "failed to build project. Submit an issue to the repo with the project \".ub\" file or \".rs\" files in \"toolchain/ubbs/\"");
		return;
	}

	HINSTANCE hGetProcIDDLL = LoadLibrary("toolchain\\ubbs\\target\\release\\ubbs.dll");

	if (!hGetProcIDDLL)
	{
		latte::logger::error(latte::logger::level::EDITOR, "failed to load ubbs");
		return;
	}

	typedef void (*f_ub_run)();
	f_ub_run ub_run = (f_ub_run)GetProcAddress(hGetProcIDDLL, "ub_run");

	if (!ub_run)
	{
		latte::logger::error(latte::logger::level::EDITOR, "failed to load ubbs \"ub_run\" function");
		return;
	}

	printf("running ub_run");
	ub_run();
	printf("done ub_run");

#endif

	printf("#########[ Started Cappuccino\n");

	if (m_debug_build)
		compile_data_debug();
	else
		compile_data_release();

	printf("...compilation succeeded\n");

	m_custom_register.reserve(10000);

	cap::preprocessor_data data(m_custom_register);

	if (!global_pre(data))
		return;

	if (!local_pre(data))
		return;

	printf("...preinitialization succeeded\n");

	if (!init(data, m_data))
		return;

	printf("...initialization succeeded\n");

	if (!local_post(data))
		return;

	if (!global_post(data))
		return;

	printf("...postinitialization succeeded\n");

	espresso::mod::block::pass::initializer init(m_variable_real_template, m_variable_bool_template, m_variable_string_template);

	init.data_size				= 0;
	init.data					= nullptr;
	init.variables_real_count	= &m_variable_real_count;
	init.variables_bool_count	= &m_variable_bool_count;
	init.variables_string_count = &m_variable_string_count;
	init.custom_register_mutex	= &m_custom_register_mutex;
	init.custom_register		= &m_custom_register;
	init.stop					= &cap::registration::stop;
	init.variable_registry		= nullptr; // isnt working right now due to changes &m_variableRegistry;
	init.debug_mode				= m_debug_build;
	init.begin_time				= &m_time_begin;
	init.block_data             = m_data;
	init.function_total_count   = m_function_total_count;
	init.function_call_count    = m_function_call_count;

	espresso::mod::block::pass* pass = new espresso::mod::block::pass(init);
	register_pass(pass);

	cap::execution_thread* thr = new cap::execution_thread(m_function_main, m_function_call_count, m_calls, pass);
	register_execution_thread(thr);

	if (m_debug_build)
		m_util_thread = std::thread(thread_util_debug);
	else
		m_util_thread = std::thread(thread_util_release);

	m_util_thread.detach();

	run_context();

	{
		m_util_finished = true;

		while (!m_util_return_finished)
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	printf("finished execution; deallocating...\n");

	for (u64 i = 0; i < m_custom_register.size(); i++)
	{
		if (m_custom_register[i] != nullptr)
			delete m_custom_register[i];
	}

	for (u64 i = 0; i < m_variable_real_template.size(); i++)
		delete[] m_variable_real_template[i];

	for (u64 i = 0; i < m_variable_bool_template.size(); i++)
		delete[] m_variable_bool_template[i];

	for (u64 i = 0; i < m_variable_string_template.size(); i++)
		delete[] m_variable_string_template[i];

	m_variable_real_template.clear();
	m_variable_bool_template.clear();
	m_variable_string_template.clear();

	m_variable_real_count.clear();
	m_variable_bool_count.clear();
	m_variable_string_count.clear();

	printf("#########[ Stopped Cappuccino\n");

	std::cout << std::flush;
}

bool cap::registration::is_all_done()
{
	// dont lock execution mutex because it is already locked
	return m_execution.size() == 0;
	/*
	for (u64 i = 0; i < m_execution.size(); i++)
	{
		if (!m_execution[i]->GetFinished())
			return false;
	}

	return true;
	*/
}

std::vector<f64*>* cap::registration::get_real_template()
{
	return &m_variable_real_template;
}

std::vector<bool*>* cap::registration::get_bool_template()
{
	return &m_variable_bool_template;
}

std::vector<std::string*>* cap::registration::get_string_template()
{
	return &m_variable_string_template;
}

const std::vector<u64>* cap::registration::get_real_count()
{
	return &m_variable_real_count;
}

const std::vector<u64>* cap::registration::get_bool_count()
{
	return &m_variable_bool_count;
}

const std::vector<u64>* cap::registration::get_string_count()
{
	return &m_variable_string_count;
}

std::mutex* cap::registration::get_custom_mutex()
{
	return &m_custom_register_mutex;
}

std::vector<void*>* cap::registration::get_custom_registry()
{
	return &m_custom_register;
}

espresso::mod::block::data** cap::registration::get_data()
{
	return m_data;
}

bool cap::registration::global_pre(cap::preprocessor_data& data)
{
	std::vector<espresso::mod::block::block*> single_blocks;

	for (u64 i = 0; i < m_function_total_count; i++)
	{
		for (u64 a = 0; a < m_function_call_count[i]; a++)
		{
			bool found = false;

			for (u64 b = 0; b < single_blocks.size(); b++)
			{
				if (single_blocks[b]->get_unlocalized_name() == m_blocks[i][a]->get_unlocalized_name())
				{
					found = true;
					break;
				}
			}

			if (!found)
				single_blocks.push_back(m_blocks[i][a]);
		}
	}

	for (u64 i = 0; i < single_blocks.size(); i++)
	{
		if (!single_blocks[i]->get_runtime_global_pre_init()(data))
		{
			latte::logger::error(latte::logger::level::EDITOR, "failed on globalPreInit");
			return false;
		}
	}

	return true;
}

bool cap::registration::global_post(cap::preprocessor_data& data)
{
	std::vector<espresso::mod::block::block*> single_blocks;

	for (u64 i = 0; i < m_function_total_count; i++)
	{
		for (u64 a = 0; a < m_function_call_count[i]; a++)
		{
			bool found = false;

			for (u64 b = 0; b < single_blocks.size(); b++)
			{
				if (single_blocks[b]->get_unlocalized_name() == m_blocks[i][a]->get_unlocalized_name())
				{
					found = true;
					break;
				}
			}

			if (!found)
				single_blocks.push_back(m_blocks[i][a]);
		}
	}

	for (u64 i = 0; i < single_blocks.size(); i++)
	{
		if (!single_blocks[i]->get_runtime_global_post_init()(data))
		{
			latte::logger::error(latte::logger::level::EDITOR, "failed on globalPostInit");
			return false;
		}
	}

	return true;
}

bool cap::registration::local_pre(cap::preprocessor_data& data)
{
	for (u64 i = 0; i < m_function_total_count; i++)
	{
		std::vector<espresso::mod::block::block*> single_blocks;

		for (u64 a = 0; a < m_function_call_count[i]; a++)
		{
			bool found = false;

			for (u64 b = 0; b < single_blocks.size(); b++)
			{
				if (single_blocks[b]->get_unlocalized_name() == m_blocks[i][a]->get_unlocalized_name())
				{
					found = true;
					break;
				}
			}

			if (!found)
				single_blocks.push_back(m_blocks[i][a]);
		}

		data.stack_idx = i;

		for (u64 a = 0; a < single_blocks.size(); a++)
		{
			if (!single_blocks[a]->get_runtime_local_pre_init()(data))
			{
				latte::logger::error(latte::logger::level::EDITOR, "failed on localPreInit");
				return false;
			}
		}
	}

	return true;
}

bool cap::registration::local_post(cap::preprocessor_data& data)
{
	for (u64 i = 0; i < m_function_total_count; i++)
	{
		std::vector<espresso::mod::block::block*> single_blocks;

		for (u64 a = 0; a < m_function_call_count[i]; a++)
		{
			bool found = false;

			for (u64 b = 0; b < single_blocks.size(); b++)
			{
				if (single_blocks[b]->get_unlocalized_name() == m_blocks[i][a]->get_unlocalized_name())
				{
					found = true;
					break;
				}
			}

			if (!found)
				single_blocks.push_back(m_blocks[i][a]);
		}

		data.stack_idx = i;

		for (u64 a = 0; a < single_blocks.size(); a++)
		{
			if (!single_blocks[a]->get_runtime_local_post_init()(data))
			{
				latte::logger::error(latte::logger::level::EDITOR, "failed on localPostInit");
				return false;
			}
		}
	}

	return true;
}

bool cap::registration::init(cap::preprocessor_data& pre_data, espresso::mod::block::data** block_data)
{
	std::vector<std::vector<espresso::mod::block::block::data_initialization>> stages;
	std::vector<std::vector<u64>> stage_stack_idx;
	std::vector<std::vector<u64>> stage_block_idx;

	for (u64 i = 0; i < m_function_total_count; i++)
	{
		for (u64 a = 0; a < m_function_call_count[i]; a++)
		{
			std::vector<std::pair<espresso::mod::block::block::data_initialization, u16>> init_stages =
				m_blocks[i][a]->get_runtime_stages();

			for (u64 b = 0; b < init_stages.size(); b++)
			{
				if (init_stages[b].second >= stages.size())
				{
					u64 stages_size = stages.size();

					for (u64 c = 0; c < (init_stages[b].second - stages_size) + 1; c++)
					{
						stages.push_back(std::vector<espresso::mod::block::block::data_initialization>());
						stage_stack_idx.push_back(std::vector<u64>());
						stage_block_idx.push_back(std::vector<u64>());
					}
				}

				stages[init_stages[b].second].push_back(init_stages[b].first);
				stage_stack_idx[init_stages[b].second].push_back(i);
				stage_block_idx[init_stages[b].second].push_back(a);
			}
		}
	}

	for (u64 i = 0; i < stages.size(); i++)
	{
		for (u64 a = 0; a < stages[i].size(); a++)
		{
			pre_data.stack_idx = stage_stack_idx[i][a];
			pre_data.block_idx = stage_block_idx[i][a];

			if (!stages[i][a](pre_data, block_data[stage_stack_idx[i][a]][stage_block_idx[i][a]]))
			{
				latte::logger::error(
					latte::logger::level::EDITOR, "failed on initialization of stage \"" + std::to_string(i) + "\"");
				return false;
			}
		}
	}

	return true;
}

bool cap::registration::test_super_base()
{
	return false;
	std::unique_lock<std::mutex> lock(*m_super_mutex);

	if (*m_super == 1)
	{
		*m_super = 0;
		printf("stopping execution...\n");

		std::unique_lock<std::mutex> lock1(m_passes_mutex);
		std::unique_lock<std::mutex> lock2(m_execution_mutex);

		end_all();
		return true;
	}
	else if (*m_super == 2)
	{
		*m_super = 0;

		if (m_break_single)
		{
			printf("can not break full while in break single mode\n");
			return true;
		}

		printf("breaking all execution...\n");

		m_break_full		= true;
		m_break_full_resume = false;

		std::unique_lock<std::mutex> lock(m_execution_mutex);

		for (u64 i = 0; i < m_execution.size(); i++)
			m_execution[i]->break_thread(&m_break_full_resume);

		return true;
	}
	else if (*m_super == 3)
	{
		*m_super = 0;

		if (!m_break_full)
		{
			printf("can not resume all if not in not in break all mode\n");
			return true;
		}

		printf("resuming all execution...\n");

		m_break_full		= false;
		m_break_full_resume = true;

		return true;
	}

	return false;
}

bool cap::registration::test_super_debug()
{
	return false;
	if (*m_super == 4)
	{
		*m_super = 0;

		if (m_break_full)
		{
			printf("can not break single while in break all mode\n");
			return true;
		}

		printf("breaking single execution...\n");

		m_break_single		  = true;
		m_break_single_resume = false;

		std::unique_lock<std::mutex> lock(m_execution_mutex);
		m_execution[reinterpret_cast<u64>(m_super_data)]->break_thread(&m_break_single_resume);

		return true;
	}
	else if (*m_super == 6)
	{
		*m_super = 0;

		if (!m_break_single)
		{
			printf("can not resume single if not in not in break single mode\n");
			return true;
		}

		printf("resuming single execution...\n");

		m_break_single		  = false;
		m_break_single_resume = true;

		return true;
	}
	else if (*m_super == 5)
	{
		*m_super = 0;

		if (!m_break_single)
		{
			printf("can not step single if not in not in break single mode\n");
			return true;
		}

		printf("stepping single execution...\n");

		std::unique_lock<std::mutex> lock(m_execution_mutex);
		m_execution[reinterpret_cast<u64>(m_super_data)]->step();

		return true;
	}

	return false;
}

void cap::registration::compile_data_debug()
{
	u64 temp_total	 = 0;
	u64 variable_idx = 0;
	std::vector<std::vector<espresso::mod::block::data::interpretation>> temp_regsitry_types;
	std::vector<std::vector<void*>> temp_regsitry_values;

	auto add_to_registry =
		[&](const std::string& name, u64 idx, const espresso::mod::block::data::interpretation& interp, void* use = nullptr)
	{
		for (u64 i = 0; i < m_variable_registry[idx].size(); i++)
		{
			if (m_variable_registry[idx][i] == name)
			{
				variable_idx = i;
				return false;
			}
		}

		variable_idx = m_variable_registry[idx].size();
		temp_total++;
		m_variable_registry[idx].push_back(name);
		temp_regsitry_types[idx].push_back(interp);
		temp_regsitry_values[idx].push_back(use);

		return true;
	};

	std::vector<std::vector<std::vector<u64>*>*> hauled_variables_plane;

	for (u64 i = 0; i < m_function_total_count; i++)
	{
		m_variable_registry.push_back(std::vector<std::string>());
		temp_regsitry_types.push_back(std::vector<espresso::mod::block::data::interpretation>());
		temp_regsitry_values.push_back(std::vector<void*>());
		m_variable_registry_offsets.push_back(temp_total);

		std::vector<std::vector<u64>*>* hauled_variables_stack = new std::vector<std::vector<u64>*>();

		for (u64 a = 0; a < m_function_call_count[i]; a++)
		{
			const std::vector<void*>& data						  = m_data[i][a].get_data();
			const std::vector<espresso::mod::block::data::type>& types = m_data[i][a].get_types();
			const std::vector<espresso::mod::block::data::interpretation>& interpretations =
				m_data[i][a].get_interpretations();

			std::vector<u64>* hauled_variables_block = new std::vector<u64>();

			for (u64 b = 0; b < data.size(); b++)
			{
				const u64 count_total = temp_total - m_variable_registry[i].size();

				if (types[b] == espresso::mod::block::data::type::VAR)
				{
					if (interpretations[b] == espresso::mod::block::data::interpretation::ANY)
					{
						bool found = false;

						for (u64 c = 0; c < m_variable_registry[i].size(); c++)
						{
							if (m_variable_registry[i][c] == "_L_" + *(std::string*)data[b])
							{
								hauled_variables_block->push_back(count_total);
								m_data[i][a].set_interpretation(temp_regsitry_types[i][c], b);

								found = true;
								break;
							}
						}

						if (!found)
							latte::logger::warn(
								latte::logger::level::EDITOR,
								"*any* variable \"" + *(std::string*)data[b] + "\" was not found");
					}
					else
					{
						add_to_registry("_L_" + *(std::string*)data[b], i, interpretations[b]);
						hauled_variables_block->push_back(variable_idx);
					}
				}
				else
				{
					char buffer[20];
					sprintf(buffer, "_R_%lu_%lu_%lu", i, a, b);

					if (interpretations[b] == espresso::mod::block::data::interpretation::REAL)
						add_to_registry(
							std::string(buffer),
							i,
							espresso::mod::block::data::interpretation::REAL,
							new f64(*(f64*)data[b]));
					else if (interpretations[b] == espresso::mod::block::data::interpretation::BOOL)
						add_to_registry(
							std::string(buffer),
							i,
							espresso::mod::block::data::interpretation::BOOL,
							new bool(*(bool*)data[b]));
					else if (interpretations[b] == espresso::mod::block::data::interpretation::STRING)
						add_to_registry(
							std::string(buffer),
							i,
							espresso::mod::block::data::interpretation::STRING,
							new std::string(*(std::string*)data[b]));

					hauled_variables_block->push_back(variable_idx);
				}
			}

			hauled_variables_stack->push_back(hauled_variables_block);
		}

		hauled_variables_plane.push_back(hauled_variables_stack);
	}

	for (u64 i = 0; i < m_function_total_count; i++)
	{
		m_variable_real_count.push_back(m_variable_registry[i].size());
		m_variable_bool_count.push_back(m_variable_registry[i].size());
		m_variable_string_count.push_back(m_variable_registry[i].size());
	}

	for (u64 i = 0; i < m_function_total_count; i++)
	{
		m_variable_real_template.push_back(new f64[m_variable_real_count[i]]);
		m_variable_bool_template.push_back(new bool[m_variable_bool_count[i]]);
		m_variable_string_template.push_back(new std::string[m_variable_string_count[i]]);

		for (u64 a = 0; a < m_variable_real_count[i]; a++)
		{
			if (temp_regsitry_values[i][a] != nullptr
				&& temp_regsitry_types[i][a] == espresso::mod::block::data::interpretation::REAL)
				m_variable_real_template.back()[a] = *(f64*)temp_regsitry_values[i][a];
		}

		for (u64 a = 0; a < m_variable_bool_count[i]; a++)
		{
			if (temp_regsitry_values[i][a] != nullptr
				&& temp_regsitry_types[i][a] == espresso::mod::block::data::interpretation::BOOL)
				m_variable_bool_template.back()[a] = *(bool*)temp_regsitry_values[i][a];
		}

		for (u64 a = 0; a < m_variable_string_count[i]; a++)
		{
			if (temp_regsitry_values[i][a] != nullptr
				&& temp_regsitry_types[i][a] == espresso::mod::block::data::interpretation::STRING)
				m_variable_string_template.back()[a] = *(std::string*)temp_regsitry_values[i][a];
		}

		for (u64 a = 0; a < m_function_call_count[i]; a++)
		{
			m_data[i][a].clear_data();
			m_data[i][a].set_runtime_data(*hauled_variables_plane[i]->at(a));
			m_data[i][a].set_data_templates(i);
		}
	}
}

void cap::registration::compile_data_release()
{
	u64 variable_idx = 0;

	std::vector<std::vector<std::string>> temp_registry_real;
	std::vector<std::vector<f64*>> temp_regsitry_value_real;
	std::vector<u64> temp_offset_real;
	u64 temp_total_real = 0;

	std::vector<std::vector<std::string>> temp_registry_bool;
	std::vector<std::vector<bool*>> temp_regsitry_value_bool;
	std::vector<u64> temp_offset_bool;
	u64 temp_total_bool = 0;

	std::vector<std::vector<std::string>> temp_registry_string;
	std::vector<std::vector<std::string*>> temp_regsitry_value_string;
	std::vector<u64> temp_offset_string;
	u64 temp_total_string = 0;

	auto add_to_registry_real = [&](const std::string& name, u64 idx, f64* use = nullptr)
	{
		for (u64 i = 0; i < temp_registry_real[idx].size(); i++)
		{
			if (temp_registry_real[idx][i] == name)
			{
				variable_idx = i;
				return false;
			}
		}

		variable_idx = temp_registry_real[idx].size();
		temp_total_real++;
		temp_registry_real[idx].push_back(name);
		temp_regsitry_value_real[idx].push_back(use);

		return true;
	};

	auto add_to_registry_bool = [&](const std::string& name, u64 idx, bool* use = nullptr)
	{
		for (u64 i = 0; i < temp_registry_bool[idx].size(); i++)
		{
			if (temp_registry_bool[idx][i] == name)
			{
				variable_idx = i;
				return false;
			}
		}

		variable_idx = temp_registry_bool[idx].size();
		temp_total_bool++;
		temp_registry_bool[idx].push_back(name);
		temp_regsitry_value_bool[idx].push_back(use);

		return true;
	};

	auto add_to_registry_string = [&](const std::string& name, u64 idx, std::string* use = nullptr)
	{
		for (u64 i = 0; i < temp_registry_string[idx].size(); i++)
		{
			if (temp_registry_string[idx][i] == name)
			{
				variable_idx = i;
				return false;
			}
		}

		variable_idx = temp_registry_string[idx].size();
		temp_total_string++;
		temp_registry_string[idx].push_back(name);
		temp_regsitry_value_string[idx].push_back(use);

		return true;
	};

	std::vector<std::vector<std::vector<u64>*>*> hauled_variables_plane;

	for (u64 i = 0; i < m_function_total_count; i++)
	{
		temp_registry_real.push_back(std::vector<std::string>());
		temp_registry_bool.push_back(std::vector<std::string>());
		temp_registry_string.push_back(std::vector<std::string>());

		temp_regsitry_value_real.push_back(std::vector<f64*>());
		temp_regsitry_value_bool.push_back(std::vector<bool*>());
		temp_regsitry_value_string.push_back(std::vector<std::string*>());

		temp_offset_real.push_back(temp_total_real);
		temp_offset_bool.push_back(temp_total_bool);
		temp_offset_string.push_back(temp_total_string);

		std::vector<std::vector<u64>*>* hauled_variables_stack = new std::vector<std::vector<u64>*>();

		for (u64 a = 0; a < m_function_call_count[i]; a++)
		{
			const std::vector<void*>& data						  = m_data[i][a].get_data();
			const std::vector<espresso::mod::block::data::type>& types = m_data[i][a].get_types();
			const std::vector<espresso::mod::block::data::interpretation>& interpretations =
				m_data[i][a].get_interpretations();

			std::vector<u64>* hauled_variables_block = new std::vector<u64>();

			for (u64 b = 0; b < data.size(); b++)
			{
				if (types[b] == espresso::mod::block::data::type::VAR)
				{
					if (interpretations[b] == espresso::mod::block::data::interpretation::ANY)
					{
						bool found = false;

						if (!found)
						{
							for (u64 c = 0; c < temp_registry_real.size(); c++)
							{
								for (u64 d = 0; d < temp_registry_real[c].size(); d++)
								{
									if (temp_registry_real[c][d] == "_L_" + *(std::string*)data[b])
									{
										hauled_variables_block->push_back(c);
										m_data[i][a].set_interpretation(espresso::mod::block::data::interpretation::REAL, b);

										found = true;
										break;
									}
								}

								if (found)
									break;
							}
						}

						if (!found)
						{
							for (u64 c = 0; c < temp_registry_bool.size(); c++)
							{
								for (u64 d = 0; d < temp_registry_bool[c].size(); d++)
								{
									if (temp_registry_bool[c][d] == "_L_" + *(std::string*)data[b])
									{
										hauled_variables_block->push_back(c);
										m_data[i][a].set_interpretation(espresso::mod::block::data::interpretation::BOOL, b);

										found = true;
										break;
									}
								}

								if (found)
									break;
							}
						}

						if (!found)
						{
							for (u64 c = 0; c < temp_registry_string.size(); c++)
							{
								for (u64 d = 0; d < temp_registry_string[c].size(); d++)
								{
									if (temp_registry_string[c][d] == "_L_" + *(std::string*)data[b])
									{
										hauled_variables_block->push_back(c);
										m_data[i][a].set_interpretation(
											espresso::mod::block::data::interpretation::STRING, b);

										found = true;
										break;
									}
								}

								if (found)
									break;
							}
						}

						if (!found)
							latte::logger::warn(
								latte::logger::level::EDITOR,
								"*any* variable \"" + *(std::string*)data[b] + "\" was not found");
					}
					else
					{
						if (interpretations[b] == espresso::mod::block::data::interpretation::REAL)
						{
							add_to_registry_real("_L_" + *(std::string*)data[b], i);
							hauled_variables_block->push_back(variable_idx);
						}
						else if (interpretations[b] == espresso::mod::block::data::interpretation::BOOL)
						{
							add_to_registry_bool("_L_" + *(std::string*)data[b], i);
							hauled_variables_block->push_back(variable_idx);
						}
						else if (interpretations[b] == espresso::mod::block::data::interpretation::STRING)
						{
							add_to_registry_string("_L_" + *(std::string*)data[b], i);
							hauled_variables_block->push_back(variable_idx);
						}
					}
				}
				else
				{
					char buffer[20];
					sprintf(buffer, "_R_%lu_%lu_%lu", i, a, b);

					if (interpretations[b] == espresso::mod::block::data::interpretation::REAL)
					{
						add_to_registry_real(std::string(buffer), i, new f64(*(f64*)data[b]));
						hauled_variables_block->push_back(variable_idx);
					}
					else if (interpretations[b] == espresso::mod::block::data::interpretation::BOOL)
					{
						add_to_registry_bool(std::string(buffer), i, new bool(*(bool*)data[b]));
						hauled_variables_block->push_back(variable_idx);
					}
					else if (interpretations[b] == espresso::mod::block::data::interpretation::STRING)
					{
						add_to_registry_string(std::string(buffer), i, new std::string(*(std::string*)data[b]));
						hauled_variables_block->push_back(variable_idx);
					}
				}
			}

			hauled_variables_stack->push_back(hauled_variables_block);
		}

		hauled_variables_plane.push_back(hauled_variables_stack);
	}

	for (u64 i = 0; i < m_function_total_count; i++)
	{
		m_variable_real_count.push_back(temp_registry_real[i].size());
		m_variable_bool_count.push_back(temp_registry_bool[i].size());
		m_variable_string_count.push_back(temp_registry_string[i].size());
	}

	for (u64 i = 0; i < m_function_total_count; i++)
	{
		m_variable_real_template.push_back(new f64[m_variable_real_count[i]]);
		m_variable_bool_template.push_back(new bool[m_variable_bool_count[i]]);
		m_variable_string_template.push_back(new std::string[m_variable_string_count[i]]);

		for (u64 a = 0; a < m_variable_real_count[i]; a++)
		{
			if (temp_regsitry_value_real[i][a] != nullptr)
				m_variable_real_template.back()[a] = *(f64*)temp_regsitry_value_real[i][a];
		}

		for (u64 a = 0; a < m_variable_bool_count[i]; a++)
		{
			if (temp_regsitry_value_bool[i][a] != nullptr)
				m_variable_bool_template.back()[a] = *(bool*)temp_regsitry_value_bool[i][a];
		}

		for (u64 a = 0; a < m_variable_string_count[i]; a++)
		{
			if (temp_regsitry_value_string[i][a] != nullptr)
				m_variable_string_template.back()[a] = *(std::string*)temp_regsitry_value_string[i][a];
		}

		for (u64 a = 0; a < m_function_call_count[i]; a++)
		{
			m_data[i][a].clear_data();
			m_data[i][a].set_runtime_data(*hauled_variables_plane[i]->at(a));
			m_data[i][a].set_data_templates(i);
		}
	}
}

void cap::registration::run_context()
{
	if (m_debug_build)
	{
		bool flagged_stop = false;

		while (!m_all_done)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));

			if (test_super_base() || test_super_debug())
				flagged_stop = true;
		}

		if (flagged_stop)
			printf("...stopped execution\n");
	}
	else
	{
		bool flagged_stop = false;

		while (!m_all_done)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));

			if (test_super_base())
				flagged_stop = true;
		}

		if (flagged_stop)
			printf("...stopped execution\n");
	}
}

std::mutex cap::registration::m_passes_mutex;

std::vector<espresso::mod::block::pass*> cap::registration::m_passes;

std::vector<bool> cap::registration::m_passes_flagged;

std::mutex cap::registration::m_execution_mutex;

std::vector<cap::execution_thread*> cap::registration::m_execution;

std::vector<bool> cap::registration::m_execution_flagged;

std::vector<bool> cap::registration::m_execution_join;

u64 cap::registration::m_function_main;

u64* cap::registration::m_function_call_count;

u64 cap::registration::m_function_total_count;

cap::execution_thread::function_stack_list cap::registration::m_calls;

espresso::mod::block::data** cap::registration::m_data;

espresso::mod::block::block*** cap::registration::m_blocks;

std::vector<u64> cap::registration::m_variable_real_count;

std::vector<u64> cap::registration::m_variable_bool_count;

std::vector<u64> cap::registration::m_variable_string_count;

std::vector<f64*> cap::registration::m_variable_real_template;

std::vector<bool*> cap::registration::m_variable_bool_template;

std::vector<std::string*> cap::registration::m_variable_string_template;

std::mutex cap::registration::m_custom_register_mutex;

std::vector<void*> cap::registration::m_custom_register;

std::atomic<bool> cap::registration::m_util_finished;

std::atomic<bool> cap::registration::m_util_return_finished;

std::atomic<bool> cap::registration::m_all_done;

std::atomic<bool> cap::registration::m_stop;

std::thread cap::registration::m_util_thread;

bool cap::registration::m_debug_build;

u8* cap::registration::m_super;

i64* cap::registration::m_super_data;

std::mutex* cap::registration::m_super_mutex;

std::chrono::steady_clock::time_point cap::registration::m_time_begin;

bool cap::registration::m_break_full;

bool cap::registration::m_break_single;

i64 cap::registration::m_break_single_data;

std::atomic<bool> cap::registration::m_break_full_resume;

std::atomic<bool> cap::registration::m_break_single_resume;

std::vector<std::vector<std::string>> cap::registration::m_variable_registry;

std::vector<u64> cap::registration::m_variable_registry_offsets;
