#pragma once
#include "config.h"

#include <cappuccino/utils.hpp>

namespace rhr::handler
{
///
class context
{
public:
	///
	enum flag : u32
	{
		TEXT_SELECTION_ONLY = 0x1, /// For text that is selected/highlighted.
		TEXT_STANDING_ONLY  = 0x2, /// For text that is NOT selected/highlighted.

		OBJECT_SHARED  = 0x4, /// Duplicating and deleting.

		/// All text field operations.
		TEXT = TEXT_SELECTION_ONLY | TEXT_STANDING_ONLY,

		/// All object operations.
		OBJECT = OBJECT_SHARED,
	};

	///
	static void initialize();

	///
	static void open(flag context_flags);

	///
	static void close();

	///
	static void run_imgui();

	///
	static bool is_open();

	///
	static const glm::vec<2, i32>& get_position();

	///
	static const glm::vec<2, i32>& get_bounds();
private:
	///
	static std::vector<u32> m_flags;

	///
	static bool m_open;

	///
	static bool m_flag_open;

	///
	static glm::vec<2, i32> m_bounds;

	///
	static glm::vec<2, i32> m_position;
};
}