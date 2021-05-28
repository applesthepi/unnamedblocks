#pragma once
#include "ModVin/ModCategories.h"
#include <Espresso/Espresso.h>

class BlockSystemWindowMouseRight : public ModBlock
{
public:
	const char* GetUnlocalizedName() const override;
	const char* GetCategory() const override;

	blockExecution PullExecuteDebug() const override;
	blockExecution PullExecuteRelease() const override;

	const std::vector<BlockArgumentInitializer> GetArguments() const override;
};
