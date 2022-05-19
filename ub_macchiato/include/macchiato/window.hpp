#pragma once
#include "config.h"

#include "device.hpp"

#include <latte/utils.hpp>

namespace mac::window
{
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
	mac::window::mouse_operation operation;
	mac::window::mouse_button button;
};

///
enum class capture
{
	BLOCK, PASS
};

using callback_key = void(const mac::window::key_state&);
using callback_text = void(const mac::window::key_state&);
using callback_mouse = mac::window::capture(const mac::window::mouse_state&);

///
struct state
{
	///
	glfw::window* window;

	///
	mac::device::state* device;

	///
	vk::surface_khr surface;

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
};

///
void initialize();

///
mac::window::state* create(const std::string& title, glm::vec<2, i32> size);

///
void destroy(mac::window::state* window_state);

///
void setup_rendering(mac::window::state* window_state);

///
u64 register_key_callback(mac::window::state* window_state, std::function<mac::window::callback_key> key_callback);

///
u64 register_text_callback(mac::window::state* window_state, std::function<mac::window::callback_text> text_callback);

///
u64 register_mouse_callback(mac::window::state* window_state, i64 layer, std::function<mac::window::callback_mouse> mouse_callback);

///
void unregister_key_callback(mac::window::state* window_state, u64 key_callback_idx);

///
void unregister_text_callback(mac::window::state* window_state, u64 text_callback_idx);

///
void unregister_mouse_callback(mac::window::state* window_state, u64 mouse_callback_idx);

///
std::optional<i64> check_key_callback(mac::window::state* window_state, u64 key_callback_idx);

///
std::optional<i64> check_text_callback(mac::window::state* window_state, u64 text_callback_idx);

///
std::optional<i64> check_mouse_callback(mac::window::state* window_state, u64 mouse_callback_idx);

// Lower layers get invoked first, then it goes up.
i64 layer_text  = 0;
i64 layer_block = 1000000;
i64 layer_collection = 2000000;
i64 layer_plane = 3000000;
}
