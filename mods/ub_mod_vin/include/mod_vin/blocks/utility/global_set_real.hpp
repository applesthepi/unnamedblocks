#pragma once
#include "mod_vin/categories.hpp"

class BlockUtilityGlobalSetReal : public ModBlock
{
public:
	const char* get_unlocalized_name() const override;
	const char* get_category() const override;

	cap::mod::block::block::execution pull_execute_debug() const override;
	cap::mod::block::block::execution pull_execute_release() const override;

	blockInitialization GetRuntimeGlobalPreInit() const override;
	blockInitialization GetRuntimeGlobalPostInit() const override;

	std::vector<std::pair<blockDataInitialization, u16>> GetRuntimeStages() const override;

	std::vector<cap::mod::block::block::argument::initializer> get_arguments() const override;
};
