#include "input.hpp"

void mac::input::callback::framebuffer_resize(glfw::window* glfw_window, i32 width, i32 height)
{
	auto window = reinterpret_cast<mac::input::state*>(glfw::get_window_user_pointer(glfw_window));
	// TODO: swapchain recreation flag
	//window->flag_swapchain_recreation({width, height});
}

void mac::input::callback::key(glfw::window* glfw_window, i32 key, i32 scancode, i32 operation, i32 mode)
{
	auto input_state = reinterpret_cast<mac::input::state*>(glfw::get_window_user_pointer(glfw_window));

	switch (operation)
	{
	case GLFW_PRESS:
	{
		switch (key)
		{
		case GLFW_KEY_LEFT_SHIFT:
			input_state->shift_down = true;
			break;
		case GLFW_KEY_LEFT_CONTROL:
			input_state->control_down = true;
			break;
		case GLFW_KEY_LEFT_ALT:
			input_state->alt_down = true;
			break;
		default:
			break;
		}

		auto local_key_state = key_state {
			.key = static_cast<i16>(key),
			.down = true,
			.shift = input_state->shift_down,
			.ctrl = input_state->control_down,
			.alt = input_state->alt_down,
		};

		for (const auto& callback : input_state->key_callbacks)
			callback(local_key_state);

		for (const auto& callback : input_state->text_callbacks)
			callback(local_key_state);

		break;
	}
	case GLFW_RELEASE:
	{
		switch (key)
		{
		case GLFW_KEY_LEFT_SHIFT:
			input_state->shift_down = false;
			break;
		case GLFW_KEY_LEFT_CONTROL:
			input_state->control_down = false;
			break;
		case GLFW_KEY_LEFT_ALT:
			input_state->alt_down = false;
			break;
		default:
			break;
		}

		auto local_key_state = key_state {
			.key = static_cast<i16>(key),
			.down = false,
			.shift = input_state->shift_down,
			.ctrl = input_state->control_down,
			.alt = input_state->alt_down,
		};

		for (const auto& callback : input_state->key_callbacks)
			callback(local_key_state);

		break;
	}
	case GLFW_REPEAT:
	{
		auto local_key_state = key_state {
			.key = static_cast<i16>(key),
			.down = true,
			.shift = input_state->shift_down,
			.ctrl = input_state->control_down,
			.alt = input_state->alt_down,
		};

		for (const auto& callback : input_state->text_callbacks)
			callback(local_key_state);

		break;
	}
	default:
		break;
	}
}

void mac::input::callback::mouse_button(glfw::window* glfw_window, i32 button, i32 operation, i32 mods)
{
	auto input_state = reinterpret_cast<mac::input::state*>(glfw::get_window_user_pointer(glfw_window));
	auto mouse_button = mac::input::mouse_button::LEFT;
	auto mouse_operation = mac::input::mouse_operation::PRESS;

	switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
	{
		mouse_button = mac::input::mouse_button::LEFT;
		break;
	}
	case GLFW_MOUSE_BUTTON_RIGHT:
	{
		mouse_button = mac::input::mouse_button::RIGHT;
		break;
	}
	case GLFW_MOUSE_BUTTON_MIDDLE:
	{
		mouse_button = mac::input::mouse_button::MIDDLE;
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

		if ((!input_state->press_log.empty()
			&& std::chrono::duration_cast<std::chrono::milliseconds>(now - input_state->press_log.back()).count() > 300)
			|| input_state->press_log.size() >= 3)
			input_state->press_log.clear();

		input_state->press_log.emplace_back(now);

		switch (input_state->press_log.size())
		{
		case 1:
			mouse_operation = mac::input::mouse_operation::PRESS;
			break;
		case 2:
			mouse_operation = mac::input::mouse_operation::DOUBLE_PRESS;
			break;
		case 3:
			mouse_operation = mac::input::mouse_operation::TRIPLE_PRESS;
			break;
		default:
			break;
		}

		auto local_mouse_state = mouse_state {
			.position = input_state->mouse_position,
			.scroll = 0.0f,
			.operation = mouse_operation,
			.button = mouse_button
		};

		for (const auto& callback : input_state->mouse_callbacks)
		{
			if (callback(local_mouse_state) == mac::input::capture::BLOCK)
				break;
		}

		break;
	}
	case GLFW_RELEASE:
	{
		auto local_mouse_state = mouse_state {
			.position = input_state->mouse_position,
			.scroll = 0.0f,
			.operation = mac::input::mouse_operation::RELEASE,
			.button = mouse_button
		};

		for (const auto& callback : input_state->mouse_callbacks)
		{
			if (callback(local_mouse_state) == mac::input::capture::BLOCK)
				break;
		}

		local_mouse_state.operation = mac::input::mouse_operation::CLICK;

		for (const auto& callback : input_state->mouse_callbacks)
		{
			if (callback(local_mouse_state) == mac::input::capture::BLOCK)
				break;
		}

		break;
	}
	default:
		break;
	}
}

