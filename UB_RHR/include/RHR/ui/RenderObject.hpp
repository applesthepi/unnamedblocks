#pragma once
#include "config.h"

#include "ui/Vertex.hpp"
#include "ui/interfaces/IRenderable.hpp"
#include "ui/interfaces/IEnableable.hpp"

#include <Cappuccino/Utils.hpp>

#define RENDER_OBJECT_ALLOC_FACTOR static_cast<size_t>(2)

class RenderObject : public IRenderable, public IEnableable
{
public:
	RenderObject(bool ui);
	RenderObject(bool ui, const std::string& texturePath);

	//void SetWeak(std::weak_ptr<IRenderable>&& weak);
	void SetPosition(const glm::vec<3, double>& position);
	void SetTexture(const std::string& texture);
	void UpdateVertices(const std::vector<Vertex>* vertices, const std::vector<uint32_t>* indices, bool updateBuffersNow = false);
	void UpdateVertices(const Vertex* vertices, uint32_t vertexCount, const uint32_t* indices, uint32_t indexCount, bool updateBuffersNow = false);
	void UpdateVertices(const Vertex* vertices, uint32_t vertexCount, bool updateBuffersNow = false);

	void SetQueue(uint8_t queue);
	
	/// Add draw calls to cmd buffer prebound by Renderer.
	void OnRender() override;

	/// Updates mesh on cpu side.
	void OnUpdateBuffers() override;

	/// Regenerates descriptor sets including uniforms.
	void OnReloadSwapChain() override;

private:
	void UpdateUniforms(bool ui);
	//void MarkDirty();

	bool m_HasVertexBufferData;
	bool m_HasIndexBufferData;

	size_t m_VertexCount;
	size_t m_IndexCount;

	size_t m_VertexAllocCount;
	size_t m_IndexAllocCount;

	Vertex* m_Vertices;
	uint32_t* m_Indices;

	VkBuffer m_VertexBuffer;
	VkDeviceMemory m_VertexBufferMemory;

	VkBuffer m_VertexStagingBuffer;
	VkDeviceMemory m_VertexStagingBufferMemory;

	VkBuffer m_IndexBuffer;
	VkDeviceMemory m_IndexBufferMemory;

	VkBuffer m_IndexStagingBuffer;
	VkDeviceMemory m_IndexStagingBufferMemory;

	VkBuffer m_UniformBuffer;
	VkDeviceMemory m_UniformBufferMemory;

	VkImage m_Image;
	VkImageView m_ImageView;
	VkDeviceMemory m_ImageMemory;

	VkDescriptorSet m_DescriptorSet;
	uint8_t m_Queue;

	bool m_Dirty;
	bool m_InitImage;
	bool m_UI;
	//bool m_HasWeak;
	std::string m_TexturePath;
	glm::vec<3, double> m_Position;
	//std::weak_ptr<IRenderable> m_Weak;
};