#include "object.hpp"

#include "macchiato/window.hpp"

mac::object::object(void* window_state, u32 vertex_stride)
	: m_window_state(window_state)
	, m_logical_device(reinterpret_cast<mac::window::state*>(window_state)->device_state->logical_device)
	, m_vertices(nullptr)
	, m_indices(nullptr)
	, m_vertex_count(0)
	, m_index_count(0)
	, m_vertex_stride(vertex_stride)
	, m_vertex_staging_buffer(nullptr)
	, m_vertex_buffer(nullptr)
	, m_vertex_staging_buffer_allocation(nullptr)
	, m_vertex_buffer_allocation(nullptr)
	, m_index_staging_buffer(nullptr)
	, m_index_buffer(nullptr)
	, m_index_staging_buffer_allocation(nullptr)
	, m_index_buffer_allocation(nullptr)
{

}

void mac::object::set_data(void* vertices, u32 vertex_count, void* indices, u32 index_count)
{
#ifndef NDEBUG
	if (vertex_count == 0 || index_count == 0)
	{
		latte::logger::error(latte::logger::level::SYSTEM, "failed to set data; vertex count: " + std::to_string(m_vertex_count) + " index count: " + std::to_string(m_index_count));
		return;
	}
#endif

	m_vertices = vertices;
	m_indices = indices;

	m_vertex_count = vertex_count;
	m_index_count = index_count;

	reinterpret_cast<mac::window::state*>(m_window_state)->dirty_objects.emplace_back(this);
}

void mac::object::update_buffers(vma::allocator& allocator, vk::command_buffer& command_buffer)
{
#ifndef NDEBUG
	if (!m_vertices || !m_indices)
	{
		latte::logger::error(latte::logger::level::SYSTEM, "failed to update buffers of object; vertices or indices is nullptr");
		return;
	}

	if (m_vertex_count == 0 || m_index_count == 0)
	{
		latte::logger::error(latte::logger::level::SYSTEM, "failed to set data; vertex count: " + std::to_string(m_vertex_count) + " index count: " + std::to_string(m_index_count));
		return;
	}
#endif
	if (m_vertex_buffer)
	{
		// TODO: delete buffers 3 frames+ later

		//vma::destroy_buffer(allocator, m_vertex_staging_buffer, m_vertex_staging_buffer_allocation);
		//vma::destroy_buffer(allocator, m_vertex_buffer, m_vertex_buffer_allocation);
		//vma::destroy_buffer(allocator, m_index_staging_buffer, m_index_staging_buffer_allocation);
		//vma::destroy_buffer(allocator, m_index_buffer, m_index_buffer_allocation);

		m_vertex_staging_buffer = nullptr;
		m_vertex_buffer = nullptr;
		m_vertex_staging_buffer_allocation = nullptr;
		m_vertex_buffer_allocation = nullptr;

		m_index_staging_buffer = nullptr;
		m_index_buffer = nullptr;
		m_index_staging_buffer_allocation = nullptr;
		m_index_buffer_allocation = nullptr;
	}

	auto function_update_buffer = [&allocator](
		vk::buffer& buffer,
		vk::buffer& staging_buffer,
		vma::allocation& buffer_allocation,
		vma::allocation& staging_buffer_allocation,
		usize buffer_size,
		vk::buffer_usage_flag_bits buffer_usage_flag_bits
	)
	{
		vk::buffer_create_info buffer_create_info;
		vma::allocation_create_info allocation_create_info;

		// CPU BUFFER

		buffer_create_info		  = {VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
		buffer_create_info.size  = buffer_size;
		buffer_create_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

		allocation_create_info		 = {};
		allocation_create_info.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;

		vma::create_buffer(
			allocator,
			&buffer_create_info,
			&allocation_create_info,
			&staging_buffer,
			&staging_buffer_allocation,
			nullptr
		);

		// GPU BUFFER

		buffer_create_info		  = {VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
		buffer_create_info.size  = buffer_size;
		buffer_create_info.usage = buffer_usage_flag_bits | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

		allocation_create_info		 = {};
		allocation_create_info.usage = VMA_MEMORY_USAGE_GPU_ONLY;

		vma::create_buffer(
			allocator,
			&buffer_create_info,
			&allocation_create_info,
			&buffer,
			&buffer_allocation,
			nullptr
		);
	};

	// VERTEX BUFFER

	usize vertex_buffer_size = m_vertex_stride * m_vertex_count;

	function_update_buffer(
		m_vertex_buffer,
		m_vertex_staging_buffer,
		m_vertex_buffer_allocation,
		m_vertex_staging_buffer_allocation,
		vertex_buffer_size,
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT
	);

	// INDEX BUFFER

	usize index_buffer_size = sizeof(u32) * m_index_count;

	function_update_buffer(
		m_index_buffer,
		m_index_staging_buffer,
		m_index_buffer_allocation,
		m_index_staging_buffer_allocation,
		index_buffer_size,
		VK_BUFFER_USAGE_INDEX_BUFFER_BIT
	);

	// CPU ---> GPU

	vk::buffer_copy copy_region = {};
	void* data;

	// VERTICES

	vma::map_memory(allocator, m_vertex_staging_buffer_allocation, &data);
	memcpy(data, static_cast<const void*>(m_vertices), vertex_buffer_size);
	vma::unmap_memory(allocator, m_vertex_staging_buffer_allocation);

	copy_region.size = vertex_buffer_size;
	vk::cmd::copy_buffer(command_buffer, m_vertex_staging_buffer, m_vertex_buffer, 1, &copy_region);

	// INDICES

	vma::map_memory(allocator, m_index_staging_buffer_allocation, &data);
	memcpy(data, static_cast<const void*>(m_indices), index_buffer_size);
	vma::unmap_memory(allocator, m_index_staging_buffer_allocation);

	copy_region.size = index_buffer_size;
	vk::cmd::copy_buffer(command_buffer, m_index_staging_buffer, m_index_buffer, 1, &copy_region);
}

void mac::object::render(vk::command_buffer& command_buffer, vma::allocator& allocator, vk::pipeline_layout& pipeline_layout)
{
	update_ubos(allocator);

	std::array<vk::buffer, 1> vertex_buffers      = { m_vertex_buffer };
	std::array<vk::device_size, 1> vertex_offsets = { 0 };

	vk::cmd::bind_vertex_buffers(command_buffer, 0, 1, vertex_buffers.data(), vertex_offsets.data());
	vk::cmd::bind_index_buffer(command_buffer, m_index_buffer, 0, VK_INDEX_TYPE_UINT32);

	std::vector<vk::descriptor_set> descriptor_sets;
	mac::descriptor_set::simplify_descriptor_sets(m_descriptor_set_instances, descriptor_sets);

	vk::cmd::bind_descriptor_sets(
		command_buffer,
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		pipeline_layout,
		0, descriptor_sets.size(),
		descriptor_sets.data(),
		0, nullptr
	);

	vk::cmd::draw_indexed(command_buffer, m_index_count, 1, 0, 0, 0);
}
