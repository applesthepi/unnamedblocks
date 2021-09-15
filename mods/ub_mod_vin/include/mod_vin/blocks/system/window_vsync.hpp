#pragma once
#include "ModVin/ModCategories.hpp"
#include <Espresso/Espresso.hpp>

class BlockSystemWindowVsync : public ModBlock
{
public:
	const char* get_unlocalized_name() const override;
	const char* GetCategory() const override;

	blockExecution PullExecuteDebug() const override;
	blockExecution PullExecuteRelease() const override;

	const std::vector<BlockArgumentInitializer> GetArguments() const override;
};
