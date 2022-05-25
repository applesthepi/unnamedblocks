#pragma once
#include "config.h"

#include "macchiato/vertices/vertex.hpp"
#include "macchiato/descriptor_set.hpp"

#include <latte/utils.hpp>

namespace mac
{
class object
{
public:
	object(void* window_state, u32 vertex_stride);

	///
	void set_data(void* vertices, u32 vertex_count, void* indices, u32 index_count);

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
	void* m_window_state;

	///
	vk::device& m_logical_device;

	///
	std::vector<mac::descriptor_set::state*> m_descriptor_set_state;

	///
	std::vector<mac::descriptor_set::instance*> m_descriptor_set_instances;

	///
	void* m_vertices;
	void* m_indices;

	///
	u32 m_vertex_count;
	u32 m_index_count;

	///
	u32 m_vertex_stride;

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