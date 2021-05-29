#include "RenderRectangle.h"
#include "../Renderer.h"

vui::RenderRectangle::RenderRectangle(bool disableDirty)
    : m_Position({ 0, 0 })
    , m_Size({ 0, 0 })
    , m_ParentPosition({ 0, 0 })
    , m_ParentSize({ 100, 100 })
    , m_Color({ 1.0f, 1.0f, 1.0f })
    , m_Depth(10)
    , m_Enabled(false)
    , m_HasColor(false)
    , m_HasTexture(false)
    , m_Dirty(false)
	, m_DisableDirty(disableDirty)
    , m_RenderObject(std::make_shared<RenderObject>())
{
	m_RenderObject->SetWeak(m_RenderObject);
}

void vui::RenderRectangle::SetWeak(std::weak_ptr<RenderRectangle>&& weak)
{
	m_Weak = std::move(weak);
}

void vui::RenderRectangle::SetPosition(glm::vec<2, int32_t> position)
{
	m_Position = position;
	MarkDirty();
}

void vui::RenderRectangle::SetSize(glm::vec<2, int32_t> size)
{
	m_Size = size;
	MarkDirty();
}

void vui::RenderRectangle::SetColor(glm::vec<3, float> color)
{
	m_Color = color;
	m_HasColor = true;
	MarkDirty();
}

void vui::RenderRectangle::SetTexture(const std::string& texture)
{
	m_Texture = texture;
	m_RenderObject->SetTexture(texture);
	m_HasTexture = true;
	MarkDirty();
}

void vui::RenderRectangle::SetDepth(uint32_t depth)
{
	m_Depth = depth;
	MarkDirty();
}

void vui::RenderRectangle::SetEnabled(bool enabled)
{
	m_Enabled = enabled;
	// dont need to mark dirty
}

void vui::RenderRectangle::SetParent(glm::vec<2, int32_t> position, glm::vec<2, int32_t> size)
{
	m_ParentPosition = position;
	m_ParentSize = size;
	MarkDirty();
}

void vui::RenderRectangle::Render()
{
	if (m_Enabled)
	{
		if (m_HasTexture)
			vkCmdBindPipeline(Renderer::ActiveCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, Renderer::UITexturePipeline);
		else
			vkCmdBindPipeline(Renderer::ActiveCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, Renderer::UIPipeline);

		m_RenderObject->Render(true);
	}
}

void vui::RenderRectangle::UpdateBuffers()
{
	m_Dirty = false;

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	vertices.reserve(4);
	indices.reserve(6);

	glm::vec<2, int32_t> position = m_Position + m_ParentPosition;

	Vertex v0 = Vertex({ static_cast<float>(position.x), static_cast<float>(position.y), static_cast<int32_t>(m_Depth) * -1 }, m_Color, { 0.0f, 0.0f });
	Vertex v1 = Vertex({ static_cast<float>(position.x + m_Size.x), static_cast<float>(position.y), static_cast<int32_t>(m_Depth) * -1 }, m_Color, { 1.0f, 0.0f });
	Vertex v2 = Vertex({ static_cast<float>(position.x + m_Size.x), static_cast<float>(position.y + m_Size.y), static_cast<int32_t>(m_Depth) * -1 }, m_Color, { 1.0f, 1.0f });
	Vertex v3 = Vertex({ static_cast<float>(position.x), static_cast<float>(position.y + m_Size.y), static_cast<int32_t>(m_Depth) * -1 }, m_Color, { 0.0f, 1.0f });

	vertices.push_back(v0);
	vertices.push_back(v1);
	vertices.push_back(v2);
	vertices.push_back(v3);

	indices.push_back(1);
	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(2);
	indices.push_back(0);
	indices.push_back(3);

	m_RenderObject->UpdateVertices(&vertices, &indices, true);
}

void vui::RenderRectangle::ReloadSwapChain()
{
	m_RenderObject->InitDescriptorSet();
}

void vui::RenderRectangle::MarkDirty()
{
	if (m_Dirty || m_DisableDirty)
		return;

	m_Dirty = true;
	Renderer::AddDirtyUI(m_Weak);
}