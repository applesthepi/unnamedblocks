#pragma once
#include "ModVin/ModCategories.h"
#include <Espresso/Espresso.h>

class BlockYes : public ModBlock
{
public:
	const char* GetUnlocalizedName() const override;
	blockExecution PullExecuteDebug() const override;
	blockExecution PullExecuteRelease() const override;
	const char* GetCategory() const override;
	const ModBlockFlags GetFlags() const override;

	blockDataInitialization GetRuntimeInit() const override;

	const std::vector<BlockArgumentInitializer> GetArguments() const override;
};
