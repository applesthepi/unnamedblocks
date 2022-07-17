#include "shape_entity.hpp"

mac::shape_entity::shape_entity()
	: m_shape(nullptr)
{}

mac::shape_entity::shape_entity(mac::shape* shape)
	: m_shape(shape)
{}

mac::shape* mac::shape_entity::get_shape()
{
	return m_shape;
}

void mac::shape_entity::set_shape(mac::shape* shape)
{
	m_shape = shape;
}

void mac::shape_entity::ui_initialize(mac::window::state* window_state)
{}

void mac::shape_entity::ui_transform_update(i_ui::transform_update_spec transform_update_spec)
{
	m_shape->set_position(get_position_physical_absolute());
	m_shape->update();
}

void mac::shape_entity::ui_frame_update(f64 delta_time)
{}

void mac::shape_entity::ui_render()
{}

void mac::shape_entity::ui_reload_swap_chain()
{}

void mac::shape_entity::ui_update_buffers()
{}

void mac::shape_entity::ui_chain_update_buffers()
{}

void mac::shape_entity::ui_static_offset_update()
{}

void mac::shape_entity::ui_serialize(latte::serializer::node& node)
{}

void mac::shape_entity::ui_deserialize(latte::serializer::node& node)
{}