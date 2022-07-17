#pragma once
#include "config.h"

#include "macchiato/shapes/rectangle.hpp"
#include "macchiato/interfaces/i_ui.hpp"

#include <latte/utils.hpp>

namespace mac
{
class shape_entity : public mac::i_ui
{
public:
	shape_entity();
	shape_entity(mac::shape* shape);
	
	///
	mac::shape* get_shape();

	///
	void set_shape(mac::shape* shape);
private:
	void ui_initialize(mac::window::state* window_state) override;
	void ui_transform_update(i_ui::transform_update_spec transform_update_spec) override;
	void ui_frame_update(f64 delta_time) override;
	void ui_render() override;
	void ui_reload_swap_chain() override;
	void ui_update_buffers() override;
	void ui_chain_update_buffers() override;
	void ui_static_offset_update() override;
	void ui_serialize(latte::serializer::node& node) override;
	void ui_deserialize(latte::serializer::node& node) override;

	///
	mac::shape* m_shape;
};
}