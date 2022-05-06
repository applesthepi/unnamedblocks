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
	struct mouse_button_data
	{
		glm::vec<2, i32> position;
		f32 scroll;
		rhr::handler::input::mouse_operation operation;
		rhr::handler::input::mouse_button button;
		void* data;
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
	struct stream
	{
		///
		std::shared_mutex key_text_mutex;

		///
		std::shared_mutex mouse_mutex;

		///
		std::vector<void (*)(key_state state, void* data)> key_callbacks;

		///
		std::vector<void*> key_datas;

		///
		std::vector<void (*)(key_state state, void* data)> text_callbacks;

		///
		std::vector<void*> text_datas;

		///
		std::vector<std::function<void(mouse_button_data)>>
			mouse_callbacks;

		/// Stores an index for each callback to identify it for unregistering it.
		std::vector<u64> mouse_callback_indices;

		/// Incremented for every mouse callback added.
		u64 mouse_callback_index;

		///
		std::vector<void*> mouse_datas;

		///
		std::vector<std::vector<bool (*)(
			glm::vec<2, i32> position, f32 scroll, mouse_operation operation, mouse_button button, void* data
		)>>
			bullish_mouse_callbacks;

		///
		std::vector<std::vector<void*>> bullish_mouse_datas;

		///
		std::vector<TIME_POINT> press_log;
	};

	///
	static void initialize();

	/// Retrieves the stream from the input handler to link with another input handler.
	static stream* get_stream();

	/// Adds operating callbacks to use.
	static void add_alt_stream(stream* alt_stream);

	///
	static void register_key_callback(void (* callback)(key_state state, void* data), void* data);

	///
	static void unregister_key_callback(void (* callback)(key_state state, void* data));

	///
	static void register_text_callback(void (* callback)(key_state state, void* data), void* data);

	///
	static void unregister_text_callback(void (* callback)(key_state state, void* data));

	///
	static u64 register_mouse_callback(
		std::function<void(mouse_button_data)> callback,
		void* data
	);

	///
	static void unregister_mouse_callback(u64 key);

	/////
	//static void register_bullish_mouse_callback(
	//	bool (* callback)(
	//		glm::vec<2, i32> position, f32 scroll, mouse_operation operation, mouse_button button, void* data
	//	),
	//	u8 layer,
	//	void* data
	//);
	//
	/////
	//static void unregister_bullish_mouse_callback(
	//	bool (* callback)(
	//		glm::vec<2, i32> position, f32 scroll, mouse_operation operation, mouse_button button, void* data
	//	),
	//	u8 layer
	//);

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
	static mouse_button m_last_mouse_button;

	///
	static stream* m_stream_main;

	///
	static std::vector<stream*> m_stream_alt;
};
}