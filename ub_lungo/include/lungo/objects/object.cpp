#include "object.hpp"

#include "lungo/registries/char_texture.hpp"
#include "lungo/renderer.hpp"
#include "lungo/tools.hpp"

rhr::render::object::object::object(bool ui)
	: i_enableable(true)
	, m_has_vertices(false)
	, m_has_indices(false)
	, m_vertex_count(0)
	, m_index_count(0)
	, m_vertex_alloc_count(0)
	, m_index_alloc_count(0)
	, m_vertices(nullptr)
	, m_indices(nullptr)
	, m_queue(0)
	, m_dirty(false)
	, m_init_image(true)
	, m_ui(ui)
	, m_has_texture(false)
	, m_vertex_buffer(nullptr)
	, m_vertex_staging_buffer(nullptr)
	, m_index_buffer(nullptr)
	, m_index_staging_buffer(nullptr)
	, m_texture_type(texture_type::CUSTOM)
	, m_font_size(4)
	, m_offset(nullptr)
{}

rhr::render::object::object::object(bool ui, const std::string& texture_path)
	: object(ui)
{
	m_texture_path = texture_path;
}

void rhr::render::object::object::set_offset(glm::vec<2, i32>* offset)
{
	m_offset = offset;
}

void rhr::render::object::object::set_texture(const std::string& texture)
{
	m_has_texture  = true;
	m_texture_path = texture;
	m_texture_type = texture_type::CUSTOM;
}

void rhr::render::object::object::set_texture_char(
	rhr::registry::char_texture::texture_type texture_type, u16 font_size)
{
	m_has_texture = true;
	m_texture_path.clear();
	m_texture_type		= texture_type::TEXT_SHEET;
	m_char_texture_type = texture_type;
	m_font_size			= font_size;
}

void rhr::render::object::object::set_texture_char_size(u16 font_size)
{
	m_font_size = font_size;
	mark_dirty();
}

void rhr::render::object::object::update_vertices(
	const std::vector<rhr::render::vertex>* vertices, const std::vector<u32>* indices, bool update_buffers_now)
{
	if (vertices->size() < 3 || indices->size() < 3)
	{
		latte::logger::error(latte::logger::level::SYSTEM, "object does not support less then 3 vertices or indices");
		return;
	}

	usize vertices_size = sizeof(rhr::render::vertex) * vertices->size();
	usize indices_size	= sizeof(u32) * indices->size();

	if (vertices->size() > m_vertex_alloc_count)
	{
		rhr::render::vertex* staged_vertices = new rhr::render::vertex[vertices->size() * RENDER_OBJECT_ALLOC_FACTOR];

		if (!staged_vertices)
			latte::logger::fatal(latte::logger::level::SYSTEM, "out of memory");

		memcpy(staged_vertices, vertices->data(), vertices_size);

		m_vertex_count		 = vertices->size();
		m_vertex_alloc_count = vertices->size() * RENDER_OBJECT_ALLOC_FACTOR;

		if (m_vertices != nullptr)
			delete[] m_vertices;

		m_vertices = staged_vertices;
	}
	else
	{
		memcpy(m_vertices, vertices->data(), vertices_size);
		m_vertex_count = vertices->size();
	}

	if (indices->size() > m_index_alloc_count)
	{
		u32* staged_indices = new u32[indices->size() * RENDER_OBJECT_ALLOC_FACTOR];

		if (!staged_indices)
			latte::logger::fatal(latte::logger::level::SYSTEM, "out of memory");

		memcpy(staged_indices, indices->data(), indices_size);

		m_index_count		= indices->size();
		m_index_alloc_count = indices->size() * RENDER_OBJECT_ALLOC_FACTOR;

		if (m_indices != nullptr)
			delete[] m_indices;

		m_indices = staged_indices;
	}
	else
	{
		memcpy(m_indices, indices->data(), indices_size);
		m_index_count = indices->size();
	}

	m_has_indices = true;

	if (update_buffers_now)
		update_buffers();
	else
		mark_dirty();
}

