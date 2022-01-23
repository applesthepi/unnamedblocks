#pragma once
#include "config.h"

#include "rhr/rendering/interfaces/i_dicolorable.hpp"
#include "rhr/rendering/interfaces/i_ui.hpp"
#include "rhr/rendering/objects/rectangle.hpp"

#include "rhr/handlers/input.hpp"
#include <cappuccino/color.hpp>
#include <cappuccino/utils.hpp>

namespace rhr::render::object
{
///
class button
	: public rhr::render::interfaces::i_ui
	, public rhr::render::interfaces::i_dicolorable
{
public:
	///
	button(const cap::color& primary_color, const cap::color& secondary_color);

	///
	~button();

	///
	void set_depth(i32 depth);

	/// Callback called when button is clicked.
	/// \param Callback.
	/// \param Data is passed as a parameter in the callback.
	void set_callback(void (*callback)(void*), void* data);

	///
	void mouse_update(glm::vec<2, i32> position, f32 scroll, MouseOperation operation);

	///
	void enable_fill_width(bool enable);

protected:
	///
	void ui_transform_update() override;

	///
	void ui_render() override;

	///
	void ui_reload_swap_chain() override;

	///
	void ui_update_buffers() override;

	///
	void ui_frame_update(f64 delta_time) override;

	///
	void post_color_update() override;

private:
	///
	std::shared_ptr<rhr::render::object::rectangle> m_background;

	/// Callback called when button is clicked.
	void (*m_callback)(void*);

	/// Callback data is passed by a parameter in the callback when the button is pressed.
	void* m_callback_data;

	///
	bool m_enable_fill_width;
};
} // namespace rhr::render::object