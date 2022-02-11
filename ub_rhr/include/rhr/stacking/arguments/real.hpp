#pragma once
#include "config.h"

#include "rhr/registries/char_texture.hpp"
#include "rhr/rendering/objects/object.hpp"
#include "rhr/rendering/objects/text.hpp"
#include "rhr/stacking/arguments/argument.hpp"

#include "rhr/handlers/input.hpp"
#include <cappuccino/utils.hpp>

namespace rhr::stack::argument
{
class real : public rhr::stack::argument::argument
{
public:
	real(const cap::color& color, std::function<void()>* function_collection_update, glm::vec<2, i32>* plane_offset);

	cap::mod::block::block::argument::type get_type() override;
	u32 get_width() override;
	bool has_data() override;
	bool drag_bounds(glm::vec<2, i32> position) override;
	rhr::stack::argument::argument::padding_style get_padding_style() override;
	void set_plane_offset(glm::vec<2, i32>* plane_offset) override;

private:
	void ui_transform_update(i_ui::transform_update_spec transform_update_spec) override;
	void ui_render() override;
	void ui_reload_swap_chain() override;
	void ui_update_buffers() override;
	void ui_chain_update_buffers() override;
	void ui_frame_update(f64 delta_time) override;
	void on_set_data() override;
	void on_set_mode(cap::mod::block::block::argument::variable_mode mode) override;

	/// Decoration.
	std::shared_ptr<rhr::render::object::object> m_decor_left_top;
	std::shared_ptr<rhr::render::object::object> m_decor_left_bottom;
	std::shared_ptr<rhr::render::object::object> m_decor_right_top;
	std::shared_ptr<rhr::render::object::object> m_decor_right_bottom;

	/// Renderable text element.
	std::shared_ptr<rhr::render::object::text> m_text;

	///
	std::function<void(glm::vec<2, i32> position, f32 scroll, MouseOperation operation, MouseButton button)>
		m_mouse_button;

	///
	std::function<void()> m_function_text_update;
};
} // namespace rhr::stack::argument