#pragma once
#include "config.h"

#include "rhr/rendering/cardinal.hpp"
#include "rhr/rendering/interfaces/i_colorable.hpp"
#include "rhr/rendering/interfaces/i_ui.hpp"
#include "rhr/rendering/objects/object.hpp"

#include <cappuccino/utils.hpp>

namespace rhr::render::object
{
///
class rectangle
	: public rhr::render::interfaces::i_ui
	, public rhr::render::interfaces::i_colorable
{
public:
	///
	rectangle();

	///
	void set_offset(glm::vec<2, i32>* offset);

	/// Sets texture of rectangle.
	/// \param Texture path.
	void set_texture(const std::string& texture);

	/// Sets internal texture of rectangle.
	/// \param Internal texture.
	void set_texture(rhr::registry::char_texture::texture_type type, u16 font_size);

	/// Sets Z depth.
	/// \param Z depth.
	void set_depth(i32 depth);

	///
	void enable_border(bool enable_border, rhr::render::cardinal::local_horizontal overhang_horizontal, rhr::render::cardinal::local_vertical overhang_vertical);

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

	/// Abstracted RenderObject to render the rectangle.
	std::shared_ptr<rhr::render::object::object> m_render_object;

	bool m_has_color;
	bool m_has_texture;
	bool m_in_bounds;
	bool m_border_enabled;

	std::string m_texture;
	rhr::render::object::object::texture_type m_texture_type;
	i32 m_depth;

	glm::vec<2, i32> m_use_size;

	rhr::render::cardinal::local_horizontal m_overhang_horizontal;
	rhr::render::cardinal::local_vertical m_overhang_vertical;
};
} // namespace rhr::render::object