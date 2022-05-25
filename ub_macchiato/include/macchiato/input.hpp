#pragma once
#include "config.h"

#include <latte/utils.hpp>

namespace mac::input
{
// Lower layers get invoked first, then it goes up.
static i64 layer_text  = 0;
static i64 layer_block = 1000000;
static i64 layer_collection = 2000000;
static i64 layer_plane = 3000000;

///
enum class mouse_operation
{
	HOVER_ON,
	HOVER_OFF,
	CLICK,
	PRESS,
	DOUBLE_PRESS,
	TRIPLE_PRESS,
	RELEASE,
	MOVE,
	SCROLL_VERTICAL,
	SCROLL_HORIZONTAL
};

///
enum class mouse_button
{
	LEFT,
	MIDDLE,
	RIGHT
};

///
struct key_state
{
	i16 key;

	bool down;
	bool shift;
	bool ctrl;
	bool alt;
};

///
struct mouse_state
{
	glm::vec<2, i32> position;
	f32 scroll;
	mac::input::mouse_operation operation;
	mac::input::mouse_button button;
};

///
enum class capture
{
	BLOCK, PASS
};

using callback_key = void(const mac::input::key_state&);
using callback_text = void(const mac::input::key_state&);
using callback_mouse = mac::input::capture(const mac::input::mouse_state&);

///
struct state
{
	///
	std::vector<std::function<callback_key>> key_callbacks;

	///
	std::vector<u64> key_callback_idx;

	///
	u64 key_callback_idx_counter;

	///
	std::vector<std::function<callback_text>> text_callbacks;

	///
	std::vector<u64> text_callback_idx;

	///
	u64 text_callback_idx_counter;

	///
	std::vector<std::function<callback_mouse>> mouse_callbacks;

	///
	std::vector<i64> mouse_callback_layers;

	///
	std::vector<u64> mouse_callback_idx;

	///
	u64 mouse_callback_idx_counter;

	///
	bool shift_down;

	///
	bool control_down;

	///
	bool alt_down;

	///
	std::vector<TIME_POINT> press_log;

	///
	glm::vec<2, i32> mouse_position;

	///
	glm::vec<2, i32> screen_position;

	///
	bool focused;

	///
	std::atomic_bool framebuffer_resize;

	///
	std::atomic_bool size_zero;
};

namespace callback
{
///
void framebuffer_resize(glfw::window* glfw_window, i32 width, i32 height);

///
void key(glfw::window* glfw_window, i32 key, i32 scancode, i32 operation, i32 mode);

///
void mouse_button(glfw::window* glfw_window, i32 button, i32 operation, i32 mods);

///
void scroll(glfw::window* glfw_window, f64 x_offset, f64 y_offset);

///
void cursor_position(glfw::window* glfw_window, f64 x_position, f64 y_position);

///
void window_position(glfw::window* glfw_window, i32 x, i32 y);

///
void window_focus_callback(glfw::window* glfw_window, i32 focused);
}

///
mac::input::state* create();

///
void destroy(mac::input::state* input_state);

///
u64 register_key_callback(mac::input::state* input_state, std::function<mac::input::callback_key> key_callback);

///
u64 register_text_callback(mac::input::state* input_state, std::function<mac::input::callback_text> text_callback);

///
u64 register_mouse_callback(mac::input::state* input_state, i64 layer, std::function<mac::input::callback_mouse> mouse_callback);

///
void unregister_key_callback(mac::input::state* input_state, u64 key_callback_idx);

///
void unregister_text_callback(mac::input::state* input_state, u64 text_callback_idx);

///
void unregister_mouse_callback(mac::input::state* input_state, u64 mouse_callback_idx);

///
std::optional<i64> check_key_callback(mac::input::state* input_state, u64 key_callback_idx);

///
std::optional<i64> check_text_callback(mac::input::state* input_state, u64 text_callback_idx);

///
std::optional<i64> check_mouse_callback(mac::input::state* input_state, u64 mouse_callback_idx);
}