void mac::input::callback::scroll(glfw::window* glfw_window, f64 x_offset, f64 y_offset)
{
	auto input_state = reinterpret_cast<mac::input::state*>(glfw::get_window_user_pointer(glfw_window));
	auto local_mouse_state = mouse_state {
		.position = input_state->mouse_position,
		.scroll = static_cast<f32>(y_offset),
		.operation = mac::input::mouse_operation::SCROLL_VERTICAL,
		.button = mac::input::mouse_button::MIDDLE,
	};

	local_mouse_state.operation = input_state->shift_down ?
								  mac::input::mouse_operation::SCROLL_HORIZONTAL :
								  mac::input::mouse_operation::SCROLL_VERTICAL;

	for (const auto& callback : input_state->mouse_callbacks)
	{
		if (callback(local_mouse_state) == mac::input::capture::BLOCK)
			break;
	}
}

void mac::input::callback::cursor_position(glfw::window* glfw_window, f64 x_position, f64 y_position)
{
	auto input_state = reinterpret_cast<mac::input::state*>(glfw::get_window_user_pointer(glfw_window));
	input_state->mouse_position = { x_position, y_position };

	auto local_mouse_state = mouse_state {
		.position = input_state->mouse_position,
		.scroll = 0.0f,
		.operation = mac::input::mouse_operation::MOVE,
		.button = mac::input::mouse_button::MIDDLE,
	};

	for (const auto& callback : input_state->mouse_callbacks)
	{
		if (callback(local_mouse_state) == mac::input::capture::BLOCK)
			break;
	}
}

void mac::input::callback::window_position(glfw::window* glfw_window, i32 x, i32 y)
{
	auto input_state = reinterpret_cast<mac::input::state*>(glfw::get_window_user_pointer(glfw_window));
	input_state->screen_position = { x, y };
}

void mac::input::callback::window_focus_callback(glfw::window* glfw_window, i32 focused)
{
	auto input_state = reinterpret_cast<mac::input::state*>(glfw::get_window_user_pointer(glfw_window));
	input_state->focused = focused;
}

mac::input::state* mac::input::create()
{
	return new mac::input::state {
		.key_callback_idx_counter = 0,
		.text_callback_idx_counter = 0,
		.mouse_callback_idx_counter = 0,
		.shift_down = false,
		.control_down = false,
		.alt_down = false,
		.mouse_position = {},
		.screen_position = {},
		.focused = true,
	};
}

void destroy(mac::input::state* input_state)
{
	delete input_state;
}

u64 mac::input::register_key_callback(mac::input::state* input_state, std::function<mac::input::callback_key> key_callback)
{
	input_state->key_callbacks.emplace_back(std::move(key_callback));
	return input_state->key_callback_idx.emplace_back(input_state->key_callback_idx_counter++);
}

