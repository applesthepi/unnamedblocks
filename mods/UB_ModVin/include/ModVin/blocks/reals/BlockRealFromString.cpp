#include "BlockRealFromString.h"
#include <string>

static void ExecuteRelease(ModBlockPass* pass)
{
	pass->GetReal(0) = std::stod(pass->GetString(1));
}

static void ExecuteDebug(ModBlockPass* pass)
{
	double nd = 0.0;

	try
	{
		nd = std::stod(pass->GetString(1));
	}
	catch (std::invalid_argument&)
	{
		pass->LogError("failed to set real \"" + std::to_string(pass->GetReal(0)) + "\" to string \"" + pass->GetString(1) + "\"; the string could not be parsed into a real", LoggerFatality::BREAK);
		return;
	}

	pass->GetReal(0) = nd;
}

const char* BlockRealFromString::GetUnlocalizedName() const
{
	return "vin_real_from_string";
}

const char* BlockRealFromString::GetCategory() const
{
	return CATEGORY_REALS;
}

blockExecution BlockRealFromString::PullExecuteRelease() const
{
	return ExecuteRelease;
}

blockExecution BlockRealFromString::PullExecuteDebug() const
{
	return ExecuteDebug;
}

const std::vector<BlockArgumentInitializer> BlockRealFromString::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "set"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::RESTRICTED, BlockArgumentVariableMode::VAR, "variable"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "to"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::RESTRICTED, BlockArgumentVariableMode::VAR, "variable"));

	return args;
}