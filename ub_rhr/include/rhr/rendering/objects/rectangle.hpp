#pragma once
#include "config.h"

#include "rhr/rendering/interfaces/i_positionable.hpp"
#include "rhr/rendering/interfaces/i_sizeable.hpp"
#include "rhr/rendering/interfaces/i_enableable.hpp"
#include "rhr/rendering/interfaces/i_colorable.hpp"
#include "rhr/rendering/objects/object.hpp"

#include <cappuccino/utils.hpp>

namespace rhr::render::object
{
///
class rectangle : public rhr::render::interfaces::i_renderable, public rhr::render::interfaces::i_positionable<2, i32>, public rhr::render::interfaces::i_sizeable<2, i32>, public rhr::render::interfaces::i_enableable, public rhr::render::interfaces::i_colorable
{
public:
	///
	rectangle();

	/// Sets texture of rectangle.
	/// \param Texture path.
	void set_texture(const std::string& texture);

	/// Sets internal texture of rectangle.
	/// \param Internal texture.
	void set_texture(rhr::registry::char_texture::texture_type type);

	/// Sets Z depth.
	/// \param Z depth.
	void set_depth(i32 depth);
private:
	void on_render() override;

	void on_update_buffers() override;

	void on_reload_swap_chain() override;

	void post_position_update() override;

	void post_color_update() override;

	void post_size_update() override;

	/// Abstracted RenderObject to render the rectangle.
	std::shared_ptr<rhr::render::object::object> m_render_object;

	bool m_has_color;
	bool m_has_texture;
	bool m_in_bounds;

	std::string m_texture;
	rhr::render::object::object::texture_type m_texture_type;
	i32 m_depth;

	glm::vec<2, i32> m_use_size;
};
}