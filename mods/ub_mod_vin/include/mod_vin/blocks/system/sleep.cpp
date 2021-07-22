#include "BlockSystemSleep.hpp"
#include <thread>
#include <chrono>
#include <cmath>

static void ExecuteRelease(ModBlockPass* pass)
{
	std::this_thread::sleep_for(std::chrono::milliseconds((uint64_t)pass->GetReal(0)));
}

static void ExecuteDebug(ModBlockPass* pass)
{
	double ms = pass->GetReal(0);

	if (std::floor(ms) != ms)
	{
		pass->LogError("failed to sleep for \"" + std::to_string(ms) + "\" seconds; milliseconds is a decimal", LoggerFatality::BREAK);
		return;
	}

	int64_t msInt = ms;

	if (msInt < 0)
	{
		pass->LogError("cannot sleep for \"" + std::to_string(msInt) + "\" milliseconds; milliseconds is negitive", LoggerFatality::BREAK);
		return;
	}

	uint32_t msUint = ms;

	std::this_thread::sleep_for(std::chrono::milliseconds(msUint));
}

const char* BlockSystemSleep::GetUnlocalizedName() const
{
	return "vin_system_sleep";
}

const char* BlockSystemSleep::GetCategory() const
{
	return CATEGORY_SYSTEM;
}

blockExecution BlockSystemSleep::PullExecuteRelease() const
{
	return ExecuteRelease;
}

blockExecution BlockSystemSleep::PullExecuteDebug() const
{
	return ExecuteDebug;
}

const std::vector<BlockArgumentInitializer> BlockSystemSleep::GetArguments() const
{
	std::vector<BlockArgumentInitializer> args;

	args.push_back(BlockArgumentInitializer(BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "sleep"));
	args.push_back(BlockArgumentInitializer(BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "1000"));

	return args;
}
