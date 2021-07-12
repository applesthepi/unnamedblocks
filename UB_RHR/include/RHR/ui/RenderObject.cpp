#include "RenderObject.hpp"

#include "ui/RenderTools.hpp"
#include "ui/Renderer.hpp"

RenderObject::RenderObject(bool ui)
    : IEnableable(true)
	, m_HasVertexBufferData(false)
    , m_HasIndexBufferData(false)
    , m_VertexCount(0)
    , m_IndexCount(0)
    , m_VertexAllocCount(0)
    , m_IndexAllocCount(0)
    , m_Vertices(nullptr)
    , m_Indices(nullptr)
    , m_Queue(0)
    , m_Dirty(false)
    , m_InitImage(true)
	//, m_HasWeak(false)
	, m_UI(ui)
    , m_TexturePath("")
    //, m_Position({ 0.0, 0.0, 0.0 })
    , m_VertexBuffer(nullptr)
    , m_VertexBufferMemory(nullptr)
    , m_VertexStagingBuffer(nullptr)
    , m_VertexStagingBufferMemory(nullptr)
    , m_IndexBuffer(nullptr)
    , m_IndexBufferMemory(nullptr)
    , m_IndexStagingBuffer(nullptr)
    , m_IndexStagingBufferMemory(nullptr)
{
}

RenderObject::RenderObject(bool ui, const std::string& texturePath)
    : RenderObject(ui)
{
	m_TexturePath = texturePath;
}

//void RenderObject::SetWeak(std::weak_ptr<IRenderable>&& weak)
//{
//	m_Weak = std::move(weak);
//	m_HasWeak = true;
//}

//void RenderObject::SetPosition(const glm::vec<3, double>& position)
//{
//	m_Position = position;
//}

void RenderObject::SetTexture(const std::string& texture)
{
	m_TexturePath = texture;
}

void RenderObject::UpdateVertices(const std::vector<Vertex>* vertices, const std::vector<uint32_t>* indices, bool updateBuffersNow)
{
	if (vertices->size() < 3 || indices->size() < 3)
	{
		Logger::Error("RenderObject does not support less then 3 vertices or indices");
		return;
	}

	size_t verticesSize = sizeof(Vertex) * vertices->size();
	size_t indicesSize = sizeof(uint32_t) * indices->size();

	if (vertices->size() > m_VertexAllocCount)
	{
		Vertex* stagedVertices = new Vertex[vertices->size() * RENDER_OBJECT_ALLOC_FACTOR];

		if (!stagedVertices)
			Logger::Fatal("out of memory");

		memcpy(stagedVertices, vertices->data(), verticesSize);

		m_VertexCount = vertices->size();
		m_VertexAllocCount = vertices->size() * RENDER_OBJECT_ALLOC_FACTOR;

		if (m_Vertices != nullptr)
			delete[] m_Vertices;

		m_Vertices = stagedVertices;
	}
	else
	{
		memcpy(m_Vertices, vertices->data(), verticesSize);
		m_VertexCount = vertices->size();
	}

	if (indices->size() > m_IndexAllocCount)
	{
		uint32_t* stagedIndices = new uint32_t[indices->size() * RENDER_OBJECT_ALLOC_FACTOR];

		if (!stagedIndices)
			Logger::Fatal("out of memory");

		memcpy(stagedIndices, indices->data(), indicesSize);

		m_IndexCount = indices->size();
		m_IndexAllocCount = indices->size() * RENDER_OBJECT_ALLOC_FACTOR;

		if (m_Indices != nullptr)
			delete[] m_Indices;

		m_Indices = stagedIndices;
	}
	else
	{
		memcpy(m_Indices, indices->data(), indicesSize);
		m_IndexCount = indices->size();
	}

	m_HasIndexBufferData = true;

	if (updateBuffersNow)
		UpdateBuffers();
	else
		MarkDirty();
}

void RenderObject::UpdateVertices(const Vertex* vertices, uint32_t vertexCount, const uint32_t* indices, uint32_t indexCount, bool updateBuffersNow)
{
	if (vertexCount < 3 || indexCount < 3)
	{
		Logger::Error("RenderObject does not support less then 3 vertices or indices");
		return;
	}

	size_t verticesSize = sizeof(Vertex) * vertexCount;
	size_t indicesSize = sizeof(uint32_t) * indexCount;

	if (vertexCount > m_VertexAllocCount)
	{
		Vertex* stagedVertices = new Vertex[vertexCount * RENDER_OBJECT_ALLOC_FACTOR];

		if (!stagedVertices)
			Logger::Fatal("out of memory");

		memcpy(stagedVertices, vertices, verticesSize);

		m_VertexCount = vertexCount;
		m_VertexAllocCount = vertexCount * RENDER_OBJECT_ALLOC_FACTOR;

		if (m_Vertices != nullptr)
			delete[] m_Vertices;

		m_Vertices = stagedVertices;
	}
	else
	{
		memcpy(m_Vertices, vertices, verticesSize);
		m_VertexCount = vertexCount;
	}

	if (indexCount > m_IndexAllocCount)
	{
		uint32_t* stagedIndices = new uint32_t[indexCount * RENDER_OBJECT_ALLOC_FACTOR];

		if (!stagedIndices)
			Logger::Fatal("out of memory");

		memcpy(stagedIndices, indices, indicesSize);

		m_IndexCount = indexCount;
		m_IndexAllocCount = indexCount * RENDER_OBJECT_ALLOC_FACTOR;

		if (m_Indices != nullptr)
			delete[] m_Indices;

		m_Indices = stagedIndices;
	}
	else
	{
		memcpy(m_Indices, indices, indicesSize);
		m_IndexCount = indexCount;
	}

	m_HasIndexBufferData = true;

	if (updateBuffersNow)
		UpdateBuffers();
	else
		MarkDirty();
}

