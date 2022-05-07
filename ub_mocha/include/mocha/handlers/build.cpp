#include "build.hpp"

#include "mocha/handlers/project.hpp"

#include <espresso/mod/block/data.hpp>
#include <cappuccino/registration.hpp>
#include <latte/file_synthesiser.hpp>

#if LINUX
#include <spawn.h>
#include <sys/wait.h>
#include <dlfcn.h>
extern char **environ;
#else
#include <windows.h>
#endif

#define UB_BUILD_FAILED "Failed to build project. Submit an issue with the relevant project files for help."

void thread_build(cap::build_system::method build_method, cap::build_system::type build_type)
{
	auto build_file = latte::file_synthesiser::write("toolchain/ubbs/src/lib.rs");

	u64 function_main		 = 0;
	bool function_main_found = false;

	std::vector<std::shared_ptr<rhr::stack::stack>> stacks;

	// Get all the stacks regardless of collection status.

	for (const auto& collection : rhr::stack::plane::primary_plane->get_collections())
	{
		for (const auto& stack : collection->get_stacks())
			stacks.push_back(stack);
	}

	for (usize i = 0; i < stacks.size(); i++)
	{
		if (stacks[i]->get_blocks().empty())
		{
			latte::logger::warn(
				latte::logger::level::EDITOR,
				"stack is empty; ignoring");
			continue;
		}

		if (std::string(stacks[i]->get_blocks()[0]->get_esp_block()->get_unlocalized_name()) == "b_essentials_system_main")
		{
			if (function_main_found)
			{
				latte::logger::error(
					latte::logger::level::EDITOR,
					"program has more than 1 entry points; can not run program without exactly 1 entry point (b_essentials_main)");
				rhr::handler::build::confirm_terminated();
				return;
			}

			function_main		= i;
			function_main_found = true;
		}

		//
		// R VALUES
		//

		// If an argument is an r value (esp::argument::mode::RAW), then it must be defined at the top of the stack
		// function. For every r value, we take the ubbs instance of its type and the editor data to construct the
		// value at the top of the stacking function.

		// Instance records for the r values from the type that it is.
		std::vector<std::string> type_instances_r;

		// Instance record data for the r values from what it was in the editor.
		std::vector<std::string> type_instance_data_r;

		//
		// L VALUES
		//

		// If an argument is an l value (esp::argument::mode::VAR), then it can be used multiple times throughout the
		// stack function. So we need to define and initialize it at the top and continue to reference it throughout
		// the stack function. For every l value, we take the ubbs instance of its type and the editor data (name) to
		// construct the value at the top of the stacking function.

		// Variable names to which they are referred to as in the editor.
		std::vector<std::string> type_instance_names_l;

		// Variable names to which they are referred to as in ubbs.
		std::vector<std::string> type_instance_ubbs_names_l;

		// Instance records for the l values from the type that it is.
		std::vector<std::string> type_instances_l;

		//
		// BLOCKS
		//

		// Instance record data for the block for ubbs.
		std::vector<std::string> block_instance;

		// For every block, there is a list of argument names. These are the names referred to in ubbs.
		std::vector<std::vector<std::string>> block_arguments;

		for (usize a = 0; a < stacks[i]->get_blocks().size(); a++)
		{
			block_instance.emplace_back(stacks[i]->get_blocks()[a]->get_esp_block()->get_ubbs_instance());

			auto& ubbs_arguments = block_arguments.emplace_back();

			for (usize b = 0; b < stacks[i]->get_blocks()[a]->get_arguments().size(); b++)
			{
				auto& mocha_argument = stacks[i]->get_blocks()[a]->get_arguments()[b];
				auto esp_argument = mocha_argument.get_esp_argument();
				auto esp_argument_state = stacks[i]->get_blocks()[a]->get_arguments()[b].get_esp_argument_state();

				switch (esp_argument_state->mode)
				{
				case esp::argument::mode::RAW:
				{
					type_instances_r.emplace_back(esp_argument->get_type()->get_ubbs_instance_r());
					type_instance_data_r.emplace_back(esp_argument->get_ubbs_value(esp_argument_state));

					auto& ubbs_argument = ubbs_arguments.emplace_back();
					ubbs_argument += "r_";
					ubbs_argument += std::to_string(type_instances_r.size() - 1);

					break;
				}
				case esp::argument::mode::VAR:
				{
					bool found_var = false;
					std::string found_var_name;
					const auto ubbs_value = esp_argument->get_ubbs_value(esp_argument_state);

					for (const auto& var_name : type_instance_ubbs_names_l)
					{
						if (var_name == ubbs_value)
						{
							found_var = true;
							found_var_name = var_name;
							break;
						}
					}

					if (!found_var)
					{
						type_instance_names_l.emplace_back(ubbs_value);
						found_var_name = type_instance_ubbs_names_l.emplace_back("l_" + std::to_string(type_instance_ubbs_names_l.size()));
						type_instances_l.emplace_back(esp_argument->get_type()->get_ubbs_instance_l());

						auto& ubbs_argument = ubbs_arguments.emplace_back();
						ubbs_argument = found_var_name;
					}
					else
					{
						auto& ubbs_argument = ubbs_arguments.emplace_back();
						ubbs_argument = found_var_name;
					}

					break;
				}
				}
			}
		}

		//
		// R VALUE EVALUATION
		//

		std::vector<std::string> type_instances_r_replacements;

		{
			u8 repeating = 0;

			for (usize a = 0; a < type_instances_r.size(); a++)
			{
				std::string& instance_r = type_instances_r_replacements.emplace_back();
				instance_r.reserve(50);

				for (usize b = 0; b < type_instances_r[a].size(); b++)
				{
					char next_char = type_instances_r[a][b];

					if ((next_char == '<' && repeating >= 0 && repeating < 3) ||
						(next_char == '>' && repeating >= 3 && repeating < 6))
						repeating++;
					else
						instance_r += next_char;

					if (repeating == 6)
					{
						instance_r += type_instance_data_r[a];
						repeating = 0;
					}
				}
			}
		}

		//
		// BLOCK EVALUATION
		//

		std::vector<std::string> block_instance_replacements;

		{

			for (usize a = 0; a < block_instance.size(); a++)
			{
				// 0 - next char
				// 1 - opening
				// 2 - arg idx
				// 3 - closing
				u8 stage = 0;
				u8 repeating = 0;

				std::string& instance = block_instance_replacements.emplace_back();
				instance.reserve(100);

				std::string reserve;
				std::string arg_idx;

				for (usize b = 0; b < block_instance[a].size(); b++)
				{
					char next_char = block_instance[a][b];

					switch (stage)
					{
					case 0:
					{
						if (next_char == '<')
						{
							stage = 1;
							repeating = 1;

							reserve = next_char;
						}
						else
							instance += next_char;

						break;
					}
					case 1:
					{
						if (next_char == '<')
						{
							// CONTINUE

							repeating++;
							reserve += next_char;
						}
						else
						{
							// RESET

							repeating = 0;
							stage = 0;

							instance += reserve;
							reserve.clear();
							arg_idx.clear();
						}

						if (repeating >= 3)
						{
							// NEXT

							repeating = 0;
							stage = 2;
						}

						break;
					}
					case 2:
					{
						if (next_char >= '0' && next_char <= '9')
						{
							// CONTINUE

							repeating++;
							reserve += next_char;
							arg_idx += next_char;
						}
						else if (next_char == '>')
						{
							// NEXT

							reserve += next_char;

							repeating = 1;
							stage = 3;
						}
						else
						{
							// RESET

							repeating = 0;
							stage = 0;

							instance += reserve;
							reserve.clear();
							arg_idx.clear();
						}

						break;
					}
					case 3:
					{
						if (next_char == '>')
						{
							// CONTINUE

							repeating++;
							reserve += next_char;
						}
						else
						{
							// RESET

							repeating = 0;
							stage = 0;

							instance += reserve;
							reserve.clear();
							arg_idx.clear();
						}

						if (repeating >= 3)
						{
							// NEXT (FINISH)

							repeating = 0;
							stage = 0;

							usize u_arg_idx = std::stoull(arg_idx);
							const auto& block_arg = block_arguments[a];

							if (u_arg_idx >= block_arg.size())
							{
								latte::logger::error(
									latte::logger::level::EDITOR,
									"failed to index block argument. Idx given \"" + arg_idx + "\" is out of bounds. Block has \"" + std::to_string(block_arg.size()) + "\" arguments.");
								rhr::handler::build::confirm_terminated();
								return;
							}

							instance += block_arg[u_arg_idx];

							reserve.clear();
							arg_idx.clear();
						}

						break;
					}
					}
				}
			}
		}

		//
		// BUILD RUST FILE
		//

		std::string fn_str;
		fn_str.reserve(1000);

		fn_str += "fn sx_";
		fn_str += std::to_string(i);
		fn_str += "() {\n\t// R VALUES\n";

		// R VALUES

		for (usize a = 0; a < type_instances_r_replacements.size(); a++)
		{
			fn_str += "\tlet r_";
			fn_str += std::to_string(a);
			fn_str += ": ";
			fn_str += type_instances_r_replacements[a];
			fn_str += ";\n";
		}

		// L VALUES

		fn_str += "\t\n\t// L VALUES\n";

		for (usize a = 0; a < type_instances_l.size(); a++)
		{
			fn_str += "\tlet mut l_";
			fn_str += std::to_string(a);
			fn_str += ": ";
			fn_str += type_instances_l[a];
			fn_str += ";\n";
		}

		fn_str += "\t\n\t// BLOCKS\n";

		for (auto& replacement : block_instance_replacements)
		{
			fn_str += "\t";
			fn_str += replacement;
			fn_str += "\n";
		}

		fn_str += "}\n";

		build_file->push_back(fn_str);
	}

	std::string fn_str;
	fn_str.reserve(100);

	fn_str += "#[no_mangle]\npub extern \"C\" fn ub_run() {\n\tsx_";
	fn_str += std::to_string(function_main);
	fn_str += "();\n}";

	build_file->push_back(fn_str);

	build_file->overwrite();

	if (!function_main_found)
	{
		latte::logger::error(
			latte::logger::level::EDITOR,
			"program has no entry points; can not run program without exactly 1 entry point (essentials_main)");
		rhr::handler::build::confirm_terminated();
		return;
	}

	//
	// PERFORM BUILD
	//

	typedef void (*f_ub_run)();

#if LINUX

	pid_t pid = 0;
	char* argv[] = { "sh", "-c", "cd toolchain/ubbs/ && ./cargo build --release", NULL };
	i32 pstatus = posix_spawn(&pid, "/bin/sh", NULL, NULL, argv, environ);

	if (pstatus)
	{
		latte::logger::error(latte::logger::level::EDITOR, "failed to create a posix process: " + std::to_string(pstatus));
		return;
	}

	do
	{
		if (waitpid(pid, &pstatus, 0) == -1)
		{
			latte::logger::error(latte::logger::level::EDITOR, "failed to wait for posix process");
			return;
		}
	}
	while (!WIFEXITED(pstatus) && !WIFSIGNALED(pstatus));

	if (pstatus != 0)
	{
		latte::logger::error(latte::logger::level::EDITOR, UB_BUILD_FAILED);
		return;
	}

	void* so = dlopen("toolchain/ubbs/target/release/libubbs.so", RTLD_NOW);

	if (!so)
	{
		latte::logger::error(latte::logger::level::EDITOR, "failed to load ubbs");
		return;
	}

	f_ub_run ub_run = (f_ub_run)dlsym(so, "ub_run");

	if (!ub_run)
	{
		latte::logger::error(latte::logger::level::EDITOR, "failed to load ubbs \"ub_run\" function");
		return;
	}

	printf("running ub_run");
	ub_run();
	printf("done ub_run");

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
		latte::logger::error(latte::logger::level::EDITOR, UB_BUILD_FAILED);
		return;
	}

	HINSTANCE hGetProcIDDLL = LoadLibrary("toolchain\\ubbs\\target\\release\\ubbs.dll");

	if (!hGetProcIDDLL)
	{
		latte::logger::error(latte::logger::level::EDITOR, "failed to load ubbs");
		return;
	}

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

	rhr::handler::build::confirm_terminated();
}

void rhr::handler::build::initialize()
{
	m_terminate = false;
	m_status	= cap::build_system::status::READY;
}

void rhr::handler::build::execute(cap::build_system::method build_method, cap::build_system::type build_type)
{
	if (m_status == cap::build_system::status::RUNNING)
	{
		latte::logger::error(latte::logger::level::EDITOR, "can not execute a build while already running");
		return;
	}

	m_status = cap::build_system::status::RUNNING;

	if (m_thread.joinable())
		m_thread.join();

	m_thread = std::thread(thread_build, build_method, build_type);
}

cap::build_system::status rhr::handler::build::get_status()
{
	return m_status;
}

void rhr::handler::build::terminate()
{
	if (m_status == cap::build_system::status::RUNNING)
		m_terminate = true;
}

void rhr::handler::build::confirm_terminated()
{
	m_terminate = false;
	m_status	= cap::build_system::status::READY;
}

std::thread rhr::handler::build::m_thread;
std::atomic<bool> rhr::handler::build::m_terminate;
cap::build_system::status rhr::handler::build::m_status;