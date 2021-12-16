#include "context.hpp"

void rhr::handler::context::initialize()
{
	m_open = false;
	m_flag_open = false;
	m_bounds = {};
	m_position = {};
	m_callback_context = std::nullopt;

	// setup info

	flag::INFO_TEXT_SELECTION_ONLY = { &flag::INFO_TEXT, rhr::handler::context::flag::TEXT_SELECTION_ONLY,  { "cut", "copy", "paste", "delete" } };
	flag::INFO_TEXT_STANDING_ONLY = { &flag::INFO_TEXT, rhr::handler::context::flag::TEXT_STANDING_ONLY, { "paste" } };

	flag::INFO_OBJECT_STACKING_STACK = { &flag::INFO_OBJECT_STACKING, rhr::handler::context::flag::OBJECT_STACKING_STACK, { "duplicate stack", "delete stack" } };
	flag::INFO_OBJECT_STACKING_BLOCK = { &flag::INFO_OBJECT_STACKING, rhr::handler::context::flag::OBJECT_STACKING_BLOCK, { "duplicate block", "delete block" } };

	flag::INFO_TEXT = { &flag::INFO_TEXT, rhr::handler::context::flag::TEXT, { "cut", "copy", "paste", "delete" } };
	flag::INFO_OBJECT_STACKING = { &flag::INFO_OBJECT_STACKING, rhr::handler::context::flag::OBJECT_STACKING, { "duplicate stack", "delete stack", "duplicate block", "delete block" } };

	// create imgui functions

	flag::hashed_imgui.reserve(32);

	generate_functions(flag::INFO_TEXT_SELECTION_ONLY);
	generate_functions(flag::INFO_TEXT_STANDING_ONLY);

	generate_functions(flag::INFO_OBJECT_STACKING_STACK);
	generate_functions(flag::INFO_OBJECT_STACKING_BLOCK);

	generate_functions(flag::INFO_TEXT);
	generate_functions(flag::INFO_OBJECT_STACKING);

	// setup hashed menu identifier

	flag::hashed_menu_text[0] = flag::menu_text::CUT;
	flag::hashed_menu_text[1] = flag::menu_text::COPY;
	flag::hashed_menu_text[2] = flag::menu_text::PASTE;
	flag::hashed_menu_text[3] = flag::menu_text::DELETE_;

	flag::hashed_menu_object[0] = flag::menu_object::DUPLICATE_STACK;
	flag::hashed_menu_object[1] = flag::menu_object::DELETE_STACK;
	flag::hashed_menu_object[2] = flag::menu_object::DUPLICATE_BLOCK;
	flag::hashed_menu_object[3] = flag::menu_object::DELETE_BLOCK;
}

void rhr::handler::context::generate_functions(rhr::handler::context::flag::info& flag_info)
{
	std::vector<std::string> menu_items = flag_info.parent_info->items;
	std::vector<bool> menu_items_enabled;

	// gather items and enabled state

	for (auto& menu_item : menu_items)
	{
		bool found = false;
		for (auto& item : flag_info.items)
		{
			if (menu_item == item)
			{
				found = true;
				break;
			}
		}

		menu_items_enabled.push_back(found);
	}

	RHR_HANDLER_CONTEXT_FLAG_PREFIX menu_flag = flag_info.flag;

	// make imgui function for flag

	auto hash_imgui = [menu_items, menu_items_enabled, menu_flag](std::optional<std::function<void(RHR_HANDLER_CONTEXT_FLAG_PREFIX, u8)>>& context_callback)
	{
		for (u8 i = 0; i < static_cast<u8>(menu_items.size()); i++)
		{
			if (!menu_items_enabled[i])
				ImGui::BeginDisabled();

			if (ImGui::MenuItem(menu_items[i].c_str()))
			{
				if (context_callback.has_value())
					context_callback.value()(menu_flag, i);
				else
					cap::logger::warn(cap::logger::level::SYSTEM, "context menu has no callback \"" + menu_items[i] + "\"");
			}

			if (!menu_items_enabled[i])
				ImGui::EndDisabled();
		}
	};

	flag::hashed_imgui[flag_info.flag] = hash_imgui;
}

