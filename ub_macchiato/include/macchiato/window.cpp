#include "window.hpp"

namespace mac::window::callback
{
void framebuffer_resize(glfw::window* glfw_window, i32 width, i32 height)
{
	auto window = reinterpret_cast<mac::window::state*>(glfw::get_window_user_pointer(glfw_window));
	// TODO: swapchain recreation flag
	//window->flag_swapchain_recreation({width, height});
}

void key(glfw::window* glfw_window, i32 key, i32 scancode, i32 operation, i32 mode)
{
	auto window_state = reinterpret_cast<mac::window::state*>(glfw::get_window_user_pointer(glfw_window));

	switch (operation)
	{
	case GLFW_PRESS:
	{
		switch (key)
		{
		case GLFW_KEY_LEFT_SHIFT:
			window_state->shift_down = true;
			break;
		case GLFW_KEY_LEFT_CONTROL:
			window_state->control_down = true;
			break;
		case GLFW_KEY_LEFT_ALT:
			window_state->alt_down = true;
			break;
		default:
			break;
		}

		auto local_key_state = key_state {
			.key = static_cast<i16>(key),
			.down = true,
			.shift = window_state->shift_down,
			.ctrl = window_state->control_down,
			.alt = window_state->alt_down,
		};

		for (const auto& callback : window_state->key_callbacks)
			callback(local_key_state);

		for (const auto& callback : window_state->text_callbacks)
			callback(local_key_state);

		break;
	}
	case GLFW_RELEASE:
	{
		switch (key)
		{
		case GLFW_KEY_LEFT_SHIFT:
			window_state->shift_down = false;
			break;
		case GLFW_KEY_LEFT_CONTROL:
			window_state->control_down = false;
			break;
		case GLFW_KEY_LEFT_ALT:
			window_state->alt_down = false;
			break;
		default:
			break;
		}

		auto local_key_state = key_state {
			.key = static_cast<i16>(key),
			.down = false,
			.shift = window_state->shift_down,
			.ctrl = window_state->control_down,
			.alt = window_state->alt_down,
		};

		for (const auto& callback : window_state->key_callbacks)
			callback(local_key_state);

		break;
	}
	case GLFW_REPEAT:
	{
		auto local_key_state = key_state {
			.key = static_cast<i16>(key),
			.down = true,
			.shift = window_state->shift_down,
			.ctrl = window_state->control_down,
			.alt = window_state->alt_down,
		};

		for (const auto& callback : window_state->text_callbacks)
			callback(local_key_state);

		break;
	}
	default:
		break;
	}
}

void mouse_button(glfw::window* glfw_window, i32 button, i32 operation, i32 mods)
{
	auto window_state = reinterpret_cast<mac::window::state*>(glfw::get_window_user_pointer(glfw_window));
	auto mouse_button = mac::window::mouse_button::LEFT;
	auto mouse_operation = mac::window::mouse_operation::PRESS;

	switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
	{
		mouse_button = mac::window::mouse_button::LEFT;
		break;
	}
	case GLFW_MOUSE_BUTTON_RIGHT:
	{
		mouse_button = mac::window::mouse_button::RIGHT;
		break;
	}
	case GLFW_MOUSE_BUTTON_MIDDLE:
	{
		mouse_button = mac::window::mouse_button::MIDDLE;
		break;
	}
	default:
		break;
	};

	switch (operation)
	{
	case GLFW_PRESS:
	{
		TIME_POINT now = TIME_POINT_NOW;

		if ((!window_state->press_log.empty()
			&& std::chrono::duration_cast<std::chrono::milliseconds>(now - window_state->press_log.back()).count() > 300)
			|| window_state->press_log.size() >= 3)
			window_state->press_log.clear();

		window_state->press_log.emplace_back(now);

		switch (window_state->press_log.size())
		{
		case 1:
			mouse_operation = mac::window::mouse_operation::PRESS;
			break;
		case 2:
			mouse_operation = mac::window::mouse_operation::DOUBLE_PRESS;
			break;
		case 3:
			mouse_operation = mac::window::mouse_operation::TRIPLE_PRESS;
			break;
		default:
			break;
		}

		auto local_mouse_state = mouse_state {
			.position = window_state->mouse_position,
			.scroll = 0.0f,
			.operation = mouse_operation,
			.button = mouse_button
		};

		for (const auto& callback : window_state->mouse_callbacks)
		{
			if (callback(local_mouse_state) == mac::window::capture::BLOCK)
				break;
		}

		break;
	}
	case GLFW_RELEASE:
	{
		auto local_mouse_state = mouse_state {
			.position = window_state->mouse_position,
			.scroll = 0.0f,
			.operation = mac::window::mouse_operation::RELEASE,
			.button = mouse_button
		};

		for (const auto& callback : window_state->mouse_callbacks)
		{
			if (callback(local_mouse_state) == mac::window::capture::BLOCK)
				break;
		}

		local_mouse_state.operation = mac::window::mouse_operation::CLICK;

		for (const auto& callback : window_state->mouse_callbacks)
		{
			if (callback(local_mouse_state) == mac::window::capture::BLOCK)
				break;
		}

		break;
	}
	default:
		break;
	}
}

