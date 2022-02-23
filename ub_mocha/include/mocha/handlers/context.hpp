#pragma once
#include "config.h"

#include <latte/utils.hpp>

#define RHR_HANDLER_CONTEXT_FLAG_PREFIX u32

namespace rhr::handler
{
///
class context
{
public:
	///
	struct flag
	{
		///
		struct info
		{
			///
			info* parent_info;

			///
			RHR_HANDLER_CONTEXT_FLAG_PREFIX flag;

			///
			std::vector<std::string> items;
		};

		/// For text that is selected/highlighted.
		static RHR_HANDLER_CONTEXT_FLAG_PREFIX TEXT_SELECTION_ONLY;
		static info INFO_TEXT_SELECTION_ONLY;

		/// For text that is NOT selected/highlighted.
		static RHR_HANDLER_CONTEXT_FLAG_PREFIX TEXT_STANDING_ONLY;
		static info INFO_TEXT_STANDING_ONLY;

		///
		static RHR_HANDLER_CONTEXT_FLAG_PREFIX OBJECT_STACKING_STACK;
		static info INFO_OBJECT_STACKING_STACK;

		///
		static RHR_HANDLER_CONTEXT_FLAG_PREFIX OBJECT_STACKING_BLOCK;
		static info INFO_OBJECT_STACKING_BLOCK;

		/// All text field operations.
		static RHR_HANDLER_CONTEXT_FLAG_PREFIX TEXT;
		static info INFO_TEXT;

		/// Duplicating and deleting.
		static RHR_HANDLER_CONTEXT_FLAG_PREFIX OBJECT_STACKING;
		static info INFO_OBJECT_STACKING;

		///
		enum class menu_text
		{
			CUT,
			COPY,
			PASTE,
			DELETE_
		};
		enum class menu_object
		{
			DUPLICATE_STACK,
			DELETE_STACK,
			DUPLICATE_BLOCK,
			DELETE_BLOCK
		};

		///
		static std::unordered_map<u8, menu_text> hashed_menu_text;
		static std::unordered_map<u8, menu_object> hashed_menu_object;

		///
		static std::unordered_map<
			RHR_HANDLER_CONTEXT_FLAG_PREFIX,
			std::function<void(std::optional<std::function<void(RHR_HANDLER_CONTEXT_FLAG_PREFIX, u8)>>&)>>
			hashed_imgui;
	};

	///
	static void initialize();

	///
	static void generate_functions(flag::info& flag_info);

	///
	static void open(
		RHR_HANDLER_CONTEXT_FLAG_PREFIX context_flags,
		std::optional<std::function<void(RHR_HANDLER_CONTEXT_FLAG_PREFIX, u8)>> callback_context);

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
	static std::vector<RHR_HANDLER_CONTEXT_FLAG_PREFIX> m_flags;

	///
	static bool m_open;

	///
	static bool m_flag_open;

	///
	static glm::vec<2, i32> m_bounds;

	///
	static glm::vec<2, i32> m_position;

	///
	static std::optional<std::function<void(RHR_HANDLER_CONTEXT_FLAG_PREFIX, u8)>> m_callback_context;
};
} // namespace rhr::handler