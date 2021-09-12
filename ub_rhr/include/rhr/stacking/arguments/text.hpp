#pragma once
#include "config.h"

#include "rhr/stacking/arguments/argument.hpp"
#include "rhr/registries/char_texture.hpp"
#include "rhr/rendering/objects/object.hpp"
#include "rhr/rendering/objects/text.hpp"

#include <cappuccino/utils.hpp>
#include <espresso/input_handler.hpp>

namespace rhr::stack::argument
{
///
class text : public rhr::stack::argument::argument
{
public:
	text(const cap::color& color, std::function<void()>* function_collection_update);

	/// How to interpret the data.
	/// \return Type of argument.
	BlockArgumentType get_type() override;

	/// Get width for surrounding argument spacing.
	/// \return Width of argument.
	u32 get_width() override;

	/// Whether or not the argument contains data for storing.
	/// \return Has data.
	bool has_data() override;
private:
	/// Add draw calls to cmd buffer prebound by Renderer.
	void on_render() override;

	/// Updates mesh on cpu side.
	void on_update_buffers() override;

	/// Regenerates descriptor sets including uniforms.
	void on_reload_swap_chain() override;

	/// Runs every frame before rendering.
	/// \param Seconds since last frame.
	void on_frame_update(f64 delta_time) override;

	///
	void post_position_update() override;

	/// Sets data. Data of the argument is stored as a string.
	void on_set_data() override;

	/// Renderable text element.
	std::shared_ptr<rhr::render::object::text> m_text;
};
}