void scroll(glfw::window* glfw_window, f64 x_offset, f64 y_offset)
{
	auto window_state = reinterpret_cast<mac::window::state*>(glfw::get_window_user_pointer(glfw_window));
	auto local_mouse_state = mouse_state {
		.position = window_state->mouse_position,
		.scroll = static_cast<f32>(y_offset),
		.operation = mac::window::mouse_operation::SCROLL_VERTICAL,
		.button = mac::window::mouse_button::MIDDLE,
	};

	local_mouse_state.operation = window_state->shift_down ?
		mac::window::mouse_operation::SCROLL_HORIZONTAL :
		mac::window::mouse_operation::SCROLL_VERTICAL;

	for (const auto& callback : window_state->mouse_callbacks)
	{
		if (callback(local_mouse_state) == mac::window::capture::BLOCK)
			break;
	}
}

void cursor_position(glfw::window* glfw_window, f64 x_position, f64 y_position)
{
	auto window_state = reinterpret_cast<mac::window::state*>(glfw::get_window_user_pointer(glfw_window));
	window_state->mouse_position = { x_position, y_position };

	auto local_mouse_state = mouse_state {
		.position = window_state->mouse_position,
		.scroll = 0.0f,
		.operation = mac::window::mouse_operation::MOVE,
		.button = mac::window::mouse_button::MIDDLE,
	};

	for (const auto& callback : window_state->mouse_callbacks)
	{
		if (callback(local_mouse_state) == mac::window::capture::BLOCK)
			break;
	}
}

void window_position(glfw::window* glfw_window, i32 x, i32 y)
{
	auto window_state = reinterpret_cast<mac::window::state*>(glfw::get_window_user_pointer(glfw_window));
	window_state->screen_position = { x, y };
}
}

void mac::window::initialize()
{
	glfwInit();
}

mac::window::state* mac::window::create(const std::string& title, glm::vec<2, i32> size)
{
	auto* window_state = new mac::window::state {
		.window = nullptr,
		.device = nullptr,
	};

	glfw::set_window_hint(GLFW_CLIENT_API, GLFW_NO_API);
	glfw::set_window_hint(GLFW_RESIZABLE, GLFW_TRUE);
	window_state->window = glfw::create_window(size.x, size.y, title.c_str(), nullptr, nullptr);

	if (window_state->window == nullptr)
	{
		latte::logger::fatal(latte::logger::level::SYSTEM, "failed to create window");
		return nullptr;
	}

	window_state->device = mac::device::create(true, title);

	if (glfw::create_window_surface(window_state->device->instance, window_state->window, nullptr, &window_state->surface) != VK_SUCCESS)
	{
		latte::logger::fatal(latte::logger::level::SYSTEM, "failed to create window surface");
		return nullptr;
	}

	glfw::set_framebuffer_size_callback(window_state->window, mac::window::callback::framebuffer_resize);
	glfw::set_key_callback(window_state->window, mac::window::callback::key);
	glfw::set_mouse_button_callback(window_state->window, mac::window::callback::mouse_button);
	glfw::set_scroll_callback(window_state->window, mac::window::callback::scroll);
	glfw::set_cursor_position_callback(window_state->window, mac::window::callback::cursor_position);
	glfw::set_window_position_callback(window_state->window, mac::window::callback::window_position);

	glfw::set_window_user_pointer(window_state->window, reinterpret_cast<void*>(window_state));
	glfw::get_window_position(window_state->window, &window_state->screen_position.x, &window_state->screen_position.y);

	return window_state;
}

