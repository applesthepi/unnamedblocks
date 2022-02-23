#pragma once
#include "config.h"

#include "lungo/interfaces/i_colorable.hpp"
#include "lungo/interfaces/i_ui.hpp"
#include "lungo/objects/object.hpp"

#include <latte/utils.hpp>

namespace rhr::render::object
{
///
class line
	: public rhr::render::interfaces::i_ui
	, public rhr::render::interfaces::i_colorable
{
public:
	line();

	/// Sets Z depth.
	/// \param Z depth.
	void set_depth(i32 depth);

	///
	void enable_border(bool enable_border);

	///
	void set_point_1(const glm::vec<2, i32>& point);

	///
	void set_point_2(const glm::vec<2, i32>& point);

	///
	void set_line_half_width(u8 line_half_width);

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
	void post_color_update() override;

private:
	/// Abstracted RenderObject to render the rectangle.
	std::shared_ptr<rhr::render::object::object> m_render_object;

	///
	bool m_in_bounds;

	///
	bool m_border_enabled;

	///
	i32 m_depth;

	///
	glm::vec<2, i32> m_point_1;

	///
	glm::vec<2, i32> m_point_2;

	///
	u8 m_line_half_width;
};
} // namespace rhr::render::object