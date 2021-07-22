#include "BlockStringSubString.hpp"

static void ExecuteRelease(ModBlockPass* pass)
{
	pass->GetString(3) = pass->GetString(0).substr((uint64_t)pass->GetReal(1), (uint64_t)pass->GetReal(2));
}

static void ExecuteDebug(ModBlockPass* pass)
{
	std::string& str = pass->GetString(0);
	double& offset = pass->GetReal(1);
	double& count = pass->GetReal(2);
	std::string& finalStr = pass->GetString(3);

	if (std::floor(offset) != offset)
	{
		pass->LogError("failed to substring at \"" + std::to_string(offset) + "\" for count \"" + std::to_string(count) + "\" from string \"" + str + "\" to replacing string \"" + finalStr + "\"; offset is a decimal", LoggerFatality::BREAK);
		return;
	}

	if (std::floor(count) != count)
	{
		pass->LogError("failed to substring at \"" + std::to_string(offset) + "\" for count \"" + std::to_string(count) + "\" from string \"" + str + "\" to replacing string \"" + finalStr + "\"; count is a decimal", LoggerFatality::BREAK);
		return;
	}

	int64_t idxAt = offset;
	int64_t idxCount = count;

	if (idxAt >= static_cast<int64_t>(str.length()) || idxAt < 0)
	{
		pass->LogError("failed to substring at \"" + std::to_string(idxAt) + "\" for count \"" + std::to_string(idxCount) + "\" from string \"" + str + "\" to replacing string \"" + finalStr + "\"; offset index out of range", LoggerFatality::BREAK);
		return;
	}

	if (idxCount + idxAt > static_cast<int64_t>(str.length()))
	{
		pass->LogError("failed to substring at \"" + std::to_string(idxAt) + "\" for count \"" + std::to_string(idxCount) + "\" from string \"" + str + "\" to replacing string \"" + finalStr + "\"; count to high", LoggerFatality::BREAK);
		return;
	}

	if (idxCount < 0)
	{
		pass->LogError("failed to substring at \"" + std::to_string(idxAt) + "\" for count \"" + std::to_string(idxCount) + "\" from string \"" + str + "\" to replacing string \"" + finalStr + "\"; count index out of range", LoggerFatality::BREAK);
		return;
	}

	finalStr = str.substr(idxAt, idxCount);
}

const char* BlockStringSubString::GetUnlocalizedName() const
{
	return "vin_string_sub_string";
}

const char* BlockStringSubString::GetCategory() const
{
	return CATEGORY_STRINGS;
}

blockExecution BlockStringSubString::PullExecuteRelease() const
{
	return ExecuteRelease;
}

blockExecution BlockStringSubString::PullExecuteDebug() const
{
	return ExecuteDebug;
}

const std::vector<BlockArgumentInitializer> BlockStringSubString::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "variable"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "at"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "0"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "count"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "1"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "for"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::RESTRICTED, BlockArgumentVariableMode::VAR, "variable"));

	return args;
}
