#pragma once
#include "config.h"

#include "mocha/registries/char_texture.hpp"
#include "lungo/objects/object.hpp"
#include "lungo/objects/text.hpp"
#include "mocha/stacking/arguments/argument.hpp"

#include "mocha/handlers/input.hpp"
#include <latte/utils.hpp>

namespace rhr::stack::argument
{
///
class text : public rhr::stack::argument::argument
{
public:
	text(const espresso::color& color, std::function<void()>* function_collection_update, glm::vec<2, i32>* plane_offset);

	espresso::mod::block::block::argument::type get_type() override;
	u32 get_width() override;
	bool has_data() override;
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

	/// Renderable text element.
	std::shared_ptr<rhr::render::object::text> m_text;
};
} // namespace rhr::stack::argument