void rhr::render::object::object::update_vertices(
	const rhr::render::vertex* vertices, u32 vertex_count, const u32* indices, u32 index_count, bool update_buffers_now)
{
	if (vertex_count < 3 || index_count < 3)
	{
		latte::logger::error(latte::logger::level::SYSTEM, "object does not support less then 3 vertices or indices");
		return;
	}

	usize vertices_size = sizeof(rhr::render::vertex) * vertex_count;
	usize indices_size	= sizeof(u32) * index_count;

	if (vertex_count > m_vertex_alloc_count)
	{
		rhr::render::vertex* staged_vertices = new rhr::render::vertex[vertex_count * RENDER_OBJECT_ALLOC_FACTOR];

		if (!staged_vertices)
			latte::logger::fatal(latte::logger::level::SYSTEM, "out of memory");

		memcpy(staged_vertices, vertices, vertices_size);

		m_vertex_count		 = vertex_count;
		m_vertex_alloc_count = vertex_count * RENDER_OBJECT_ALLOC_FACTOR;

		if (m_vertices != nullptr)
			delete[] m_vertices;

		m_vertices = staged_vertices;
	}
	else
	{
		memcpy(m_vertices, vertices, vertices_size);
		m_vertex_count = vertex_count;
	}

	if (index_count > m_index_alloc_count)
	{
		u32* staged_indices = new u32[index_count * RENDER_OBJECT_ALLOC_FACTOR];

		if (!staged_indices)
			latte::logger::fatal(latte::logger::level::SYSTEM, "out of memory");

		memcpy(staged_indices, indices, indices_size);

		m_index_count		= index_count;
		m_index_alloc_count = index_count * RENDER_OBJECT_ALLOC_FACTOR;

		if (m_indices != nullptr)
			delete[] m_indices;

		m_indices = staged_indices;
	}
	else
	{
		memcpy(m_indices, indices, indices_size);
		m_index_count = index_count;
	}

	m_has_indices = true;

	if (update_buffers_now)
		update_buffers();
	else
		mark_dirty();
}

void rhr::render::object::object::update_vertices(
	const rhr::render::vertex* vertices, u32 vertex_count, bool update_buffers_now)
{
	if (vertex_count < 3)
	{
		latte::logger::error(latte::logger::level::SYSTEM, "object does not support less then 3 vertices or indices");
		return;
	}

	usize vertices_size = sizeof(rhr::render::vertex) * vertex_count;

	if (vertex_count > m_vertex_alloc_count)
	{
		rhr::render::vertex* staged_vertices = new rhr::render::vertex[vertex_count * RENDER_OBJECT_ALLOC_FACTOR];

		if (!staged_vertices)
			latte::logger::fatal(latte::logger::level::SYSTEM, "out of memory");

		memcpy(staged_vertices, vertices, vertices_size);

		m_vertex_count		 = vertex_count;
		m_vertex_alloc_count = vertex_count * RENDER_OBJECT_ALLOC_FACTOR;

		if (m_vertices != nullptr)
			delete[] m_vertices;

		m_vertices = staged_vertices;
	}
	else
	{
		memcpy(m_vertices, vertices, vertices_size);
		m_vertex_count = vertex_count;
	}

	m_has_indices = false;

	if (update_buffers_now)
		update_buffers();
	else
		mark_dirty();
}

void rhr::render::object::object::on_render()
{
	if (!m_has_vertices || !m_enabled)
		return;

	update_uniforms(m_ui);

	vk::buffer vb[]			  = {m_vertex_buffer};
	vk::device_size offsets[] = {0};

	if (m_ui)
	{
		if (m_has_texture)
			rhr::render::renderer::get_window_primary()->bind_texture_pipeline(&m_descriptor_set);
		else
			rhr::render::renderer::get_window_primary()->bind_color_pipeline(&m_descriptor_set);
	}
	else
		latte::logger::fatal(latte::logger::level::SYSTEM, "not implemented");

	vk::command_buffer* active_command_buffer =
		rhr::render::renderer::get_window_primary()->get_active_command_buffer();
	vk::cmd::bind_vertex_buffers(*active_command_buffer, 0, 1, vb, offsets);

	if (m_has_indices)
	{
		vk::cmd::bind_index_buffer(*active_command_buffer, m_index_buffer, 0, VK_INDEX_TYPE_UINT32);
		vk::cmd::draw_indexed(*active_command_buffer, static_cast<u32>(m_index_count), 1, 0, 0, 0);
	}
	else
		vk::cmd::draw(*active_command_buffer, static_cast<u32>(m_vertex_count), 1, 0, 0);
}

