#pragma once
#include "mod_vin/categories.hpp"

class BlockUtilityListIndexBool : public ModBlock
{
public:
	const char* get_unlocalized_name() const override;
	const char* GetCategory() const override;

	blockExecution PullExecuteDebug() const override;
	blockExecution PullExecuteRelease() const override;

	const std::vector<BlockArgumentInitializer> GetArguments() const override;
};