u64 mac::input::register_text_callback(mac::input::state* input_state, std::function<mac::input::callback_text> text_callback)
{
	input_state->text_callbacks.emplace_back(std::move(text_callback));
	return input_state->text_callback_idx.emplace_back(input_state->text_callback_idx_counter++);
}

u64 mac::input::register_mouse_callback(
	mac::input::state* input_state, i64 layer, std::function<mac::input::callback_mouse> mouse_callback
)
{
	i64 insert_index = 0;

	for (i64 i = 0; i < input_state->mouse_callback_layers.size(); i++)
	{
		if (input_state->mouse_callback_layers[i] > layer)
		{
			insert_index = i;
			break;
		}
	}

	u64 use_idx = input_state->mouse_callback_idx_counter++;

	input_state->mouse_callbacks.insert(input_state->mouse_callbacks.begin() + insert_index, std::move(mouse_callback));
	input_state->mouse_callback_layers.insert(input_state->mouse_callback_layers.begin() + insert_index, layer);
	input_state->mouse_callback_idx.insert(input_state->mouse_callback_idx.begin() + insert_index, use_idx);

	return use_idx;
}

void mac::input::unregister_key_callback(mac::input::state* input_state, u64 key_callback_idx)
{
	auto idx = check_key_callback(input_state, key_callback_idx);

	if (!idx.has_value())
	{
		latte::logger::error(latte::logger::level::SYSTEM, __FILE__, __LINE__, "failed to unregister callback");
		return;
	}

	input_state->key_callbacks.erase(input_state->key_callbacks.begin() + idx.value());
	input_state->key_callback_idx.erase(input_state->key_callback_idx.begin() + idx.value());
}

void mac::input::unregister_text_callback(mac::input::state* input_state, u64 text_callback_idx)
{
	auto idx = check_key_callback(input_state, text_callback_idx);

	if (!idx.has_value())
	{
		latte::logger::error(latte::logger::level::SYSTEM, __FILE__, __LINE__, "failed to unregister callback");
		return;
	}

	input_state->text_callbacks.erase(input_state->text_callbacks.begin() + idx.value());
	input_state->text_callback_idx.erase(input_state->text_callback_idx.begin() + idx.value());
}

void mac::input::unregister_mouse_callback(mac::input::state* input_state, u64 mouse_callback_idx)
{
	auto idx = check_key_callback(input_state, mouse_callback_idx);

	if (!idx.has_value())
	{
		latte::logger::error(latte::logger::level::SYSTEM, __FILE__, __LINE__, "failed to unregister callback");
		return;
	}

	input_state->mouse_callbacks.erase(input_state->mouse_callbacks.begin() + idx.value());
	input_state->mouse_callback_layers.erase(input_state->mouse_callback_layers.begin() + idx.value());
	input_state->mouse_callback_idx.erase(input_state->mouse_callback_idx.begin() + idx.value());
}

std::optional<i64> mac::input::check_key_callback(mac::input::state* input_state, u64 key_callback_idx)
{
	for (i64 i = 0; i < input_state->key_callback_idx.size(); i++)
	{
		if (input_state->key_callback_idx[i] == key_callback_idx)
			return { i };
	}

	return { };
}

std::optional<i64> mac::input::check_text_callback(mac::input::state* input_state, u64 text_callback_idx)
{
	for (i64 i = 0; i < input_state->text_callback_idx.size(); i++)
	{
		if (input_state->text_callback_idx[i] == text_callback_idx)
			return { i };
	}

	return { };
}

std::optional<i64> mac::input::check_mouse_callback(mac::input::state* input_state, u64 mouse_callback_idx)
{
	for (i64 i = 0; i < input_state->mouse_callback_idx.size(); i++)
	{
		if (input_state->mouse_callback_idx[i] == mouse_callback_idx)
			return { i };
	}

	return { };
}