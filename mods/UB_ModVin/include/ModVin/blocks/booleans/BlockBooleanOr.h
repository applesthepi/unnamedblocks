#pragma once
#include "ModVin/ModCategories.h"
#include <Espresso/Espresso.h>

class BlockBooleanOr : public ModBlock
{
public:
	const char* GetUnlocalizedName() const override;
	const char* GetCategory() const override;

	blockExecution PullExecuteRelease() const override;
	blockExecution PullExecuteDebug() const override;

	const std::vector<BlockArgumentInitializer> GetArguments() const override;
};