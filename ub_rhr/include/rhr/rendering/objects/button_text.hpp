#pragma once
#include "config.h"

#include "rhr/rendering/objects/button.hpp"
#include "rhr/rendering/objects/text.hpp"

#include <cappuccino/utils.hpp>

namespace rhr::render::object
{
///
class button_text : public rhr::render::object::button
{
public:
	///
	button_text(const cap::color& primary_color, const cap::color& secondary_color, const std::string& text);
private:
	///
	virtual void ui_transform_update();

	///
	virtual void ui_render();

	///
	virtual void ui_reload_swap_chain();

	///
	virtual void ui_update_buffers();

	///
	virtual void ui_frame_update(f64 delta_time);

	///
	std::shared_ptr<rhr::render::object::text> m_text;
};
}