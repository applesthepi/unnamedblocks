#include "input.hpp"

#include "rhr/handlers/context.hpp"
#include "rhr/rendering/renderer.hpp"

void InputHandler::Initialization()
{
	m_MouseWasDown = false;
	m_ShiftDown	   = false;

	m_MousePosition = {0, 0};
}

void InputHandler::RegisterKeyCallback(void (*callback)(key_state state, void* data), void* data)
{
	std::unique_lock lock(m_KeyTextMutex);

	m_KeyCallbacks.push_back(callback);
	m_KeyDatas.push_back(data);
}

void InputHandler::UnregisterKeyCallback(void (*callback)(key_state state, void* data))
{
	std::unique_lock lock(m_KeyTextMutex);

	for (usize i = 0; i < m_KeyCallbacks.size(); i++)
	{
		if (m_KeyCallbacks[i] == callback)
		{
			m_KeyCallbacks.erase(m_KeyCallbacks.begin() + i);
			m_KeyDatas.erase(m_KeyDatas.begin() + i);
			return;
		}
	}

	cap::logger::error(cap::logger::level::SYSTEM, "failed to unregister key callback");
}

void InputHandler::RegisterTextCallback(void (*callback)(key_state state, void* data), void* data)
{
	std::unique_lock lock(m_KeyTextMutex);

	m_TextCallbacks.push_back(callback);
	m_TextDatas.push_back(data);
}

void InputHandler::UnregisterTextCallback(void (*callback)(key_state state, void* data))
{
	std::unique_lock lock(m_KeyTextMutex);

	for (usize i = 0; i < m_TextCallbacks.size(); i++)
	{
		if (m_TextCallbacks[i] == callback)
		{
			m_TextCallbacks.erase(m_TextCallbacks.begin() + i);
			m_TextDatas.erase(m_TextDatas.begin() + i);
			return;
		}
	}

	cap::logger::error(cap::logger::level::SYSTEM, "failed to unregister text callback");
}

void InputHandler::RegisterMouseCallback(void (*callback)(glm::vec<2, i32> position, f32 scroll, MouseOperation operation, MouseButton button, void* data), void* data)
{
	std::unique_lock lock(m_MouseMutex);

	m_MouseCallbacks.push_back(callback);
	m_MouseDatas.push_back(data);
}

void InputHandler::UnregisterMouseCallback(void (*callback)(glm::vec<2, i32> position, f32 scroll, MouseOperation operation, MouseButton button, void* data))
{
	std::unique_lock lock(m_MouseMutex);

	for (usize i = 0; i < m_MouseCallbacks.size(); i++)
	{
		if (m_MouseCallbacks[i] == callback)
		{
			m_MouseCallbacks.erase(m_MouseCallbacks.begin() + i);
			m_MouseDatas.erase(m_MouseDatas.begin() + i);
			return;
		}
	}

	cap::logger::error(cap::logger::level::SYSTEM, "failed to unregister mouse callback");
}

void InputHandler::RegisterBullishMouseCallback(
	bool (*callback)(glm::vec<2, i32> position, f32 scroll, MouseOperation operation, MouseButton button, void* data), u8 layer, void* data)
{
	std::unique_lock lock(m_MouseMutex);

	if (layer >= m_BullishMouseCallbacks.size())
	{
		for (u8 i = 0; i < layer - m_BullishMouseCallbacks.size() + 1; i++)
		{
			m_BullishMouseCallbacks.push_back(std::vector<bool (*)(glm::vec<2, i32> position, f32 scroll, MouseOperation operation, MouseButton button, void* data)>());
			m_BullishMouseDatas.push_back(std::vector<void*>());
		}
	}

	m_BullishMouseCallbacks[layer].push_back(callback);
	m_BullishMouseDatas[layer].push_back(data);
}

