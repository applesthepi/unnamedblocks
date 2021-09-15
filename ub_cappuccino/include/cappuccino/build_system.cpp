#include "build_system.hpp"

#include "cappuccino/registration.hpp"

void cap::build_system::set_main(u64 main)
{
	cap::registration::set_function_main(main);
}

void cap::build_system::set_function_call_count(u64* function_call_count)
{
	cap::registration::set_function_call_count(function_call_count);
}

void cap::build_system::set_function_total_count(u64 function_total_count)
{
	cap::registration::set_function_total_count(function_total_count);
}

void cap::build_system::set_calls(void(***calls)(cap::mod::block::pass*))
{
	cap::registration::set_calls(calls);
}

void cap::build_system::set_function_data(cap::mod::block::data** function_data)
{
	cap::registration::set_data(function_data);
}

void cap::build_system::set_mod_blocks(cap::mod::block::block*** mod_blocks)
{
	cap::registration::set_blocks(mod_blocks);
}

void cap::build_system::execute(cap::build_system::method build_method, cap::build_system::type build_type)
{
	if (build_method == cap::build_system::method::QUICK_BUILD) {
		cap::registration::run();
	}
}

void cap::build_system::setup()
{
	cap::registration::initialize();
}
