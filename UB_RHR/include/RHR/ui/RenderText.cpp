#include "RenderText.hpp"

#include "ui/Renderer.hpp"
#include "ui/Vertex.hpp"
#include "stacking/Block.hpp"
#include "registries/UIRegistry.hpp"

vui::RenderText::RenderText()
	: IDiColorable(Color().FromNormalized({ 0.0f, 0.0f, 0.0f, 1.0f }), Color().FromU8({ 25, 25, 25, 255 }))
	, IEnableable(true)
	, m_Depth(10)
	, m_RenderObjectBackground(std::make_shared<RenderObject>(true))
	, m_RenderObjectText(std::make_shared<RenderObject>(true))
{
	m_RenderObjectBackground->SetWeak(m_RenderObjectBackground);
	m_RenderObjectText->SetWeak(m_RenderObjectText);
	m_RenderObjectText->SetTexture(RenderObject::TextureType::TEXT_SHEET);
}

void vui::RenderText::SetText(const std::string& text)
{
	if (text.size() == 0)
	{
		m_Text.clear();
		m_RenderObjectText->SetEnabled(false);
		m_Size = { 10, Block::Height - (Block::Padding * 2) };
	}
	else
	{
		m_Text = text;
		m_RenderObjectText->SetEnabled(true);
		UpdateSize();
	}

	MarkDirty();
}

void vui::RenderText::SetDepth(uint32_t depth)
{
	m_Depth = depth;
	MarkDirty();
}

void vui::RenderText::SetPadding(int32_t padding)
{
	m_Padding = padding;
	UpdateSize();
	MarkDirty();
}

void vui::RenderText::UpdateSize()
{
	int32_t running_x = m_Padding;

	for (size_t i = 0; i < m_Text.size(); i++)
		running_x += UIRegistry::GetCharTextureCoords(m_Text[i]).Advance.x >> 6;
	
	m_Size = { running_x + m_Padding, Block::Height - (Block::Padding * 2) };
}

void vui::RenderText::OnRender()
{
	if (m_Enabled)
	{
		vkCmdBindPipeline(Renderer::ActiveCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, Renderer::UIPipeline);
		m_RenderObjectBackground->Render();

		vkCmdBindPipeline(Renderer::ActiveCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, Renderer::UITexturePipeline);
		m_RenderObjectText->Render();
	}
}

void vui::RenderText::OnUpdateBuffers()
{
	ClearDirty();

	{
		Vertex vertices[4];
		uint32_t indices[6];

		vertices[0] = Vertex({ 0.0f, 0.0f, 0.0f }, m_ColorSecondary.GetNormalized(), { 0.0f, 0.0f });
		vertices[1] = Vertex({ static_cast<float>(m_Size.x), 0.0f, 0.0f }, m_ColorSecondary.GetNormalized(), { 1.0f, 0.0f });
		vertices[2] = Vertex({ static_cast<float>(m_Size.x), static_cast<float>(m_Size.y), 0.0f }, m_ColorSecondary.GetNormalized(), { 1.0f, 1.0f });
		vertices[3] = Vertex({ 0.0f, static_cast<float>(m_Size.y), 0.0f }, m_ColorSecondary.GetNormalized(), { 0.0f, 1.0f });

		indices[0] = 1;
		indices[1] = 0;
		indices[2] = 2;
		indices[3] = 2;
		indices[4] = 0;
		indices[5] = 3;

		m_RenderObjectBackground->UpdateVertices(vertices, 4, indices, 6, true);
	}

	if (m_RenderObjectText->GetEnabled())
	{
		Vertex* vertices = (Vertex*)alloca(sizeof(Vertex) * m_Text.size() * 4);
		uint32_t* indices = (uint32_t*)alloca(sizeof(uint32_t) * m_Text.size() * 6);

		int32_t running_x = m_Padding;

		for (size_t i = 0; i < m_Text.size(); i++)
		{
			UIRegistry::CharTextureData char_data = UIRegistry::GetCharTextureCoords(m_Text[i]);
			float y_offset = static_cast<float>(Block::HeightContent) + (-1.0f * static_cast<float>(char_data.Offset.y)) - static_cast<float>(Block::Padding);

			vertices[i * 4 + 0] = Vertex({ static_cast<float>(running_x + char_data.Offset.x), y_offset, 0.0f }, { 1.0f, 1.0f , 1.0f }, { char_data.First.x, char_data.First.y });
			vertices[i * 4 + 1] = Vertex({ static_cast<float>(running_x + char_data.Offset.x + char_data.Size.x), y_offset, 0.0f }, { 1.0f, 1.0f , 1.0f }, { char_data.Second.x, char_data.First.y });
			vertices[i * 4 + 2] = Vertex({ static_cast<float>(running_x + char_data.Offset.x + char_data.Size.x), static_cast<float>(char_data.Size.y) + y_offset, 0.0f }, { 1.0f, 1.0f , 1.0f }, { char_data.Second.x, char_data.Second.y });
			vertices[i * 4 + 3] = Vertex({ static_cast<float>(running_x + char_data.Offset.x), static_cast<float>(char_data.Size.y) + y_offset, 0.0f }, { 1.0f, 1.0f , 1.0f }, { char_data.First.x, char_data.Second.y });

			indices[i * 6 + 0] = i * 4 + 1;
			indices[i * 6 + 1] = i * 4 + 0;
			indices[i * 6 + 2] = i * 4 + 2;
			indices[i * 6 + 3] = i * 4 + 2;
			indices[i * 6 + 4] = i * 4 + 0;
			indices[i * 6 + 5] = i * 4 + 3;

			running_x += char_data.Advance.x >> 6;
		}

		m_RenderObjectText->UpdateVertices(vertices, m_Text.size() * 4, indices, m_Text.size() * 6, true);
		m_Size = { running_x + m_Padding, Block::Height - (Block::Padding * 2) };
	}
}

void vui::RenderText::OnReloadSwapChain()
{
	m_RenderObjectBackground->ReloadSwapChain();
	m_RenderObjectText->ReloadSwapChain();
}

void vui::RenderText::PostPositionUpdate()
{
	glm::vec<2, int32_t> position = m_Position + m_SuperOffset;
	m_RenderObjectBackground->SetSuperOffset({static_cast<double>(position.x), static_cast<double>(position.y), static_cast<double>(m_Depth) });
	m_RenderObjectText->SetSuperOffset({ static_cast<double>(position.x), static_cast<double>(position.y), static_cast<double>(m_Depth) - 0.1 });

	MarkDirty();
}

void vui::RenderText::PostSizeUpdate()
{
	MarkDirty();
}
