#pragma once
#include "config.h"

#include "lungo/objects/button.hpp"
#include "lungo/objects/text.hpp"

#include <latte/utils.hpp>

namespace rhr::render::object
{
///
class button_text : public rhr::render::object::button
{
public:
	///
	button_text(
		const espresso::color& primary_color,
		const espresso::color& secondary_color,
		const std::string& text,
		glm::vec<2, i32>* plane_offset);

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