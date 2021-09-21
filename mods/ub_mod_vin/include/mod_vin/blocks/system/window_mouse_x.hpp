#pragma once
#include "ModVin/ModCategories.hpp"
#include <Espresso/Espresso.hpp>

class BlockSystemWindowMouseX : public ModBlock
{
public:
	const char* get_unlocalized_name() const override;
	const char* get_category() const override;

	cap::mod::block::block::execution pull_execute_debug() const override;
	cap::mod::block::block::execution pull_execute_release() const override;

	std::vector<cap::mod::block::block::argument::initializer> get_arguments() const override;
};