void RenderObject::UpdateVertices(const Vertex* vertices, uint32_t vertexCount, bool updateBuffersNow)
{
	if (vertexCount < 3)
	{
		Logger::Error("RenderObject does not support less then 3 vertices or indices");
		return;
	}

	size_t verticesSize = sizeof(Vertex) * vertexCount;

	if (vertexCount > m_VertexAllocCount)
	{
		Vertex* stagedVertices = new Vertex[vertexCount * RENDER_OBJECT_ALLOC_FACTOR];

		if (!stagedVertices)
			Logger::Fatal("out of memory");

		memcpy(stagedVertices, vertices, verticesSize);

		m_VertexCount = vertexCount;
		m_VertexAllocCount = vertexCount * RENDER_OBJECT_ALLOC_FACTOR;

		if (m_Vertices != nullptr)
			delete[] m_Vertices;

		m_Vertices = stagedVertices;
	}
	else
	{
		memcpy(m_Vertices, vertices, verticesSize);
		m_VertexCount = vertexCount;
	}

	m_HasIndexBufferData = false;

	if (updateBuffersNow)
		UpdateBuffers();
	else
		MarkDirty();
}

void RenderObject::OnRender()
{
	if (!m_HasVertexBufferData)
		return;

	UpdateUniforms(m_UI);

	VkBuffer vb[] = { m_VertexBuffer };
	VkDeviceSize offsets[] = { 0 };

	if (m_UI)
	{
		if (m_TexturePath.size() > 0)
		{
			vkCmdBindDescriptorSets(Renderer::ActiveCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, Renderer::UITexturePipelineLayout, 0, 1, &m_DescriptorSet, 0, nullptr);
			vkCmdBindPipeline(Renderer::ActiveCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, Renderer::UITexturePipeline);
		}
		else
		{
			vkCmdBindDescriptorSets(Renderer::ActiveCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, Renderer::UIPipelineLayout, 0, 1, &m_DescriptorSet, 0, nullptr);
			vkCmdBindPipeline(Renderer::ActiveCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, Renderer::UIPipeline);
		}
	}
	else
	{
		vkCmdBindDescriptorSets(Renderer::ActiveCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, Renderer::BlocksPipelineLayout, 0, 1, &m_DescriptorSet, 0, nullptr);
		vkCmdBindPipeline(Renderer::ActiveCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, Renderer::UIPipeline);
	}

	vkCmdBindVertexBuffers(Renderer::ActiveCommandBuffer, 0, 1, vb, offsets);

	if (m_HasIndexBufferData)
	{
		vkCmdBindIndexBuffer(Renderer::ActiveCommandBuffer, m_IndexBuffer, 0, VK_INDEX_TYPE_UINT32);
		vkCmdDrawIndexed(Renderer::ActiveCommandBuffer, static_cast<uint32_t>(m_IndexCount), 1, 0, 0, 0);
	}
	else
		vkCmdDraw(Renderer::ActiveCommandBuffer, static_cast<uint32_t>(m_VertexCount), 1, 0, 0);
}

