#pragma once
#include "config.h"

#include "cappuccino/mod/block/block.hpp"
#include "cappuccino/mod/block/pass.hpp"

#include <cappuccino/utils.hpp>

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
	static void set_calls(void (***calls)(cap::mod::block::pass*));

	///
	static void set_function_data(cap::mod::block::data** function_data);

	///
	static void set_mod_blocks(cap::mod::block::block*** mod_blocks);

	///
	static void execute(method build_method, type build_type);

	///
	static void setup();
};
} // namespace cap