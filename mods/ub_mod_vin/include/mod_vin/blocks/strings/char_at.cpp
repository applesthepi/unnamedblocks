#include "char_at.hpp"

static void execute_release(cap::mod::block::pass* pass)
{
	pass->get_string(2) = pass->get_string(0).at((u64)pass->get_real(1));
}

static void execute_debug(cap::mod::block::pass* pass)
{
	std::string& str = pass->get_string(0);
	f64& ch_at = pass->get_real(1);
	std::string& final_str = pass->get_string(2);

	if (std::floor(ch_at) != ch_at)
	{
		pass->log_error("failed to get char at \"" + std::to_string(ch_at) + "\" from string \"" + str + "\" to replacing string \"" + final_str + "\"; index is a decimal", cap::mod::block::pass::logger_fatality::BREAK);
		return;
	}

	i64 idx = ch_at;

	if (idx >= static_cast<i64>(str.length()) || idx < 0)
	{
		pass->log_error("failed to get char at \"" + std::to_string(idx) + "\" from string \"" + str + "\" to replacing string \"" + final_str + "\"; index out of range", cap::mod::block::pass::logger_fatality::BREAK);
		return;
	}

	final_str = str.at(idx);
}

const char* mod_vin::block::string::char_at::get_unlocalized_name() const
{
	return "vin_string_char_at";
}

const char* mod_vin::block::string::char_at::get_category() const
{
	return CATEGORY_STRINGS;
}

cap::mod::block::block::execution mod_vin::block::string::char_at::pull_execute_release() const
{
	return execute_release;
}

cap::mod::block::block::execution mod_vin::block::string::char_at::pull_execute_debug() const
{
	return execute_debug;
}

std::vector<cap::mod::block::block::argument::initializer> mod_vin::block::string::char_at::get_arguments() const
{
	return {
		{ cap::mod::block::block::argument::type::STRING, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::VAR, "variable" },
		{ cap::mod::block::block::argument::type::TEXT, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "at" },
		{ cap::mod::block::block::argument::type::REAL, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "0" },
		{ cap::mod::block::block::argument::type::TEXT, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "for" },
		{ cap::mod::block::block::argument::type::STRING, cap::mod::block::block::argument::variable_mode_restriction::RESTRICTED, cap::mod::block::block::argument::variable_mode::VAR, "variable" }
	};
}
