#pragma once
#include "config.h"

#include "macchiato/vertex.hpp"
#include "macchiato/descriptor_set.hpp"

#include <latte/utils.hpp>

namespace mac
{
class object
{
public:
	object(vk::device& logical_device);

	///
	void set_data(vma::allocator& allocator, vk::command_buffer& command_buffer, std::vector<mac::vertex>* vertices, std::vector<u32>* indices);

	///
	void update_buffers(vma::allocator& allocator, vk::command_buffer& command_buffer);

	///
	void render(vk::command_buffer& command_buffer, vma::allocator& allocator, vk::pipeline_layout& pipeline_layout);

	///
	virtual void initial_update_buffers(vma::allocator& allocator, vk::command_buffer& command_buffer) = 0;
protected:
	///
	virtual void update_ubos(vma::allocator& allocator) = 0;

	///
	vk::device& m_logical_device;

	///
	std::vector<mac::descriptor_set::state*> m_descriptor_set_state;

	///
	std::vector<mac::descriptor_set::instance*> m_descriptor_set_instances;

	///
	std::vector<mac::vertex>* m_vertices;

	///
	std::vector<u32>* m_indices;

	///
	vk::buffer m_vertex_staging_buffer;
	vk::buffer m_vertex_buffer;
	vma::allocation m_vertex_staging_buffer_allocation;
	vma::allocation m_vertex_buffer_allocation;

	///
	vk::buffer m_index_staging_buffer;
	vk::buffer m_index_buffer;
	vma::allocation m_index_staging_buffer_allocation;
	vma::allocation m_index_buffer_allocation;
};
}