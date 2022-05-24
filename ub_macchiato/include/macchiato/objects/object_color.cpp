#include "object_color.hpp"

#include "macchiato/vertex_std.hpp"



mac::object_color::object_color(mac::window::state* window_state, mac::ubo_cam* ubo_cam)
	: mac::object(reinterpret_cast<void*>(window_state), sizeof(mac::vertex_std))
	, m_ubo_cam(ubo_cam)
	, m_ubo_obj({})
{

}

mac::ubo_obj::uniform& mac::object_color::ubo_obj()
{
	return m_ubo_obj.uniform_data;
}

void mac::object_color::initial_update_buffers(vma::allocator& allocator, vk::command_buffer& command_buffer)
{
	create_ubos(reinterpret_cast<mac::window::state*>(m_window_state));
}

void mac::object_color::create_ubos(mac::window::state* window_state)
{
	// TODO: TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP
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

void mac::object_color::update_ubos(vma::allocator& allocator)
{
	auto window_state = reinterpret_cast<mac::window::state*>(m_window_state);

	// TODO: TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP
	mac::descriptor_set::update_descriptor_buffer(
		m_descriptor_set_instances[0]->descriptor_buffers[0],
		allocator,
		m_ubo_cam
	);

	mac::descriptor_set::update_descriptor_buffer(
		m_descriptor_set_instances[1]->descriptor_buffers[0],
		allocator,
		&m_ubo_obj
	);
}
