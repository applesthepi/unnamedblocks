#pragma once
#include "mod_vin/categories.hpp"
#include <espresso/mod/data.hpp>

class BlockStringFromBool : public ModBlock
{
public:
	const char* GetUnlocalizedName() const override;
	const char* GetCategory() const override;

	blockExecution PullExecuteRelease() const override;
	blockExecution PullExecuteDebug() const override;

	const std::vector<BlockArgumentInitializer> GetArguments() const override;
};