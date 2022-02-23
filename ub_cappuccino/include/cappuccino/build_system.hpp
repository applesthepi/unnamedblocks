#pragma once
#include "config.h"

#include "espresso/mod/block/block.hpp"
#include <espresso/mod/block/pass.hpp>

#include <latte/utils.hpp>

namespace cap
{
///
class build_system
{
public:
	///
	enum class status
	{
		READY,
		RUNNING
	};

	///
	enum class method
	{
		QUICK_BUILD,
		FULL_BUILD
	};

	///
	enum class type
	{
		DEBUG,
		RELEASE
	};

	///
	static void set_main(u64 main);

	///
	static void set_function_call_count(u64* function_call_count);

	///
	static void set_function_total_count(u64 function_total_count);

	///
	static void set_calls(void (***calls)(espresso::mod::block::pass*));

	///
	static void set_function_data(espresso::mod::block::data** function_data);

	///
	static void set_mod_blocks(espresso::mod::block::block*** mod_blocks);

	///
	static void execute(method build_method, type build_type);

	///
	static void setup();
};
} // namespace cap