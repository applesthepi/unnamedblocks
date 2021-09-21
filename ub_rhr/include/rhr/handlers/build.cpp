#include "build.hpp"

#include "rhr/handlers/project.hpp"

#include <cappuccino/mod/block/data.hpp>

void thread_build(cap::build_system::method build_method, cap::build_system::type build_type)
{
	std::vector<std::string> function_references;
	std::vector<u32> function_ids;
	std::vector<u64> function_call_count;
	std::vector<std::vector<void(*)(cap::mod::block::pass* pass)>> function_calls;

	u64 function_main = 0;
	bool function_main_found = false;

	std::vector<std::shared_ptr<rhr::stack::stack>> stacks;

	for (u64 i = 0; i < rhr::stack::plane::primary_plane->get_collections().size(); i++)
	{
		const std::vector<std::shared_ptr<rhr::stack::stack>>& col_stacks = rhr::stack::plane::primary_plane->get_collections()[i]->get_stacks();

		for (u64 a = 0; a < col_stacks.size(); a++)
			stacks.push_back(col_stacks[a]);
	}

	cap::mod::block::data** function_data = new cap::mod::block::data*[stacks.size()];
	cap::mod::block::block*** mod_blocks = new cap::mod::block::block**[stacks.size()];
	
	for (u64 i = 0; i < stacks.size(); i++)
	{
		function_data[i] = new cap::mod::block::data[stacks[i]->get_blocks().size()];
		mod_blocks[i] = new cap::mod::block::block*[stacks[i]->get_blocks().size()];

		if (stacks[i]->get_blocks().size() >= 1 && std::string(stacks[i]->get_blocks()[0]->get_mod_block()->get_unlocalized_name()) == "vin_main")
		{
			if (function_main_found)
			{
				cap::logger::error(
				"program has more than 1 entry points; can not run program without exactly 1 entry point (vin_main)");
				rhr::handler::build::confirm_terminated();
				return;
			}

			function_main = i;
			function_main_found = true;
		}
		
		// TODO: function references to indices (use lambda code in Cappuccino/cap::registration.cpp)

		//functionReferences.push_back(*stacks->at(i)->GetBlock(0)->GetArgument(1)->GetDataRaw());
		//functionIds.push_back(functionReferences.size());

		std::vector<void(*)(cap::mod::block::pass* pass)> trans_calls;

		for (u64 a = 0; a < stacks[i]->get_blocks().size(); a++)
		{
			if (build_type == cap::build_system::type::DEBUG)
				trans_calls.push_back(stacks[i]->get_blocks()[a]->get_mod_block()->pull_execute_debug());
			else
				trans_calls.push_back(stacks[i]->get_blocks()[a]->get_mod_block()->pull_execute_release());

			mod_blocks[i][a] = (cap::mod::block::block*)stacks[i]->get_blocks()[a]->get_mod_block();

			std::vector<void*> arg_data;
			std::vector<cap::mod::block::data::type> arg_types;
			std::vector<cap::mod::block::data::interpretation> arg_interpretations;

			for (u64 b = 0; b < stacks[i]->get_blocks()[a]->get_arguments().size(); b++)
			{
				cap::mod::block::block::argument::type type = stacks[i]->get_blocks()[a]->get_arguments()[b]->get_type();

				if (stacks[i]->get_blocks()[a]->get_arguments()[b]->get_mode() == cap::mod::block::block::argument::variable_mode::VAR)
				{
					std::string* dt = new std::string();

					try
					{
						*dt = stacks[i]->get_blocks()[a]->get_arguments()[b]->get_data();
					}
					catch (const std::invalid_argument&)
					{
						cap::logger::error("invalid argument exception in preprocessor");
						return;
					}
					
					arg_data.push_back((void*)dt);
					arg_types.push_back(cap::mod::block::data::type::VAR);
					
					if (type == cap::mod::block::block::argument::type::TEXT)
						arg_interpretations.push_back(cap::mod::block::data::interpretation::TEXT);
					else if (type == cap::mod::block::block::argument::type::BOOL)
						arg_interpretations.push_back(cap::mod::block::data::interpretation::BOOL);
					else if (type == cap::mod::block::block::argument::type::REAL)
						arg_interpretations.push_back(cap::mod::block::data::interpretation::REAL);
					else if (type == cap::mod::block::block::argument::type::STRING)
						arg_interpretations.push_back(cap::mod::block::data::interpretation::STRING);
					else if (type == cap::mod::block::block::argument::type::ANY)
						arg_interpretations.push_back(cap::mod::block::data::interpretation::ANY);
				}
				else if (type == cap::mod::block::block::argument::type::STRING)
				{
					std::string* dt = new std::string();
					
					try
					{
						*dt = stacks[i]->get_blocks()[a]->get_arguments()[b]->get_data();
					}
					catch (const std::invalid_argument&)
					{
						cap::logger::error("invalid argument exception in preprocessor");
						return;
					}

					arg_data.push_back((void*)dt);
					arg_types.push_back(cap::mod::block::data::type::RAW);
					arg_interpretations.push_back(cap::mod::block::data::interpretation::STRING);
				}
				else if (type == cap::mod::block::block::argument::type::BOOL)
				{
					bool* dt = new bool;

					try
					{
						*dt = stacks[i]->get_blocks()[a]->get_arguments()[b]->get_data() == "1";
						cap::logger::debug("======================= BOOL: " + std::to_string(*dt));
					}
					catch (const std::invalid_argument&)
					{
						cap::logger::error("invalid argument exception in preprocessor");
						return;
					}

					arg_data.push_back((void*)dt);
					arg_types.push_back(cap::mod::block::data::type::RAW);
					arg_interpretations.push_back(cap::mod::block::data::interpretation::BOOL);
				}
				else if (type == cap::mod::block::block::argument::type::REAL)
				{
					f64* dt = new f64;
					
					try
					{
						*dt = std::stod(stacks[i]->get_blocks()[a]->get_arguments()[b]->get_data());
					}
					catch (const std::invalid_argument&)
					{
						cap::logger::error("invalid argument exception in preprocessor");
						return;
					}

					arg_data.push_back((void*)dt);
					arg_types.push_back(cap::mod::block::data::type::RAW);
					arg_interpretations.push_back(cap::mod::block::data::interpretation::REAL);
				}
			}
			cap::mod::block::data blockdata = cap::mod::block::data(arg_data, arg_types, arg_interpretations);
			function_data[i][a] = blockdata;
		}

		function_calls.push_back(trans_calls);
		function_call_count.push_back(trans_calls.size());
	}

	if (!function_main_found)
	{
		cap::logger::error("program has no entry points; can not run program without exactly 1 entry point (vin_main)");
		rhr::handler::build::confirm_terminated();
		return;
	}

	void (***calls)(cap::mod::block::pass*);
	calls = (void(***)(cap::mod::block::pass*))malloc(sizeof(void(**)(cap::mod::block::pass*)) * function_calls.size());

	for (u64 i = 0; i < function_calls.size(); i++)
	{
		void (**callsInside)(cap::mod::block::pass*);
		callsInside = (void(**)(cap::mod::block::pass*))malloc(sizeof(void(*)(cap::mod::block::pass*)) * function_calls[i].size());

		for (u64 a = 0; a < function_calls[i].size(); a++)
			callsInside[a] = function_calls[i][a];

		calls[i] = callsInside;
	}

	u64* functionCallCountC = new u64[function_calls.size()];

	for (u64 i = 0; i < function_call_count.size(); i++)
		functionCallCountC[i] = function_call_count[i];

	bool buildType = build_type == cap::build_system::type::DEBUG;

	u64 functionTotalCount = stacks.size();

	cap::build_system::setup();

	cap::build_system::set_main(function_main);
	cap::build_system::set_function_call_count(function_call_count.data());
	cap::build_system::set_function_total_count(functionTotalCount);
	cap::build_system::set_calls(calls);
	cap::build_system::set_function_data(function_data);
	cap::build_system::set_mod_blocks(mod_blocks);

	cap::build_system::execute(build_method, build_type);

	for (u64 i = 0; i < functionTotalCount; i++)
		free(calls[i]);
	free(calls);

	delete[] functionCallCountC;

	for (u64 i = 0; i < functionTotalCount; i++)
		delete[] function_data[i];
	delete[] function_data;

	rhr::handler::build::confirm_terminated();
}

void rhr::handler::build::initialize()
{
	m_terminate = false;
	m_status = cap::build_system::status::READY;
}

void rhr::handler::build::execute(cap::build_system::method build_method, cap::build_system::type build_type)
{
	if (m_status == cap::build_system::status::RUNNING)
	{
		cap::logger::error("can not execute a build while already running");
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
	m_status = cap::build_system::status::READY;
}

std::thread rhr::handler::build::m_thread;
std::atomic<bool> rhr::handler::build::m_terminate;
cap::build_system::status rhr::handler::build::m_status;