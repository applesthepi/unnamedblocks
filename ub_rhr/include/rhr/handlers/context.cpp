#include "context.hpp"

void rhr::handler::context::initialize()
{
	m_open = false;
	m_flag_open = false;
	m_bounds = {};
	m_position = {};
}

void rhr::handler::context::open(flag context_flags)
{
	m_flags.clear();

	if (context_flags & flag::TEXT_STANDING_ONLY)
		m_flags.push_back(flag::TEXT_STANDING_ONLY);

	if (context_flags & flag::TEXT_SELECTION_ONLY)
		m_flags.push_back(flag::TEXT_SELECTION_ONLY);

	if (context_flags & flag::OBJECT_SHARED)
		m_flags.push_back(flag::OBJECT_SHARED);

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
		{
			switch (context_flag)
			{
			case flag::TEXT_STANDING_ONLY:
			{
				ImGui::BeginDisabled();
				if (ImGui::MenuItem("cut"))
					cap::logger::debug("cut");
				ImGui::EndDisabled();

				ImGui::BeginDisabled();
				if (ImGui::MenuItem("copy"))
					cap::logger::debug("copy");
				ImGui::EndDisabled();

				if (ImGui::MenuItem("paste"))
					cap::logger::debug("paste");

				ImGui::BeginDisabled();
				if (ImGui::MenuItem("delete"))
					cap::logger::debug("delete");
				ImGui::EndDisabled();

				break;
			}
			case flag::TEXT_SELECTION_ONLY:
			{
				if (ImGui::MenuItem("cut"))
					cap::logger::debug("cut");

				if (ImGui::MenuItem("copy"))
					cap::logger::debug("copy");

				if (ImGui::MenuItem("paste"))
					cap::logger::debug("paste");

				if (ImGui::MenuItem("delete"))
					cap::logger::debug("delete");

				break;
			}
			case flag::OBJECT_SHARED:
			{
				if (ImGui::MenuItem("duplicate"))
					cap::logger::debug("duplicate");

				if (ImGui::MenuItem("delete"))
					cap::logger::debug("delete");

				break;
			}
			default:
				break;
			}
		}

//		if (ImGui::BeginMenu("Sub-menu"))
//		{
//			ImGui::MenuItem("Click me");
//			ImGui::EndMenu();
//		}

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

std::vector<u32> rhr::handler::context::m_flags;
bool rhr::handler::context::m_open;
bool rhr::handler::context::m_flag_open;
glm::vec<2, i32> rhr::handler::context::m_bounds;
glm::vec<2, i32> rhr::handler::context::m_position;