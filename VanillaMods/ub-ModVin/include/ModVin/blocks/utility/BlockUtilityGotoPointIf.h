#pragma once
#include "ModVin/ModCategories.h"
#include <Cappuccino/CappuccinoMod.h>

class BlockUtilityGotoPointIf : public ModBlock
{
public:
	const char* GetUnlocalizedName() const override;
	const char* GetCategory() const override;

	blockExecution PullExecuteDebug() const override;
	blockExecution PullExecuteRelease() const override;

	blockDataInitialization GetRuntimeInit() const override;

	const std::vector<BlockArgumentInitializer> GetArguments() const override;
};
