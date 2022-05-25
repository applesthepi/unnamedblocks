#include "object_texture.hpp"

#include "macchiato/vertices/vertex_texture.hpp"

mac::object_texture::object_texture(mac::window::state* window_state, vk::image_view& image_view)
	: mac::object(reinterpret_cast<void*>(window_state), mac::vertex_texture::info::stride_s)
	, m_ubo_obj({})
	, m_image_view(image_view)
{

}

mac::ubo_obj::uniform& mac::object_texture::ubo_obj()
{
	return m_ubo_obj.uniform_data;
}

void mac::object_texture::initial_update_buffers(vma::allocator& allocator, vk::command_buffer& command_buffer)
{
	create_ubos(reinterpret_cast<mac::window::state*>(m_window_state));
}

void mac::object_texture::update_ubos(vma::allocator& allocator)
{
	auto window_state = reinterpret_cast<mac::window::state*>(m_window_state);

	mac::descriptor_set::update_descriptor_buffer(
		m_descriptor_set_instances[1]->descriptor_buffers[0],
		allocator,
		&m_ubo_obj
	);
}

void mac::object_texture::create_ubos(mac::window::state* window_state)
{
	m_descriptor_set_instances.emplace_back(window_state->cam_instance);

	auto descriptor_image = mac::descriptor_set::create_descriptor_image(
		m_image_view,
		&mac::window::get_texture_sampler(window_state, "std")->texture_sampler
	);

	m_descriptor_set_instances.emplace_back(mac::descriptor_set::create_instance(
		mac::window::get_descriptor_set(window_state, "obj_t"),
		window_state->vma_allocator,
		m_logical_device,
		{ descriptor_image }
	));
}

void mac::object_texture::destroy_ubos()
{

}
