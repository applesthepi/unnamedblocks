#pragma once
#include "ModVin/ModCategories.h"

class BlockUtilityFunctionDefine : public ModBlock
{
public:
	const char* GetUnlocalizedName() const override;
	const char* GetCategory() const override;

	bool IsTopical() const override;

	blockExecution PullExecuteDebug() const override;
	blockExecution PullExecuteRelease() const override;

	std::vector<std::pair<blockDataInitialization, uint16_t>> GetRuntimeStages() const override;

	const std::vector<BlockArgumentInitializer> GetArguments() const override;
};
