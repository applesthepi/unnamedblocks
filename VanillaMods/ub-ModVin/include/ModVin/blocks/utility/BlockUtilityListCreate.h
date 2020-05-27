#pragma once
#include "ModVin/ModCategories.h"

class BlockUtilityListCreate : public ModBlock
{
public:
	const char* GetUnlocalizedName() const override;
	const char* GetCategory() const override;

	blockExecution PullExecuteDebug() const override;
	blockExecution PullExecuteRelease() const override;

	blockInitialization GetRuntimeGlobalPreInit() const override;

	const std::vector<BlockArgumentInitializer> GetArguments() const override;
};
