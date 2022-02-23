#include "block.hpp"

static void execute_release(espresso::mod::block::pass* pass)
{
	pass->log_warning("executed default release function");
}

static void execute_debug(espresso::mod::block::pass* pass)
{
	pass->log_warning("executed default debug function");
}

static bool runtime_initialization(cap::preprocessor_data&)
{
	return true;
}

static bool runtime_data_initialization(cap::preprocessor_data&, espresso::mod::block::data&)
{
	return true;
}

const char* espresso::mod::block::block::get_unlocalized_name() const
{
	return "vin_null";
}

const char* espresso::mod::block::block::get_category() const
{
	return "vin_null";
}

bool espresso::mod::block::block::is_topical() const
{
	return false;
}

espresso::mod::block::block::execution espresso::mod::block::block::pull_execute_release() const
{
	return execute_release;
}

espresso::mod::block::block::execution espresso::mod::block::block::pull_execute_debug() const
{
	return execute_debug;
}

espresso::mod::block::block::initialization espresso::mod::block::block::get_runtime_global_pre_init() const
{
	return runtime_initialization;
}

espresso::mod::block::block::initialization espresso::mod::block::block::get_runtime_global_post_init() const
{
	return runtime_initialization;
}

espresso::mod::block::block::initialization espresso::mod::block::block::get_runtime_local_pre_init() const
{
	return runtime_initialization;
}

espresso::mod::block::block::initialization espresso::mod::block::block::get_runtime_local_post_init() const
{
	return runtime_initialization;
}

std::vector<std::pair<espresso::mod::block::block::data_initialization, u16>>
espresso::mod::block::block::get_runtime_stages() const
{
	return {};
}

std::vector<espresso::mod::block::block::argument::initializer> espresso::mod::block::block::get_arguments() const
{
	return {espresso::mod::block::block::argument::initializer(
		espresso::mod::block::block::argument::type::TEXT,
		espresso::mod::block::block::argument::variable_mode_restriction::NONE,
		espresso::mod::block::block::argument::variable_mode::RAW,
		"NULL")};
}
