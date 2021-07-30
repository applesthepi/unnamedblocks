#pragma once
#include "config.h"

#include "rhr/rendering/interfaces/i_positionable.hpp"
#include "rhr/rendering/interfaces/i_renderable.hpp"
#include "rhr/rendering/interfaces/i_sizeable.hpp"
#include "rhr/rendering/interfaces/i_enableable.hpp"
#include "rhr/rendering/interfaces/i_dicolorable.hpp"
#include "rhr/rendering/interfaces/i_field.hpp"
#include "rhr/rendering/objects/object.hpp"

#include <cappuccino/utils.hpp>

namespace rhr::render::object
{
///
class text : public rhr::render::interfaces::i_renderable, public rhr::render::interfaces::i_positionable<2, i32>, public rhr::render::interfaces::i_sizeable<2, i32>, public rhr::render::interfaces::i_dicolorable, public rhr::render::interfaces::i_enableable, public rhr::render::interfaces::i_field
{
public:
	///
	text(rhr::registry::char_texture::texture_type texture_type = rhr::registry::char_texture::texture_type::LIGHT_NORMAL);

	/// Sets text.
	/// \param Text.
	void set_text(const std::string& text);

	/// Sets Z depth.
	/// \param Z depth.
	void set_depth(i32 depth);

	///
	void set_padding(i32 padding);

	///
	void enable_background(bool enable);
private:
	///
	void update_size();

	/// Add draw calls to cmd buffer prebound by Renderer.
	void on_render() override;

	/// Updates mesh on cpu side.
	void on_update_buffers() override;

	/// Regenerates descriptor sets including uniforms.
	void on_reload_swap_chain() override;

	///
	void post_position_update() override;

	///
	void post_size_update() override;

	///
	std::optional<usize> pick_index(glm::vec<2, i32> position, bool ignore_y) override;

	///
	std::optional<glm::vec<2, i32>> get_index_position(usize idx) override;

	/// Object to render the background rectangle.
	std::shared_ptr<rhr::render::object::object> m_render_object_background;

	/// Object to render the actual text.
	std::shared_ptr<rhr::render::object::object> m_render_object_text;

	///
	std::string m_text;

	///
	std::vector<i16> m_char_widths;

	///
	i32 m_depth;

	///
	i32 m_padding;

	///
	bool m_enable_background;
};
}