void rhr::handler::context::open(RHR_HANDLER_CONTEXT_FLAG_PREFIX context_flags, std::optional<std::function<void(RHR_HANDLER_CONTEXT_FLAG_PREFIX, u8)>> callback_context)
{
	std::function<bool(RHR_HANDLER_CONTEXT_FLAG_PREFIX)> test_bit_mask = [context_flags](RHR_HANDLER_CONTEXT_FLAG_PREFIX flag)
	{
		if ((context_flags & flag) == flag)
		{
			m_flags.push_back(flag);
			return true;
		}
		else
			return false;
	};

	m_callback_context = std::move(callback_context);
	m_flags.clear();

	if (!test_bit_mask(flag::TEXT))
	{
		test_bit_mask(flag::TEXT_STANDING_ONLY);
		test_bit_mask(flag::TEXT_SELECTION_ONLY);
	}

	if (!test_bit_mask(flag::OBJECT_STACKING))
	{
		test_bit_mask(flag::OBJECT_STACKING_STACK);
		test_bit_mask(flag::OBJECT_STACKING_BLOCK);
	}

	m_flag_open = true;
}

void rhr::handler::context::close()
{

}

void rhr::handler::context::run_imgui()
{
	if (m_flag_open)
	{
		m_flag_open = false;
		ImGui::OpenPopup("context_menu");
	}

	if (ImGui::BeginPopup("context_menu"))
	{
		m_open = true;

		for (auto context_flag : m_flags)
			flag::hashed_imgui[context_flag](m_callback_context);

		ImVec2 bounds = ImGui::GetWindowSize();
		m_bounds = { bounds.x, bounds.y };

		ImVec2 position = ImGui::GetWindowPos();
		m_position = { position.x, position.y };

		ImGui::EndPopup();
	}
	else
		m_open = false;
}

bool rhr::handler::context::is_open()
{
	return m_open;
}

const glm::vec<2, i32>& rhr::handler::context::get_position()
{
	return m_position;
}

const glm::vec<2, i32>& rhr::handler::context::get_bounds()
{
	return m_bounds;
}

RHR_HANDLER_CONTEXT_FLAG_PREFIX rhr::handler::context::flag::TEXT_SELECTION_ONLY = 0x1;
rhr::handler::context::flag::info rhr::handler::context::flag::INFO_TEXT_SELECTION_ONLY;

RHR_HANDLER_CONTEXT_FLAG_PREFIX rhr::handler::context::flag::TEXT_STANDING_ONLY = 0x2;
rhr::handler::context::flag::info rhr::handler::context::flag::INFO_TEXT_STANDING_ONLY;

RHR_HANDLER_CONTEXT_FLAG_PREFIX rhr::handler::context::flag::OBJECT_STACKING_STACK = 0x4;
rhr::handler::context::flag::info rhr::handler::context::flag::INFO_OBJECT_STACKING_STACK;

RHR_HANDLER_CONTEXT_FLAG_PREFIX rhr::handler::context::flag::OBJECT_STACKING_BLOCK = 0x8;
rhr::handler::context::flag::info rhr::handler::context::flag::INFO_OBJECT_STACKING_BLOCK;

RHR_HANDLER_CONTEXT_FLAG_PREFIX rhr::handler::context::flag::TEXT = TEXT_SELECTION_ONLY | TEXT_STANDING_ONLY;
rhr::handler::context::flag::info rhr::handler::context::flag::INFO_TEXT;

RHR_HANDLER_CONTEXT_FLAG_PREFIX rhr::handler::context::flag::OBJECT_STACKING = OBJECT_STACKING_STACK | OBJECT_STACKING_BLOCK;
rhr::handler::context::flag::info rhr::handler::context::flag::INFO_OBJECT_STACKING;

std::unordered_map<u8, rhr::handler::context::flag::menu_text> rhr::handler::context::flag::hashed_menu_text;
std::unordered_map<u8, rhr::handler::context::flag::menu_object> rhr::handler::context::flag::hashed_menu_object;

std::unordered_map<RHR_HANDLER_CONTEXT_FLAG_PREFIX, std::function<void(std::optional<std::function<void(RHR_HANDLER_CONTEXT_FLAG_PREFIX, u8)>>&)>> rhr::handler::context::flag::hashed_imgui;

std::vector<RHR_HANDLER_CONTEXT_FLAG_PREFIX> rhr::handler::context::m_flags;
bool rhr::handler::context::m_open;
bool rhr::handler::context::m_flag_open;
glm::vec<2, i32> rhr::handler::context::m_bounds;
glm::vec<2, i32> rhr::handler::context::m_position;
std::optional<std::function<void(RHR_HANDLER_CONTEXT_FLAG_PREFIX, u8)>> rhr::handler::context::m_callback_context;