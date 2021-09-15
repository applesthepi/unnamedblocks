#pragma once
#include "mod_vin/categories.hpp"
#include <espresso/mod/data.hpp>

class BlockStringLog : public block
{
public:
	const char* get_unlocalized_name() const override;
	const char* get_category() const override;

	block_execution pull_execute_release() const override;
	block_execution pull_execute_debug() const override;

	const std::vector<initializer> get_arguments() const override;
};