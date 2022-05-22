#include "object_color.hpp"

mac::object_color::object_color(mac::window::state* window_state, vk::device& logical_device)
	: mac::object(logical_device)
{

}

mac::object_color::~object_color()
{

}

void mac::object_color::set_data(mac::vertex* vertices, u32* indices)
{

}

void mac::object_color::update_buffers()
{

}

void mac::object_color::render()
{

}

void mac::object_color::create_ubos(mac::window::state* window_state)
{
	m_descriptor_set_instances.emplace_back(mac::descriptor_set::create_instance(
		mac::window::get_descriptor_set(window_state, "cam"),
		window_state->vma_allocator,
		m_logical_device,
		{}
	));

	m_descriptor_set_instances.emplace_back(mac::descriptor_set::create_instance(
		mac::window::get_descriptor_set(window_state, "obj_c"),
		window_state->vma_allocator,
		m_logical_device,
		{}
	));
}

void mac::object_color::destroy_ubos()
{

}

void mac::object_color::update_ubos()
{

}
