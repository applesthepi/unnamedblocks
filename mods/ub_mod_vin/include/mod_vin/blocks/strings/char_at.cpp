#include "BlockStringCharAt.hpp"

static void ExecuteRelease(ModBlockPass* pass)
{
	pass->GetString(2) = pass->GetString(0).at((u64)pass->GetReal(1));
}

static void ExecuteDebug(ModBlockPass* pass)
{
	std::string& str = pass->GetString(0);
	f64& chAt = pass->GetReal(1);
	std::string& finalStr = pass->GetString(2);

	if (std::floor(chAt) != chAt)
	{
		pass->LogError("failed to get char at \"" + std::to_string(chAt) + "\" from string \"" + str + "\" to replacing string \"" + finalStr + "\"; index is a decimal", LoggerFatality::BREAK);
		return;
	}

	i64 idx = chAt;

	if (idx >= static_cast<i64>(str.length()) || idx < 0)
	{
		pass->LogError("failed to get char at \"" + std::to_string(idx) + "\" from string \"" + str + "\" to replacing string \"" + finalStr + "\"; index out of range", LoggerFatality::BREAK);
		return;
	}

	finalStr = str.at(idx);
}

const char* BlockStringCharAt::get_unlocalized_name() const
{
	return "vin_string_char_at";
}

const char* BlockStringCharAt::GetCategory() const
{
	return CATEGORY_STRINGS;
}

blockExecution BlockStringCharAt::PullExecuteRelease() const
{
	return ExecuteRelease;
}

blockExecution BlockStringCharAt::PullExecuteDebug() const
{
	return ExecuteDebug;
}

const std::vector<BlockArgumentInitializer> BlockStringCharAt::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "variable"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "at"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "0"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "for"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::RESTRICTED, BlockArgumentVariableMode::VAR, "variable"));

	return args;
}
