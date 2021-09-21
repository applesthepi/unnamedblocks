#include "sub_string.hpp"

static void execute_release(cap::mod::block::pass* pass)
{
	pass->get_string(3) = pass->get_string(0).substr((u64)pass->get_real(1), (u64)pass->get_real(2));
}

static void execute_debug(cap::mod::block::pass* pass)
{
	std::string& str = pass->get_string(0);
	f64& offset = pass->get_real(1);
	f64& count = pass->get_real(2);
	std::string& final_str = pass->get_string(3);

	if (std::floor(offset) != offset)
	{
		pass->log_error("failed to substring at \"" + std::to_string(offset) + "\" for count \"" + std::to_string(count) + "\" from string \"" + str + "\" to replacing string \"" + final_str + "\"; offset is a decimal", cap::mod::block::pass::logger_fatality::BREAK);
		return;
	}

	if (std::floor(count) != count)
	{
		pass->log_error("failed to substring at \"" + std::to_string(offset) + "\" for count \"" + std::to_string(count) + "\" from string \"" + str + "\" to replacing string \"" + final_str + "\"; count is a decimal", cap::mod::block::pass::logger_fatality::BREAK);
		return;
	}

	i64 idx_at = offset;
	i64 idx_count = count;

	if (idx_at >= static_cast<i64>(str.length()) || idx_at < 0)
	{
		pass->log_error("failed to substring at \"" + std::to_string(idx_at) + "\" for count \"" + std::to_string(idx_count) + "\" from string \"" + str + "\" to replacing string \"" + final_str + "\"; offset index out of range", cap::mod::block::pass::logger_fatality::BREAK);
		return;
	}

	if (idx_count + idx_at > static_cast<i64>(str.length()))
	{
		pass->log_error("failed to substring at \"" + std::to_string(idx_at) + "\" for count \"" + std::to_string(idx_count) + "\" from string \"" + str + "\" to replacing string \"" + final_str + "\"; count to high", cap::mod::block::pass::logger_fatality::BREAK);
		return;
	}

	if (idx_count < 0)
	{
		pass->log_error("failed to substring at \"" + std::to_string(idx_at) + "\" for count \"" + std::to_string(idx_count) + "\" from string \"" + str + "\" to replacing string \"" + final_str + "\"; count index out of range", cap::mod::block::pass::logger_fatality::BREAK);
		return;
	}

	final_str = str.substr(idx_at, idx_count);
}

const char* mod_vin::block::string::sub_string::get_unlocalized_name() const
{
	return "vin_string_sub_string";
}

const char* mod_vin::block::string::sub_string::get_category() const
{
	return CATEGORY_STRINGS;
}

cap::mod::block::block::execution mod_vin::block::string::sub_string::pull_execute_release() const
{
	return execute_release;
}

cap::mod::block::block::execution mod_vin::block::string::sub_string::pull_execute_debug() const
{
	return execute_debug;
}

std::vector<cap::mod::block::block::argument::initializer> mod_vin::block::string::sub_string::get_arguments() const
{
	return {
		{ cap::mod::block::block::argument::type::STRING, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::VAR, "variable" },
		{ cap::mod::block::block::argument::type::TEXT, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "at" },
		{ cap::mod::block::block::argument::type::REAL, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "0" },
		{ cap::mod::block::block::argument::type::TEXT, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "count" },
		{ cap::mod::block::block::argument::type::REAL, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "1" },
		{ cap::mod::block::block::argument::type::TEXT, cap::mod::block::block::argument::variable_mode_restriction::NONE, cap::mod::block::block::argument::variable_mode::RAW, "for" },
		{ cap::mod::block::block::argument::type::STRING, cap::mod::block::block::argument::variable_mode_restriction::RESTRICTED, cap::mod::block::block::argument::variable_mode::VAR, "variable" }
	};
}
