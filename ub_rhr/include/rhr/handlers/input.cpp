#include "input.hpp"

#include "rhr/handlers/context.hpp"
#include "rhr/rendering/renderer.hpp"

void rhr::handler::input::initialization()
{
	m_mouse_was_down = false;
	m_shift_down	 = false;

	m_mouse_position = {0, 0};
}

void rhr::handler::input::register_key_callback(void (*callback)(key_state state, void* data), void* data)
{
	std::unique_lock lock(m_key_text_mutex);

	m_key_callbacks.push_back(callback);
	m_key_datas.push_back(data);
}

void rhr::handler::input::unregister_key_callback(void (*callback)(key_state state, void* data))
{
	std::unique_lock lock(m_key_text_mutex);

	for (usize i = 0; i < m_key_callbacks.size(); i++)
	{
		if (m_key_callbacks[i] == callback)
		{
			m_key_callbacks.erase(m_key_callbacks.begin() + i);
			m_key_datas.erase(m_key_datas.begin() + i);
			return;
		}
	}

	cap::logger::error(cap::logger::level::SYSTEM, "failed to unregister key callback");
}

void rhr::handler::input::register_text_callback(void (*callback)(key_state state, void* data), void* data)
{
	std::unique_lock lock(m_key_text_mutex);

	m_text_callbacks.push_back(callback);
	m_text_datas.push_back(data);
}

void rhr::handler::input::unregister_text_callback(void (*callback)(key_state state, void* data))
{
	std::unique_lock lock(m_key_text_mutex);

	for (usize i = 0; i < m_text_callbacks.size(); i++)
	{
		if (m_text_callbacks[i] == callback)
		{
			m_text_callbacks.erase(m_text_callbacks.begin() + i);
			m_text_datas.erase(m_text_datas.begin() + i);
			return;
		}
	}

	cap::logger::error(cap::logger::level::SYSTEM, "failed to unregister text callback");
}

void rhr::handler::input::register_mouse_callback(
	void (*callback)(glm::vec<2, i32> position, f32 scroll, input::mouse_operation operation, input::mouse_button button, void* data),
	void* data)
{
	std::unique_lock lock(m_mouse_mutex);

	m_mouse_callbacks.push_back(callback);
	m_mouse_datas.push_back(data);
}

void rhr::handler::input::unregister_mouse_callback(
	void (*callback)(glm::vec<2, i32> position, f32 scroll, input::mouse_operation operation, input::mouse_button button, void* data))
{
	std::unique_lock lock(m_mouse_mutex);

	for (usize i = 0; i < m_mouse_callbacks.size(); i++)
	{
		if (m_mouse_callbacks[i] == callback)
		{
			m_mouse_callbacks.erase(m_mouse_callbacks.begin() + i);
			m_mouse_datas.erase(m_mouse_datas.begin() + i);
			return;
		}
	}

	cap::logger::error(cap::logger::level::SYSTEM, "failed to unregister mouse callback");
}

void rhr::handler::input::register_bullish_mouse_callback(
	bool (*callback)(glm::vec<2, i32> position, f32 scroll, input::mouse_operation operation, input::mouse_button button, void* data),
	u8 layer,
	void* data)
{
	std::unique_lock lock(m_mouse_mutex);

	if (layer >= m_bullish_mouse_callbacks.size())
	{
		for (u8 i = 0; i < layer - m_bullish_mouse_callbacks.size() + 1; i++)
		{
			m_bullish_mouse_callbacks.push_back(std::vector<bool (*)(
												  glm::vec<2, i32> position,
												  f32 scroll,
												  input::mouse_operation operation,
												  input::mouse_button button,
												  void* data)>());
			m_bullish_mouse_datas.push_back(std::vector<void*>());
		}
	}

	m_bullish_mouse_callbacks[layer].push_back(callback);
	m_bullish_mouse_datas[layer].push_back(data);
}

void rhr::handler::input::unregister_bullish_mouse_callback(
	bool (*callback)(glm::vec<2, i32> position, f32 scroll, input::mouse_operation operation, input::mouse_button button, void* data),
	u8 layer)
{
	std::unique_lock lock(m_mouse_mutex);

	for (usize i = 0; i < m_bullish_mouse_callbacks[layer].size(); i++)
	{
		if (m_bullish_mouse_callbacks[layer][i] == callback)
		{
			m_bullish_mouse_callbacks[layer].erase(m_bullish_mouse_callbacks[layer].begin() + i);
			m_bullish_mouse_datas[layer].erase(m_bullish_mouse_datas[layer].begin() + i);
			return;
		}
	}

	cap::logger::error(cap::logger::level::SYSTEM, "failed to unregister bullish mouse callback");
}

