#pragma once
#include "mod_vin/categories.hpp"
#include <espresso/mod/data.hpp>

namespace mod_vin::block::real
{
class mod : public cap::mod::block::block
{
public:
	const char* get_unlocalized_name() const override;
	const char* get_category() const override;

	cap::mod::block::block::execution pull_execute_release() const override;
	cap::mod::block::block::execution pull_execute_debug() const override;

	std::vector<cap::mod::block::block::argument::initializer> get_arguments() const override;
};
}