void InputHandler::UnregisterBullishMouseCallback(bool (*callback)(glm::vec<2, i32> position, f32 scroll, MouseOperation operation, MouseButton button, void* data), u8 layer)
{
	std::unique_lock lock(m_MouseMutex);

	for (usize i = 0; i < m_BullishMouseCallbacks[layer].size(); i++)
	{
		if (m_BullishMouseCallbacks[layer][i] == callback)
		{
			m_BullishMouseCallbacks[layer].erase(m_BullishMouseCallbacks[layer].begin() + i);
			m_BullishMouseDatas[layer].erase(m_BullishMouseDatas[layer].begin() + i);
			return;
		}
	}

	cap::logger::error(cap::logger::level::SYSTEM, "failed to unregister bullish mouse callback");
}

void InputHandler::FireKey(i16 key, u8 operation)
{
	std::unique_lock lock(m_KeyTextMutex);

	if (key == GLFW_KEY_LEFT_SHIFT)
	{
		if (operation == GLFW_PRESS)
			m_ShiftDown = true;
		else if (operation == GLFW_RELEASE)
			m_ShiftDown = false;
	}

	if (key == GLFW_KEY_LEFT_CONTROL)
	{
		if (operation == GLFW_PRESS)
			m_ControlDown = true;
		else if (operation == GLFW_RELEASE)
			m_ControlDown = false;
	}

	if (key == GLFW_KEY_LEFT_ALT)
	{
		if (operation == GLFW_PRESS)
			m_AltDown = true;
		else if (operation == GLFW_RELEASE)
			m_AltDown = false;
	}

	if (operation == GLFW_PRESS)
	{
		for (usize i = 0; i < m_KeyCallbacks.size(); i++)
			m_KeyCallbacks[i]({key, true, m_ShiftDown, m_ControlDown, m_AltDown}, m_KeyDatas[i]);

		for (usize i = 0; i < m_TextCallbacks.size(); i++)
			m_TextCallbacks[i]({key, true, m_ShiftDown, m_ControlDown, m_AltDown}, m_TextDatas[i]);
	}
	else if (operation == GLFW_RELEASE)
	{
		for (usize i = 0; i < m_KeyCallbacks.size(); i++)
			m_KeyCallbacks[i]({key, false, m_ShiftDown, m_ControlDown, m_AltDown}, m_KeyDatas[i]);
	}
	else if (operation == GLFW_REPEAT)
	{
		for (usize i = 0; i < m_TextCallbacks.size(); i++)
			m_TextCallbacks[i]({key, true, m_ShiftDown, m_ControlDown, m_AltDown}, m_TextDatas[i]);
	}
}

void InputHandler::FireMouseButton(u8 button, u8 operation)
{
	m_LastMouseButton = MouseButton::LEFT;

	switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
	{
		m_LastMouseButton = MouseButton::LEFT;
		break;
	}
	case GLFW_MOUSE_BUTTON_RIGHT:
	{
		m_LastMouseButton = MouseButton::RIGHT;
		break;
	}
	case GLFW_MOUSE_BUTTON_MIDDLE:
	{
		m_LastMouseButton = MouseButton::MIDDLE;
		break;
	}
	};

	if (rhr::handler::context::is_open())
	{
		glm::vec<2, i32> context_position	   = rhr::handler::context::get_position() - rhr::render::renderer::get_window_primary()->get_window_position();
		const glm::vec<2, i32>& context_bounds = rhr::handler::context::get_bounds();

		if (m_MousePosition.x > context_position.x && m_MousePosition.x < context_position.x + context_bounds.x && m_MousePosition.y > context_position.y
			&& m_MousePosition.y < context_position.y + context_bounds.y)
		{
			return;
		}
	}

	std::unique_lock lock(m_MouseMutex);

	if (operation == GLFW_PRESS)
	{
		TIME_POINT now = std::chrono::high_resolution_clock::now();

		if ((m_PressLog.size() > 0 && std::chrono::duration_cast<std::chrono::milliseconds>(now - m_PressLog.back()).count() > 300) || m_PressLog.size() >= 3)
			m_PressLog.clear();

		m_PressLog.push_back(now);

		if (m_PressLog.size() == 1)
		{
			for (usize i = 0; i < m_MouseCallbacks.size(); i++)
				m_MouseCallbacks[i](m_MousePosition, 0.0f, MouseOperation::Press, m_LastMouseButton, m_MouseDatas[i]);
		}
		else if (m_PressLog.size() == 2)
		{
			for (usize i = 0; i < m_MouseCallbacks.size(); i++)
				m_MouseCallbacks[i](m_MousePosition, 0.0f, MouseOperation::DoublePress, m_LastMouseButton, m_MouseDatas[i]);
		}
		else if (m_PressLog.size() == 3)
		{
			for (usize i = 0; i < m_MouseCallbacks.size(); i++)
				m_MouseCallbacks[i](m_MousePosition, 0.0f, MouseOperation::TripplePress, m_LastMouseButton, m_MouseDatas[i]);
		}
	}
	else if (operation == GLFW_RELEASE)
	{
		for (usize i = 0; i < m_MouseCallbacks.size(); i++)
			m_MouseCallbacks[i](m_MousePosition, 0.0f, MouseOperation::Release, m_LastMouseButton, m_MouseDatas[i]);

		for (usize i = 0; i < m_MouseCallbacks.size(); i++)
			m_MouseCallbacks[i](m_MousePosition, 0.0f, MouseOperation::Click, m_LastMouseButton, m_MouseDatas[i]);
	}
}