void RenderObject::OnUpdateBuffers()
{
	m_Dirty = false;
	m_HasVertexBufferData = true;

	if (m_InitImage)
	{
		m_InitImage = false;

		if (m_TexturePath.size() > 0)
		{
			m_Image = RenderTools::CreateTextureImage(m_TexturePath, &m_ImageMemory);
			m_ImageView = RenderTools::CreateImageView(m_Image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
		}

		RenderTools::CreateBuffer(sizeof(UniformBufferObject), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_UniformBuffer, m_UniformBufferMemory);
		OnReloadSwapChain();
	}

	if (m_HasVertexBufferData)
	{
		if (m_VertexBuffer)
		{
			RenderTools::DeleteBuffer(m_VertexBuffer, m_VertexBufferMemory);
			RenderTools::DeleteBuffer(m_VertexStagingBuffer, m_VertexStagingBufferMemory);

			m_VertexBuffer = nullptr;
			m_VertexBufferMemory = nullptr;

			m_VertexStagingBuffer = nullptr;
			m_VertexStagingBufferMemory = nullptr;
		}

		VkDeviceSize bufferSize = sizeof(Vertex) * m_VertexCount;

		RenderTools::CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_VertexStagingBuffer, m_VertexStagingBufferMemory);

		void* data;
		vkMapMemory(Renderer::Device, m_VertexStagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, m_Vertices, (size_t)bufferSize);
		vkUnmapMemory(Renderer::Device, m_VertexStagingBufferMemory);

		RenderTools::CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_VertexBuffer, m_VertexBufferMemory);
		RenderTools::CopyBuffer(m_VertexStagingBufferMemory, m_VertexStagingBuffer, m_VertexBuffer, bufferSize);
	}

	if (m_HasIndexBufferData)
	{
		if (m_IndexBuffer)
		{
			RenderTools::DeleteBuffer(m_IndexBuffer, m_IndexBufferMemory);
			RenderTools::DeleteBuffer(m_IndexStagingBuffer, m_IndexStagingBufferMemory);

			m_IndexBuffer = nullptr;
			m_IndexBufferMemory = nullptr;

			m_IndexStagingBuffer = nullptr;
			m_IndexStagingBufferMemory = nullptr;
		}

		VkDeviceSize bufferSize = sizeof(uint32_t) * m_IndexCount;

		RenderTools::CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_IndexStagingBuffer, m_IndexStagingBufferMemory);

		void* data;
		vkMapMemory(Renderer::Device, m_IndexStagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, m_Indices, (size_t)bufferSize);
		vkUnmapMemory(Renderer::Device, m_IndexStagingBufferMemory);

		RenderTools::CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_IndexBuffer, m_IndexBufferMemory);
		RenderTools::CopyBuffer(m_IndexStagingBufferMemory, m_IndexStagingBuffer, m_IndexBuffer, bufferSize);
	}
}

void RenderObject::OnReloadSwapChain()
{
	if (m_InitImage)
		return;

	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = Renderer::DescriptorPool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = &Renderer::DescriptorSetLayout;

	vkAllocateDescriptorSets(Renderer::Device, &allocInfo, &m_DescriptorSet);

	VkDescriptorBufferInfo bufferInfo{};
	bufferInfo.buffer = m_UniformBuffer;
	bufferInfo.offset = 0;
	bufferInfo.range = sizeof(UniformBufferObject);

	if (m_TexturePath.size() > 0)
	{
		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = m_ImageView;
		imageInfo.sampler = Renderer::TextureSampler;

		std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = m_DescriptorSet;
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &bufferInfo;

		descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[1].dstSet = m_DescriptorSet;
		descriptorWrites[1].dstBinding = 1;
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pImageInfo = &imageInfo;

		vkUpdateDescriptorSets(Renderer::Device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
	else
	{
		std::array<VkWriteDescriptorSet, 1> descriptorWrites{};

		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = m_DescriptorSet;
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &bufferInfo;

		vkUpdateDescriptorSets(Renderer::Device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
}

void RenderObject::UpdateUniforms(bool ui)
{
	//static auto startTime = std::chrono::high_resolution_clock::now();
	//
	//auto currentTime = std::chrono::high_resolution_clock::now();
	//float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	UniformBufferObject ubo {};
	ubo.Model = glm::translate(glm::mat4(1.0f), { static_cast<float>(m_Position.x + m_SuperOffset.x), static_cast<float>(m_Position.y + m_SuperOffset.y), static_cast<float>(m_Position.z + m_SuperOffset.z) });
	//ubo.Model = glm::rotate(ubo.Model, time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	//ubo.View = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	//ubo.Projection = glm::perspective(glm::radians(45.0f), Renderer::SwapChainExtent.width / (float)Renderer::SwapChainExtent.height, 0.1f, 10.0f);
	ubo.View = Renderer::ViewMatrix;

	if (ui)
		ubo.Projection = Renderer::UIProjectionMatrix;
	else
	{
		ubo.Projection = Renderer::ProjectionMatrix;
		ubo.Projection[1][1] *= -1;
	}

	//ubo.Color = { 1.0f, 0.0f, static_cast<float>(m_Index) };
	ubo.Color = { 1.0f, 1.0f, 1.0f };

	void* data;
	vkMapMemory(Renderer::Device, m_UniformBufferMemory, 0, sizeof(ubo), 0, &data);
	memcpy(data, &ubo, sizeof(ubo));
	vkUnmapMemory(Renderer::Device, m_UniformBufferMemory);
}

void RenderObject::SetQueue(uint8_t queue)
{
	m_Queue = queue;
}

//void RenderObject::MarkDirty()
//{
//	if (m_Dirty || !IsWeak())
//		return;
//
//	m_Dirty = true;
//	Renderer::AddDirty(m_Weak);
//}