#include "object.hpp"

#include "rhr/rendering/tools.hpp"
#include "rhr/rendering/renderer.hpp"
#include "rhr/registries/char_texture.hpp"

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
    , m_vertex_buffer_memory(nullptr)
    , m_vertex_staging_buffer(nullptr)
    , m_vertex_staging_buffer_memory(nullptr)
    , m_index_buffer(nullptr)
    , m_index_buffer_memory(nullptr)
    , m_index_staging_buffer(nullptr)
    , m_index_staging_buffer_memory(nullptr)
	, m_texture_type(texture_type::CUSTOM)
{
}

rhr::render::object::object::object(bool ui, const std::string& texture_path)
    : object(ui)
{
	m_texture_path = texture_path;
}

//void object::SetWeak(std::weak_ptr<IRenderable>&& weak)
//{
//	m_Weak = std::move(weak);
//	m_HasWeak = true;
//}

//void object::SetPosition(const glm::vec<3, f64>& position)
//{
//	m_Position = position;
//}

void rhr::render::object::object::set_texture(const std::string& texture)
{
	m_has_texture = true;
	m_texture_path = texture;
	m_texture_type = texture_type::CUSTOM;
}

void rhr::render::object::object::set_texture_char(rhr::registry::char_texture::texture_type texture_type)
{
	m_has_texture = true;
	m_texture_path.clear();
	m_texture_type = texture_type::TEXT_SHEET;
	m_char_texture_type = texture_type;
}