void rhr::render::object::object::on_update_buffers()
{
	m_dirty		   = false;
	m_has_vertices = true;

	if (m_init_image)
	{
		m_init_image = false;

		if (m_has_texture)
		{
			if (m_texture_type == texture_type::CUSTOM)
			{
				m_image		 = rhr::render::tools::create_texture_image(m_texture_path, &m_image_memory);
				m_image_view = rhr::render::tools::create_image_view(
					m_image,
					rhr::render::renderer::get_window_primary()->get_surface_format()->format,
					VK_IMAGE_ASPECT_COLOR_BIT);
			}
			else if (m_texture_type == texture_type::TEXT_SHEET)
			{
				m_image = rhr::registry::char_texture::get_texture_map(m_font_size)->map[m_char_texture_type].image;
				m_image_view = rhr::render::tools::create_image_view(
					m_image,
					rhr::render::renderer::get_window_primary()->get_surface_format()->format,
					VK_IMAGE_ASPECT_COLOR_BIT);
			}
		}

		vk::buffer_create_info buffer_info = {VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
		buffer_info.size				   = sizeof(rhr::render::tools::uniform_buffer_object);
		buffer_info.usage				   = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

		VmaAllocationCreateInfo alloc_info = {};
		alloc_info.usage				   = VMA_MEMORY_USAGE_CPU_TO_GPU;

		vmaCreateBuffer(
			rhr::render::renderer::vma_allocator,
			&buffer_info,
			&alloc_info,
			&m_uniform_buffer,
			&m_uniform_allocation,
			nullptr);

		// rhr::render::tools::create_buffer(
		//	sizeof(rhr::render::tools::uniform_buffer_object),
		//	VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		//	VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		//	m_uniform_buffer,
		//	m_uniform_buffer_memory);
		on_reload_swap_chain();
	}

	if (m_has_vertices)
	{
		if (m_vertex_buffer)
		{
			vmaDestroyBuffer(
				rhr::render::renderer::vma_allocator, m_vertex_staging_buffer, m_vertex_staging_buffer_allocation);
			vmaDestroyBuffer(rhr::render::renderer::vma_allocator, m_vertex_buffer, m_vertex_buffer_allocation);

			m_vertex_buffer			= nullptr;
			m_vertex_staging_buffer = nullptr;
		}

		vk::buffer_create_info buffer_info;
		VmaAllocationCreateInfo alloc_info;

		usize buffer_size = sizeof(rhr::render::vertex) * m_vertex_count;

		// Create vertex cpu buffer.

		buffer_info		  = {VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
		buffer_info.size  = buffer_size;
		buffer_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

		alloc_info		 = {};
		alloc_info.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;

		vmaCreateBuffer(
			rhr::render::renderer::vma_allocator,
			&buffer_info,
			&alloc_info,
			&m_vertex_staging_buffer,
			&m_vertex_staging_buffer_allocation,
			nullptr);

		// Create vertex gpu buffer.

		buffer_info		  = {VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
		buffer_info.size  = buffer_size;
		buffer_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

		alloc_info		 = {};
		alloc_info.usage = VMA_MEMORY_USAGE_GPU_ONLY;

		vmaCreateBuffer(
			rhr::render::renderer::vma_allocator,
			&buffer_info,
			&alloc_info,
			&m_vertex_buffer,
			&m_vertex_buffer_allocation,
			nullptr);

		// Move memory.

		void* data;
		vmaMapMemory(rhr::render::renderer::vma_allocator, m_vertex_staging_buffer_allocation, &data);
		memcpy(data, m_vertices, buffer_size);
		vmaUnmapMemory(rhr::render::renderer::vma_allocator, m_vertex_staging_buffer_allocation);

		rhr::render::tools::copy_buffer(m_vertex_staging_buffer, m_vertex_buffer, buffer_size);
	}

	if (m_has_indices)
	{
		if (m_index_buffer)
		{
			vmaDestroyBuffer(
				rhr::render::renderer::vma_allocator, m_index_staging_buffer, m_index_staging_buffer_allocation);
			vmaDestroyBuffer(rhr::render::renderer::vma_allocator, m_index_buffer, m_index_buffer_allocation);

			m_index_buffer		   = nullptr;
			m_index_staging_buffer = nullptr;
		}

		vk::buffer_create_info buffer_info;
		VmaAllocationCreateInfo alloc_info;

		usize buffer_size = sizeof(u32) * m_index_count;

		// Create index cpu buffer.

		buffer_info		  = {VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
		buffer_info.size  = buffer_size;
		buffer_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

		alloc_info		 = {};
		alloc_info.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;

		vmaCreateBuffer(
			rhr::render::renderer::vma_allocator,
			&buffer_info,
			&alloc_info,
			&m_index_staging_buffer,
			&m_index_staging_buffer_allocation,
			nullptr);

		// Create index gpu buffer.

		buffer_info		  = {VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
		buffer_info.size  = buffer_size;
		buffer_info.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

		alloc_info		 = {};
		alloc_info.usage = VMA_MEMORY_USAGE_GPU_ONLY;

		vmaCreateBuffer(
			rhr::render::renderer::vma_allocator,
			&buffer_info,
			&alloc_info,
			&m_index_buffer,
			&m_index_buffer_allocation,
			nullptr);

		// Move memory.

		void* data;
		vmaMapMemory(rhr::render::renderer::vma_allocator, m_index_staging_buffer_allocation, &data);
		memcpy(data, m_indices, buffer_size);
		vmaUnmapMemory(rhr::render::renderer::vma_allocator, m_index_staging_buffer_allocation);

		rhr::render::tools::copy_buffer(m_index_staging_buffer, m_index_buffer, buffer_size);
	}
}

void rhr::render::object::object::on_reload_swap_chain()
{
	if (m_init_image)
		return;

	VkDescriptorSetAllocateInfo allocInfo {};
	allocInfo.sType				 = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool	 = *rhr::render::renderer::get_window_primary()->get_descriptor_pool();
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts		 = rhr::render::renderer::get_window_primary()->get_descriptor_set_layout();

	vkAllocateDescriptorSets(*rhr::render::renderer::get_window_primary()->get_device(), &allocInfo, &m_descriptor_set);

	VkDescriptorBufferInfo bufferInfo {};
	bufferInfo.buffer = m_uniform_buffer;
	bufferInfo.offset = 0;
	bufferInfo.range  = sizeof(rhr::render::tools::uniform_buffer_object);

	if (m_has_texture)
	{
		VkDescriptorImageInfo imageInfo {};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView	  = m_image_view;
		imageInfo.sampler	  = *rhr::render::renderer::get_window_primary()->get_texture_sampler();

		std::array<VkWriteDescriptorSet, 2> descriptorWrites {};

		descriptorWrites[0].sType			= VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet			= m_descriptor_set;
		descriptorWrites[0].dstBinding		= 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType	= VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo		= &bufferInfo;

		descriptorWrites[1].sType			= VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[1].dstSet			= m_descriptor_set;
		descriptorWrites[1].dstBinding		= 1;
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType	= VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pImageInfo		= &imageInfo;

		vkUpdateDescriptorSets(
			*rhr::render::renderer::get_window_primary()->get_device(),
			static_cast<u32>(descriptorWrites.size()),
			descriptorWrites.data(),
			0,
			nullptr);
	}
	else
	{
		std::array<VkWriteDescriptorSet, 1> descriptorWrites {};

		descriptorWrites[0].sType			= VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet			= m_descriptor_set;
		descriptorWrites[0].dstBinding		= 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType	= VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo		= &bufferInfo;

		vkUpdateDescriptorSets(
			*rhr::render::renderer::get_window_primary()->get_device(),
			static_cast<u32>(descriptorWrites.size()),
			descriptorWrites.data(),
			0,
			nullptr);
	}
}

void rhr::render::object::object::post_position_update()
{
	// mark_dirty();
}

void rhr::render::object::object::update_uniforms(bool ui)
{
	// static auto startTime = std::chrono::high_resolution_clock::now();
	//
	// auto currentTime = std::chrono::high_resolution_clock::now();
	// f32 time = std::chrono::duration<f32, std::chrono::seconds::period>(currentTime - startTime).count();

	glm::vec<3, f32> position = {
		static_cast<f32>(m_position.x + m_super_position.x),
		static_cast<f32>(m_position.y + m_super_position.y),
		static_cast<f32>(m_position.z + m_super_position.z)};

	if (m_offset != nullptr)
	{
		position.x += static_cast<f32>(m_offset->x);
		position.y += static_cast<f32>(m_offset->y);
	}

	rhr::render::tools::uniform_buffer_object ubo {};
	ubo.model = glm::translate(glm::mat4(1.0f), {position.x, position.y, -1.0f * position.z});
	// ubo.Model = glm::rotate(ubo.Model, time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	// ubo.View = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	// ubo.projection = glm::perspective(glm::radians(45.0f), rhr::render::renderer::SwapChainExtent.width /
	// (f32)rhr::render::renderer::SwapChainExtent.height, 0.1f, 10.0f);
	ubo.view = rhr::render::renderer::view_matrix;

	if (ui)
		ubo.projection = rhr::render::renderer::ui_projection_matrix;
	else
	{
		ubo.projection = rhr::render::renderer::projection_matrix;
		ubo.projection[1][1] *= -1;
	}

	ubo.color = {1.0f, 1.0f, 1.0f};

	// Move memory.

	void* data;
	vmaMapMemory(rhr::render::renderer::vma_allocator, m_uniform_allocation, &data);
	memcpy(data, &ubo, sizeof(ubo));
	vmaUnmapMemory(rhr::render::renderer::vma_allocator, m_uniform_allocation);
}

void rhr::render::object::object::set_queue(u8 queue)
{
	m_queue = queue;
}
