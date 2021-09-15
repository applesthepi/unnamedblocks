#pragma once
#include "mod_vin/categories.hpp"
#include <espresso/mod/data.hpp>

class BlockRealFromString : public ModBlock
{
public:
	const char* get_unlocalized_name() const override;
	const char* GetCategory() const override;

	blockExecution PullExecuteRelease() const override;
	blockExecution PullExecuteDebug() const override;

	const std::vector<BlockArgumentInitializer> GetArguments() const override;
};