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
	void on_render() override;

	///
	void on_update_buffers() override;

	///
	void on_reload_swap_chain() override;

	///
	void post_transform_update() override;

	///
	std::shared_ptr<rhr::render::object::rectangle> m_rectangle;
};
}