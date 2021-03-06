#pragma once
#include "ModVin/ModCategories.h"
#include <Espresso/Espresso.h>

class BlockSystemMain : public ModBlock
{
public:
	const char* GetUnlocalizedName() const override;
	const char* GetCategory() const override;

	bool IsTopical() const override;

	blockExecution PullExecuteRelease() const override;
	blockExecution PullExecuteDebug() const override;

	const std::vector<BlockArgumentInitializer> GetArguments() const override;
};