#include "build_system.hpp"

#include "cappuccino/registration.hpp"

void cap::build_system::set_main(u64 main)
{
	Registration::SetFunctionMain(main);
}

void cap::build_system::set_function_call_count(u64* function_call_count)
{
	Registration::SetFunctionCallCount(function_call_count);
}

void cap::build_system::set_function_total_count(u64 function_total_count)
{
	Registration::SetFunctionTotalCount(function_total_count);
}

void cap::build_system::set_calls(void (*** calls)(ModBlockPass*))
{
	Registration::SetCalls(calls);
}

void cap::build_system::set_function_data(ModBlockData** function_data)
{
	Registration::SetData(function_data);
}

void cap::build_system::set_mod_blocks(ModBlock*** mod_blocks)
{
	Registration::SetBlocks(mod_blocks);
}

void cap::build_system::execute(cap::build_system::method build_method, cap::build_system::type build_type)
{
	if (build_method == cap::build_system::method::QUICK_BUILD) {
		Registration::Run();
	}
}