void rhr::handler::input::fire_key(i16 key, u8 operation)
{
	std::unique_lock lock(m_key_text_mutex);

	if (key == GLFW_KEY_LEFT_SHIFT)
	{
		if (operation == GLFW_PRESS)
			m_shift_down = true;
		else if (operation == GLFW_RELEASE)
			m_shift_down = false;
	}

	if (key == GLFW_KEY_LEFT_CONTROL)
	{
		if (operation == GLFW_PRESS)
			m_control_down = true;
		else if (operation == GLFW_RELEASE)
			m_control_down = false;
	}

	if (key == GLFW_KEY_LEFT_ALT)
	{
		if (operation == GLFW_PRESS)
			m_alt_down = true;
		else if (operation == GLFW_RELEASE)
			m_alt_down = false;
	}

	if (operation == GLFW_PRESS)
	{
		for (usize i = 0; i < m_key_callbacks.size(); i++)
			m_key_callbacks[i]({key, true, m_shift_down, m_control_down, m_alt_down}, m_key_datas[i]);

		for (usize i = 0; i < m_text_callbacks.size(); i++)
			m_text_callbacks[i]({key, true, m_shift_down, m_control_down, m_alt_down}, m_text_datas[i]);
	}
	else if (operation == GLFW_RELEASE)
	{
		for (usize i = 0; i < m_key_callbacks.size(); i++)
			m_key_callbacks[i]({key, false, m_shift_down, m_control_down, m_alt_down}, m_key_datas[i]);
	}
	else if (operation == GLFW_REPEAT)
	{
		for (usize i = 0; i < m_text_callbacks.size(); i++)
			m_text_callbacks[i]({key, true, m_shift_down, m_control_down, m_alt_down}, m_text_datas[i]);
	}
}

void rhr::handler::input::fire_mouse_button(u8 button, u8 operation)
{
	m_last_mouse_button = input::mouse_button::LEFT;

	switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
	{
		m_last_mouse_button = input::mouse_button::LEFT;
		break;
	}
	case GLFW_MOUSE_BUTTON_RIGHT:
	{
		m_last_mouse_button = input::mouse_button::RIGHT;
		break;
	}
	case GLFW_MOUSE_BUTTON_MIDDLE:
	{
		m_last_mouse_button = input::mouse_button::MIDDLE;
		break;
	}
	};

	if (rhr::handler::context::is_open())
	{
		glm::vec<2, i32> context_position =
			rhr::handler::context::get_position() - rhr::render::renderer::get_window_primary()->get_window_position();
		const glm::vec<2, i32>& context_bounds = rhr::handler::context::get_bounds();

		if (m_mouse_position.x > context_position.x && m_mouse_position.x < context_position.x + context_bounds.x
			&& m_mouse_position.y > context_position.y && m_mouse_position.y < context_position.y + context_bounds.y)
		{
			return;
		}
	}

	std::unique_lock lock(m_mouse_mutex);

	if (operation == GLFW_PRESS)
	{
		TIME_POINT now = std::chrono::high_resolution_clock::now();

		if ((m_press_log.size() > 0
			 && std::chrono::duration_cast<std::chrono::milliseconds>(now - m_press_log.back()).count() > 300)
			|| m_press_log.size() >= 3)
			m_press_log.clear();

		m_press_log.push_back(now);

		if (m_press_log.size() == 1)
		{
			for (usize i = 0; i < m_mouse_callbacks.size(); i++)
				m_mouse_callbacks[i](m_mouse_position, 0.0f, input::mouse_operation::PRESS, m_last_mouse_button, m_mouse_datas[i]);
		}
		else if (m_press_log.size() == 2)
		{
			for (usize i = 0; i < m_mouse_callbacks.size(); i++)
				m_mouse_callbacks[i](
					m_mouse_position, 0.0f, input::mouse_operation::DOUBLE_PRESS, m_last_mouse_button, m_mouse_datas[i]);
		}
		else if (m_press_log.size() == 3)
		{
			for (usize i = 0; i < m_mouse_callbacks.size(); i++)
				m_mouse_callbacks[i](
					m_mouse_position, 0.0f, input::mouse_operation::TRIPLE_PRESS, m_last_mouse_button, m_mouse_datas[i]);
		}
	}
	else if (operation == GLFW_RELEASE)
	{
		for (usize i = 0; i < m_mouse_callbacks.size(); i++)
			m_mouse_callbacks[i](m_mouse_position, 0.0f, input::mouse_operation::RELEASE, m_last_mouse_button, m_mouse_datas[i]);

		for (usize i = 0; i < m_mouse_callbacks.size(); i++)
			m_mouse_callbacks[i](m_mouse_position, 0.0f, input::mouse_operation::CLICK, m_last_mouse_button, m_mouse_datas[i]);
	}
}

