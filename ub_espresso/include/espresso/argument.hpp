#pragma once
#include "config.h"

#include "espresso/type.hpp"

#include <latte/utils.hpp>

namespace esp
{
struct argument
{
	/// Restricts the argument::mode.
	enum class mode_restriction
	{
		/// User can freely change between the argument::mode.
		NONE,

		/// User can not change the argument::mode, it is stuck on whatever it was set to by default.
		RESTRICTED
	};

	/// This is the default assignment of whether it starts as var or raw input.
	enum class mode
	{
		/// If the argument can be set to var given the argument::mode_restriction, then the argument must
		/// return a valid esp::reg::type* so that ubbs can give a physical type that stores the information.
		VAR,

		/// Static data that ubbs will feed directly into code.
		RAW
	};

	/// See "dev/padding.png" for information on how argument padding works.
	enum class padding_style
	{
		NONE,
		SOFT,
		HARD
	};

	///
	bool rt_data;

	///
	esp::type* type;
};
}