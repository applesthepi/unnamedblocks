#include "block.hpp"

static void execute_release(cap::mod::block::pass* pass) { pass->log_warning("executed default release function"); }

static void execute_debug(cap::mod::block::pass* pass) { pass->log_warning("executed default debug function"); }

static bool runtime_initialization(cap::preprocessor_data&) { return true; }

static bool runtime_data_initialization(cap::preprocessor_data&, cap::mod::block::data&) { return true; }

const char* cap::mod::block::block::get_unlocalized_name() const { return "vin_null"; }

const char* cap::mod::block::block::get_category() const { return "vin_null"; }

bool cap::mod::block::block::is_topical() const { return false; }

cap::mod::block::block::execution cap::mod::block::block::pull_execute_release() const { return execute_release; }

cap::mod::block::block::execution cap::mod::block::block::pull_execute_debug() const { return execute_debug; }

cap::mod::block::block::initialization cap::mod::block::block::get_runtime_global_pre_init() const { return runtime_initialization; }

cap::mod::block::block::initialization cap::mod::block::block::get_runtime_global_post_init() const { return runtime_initialization; }

cap::mod::block::block::initialization cap::mod::block::block::get_runtime_local_pre_init() const { return runtime_initialization; }

cap::mod::block::block::initialization cap::mod::block::block::get_runtime_local_post_init() const { return runtime_initialization; }

std::vector<std::pair<cap::mod::block::block::data_initialization, u16>> cap::mod::block::block::get_runtime_stages() const { return {}; }

std::vector<cap::mod::block::block::argument::initializer> cap::mod::block::block::get_arguments() const
{
	return {cap::mod::block::block::argument::initializer(
		cap::mod::block::block::argument::type::TEXT,
		cap::mod::block::block::argument::variable_mode_restriction::NONE,
		cap::mod::block::block::argument::variable_mode::RAW,
		"NULL")};
}