void rhr::handler::input::fire_mouse_move(glm::vec<2, i32> position)
{
	std::unique_lock lock(m_mouse_mutex);
	m_mouse_position = position;

	for (usize i = 0; i < m_mouse_callbacks.size(); i++)
		m_mouse_callbacks[i](position, 0.0f, input::mouse_operation::MOVE, m_last_mouse_button, m_mouse_datas[i]);

	for (u8 i = 0; i < m_bullish_mouse_callbacks.size(); i++)
	{
		for (usize a = 0; a < m_bullish_mouse_callbacks[static_cast<usize>(i)].size(); a++)
		{
			if (m_bullish_mouse_callbacks[i][a](
					position, 0.0f, input::mouse_operation::MOVE, m_last_mouse_button, m_bullish_mouse_datas[i][a]))
				return;
		}
	}
}

void rhr::handler::input::fire_mouse_scroll(f32 scroll)
{
	std::unique_lock lock(m_mouse_mutex);

	input::mouse_operation op = input::mouse_operation::SCROLL_VERTICAL;

	if (m_shift_down)
		op = input::mouse_operation::SCROLL_HORIZONTAL;

	for (usize i = 0; i < m_mouse_callbacks.size(); i++)
		m_mouse_callbacks[i](m_mouse_position, scroll, op, m_last_mouse_button, m_mouse_datas[i]);
}

glm::vec<2, i32> rhr::handler::input::get_mouse_position()
{
	return m_mouse_position;
}

u8 rhr::handler::input::bullish_layer_popups		= 10;
u8 rhr::handler::input::bullish_layer_ui			= 15;
u8 rhr::handler::input::bullish_layer_arguments	= 20;
u8 rhr::handler::input::bullish_layer_background = 25;

bool rhr::handler::input::m_mouse_was_down;
bool rhr::handler::input::m_shift_down;
bool rhr::handler::input::m_control_down;
bool rhr::handler::input::m_alt_down;

glm::vec<2, i32> rhr::handler::input::m_mouse_position;
std::shared_mutex rhr::handler::input::m_key_text_mutex;
std::shared_mutex rhr::handler::input::m_mouse_mutex;

std::vector<void (*)(rhr::handler::input::key_state state, void* data)> rhr::handler::input::m_key_callbacks;
std::vector<void*> rhr::handler::input::m_key_datas;
std::vector<void (*)(rhr::handler::input::key_state state, void* data)> rhr::handler::input::m_text_callbacks;
std::vector<void*> rhr::handler::input::m_text_datas;
std::vector<void (*)(glm::vec<2, i32> position, f32 scroll, rhr::handler::input::mouse_operation operation, rhr::handler::input::mouse_button button, void* data)>
	rhr::handler::input::m_mouse_callbacks;
std::vector<void*> rhr::handler::input::m_mouse_datas;
std::vector<std::vector<bool (*)(
	glm::vec<2, i32> position, f32 scroll, rhr::handler::input::mouse_operation operation, rhr::handler::input::mouse_button button, void* data)>>
	rhr::handler::input::m_bullish_mouse_callbacks;
std::vector<std::vector<void*>> rhr::handler::input::m_bullish_mouse_datas;
std::vector<TIME_POINT> rhr::handler::input::m_press_log;
rhr::handler::input::mouse_button rhr::handler::input::m_last_mouse_button = input::mouse_button::LEFT;