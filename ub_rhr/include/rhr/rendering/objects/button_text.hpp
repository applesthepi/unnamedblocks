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
	void ui_transform_update(i_ui::transform_update_spec transform_update_spec) override;

	///
	void ui_render() override;

	///
	void ui_reload_swap_chain() override;

	///
	void ui_update_buffers() override;

	///
	void ui_chain_update_buffers() override;

	///
	void ui_frame_update(f64 delta_time) override;

	///
	std::shared_ptr<rhr::render::object::text> m_text;
};
} // namespace rhr::render::object