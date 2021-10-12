#pragma once
#include "config.h"

#include "rhr/stacking/arguments/argument.hpp"
#include "rhr/registries/char_texture.hpp"
#include "rhr/rendering/objects/object.hpp"
#include "rhr/rendering/objects/text.hpp"

#include <cappuccino/utils.hpp>
#include "rhr/handlers/input.hpp"

namespace rhr::stack::argument
{
///
class text : public rhr::stack::argument::argument
{
public:
	text(const cap::color& color, std::function<void()>* function_collection_update);

	/// How to interpret the data.
	/// \return Type of argument.
	cap::mod::block::block::argument::type get_type() override;

	/// Get width for surrounding argument spacing.
	/// \return Width of argument.
	u32 get_width() override;

	/// Whether or not the argument contains data for storing.
	/// \return Has data.
	bool has_data() override;

	///
	rhr::stack::argument::argument::padding_style get_padding_style() override;
private:
	///
	void ui_transform_update() override;

	///
	void ui_render() override;

	///
	void ui_reload_swap_chain() override;

	///
	void ui_update_buffers() override;

	///
	void ui_frame_update(f64 delta_time) override;

	/// Sets data. Data of the argument is stored as a string.
	void on_set_data() override;

	/// Renderable text element.
	std::shared_ptr<rhr::render::object::text> m_text;
};
}
