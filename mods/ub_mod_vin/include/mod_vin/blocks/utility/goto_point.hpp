#pragma once
#include "mod_vin/categories.hpp"
#include <cappuccino/build_system.hpp>

class BlockUtilityGotoPoint : public ModBlock
{
public:
	const char* get_unlocalized_name() const override;
	const char* GetCategory() const override;

	blockExecution PullExecuteDebug() const override;
	blockExecution PullExecuteRelease() const override;

	std::vector<std::pair<blockDataInitialization, u16>> GetRuntimeStages() const override;

	const std::vector<BlockArgumentInitializer> GetArguments() const override;
};
