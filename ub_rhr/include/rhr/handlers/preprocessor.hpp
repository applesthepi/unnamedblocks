#pragma once
#include "config.h"

#include "rhr/stacking/plane.hpp"

#include <cappuccino/utils.hpp>

#if LINUX
#include <dlfcn.h>
#else
#include <windows.h>
#endif

enum class PreProcessorStatus
{
	NOT_READY,
	RUNNING,
	DONE,
	DONE_ERROR
};

/*

super

0 - nop
1 - stop all
2 - pause all

*/
namespace rhr::handler
{
class preprocessor
{
public:
	static void initialize();

	// building
	static void clean_up();

	// executing
	static void build(bool debug_build);
	static bool is_finished();

	// util
	static void set_finished(bool finished);
	//	static Plane* GetPlaneCopy();
	static void set_super(u8 super, i16 super_data);
	static u8 get_super();
	static i64 get_super_data();

	static u8* make_super();
	static i64* get_made_data();
	static std::mutex* get_made_mutex();
private:
	static std::thread m_thread;
	static std::atomic<bool> m_finished;
	static u8* m_super;
	static i64* m_super_data;

	static std::mutex* m_super_mutex;
};
}