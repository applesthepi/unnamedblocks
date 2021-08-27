#pragma once
#include "config.h"

#include "rhr/rendering/objects/button.hpp"
#include "rhr/rendering/objects/text.hpp"

#include <cappuccino/utils.hpp>

namespace rhr::render::object
{
///
class button_text : public rhr::render::object::button
{
public:
	///
	button_text(const cap::color& primary_color, const cap::color& secondary_color, const std::string& text);
private:
	///
	void on_render() override;

	///
	void on_update_buffers() override;

	///
	void on_reload_swap_chain() override;

	///
	void post_position_update() override;

	///
	void post_size_update() override;

	///
	std::shared_ptr<rhr::render::object::text> m_text;
};
}