void mac::window::destroy(state* window_state)
{
	delete window_state;
}

void mac::window::setup_rendering(mac::window::state* window_state)
{

}

u64 mac::window::register_key_callback(mac::window::state* window_state, std::function<mac::window::callback_key> key_callback)
{
	window_state->key_callbacks.emplace_back(std::move(key_callback));
	return window_state->key_callback_idx.emplace_back(window_state->key_callback_idx_counter++);
}

u64 mac::window::register_text_callback(mac::window::state* window_state, std::function<mac::window::callback_text> text_callback)
{
	window_state->text_callbacks.emplace_back(std::move(text_callback));
	return window_state->text_callback_idx.emplace_back(window_state->text_callback_idx_counter++);
}

u64 mac::window::register_mouse_callback(
	mac::window::state* window_state, i64 layer, std::function<mac::window::callback_mouse> mouse_callback
)
{
	i64 insert_index = 0;

	for (i64 i = 0; i < window_state->mouse_callback_layers.size(); i++)
	{
		if (window_state->mouse_callback_layers[i] > layer)
		{
			insert_index = i;
			break;
		}
	}

	u64 use_idx = window_state->mouse_callback_idx_counter++;

	window_state->mouse_callbacks.insert(window_state->mouse_callbacks.begin() + insert_index, std::move(mouse_callback));
	window_state->mouse_callback_layers.insert(window_state->mouse_callback_layers.begin() + insert_index, layer);
	window_state->mouse_callback_idx.insert(window_state->mouse_callback_idx.begin() + insert_index, use_idx);

	return use_idx;
}

void mac::window::unregister_key_callback(mac::window::state* window_state, u64 key_callback_idx)
{
	auto idx = check_key_callback(window_state, key_callback_idx);

	if (!idx.has_value())
	{
		latte::logger::error(latte::logger::level::SYSTEM, __FILE__, __LINE__, "failed to unregister callback");
		return;
	}

	window_state->key_callbacks.erase(window_state->key_callbacks.begin() + idx.value());
	window_state->key_callback_idx.erase(window_state->key_callback_idx.begin() + idx.value());
}

void mac::window::unregister_text_callback(mac::window::state* window_state, u64 text_callback_idx)
{
	auto idx = check_key_callback(window_state, text_callback_idx);

	if (!idx.has_value())
	{
		latte::logger::error(latte::logger::level::SYSTEM, __FILE__, __LINE__, "failed to unregister callback");
		return;
	}

	window_state->text_callbacks.erase(window_state->text_callbacks.begin() + idx.value());
	window_state->text_callback_idx.erase(window_state->text_callback_idx.begin() + idx.value());
}

void mac::window::unregister_mouse_callback(mac::window::state* window_state, u64 mouse_callback_idx)
{
	auto idx = check_key_callback(window_state, mouse_callback_idx);

	if (!idx.has_value())
	{
		latte::logger::error(latte::logger::level::SYSTEM, __FILE__, __LINE__, "failed to unregister callback");
		return;
	}

	window_state->mouse_callbacks.erase(window_state->mouse_callbacks.begin() + idx.value());
	window_state->mouse_callback_layers.erase(window_state->mouse_callback_layers.begin() + idx.value());
	window_state->mouse_callback_idx.erase(window_state->mouse_callback_idx.begin() + idx.value());
}

std::optional<i64> mac::window::check_key_callback(mac::window::state* window_state, u64 key_callback_idx)
{
	for (i64 i = 0; i < window_state->key_callback_idx.size(); i++)
	{
		if (window_state->key_callback_idx[i] == key_callback_idx)
			return { i };
	}

	return { };
}

std::optional<i64> mac::window::check_text_callback(mac::window::state* window_state, u64 text_callback_idx)
{
	for (i64 i = 0; i < window_state->text_callback_idx.size(); i++)
	{
		if (window_state->text_callback_idx[i] == text_callback_idx)
			return { i };
	}

	return { };
}

std::optional<i64> mac::window::check_mouse_callback(mac::window::state* window_state, u64 mouse_callback_idx)
{
	for (i64 i = 0; i < window_state->mouse_callback_idx.size(); i++)
	{
		if (window_state->mouse_callback_idx[i] == mouse_callback_idx)
			return { i };
	}

	return { };
}
