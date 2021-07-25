#pragma once
#include "config.h"

#include "rhr/rendering/interfaces/i_positionable.hpp"
#include "rhr/rendering/interfaces/i_renderable.hpp"
#include "rhr/rendering/interfaces/i_sizeable.hpp"
#include "rhr/rendering/interfaces/i_enableable.hpp"
#include "rhr/rendering/interfaces/i_dicolorable.hpp"
#include "rhr/rendering/objects/object.hpp"

#include <cappuccino/utils.hpp>

namespace rhr::render::object
{
///
class text : public rhr::render::interfaces::i_renderable, public rhr::render::interfaces::i_positionable<2, i32>, public rhr::render::interfaces::i_sizeable<2, i32>, public rhr::render::interfaces::i_dicolorable, public rhr::render::interfaces::i_enableable
{
public:
	///
	text();

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

	/// Object to render the background rectangle.
	std::shared_ptr<rhr::render::object::object> m_render_object_background;

	/// Object to render the actual text.
	std::shared_ptr<rhr::render::object::object> m_render_object_text;

	///
	std::string m_text;

	///
	i32 m_depth;

	///
	i32 m_padding;

	///
	bool m_enable_background;
};
}