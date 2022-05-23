#include "object.hpp"

mac::object::object(vk::device& logical_device)
	: m_logical_device(logical_device)
	, m_vertices(nullptr)
	, m_indices(nullptr)
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

void mac::object::set_data(vma::allocator& allocator, vk::command_buffer& command_buffer, std::vector<mac::vertex>* vertices, std::vector<u32>* indices)
{
#ifndef NDEBUG
	if (vertices->empty() || indices->empty())
	{
		latte::logger::error(latte::logger::level::SYSTEM, "failed to set data; vertex count: " + std::to_string(vertices->size()) + " index count: " + std::to_string(indices->size()));
		return;
	}
#endif
	delete m_vertices;
	delete m_indices;

	m_vertices = vertices;
	m_indices = indices;

	update_buffers(allocator, command_buffer);
}

void mac::object::update_buffers(vma::allocator& allocator, vk::command_buffer& command_buffer)
{
#ifndef NDEBUG
	if (!m_vertices || !m_indices)
	{
		latte::logger::error(latte::logger::level::SYSTEM, "failed to update buffers of object; vertices or indices is nullptr");
		return;
	}

	if (m_vertices->empty() || m_indices->empty())
	{
		latte::logger::error(latte::logger::level::SYSTEM, "failed to set data; vertex count: " + std::to_string(m_vertices->size()) + " index count: " + std::to_string(m_indices->size()));
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
		usize buffer_size
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
		buffer_create_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

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

	usize vertex_buffer_size = m_vertices->front().stride() * m_vertices->size();

	function_update_buffer(
		m_vertex_buffer,
		m_vertex_staging_buffer,
		m_vertex_buffer_allocation,
		m_vertex_staging_buffer_allocation,
		vertex_buffer_size
	);

	// INDEX BUFFER

	usize index_buffer_size = sizeof(u32) * m_indices->size();

	function_update_buffer(
		m_index_buffer,
		m_index_staging_buffer,
		m_index_buffer_allocation,
		m_index_staging_buffer_allocation,
		index_buffer_size
	);

	// CPU ---> GPU

	vk::buffer_copy copy_region = {};
	void* data;

	// VERTICES

	vma::map_memory(allocator, m_vertex_staging_buffer_allocation, &data);
	memcpy(data, static_cast<const void*>(m_vertices->data()), vertex_buffer_size);
	vma::unmap_memory(allocator, m_vertex_staging_buffer_allocation);

	copy_region.size = vertex_buffer_size;
	vk::cmd::copy_buffer(command_buffer, m_vertex_staging_buffer, m_vertex_buffer, 1, &copy_region);

	// INDICES

	vma::map_memory(allocator, m_index_staging_buffer_allocation, &data);
	memcpy(data, static_cast<const void*>(m_indices->data()), index_buffer_size);
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

	vk::cmd::draw_indexed(command_buffer, static_cast<u32>(m_indices->size()), 1, 0, 0, 0);
}
