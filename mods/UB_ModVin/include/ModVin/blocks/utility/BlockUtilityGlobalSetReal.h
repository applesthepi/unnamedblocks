#pragma once
#include "ModVin/ModCategories.h"

class BlockUtilityGlobalSetReal : public ModBlock
{
public:
	const char* GetUnlocalizedName() const override;
	const char* GetCategory() const override;

	blockExecution PullExecuteDebug() const override;
	blockExecution PullExecuteRelease() const override;

	blockInitialization GetRuntimeGlobalPreInit() const override;
	blockInitialization GetRuntimeGlobalPostInit() const override;

	std::vector<std::pair<blockDataInitialization, uint16_t>> GetRuntimeStages() const override;

	const std::vector<BlockArgumentInitializer> GetArguments() const override;
};
