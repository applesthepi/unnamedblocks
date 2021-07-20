#include "RenderText.hpp"

#include "ui/Renderer.hpp"
#include "ui/Vertex.hpp"
#include "stacking/Block.hpp"

vui::RenderText::RenderText()
	: IDiColorable(Color().FromNormalized({ 0.0f, 0.0f, 0.0f, 1.0f }), Color().FromU8({ 25, 25, 25, 255 }))
	, IEnableable(true)
	, m_Depth(10)
	, m_RenderObject(std::make_shared<RenderObject>(true))
{
	m_RenderObject->SetWeak(m_RenderObject);
}

void vui::RenderText::SetText(const std::string& text)
{
	m_Text = text;
	m_Size = { text.size() * 10, Block::Height - (Block::Padding * 2) };
	MarkDirty();
}

void vui::RenderText::SetDepth(uint32_t depth)
{
	m_Depth = depth;
	MarkDirty();
}

void vui::RenderText::OnRender()
{
	if (m_Enabled)
	{
		vkCmdBindPipeline(Renderer::ActiveCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, Renderer::UIPipeline);
		m_RenderObject->Render();
	}
}

void vui::RenderText::OnUpdateBuffers()
{
	ClearDirty();

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	vertices.reserve(4);
	indices.reserve(6);

	glm::vec<2, int32_t> position = m_Position + m_SuperOffset;

	Vertex v0 = Vertex({ static_cast<float>(position.x), static_cast<float>(position.y), static_cast<int32_t>(m_Depth) * -1 }, m_ColorSecondary.GetNormalized(), { 0.0f, 0.0f });
	Vertex v1 = Vertex({ static_cast<float>(position.x + m_Size.x), static_cast<float>(position.y), static_cast<int32_t>(m_Depth) * -1 }, m_ColorSecondary.GetNormalized(), { 1.0f, 0.0f });
	Vertex v2 = Vertex({ static_cast<float>(position.x + m_Size.x), static_cast<float>(position.y + m_Size.y), static_cast<int32_t>(m_Depth) * -1 }, m_ColorSecondary.GetNormalized(), { 1.0f, 1.0f });
	Vertex v3 = Vertex({ static_cast<float>(position.x), static_cast<float>(position.y + m_Size.y), static_cast<int32_t>(m_Depth) * -1 }, m_ColorSecondary.GetNormalized(), { 0.0f, 1.0f });

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

void vui::RenderText::OnReloadSwapChain()
{
	m_RenderObject->ReloadSwapChain();
}

void vui::RenderText::PostPositionUpdate()
{
	MarkDirty();
}

void vui::RenderText::PostSizeUpdate()
{
	MarkDirty();
}
