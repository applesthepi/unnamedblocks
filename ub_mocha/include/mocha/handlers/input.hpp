#pragma once
#include "config.h"

#include <latte/utils.hpp>

namespace rhr::handler
{
///
class input
{
public:
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
	static void initialization();

	///
	static void register_key_callback(void (* callback)(key_state state, void* data), void* data);

	///
	static void unregister_key_callback(void (* callback)(key_state state, void* data));

	///
	static void register_text_callback(void (* callback)(key_state state, void* data), void* data);

	///
	static void unregister_text_callback(void (* callback)(key_state state, void* data));

	///
	static void register_mouse_callback(
		void (* callback)(
			glm::vec<2, i32> position, f32 scroll, mouse_operation operation, mouse_button button, void* data
		),
		void* data
	);

	///
	static void unregister_mouse_callback(
		void (* callback)(
			glm::vec<2, i32> position, f32 scroll, mouse_operation operation, mouse_button button, void* data
		));

	///
	static void register_bullish_mouse_callback(
		bool (* callback)(
			glm::vec<2, i32> position, f32 scroll, mouse_operation operation, mouse_button button, void* data
		),
		u8 layer,
		void* data
	);

	///
	static void unregister_bullish_mouse_callback(
		bool (* callback)(
			glm::vec<2, i32> position, f32 scroll, mouse_operation operation, mouse_button button, void* data
		),
		u8 layer
	);

	///
	static void fire_key(i16 key, u8 operation);

	///
	static void fire_mouse_button(u8 button, u8 operation);

	///
	static void fire_mouse_move(glm::vec<2, i32> position);

	///
	static void fire_mouse_scroll(f32 scroll);

	///
	static glm::vec<2, i32> get_mouse_position();

	///
	static u8 bullish_layer_popups;

	///
	static u8 bullish_layer_ui;

	///
	static u8 bullish_layer_arguments;

	///
	static u8 bullish_layer_background;

private:
	///
	static bool m_mouse_was_down;

	///
	static bool m_shift_down;

	///
	static bool m_control_down;

	///
	static bool m_alt_down;

	///
	static glm::vec<2, i32> m_mouse_position;

	///
	static std::shared_mutex m_key_text_mutex;

	///
	static std::shared_mutex m_mouse_mutex;

	///
	static std::vector<void (*)(key_state state, void* data)> m_key_callbacks;

	///
	static std::vector<void*> m_key_datas;

	///
	static std::vector<void (*)(key_state state, void* data)> m_text_callbacks;

	///
	static std::vector<void*> m_text_datas;

	///
	static std::vector<void (*)(
		glm::vec<2, i32> position, f32 scroll, mouse_operation operation, mouse_button button, void* data
	)>
		m_mouse_callbacks;

	///
	static std::vector<void*> m_mouse_datas;

	///
	static std::vector<std::vector<bool (*)(
		glm::vec<2, i32> position, f32 scroll, mouse_operation operation, mouse_button button, void* data
	)>>
		m_bullish_mouse_callbacks;

	///
	static std::vector<std::vector<void*>> m_bullish_mouse_datas;

	///
	static std::vector<TIME_POINT> m_press_log;

	///
	static mouse_button m_last_mouse_button;
};
}