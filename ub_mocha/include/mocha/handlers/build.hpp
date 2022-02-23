#pragma once
#include "config.h"

#include "mocha/stacking/plane.hpp"

#include <cappuccino/build_system.hpp>
#include <latte/utils.hpp>

#if LINUX
#include <dlfcn.h>
#else
#include <windows.h>
#endif

namespace rhr::handler
{
///
class build
{
public:
	///
	static void initialize();

	///
	static void execute(cap::build_system::method build_method, cap::build_system::type build_type);

	///
	static cap::build_system::status get_status();

	///
	static void terminate();

	///
	static void confirm_terminated();

private:
	///
	static std::thread m_thread;

	///
	static std::atomic<bool> m_terminate;

	///
	static cap::build_system::status m_status;
};
} // namespace rhr::handler