void rhr::render::object::object::update_vertices(const std::vector<rhr::render::vertex>* vertices, const std::vector<u32>* indices, bool update_buffers_now)
{
	if (vertices->size() < 3 || indices->size() < 3)
	{
		Logger::Error("object does not support less then 3 vertices or indices");
		return;
	}

	usize vertices_size = sizeof(rhr::render::vertex) * vertices->size();
	usize indices_size = sizeof(u32) * indices->size();

	if (vertices->size() > m_vertex_alloc_count)
	{
		rhr::render::vertex* staged_vertices = new rhr::render::vertex[vertices->size() * RENDER_OBJECT_ALLOC_FACTOR];

		if (!staged_vertices)
			Logger::Fatal("out of memory");

		memcpy(staged_vertices, vertices->data(), vertices_size);

		m_vertex_count = vertices->size();
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
			Logger::Fatal("out of memory");

		memcpy(staged_indices, indices->data(), indices_size);

		m_index_count = indices->size();
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

void rhr::render::object::object::update_vertices(const rhr::render::vertex* vertices, u32 vertex_count, const u32* indices, u32 index_count, bool update_buffers_now)
{
	if (vertex_count < 3 || index_count < 3)
	{
		Logger::Error("object does not support less then 3 vertices or indices");
		return;
	}

	usize vertices_size = sizeof(rhr::render::vertex) * vertex_count;
	usize indices_size = sizeof(u32) * index_count;

	if (vertex_count > m_vertex_alloc_count)
	{
		rhr::render::vertex* staged_vertices = new rhr::render::vertex[vertex_count * RENDER_OBJECT_ALLOC_FACTOR];

		if (!staged_vertices)
			Logger::Fatal("out of memory");

		memcpy(staged_vertices, vertices, vertices_size);

		m_vertex_count = vertex_count;
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
			Logger::Fatal("out of memory");

		memcpy(staged_indices, indices, indices_size);

		m_index_count = index_count;
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

void rhr::render::object::object::update_vertices(const rhr::render::vertex* vertices, u32 vertex_count, bool update_buffers_now)
{
	if (vertex_count < 3)
	{
		Logger::Error("object does not support less then 3 vertices or indices");
		return;
	}

	usize vertices_size = sizeof(rhr::render::vertex) * vertex_count;

	if (vertex_count > m_vertex_alloc_count)
	{
		rhr::render::vertex* staged_vertices = new rhr::render::vertex[vertex_count * RENDER_OBJECT_ALLOC_FACTOR];

		if (!staged_vertices)
			Logger::Fatal("out of memory");

		memcpy(staged_vertices, vertices, vertices_size);

		m_vertex_count = vertex_count;
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
	if (!m_has_vertices)
		return;

	update_uniforms(m_ui);

	vk::buffer vb[] = { m_vertex_buffer };
	vk::device_size offsets[] = { 0 };

	if (m_ui)
	{
		if (m_has_texture)
		{
			vkCmdBindDescriptorSets(rhr::render::renderer::active_command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, rhr::render::renderer::ui_texture_pipeline_layout, 0, 1, &m_descriptor_set, 0, nullptr);
			vkCmdBindPipeline(rhr::render::renderer::active_command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, rhr::render::renderer::ui_texture_pipeline);
		}
		else
		{
			vkCmdBindDescriptorSets(rhr::render::renderer::active_command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, rhr::render::renderer::ui_pipeline_layout, 0, 1, &m_descriptor_set, 0, nullptr);
			vkCmdBindPipeline(rhr::render::renderer::active_command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, rhr::render::renderer::ui_pipeline);
		}
	}
	else
	{
		vkCmdBindDescriptorSets(rhr::render::renderer::active_command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, rhr::render::renderer::blocks_pipeline_layout, 0, 1, &m_descriptor_set, 0, nullptr);
		vkCmdBindPipeline(rhr::render::renderer::active_command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, rhr::render::renderer::ui_pipeline);
	}

	vkCmdBindVertexBuffers(rhr::render::renderer::active_command_buffer, 0, 1, vb, offsets);

	if (m_has_indices)
	{
		vkCmdBindIndexBuffer(rhr::render::renderer::active_command_buffer, m_index_buffer, 0, VK_INDEX_TYPE_UINT32);
		vkCmdDrawIndexed(rhr::render::renderer::active_command_buffer, static_cast<u32>(m_index_count), 1, 0, 0, 0);
	}
	else
		vkCmdDraw(rhr::render::renderer::active_command_buffer, static_cast<u32>(m_vertex_count), 1, 0, 0);
}

void rhr::render::object::object::on_update_buffers()
{
	m_dirty = false;
	m_has_vertices = true;

	if (m_init_image)
	{
		m_init_image = false;

		if (m_has_texture)
		{
			if (m_texture_type == texture_type::CUSTOM)
			{
				m_image = rhr::render::tools::create_texture_image(m_texture_path, &m_image_memory);
				m_image_view = rhr::render::tools::create_image_view(m_image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
			}
			else if (m_texture_type == texture_type::TEXT_SHEET)
			{
				m_image = rhr::registry::char_texture::texture_map[m_char_texture_type].image;
				m_image_view = rhr::render::tools::create_image_view(m_image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
			}
		}

		rhr::render::tools::create_buffer(sizeof(rhr::render::tools::uniform_buffer_object), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_uniform_buffer, m_uniform_buffer_memory);
		on_reload_swap_chain();
	}

	if (m_has_vertices)
	{
		if (m_vertex_buffer)
		{
			rhr::render::tools::delete_buffer(m_vertex_buffer, m_vertex_buffer_memory);
			rhr::render::tools::delete_buffer(m_vertex_staging_buffer, m_vertex_staging_buffer_memory);

			m_vertex_buffer = nullptr;
			m_vertex_buffer_memory = nullptr;

			m_vertex_staging_buffer = nullptr;
			m_vertex_staging_buffer_memory = nullptr;
		}

		VkDeviceSize buffer_size = sizeof(rhr::render::vertex) * m_vertex_count;

		rhr::render::tools::create_buffer(buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_vertex_staging_buffer, m_vertex_staging_buffer_memory);

		void* data;
		vkMapMemory(rhr::render::renderer::device, m_vertex_staging_buffer_memory, 0, buffer_size, 0, &data);
		memcpy(data, m_vertices, (usize)buffer_size);
		vkUnmapMemory(rhr::render::renderer::device, m_vertex_staging_buffer_memory);

		rhr::render::tools::create_buffer(buffer_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_vertex_buffer, m_vertex_buffer_memory);
		rhr::render::tools::copy_buffer(m_vertex_staging_buffer_memory, m_vertex_staging_buffer, m_vertex_buffer, buffer_size);
	}

	if (m_has_indices)
	{
		if (m_index_buffer)
		{
			rhr::render::tools::delete_buffer(m_index_buffer, m_index_buffer_memory);
			rhr::render::tools::delete_buffer(m_index_staging_buffer, m_index_staging_buffer_memory);

			m_index_buffer = nullptr;
			m_index_buffer_memory = nullptr;

			m_index_staging_buffer = nullptr;
			m_index_staging_buffer_memory = nullptr;
		}

		VkDeviceSize buffer_size = sizeof(u32) * m_index_count;

		rhr::render::tools::create_buffer(buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_index_staging_buffer, m_index_staging_buffer_memory);

		void* data;
		vkMapMemory(rhr::render::renderer::device, m_index_staging_buffer_memory, 0, buffer_size, 0, &data);
		memcpy(data, m_indices, (usize)buffer_size);
		vkUnmapMemory(rhr::render::renderer::device, m_index_staging_buffer_memory);

		rhr::render::tools::create_buffer(buffer_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_index_buffer, m_index_buffer_memory);
		rhr::render::tools::copy_buffer(m_index_staging_buffer_memory, m_index_staging_buffer, m_index_buffer, buffer_size);
	}
}

void rhr::render::object::object::on_reload_swap_chain()
{
	if (m_init_image)
		return;

	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = rhr::render::renderer::descriptor_pool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = &rhr::render::renderer::descriptor_set_layout;

	vkAllocateDescriptorSets(rhr::render::renderer::device, &allocInfo, &m_descriptor_set);

	VkDescriptorBufferInfo bufferInfo{};
	bufferInfo.buffer = m_uniform_buffer;
	bufferInfo.offset = 0;
	bufferInfo.range = sizeof(rhr::render::tools::uniform_buffer_object);

	if (m_has_texture)
	{
		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = m_image_view;
		imageInfo.sampler = rhr::render::renderer::texture_sampler;

		std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = m_descriptor_set;
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &bufferInfo;

		descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[1].dstSet = m_descriptor_set;
		descriptorWrites[1].dstBinding = 1;
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pImageInfo = &imageInfo;

		vkUpdateDescriptorSets(rhr::render::renderer::device, static_cast<u32>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
	else
	{
		std::array<VkWriteDescriptorSet, 1> descriptorWrites{};

		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = m_descriptor_set;
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &bufferInfo;

		vkUpdateDescriptorSets(rhr::render::renderer::device, static_cast<u32>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
}

void rhr::render::object::object::post_position_update()
{
	//mark_dirty();
}

void rhr::render::object::object::update_uniforms(bool ui)
{
	//static auto startTime = std::chrono::high_resolution_clock::now();
	//
	//auto currentTime = std::chrono::high_resolution_clock::now();
	//f32 time = std::chrono::duration<f32, std::chrono::seconds::period>(currentTime - startTime).count();

	rhr::render::tools::uniform_buffer_object ubo {};
	ubo.model = glm::translate(glm::mat4(1.0f), { static_cast<f32>(m_position.x + m_super_position.x), static_cast<f32>(m_position.y + m_super_position.y), -1.0f * static_cast<f32>(m_position.z + m_super_position.z) });
	//ubo.Model = glm::rotate(ubo.Model, time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	//ubo.View = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	//ubo.projection = glm::perspective(glm::radians(45.0f), rhr::render::renderer::SwapChainExtent.width / (f32)rhr::render::renderer::SwapChainExtent.height, 0.1f, 10.0f);
	ubo.view = rhr::render::renderer::view_matrix;

	if (ui)
		ubo.projection = rhr::render::renderer::ui_projection_matrix;
	else
	{
		ubo.projection = rhr::render::renderer::projection_matrix;
		ubo.projection[1][1] *= -1;
	}

	//ubo.Color = { 1.0f, 0.0f, static_cast<f32>(m_Index) };
	ubo.color = { 1.0f, 1.0f, 1.0f };

	void* data;
	vkMapMemory(rhr::render::renderer::device, m_uniform_buffer_memory, 0, sizeof(ubo), 0, &data);
	memcpy(data, &ubo, sizeof(ubo));
	vkUnmapMemory(rhr::render::renderer::device, m_uniform_buffer_memory);
}

void rhr::render::object::object::set_queue(u8 queue)
{
	m_queue = queue;
}