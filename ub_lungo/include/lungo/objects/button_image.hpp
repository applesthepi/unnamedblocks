#pragma once
#include "config.h"

#include "lungo/objects/button.hpp"
#include "lungo/objects/text.hpp"

#include <latte/utils.hpp>

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
	std::shared_ptr<rhr::render::object::rectangle> m_rectangle;
};
} // namespace rhr::render::object