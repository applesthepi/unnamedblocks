#pragma once
#include "config.h"

#include "rhr/rendering/objects/button.hpp"
#include "rhr/rendering/objects/text.hpp"

#include <cappuccino/utils.hpp>

namespace rhr::render::object
{
///
class button_image : public rhr::render::object::button
{
public:
	///
	button_image(const std::string& texture);

private:
	///
	virtual void ui_transform_update();

	///
	virtual void ui_render();

	///
	virtual void ui_reload_swap_chain();

	///
	virtual void ui_update_buffers();

	///
	std::shared_ptr<rhr::render::object::rectangle> m_rectangle;
};
} // namespace rhr::render::object