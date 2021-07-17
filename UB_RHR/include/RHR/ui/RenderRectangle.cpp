#include "RenderRectangle.hpp"

#include "ui/Renderer.hpp"
#include "ui/Vertex.hpp"

// TODO: remove include testing
#include <iostream>

vui::RenderRectangle::RenderRectangle()
	: IEnableable(true)
	, IColorable(Color().FromNormalized({ 0.0f, 0.0f, 0.0f, 1.0f }))
	, m_Depth(10)
	, m_HasColor(false)
	, m_HasTexture(false)
	, m_RenderObject(std::make_shared<RenderObject>(true))
	//, m_WeakSet(false)
{
	m_RenderObject->SetWeak(m_RenderObject);
}

//void vui::RenderRectangle::SetWeak(std::weak_ptr<RenderRectangle>&& weak)
//{
//	m_Weak = std::move(weak);
//	m_WeakSet = true;
//}

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

//std::weak_ptr<vui::RenderRectangle>& vui::RenderRectangle::GetWeak()
//{
//	return m_Weak;
//}

//bool vui::RenderRectangle::IsWeak()
//{
//	if (!m_WeakSet)
//	{
//		Logger::Warn("check for vui::RenderRectangle::IsWeak(); failed");
//		return false;
//	}
//
//	return true;
//}

void vui::RenderRectangle::OnRender()
{
	if (m_Enabled)
		m_RenderObject->Render();
}

void vui::RenderRectangle::OnUpdateBuffers()
{
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	vertices.reserve(4);
	indices.reserve(6);

	glm::vec<2, int32_t> position = m_Position + m_SuperOffset;
	glm::vec<3, float> color = { m_Color.GetNormalized().r, m_Color.GetNormalized().g, m_Color.GetNormalized().b };

	// TODO: remove
	//std::cout << m_SuperOffset.x << ", " << m_SuperOffset.y << std::endl;

	Vertex v0 = Vertex({ static_cast<float>(position.x), static_cast<float>(position.y), static_cast<int32_t>(m_Depth) * -1 }, color, { 0.0f, 0.0f });
	Vertex v1 = Vertex({ static_cast<float>(position.x + m_Size.x), static_cast<float>(position.y), static_cast<int32_t>(m_Depth) * -1 }, color, { 1.0f, 0.0f });
	Vertex v2 = Vertex({ static_cast<float>(position.x + m_Size.x), static_cast<float>(position.y + m_Size.y), static_cast<int32_t>(m_Depth) * -1 }, color, { 1.0f, 1.0f });
	Vertex v3 = Vertex({ static_cast<float>(position.x), static_cast<float>(position.y + m_Size.y), static_cast<int32_t>(m_Depth) * -1 }, color, { 0.0f, 1.0f });

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

void vui::RenderRectangle::OnReloadSwapChain()
{
	m_RenderObject->ReloadSwapChain();
}

void vui::RenderRectangle::PostPositionUpdate()
{
	//SetSizeMax();// TODO: remove testing
	MarkDirty();
}

void vui::RenderRectangle::PostColorUpdate()
{
	MarkDirty();
}
