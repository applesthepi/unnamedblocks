#pragma once
#include "config.h"

#include "rhr/rendering/interfaces/i_enableable.hpp"
#include "rhr/rendering/interfaces/i_ui.hpp"
#include "rhr/rendering/frame.hpp"

#include <cappuccino/utils.hpp>

namespace rhr::render
{
///
class layer : public rhr::render::interfaces::i_ui
{
public:
	layer();
private:
	///
	void ui_transform_update() override;

	///
	void ui_render() override;

	///
	void ui_reload_swap_chain() override;

	///
	void ui_update_buffers() override;
};
}