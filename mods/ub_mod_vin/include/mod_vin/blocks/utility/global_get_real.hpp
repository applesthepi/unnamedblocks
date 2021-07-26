#pragma once
#include "mod_vin/categories.hpp"

class BlockUtilityGlobalGetReal : public ModBlock
{
public:
	const char* GetUnlocalizedName() const override;
	const char* GetCategory() const override;

	blockExecution PullExecuteDebug() const override;
	blockExecution PullExecuteRelease() const override;

	std::vector<std::pair<blockDataInitialization, u16>> GetRuntimeStages() const override;

	const std::vector<BlockArgumentInitializer> GetArguments() const override;
};