void InputHandler::FireMouseMove(glm::vec<2, i32> position)
{
	std::unique_lock lock(m_MouseMutex);
	m_MousePosition = position;

	for (usize i = 0; i < m_MouseCallbacks.size(); i++)
		m_MouseCallbacks[i](position, 0.0f, MouseOperation::Move, m_LastMouseButton, m_MouseDatas[i]);

	for (u8 i = 0; i < m_BullishMouseCallbacks.size(); i++)
	{
		for (usize a = 0; a < m_BullishMouseCallbacks[static_cast<usize>(i)].size(); a++)
		{
			if (m_BullishMouseCallbacks[i][a](position, 0.0f, MouseOperation::Move, m_LastMouseButton, m_BullishMouseDatas[i][a]))
				return;
		}
	}
}

void InputHandler::FireMouseScroll(f32 scroll)
{
	std::unique_lock lock(m_MouseMutex);

	MouseOperation op = MouseOperation::ScrollVertical;

	if (m_ShiftDown)
		op = MouseOperation::ScrollHorizontal;

	for (usize i = 0; i < m_MouseCallbacks.size(); i++)
		m_MouseCallbacks[i](m_MousePosition, scroll, op, m_LastMouseButton, m_MouseDatas[i]);
}

glm::vec<2, i32> InputHandler::GetMousePosition() { return m_MousePosition; }

u8 InputHandler::BullishLayerPopups		= 10;
u8 InputHandler::BullishLayerUI			= 15;
u8 InputHandler::BullishLayerArguments	= 20;
u8 InputHandler::BullishLayerBackground = 25;

bool InputHandler::m_MouseWasDown;
bool InputHandler::m_ShiftDown;
bool InputHandler::m_ControlDown;
bool InputHandler::m_AltDown;

glm::vec<2, i32> InputHandler::m_MousePosition;
std::shared_mutex InputHandler::m_KeyTextMutex;
std::shared_mutex InputHandler::m_MouseMutex;

std::vector<void (*)(InputHandler::key_state state, void* data)> InputHandler::m_KeyCallbacks;
std::vector<void*> InputHandler::m_KeyDatas;
std::vector<void (*)(InputHandler::key_state state, void* data)> InputHandler::m_TextCallbacks;
std::vector<void*> InputHandler::m_TextDatas;
std::vector<void (*)(glm::vec<2, i32> position, f32 scroll, MouseOperation operation, MouseButton button, void* data)> InputHandler::m_MouseCallbacks;
std::vector<void*> InputHandler::m_MouseDatas;
std::vector<std::vector<bool (*)(glm::vec<2, i32> position, f32 scroll, MouseOperation operation, MouseButton button, void* data)>> InputHandler::m_BullishMouseCallbacks;
std::vector<std::vector<void*>> InputHandler::m_BullishMouseDatas;
std::vector<TIME_POINT> InputHandler::m_PressLog;
MouseButton InputHandler::m_LastMouseButton = MouseButton::LEFT;