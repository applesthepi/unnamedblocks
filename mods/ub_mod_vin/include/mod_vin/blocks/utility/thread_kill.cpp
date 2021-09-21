#include "BlockUtilityThreadKill.hpp"

#include <Cappuccino/Registration.hpp>

static void execute_release(cap::mod::block::pass* pass)
{
	ExecutionThread* thr = (ExecutionThread*)pass->CustomGet(pass->get_real(0));
	thr->End();
	
	Registration::UnRegisterPass(thr->GetPass());
	Registration::UnRegisterExecutionThread(thr, false);

	pass->CustomFree(pass->get_real(0), false);
}

static void execute_debug(cap::mod::block::pass* pass)
{
	execute_release(pass);
}

const char* BlockUtilityThreadKill::get_unlocalized_name() const
{
	return "vin_utility_thread_kill";
}

const char* BlockUtilityThreadKill::get_category() const
{
	return CATEGORY_UTILITY;
}

cap::mod::block::block::execution BlockUtilityThreadKill::pull_execute_debug() const
{
	return execute_debug;
}

cap::mod::block::block::execution BlockUtilityThreadKill::pull_execute_release() const
{
	return execute_release;
}

std::vector<cap::mod::block::block::argument::initializer> BlockUtilityThreadKill::get_arguments() const
{
	std::vector<cap::mod::block::block::argument::initializer> args;

	{ cap::mod::block::block::argument::type::TEXT, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "kill thread"));
	{ cap::mod::block::block::argument::type::REAL, cap::mod::block::block::argument::variable_mode_restriction::RESTRICTED, cap::mod::block::block::argument::variable_mode::VAR, "thread"));

	return args;
}
