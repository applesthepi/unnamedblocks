#include "ArgumentReal.hpp"

#include "ui/Renderer.hpp"
#include "stacking/Block.hpp"

#define ARG_REAL_DECORE_WIDTH 6

ArgumentReal::ArgumentReal(const Color& color)
	: Argument(color)
	, m_Text(std::make_shared<vui::RenderText>())
	, m_DecorLeftTop(std::make_shared<RenderObject>(true))
	, m_DecorLeftBottom(std::make_shared<RenderObject>(true))
	, m_DecorRightTop(std::make_shared<RenderObject>(true))
	, m_DecorRightBottom(std::make_shared<RenderObject>(true))
{
	m_DecorLeftTop->SetWeak(m_DecorLeftTop);
	m_DecorLeftBottom->SetWeak(m_DecorLeftBottom);
	m_DecorRightTop->SetWeak(m_DecorRightTop);
	m_DecorRightBottom->SetWeak(m_DecorRightBottom);

	m_Text->SetWeak(m_Text);
	m_Text->SetDepth(Renderer::depth_argument_text);
	m_Text->SetColorPrimary(Color::TextPrimaryColor);
	m_Text->SetColorSecondary(color);
	m_Text->SetPadding(Argument::Padding);
}

BlockArgumentType ArgumentReal::GetType()
{
	return BlockArgumentType::REAL;
}

uint32_t ArgumentReal::GetWidth()
{
	return m_Text->GetSize().x + (2 * ARG_REAL_DECORE_WIDTH);
}

bool ArgumentReal::HasData()
{
	return true;
}

void ArgumentReal::OnRender()
{
	m_Text->Render();

	m_DecorLeftTop->Render();
	m_DecorLeftBottom->Render();
	m_DecorRightTop->Render();
	m_DecorRightBottom->Render();
}

void ArgumentReal::OnUpdateBuffers()
{
	m_Text->UpdateBuffers();

	int32_t fullWidth = GetWidth();

	Vertex vertices[12];
	float otherSide = ARG_REAL_DECORE_WIDTH + m_Text->GetSize().x;

	vertices[0] = Vertex({ static_cast<float>(ARG_REAL_DECORE_WIDTH), 0, 0 }, m_BlockColor.GetNormalized(), { 0.0f, 0.0f });
	vertices[1] = Vertex({ 0, 0, 0 }, m_BlockColor.GetNormalized(), { 0.0f, 0.0f });
	vertices[2] = Vertex({ static_cast<float>(ARG_REAL_DECORE_WIDTH), static_cast<float>(Block::HeightContent / 2), 0 }, m_BlockColor.GetNormalized(), { 0.0f, 0.0f });

	vertices[3] = Vertex({ static_cast<float>(ARG_REAL_DECORE_WIDTH), static_cast<float>(Block::HeightContent / 2), 0 }, m_BlockColor.GetNormalized(), { 0.0f, 0.0f });
	vertices[4] = Vertex({ 0, static_cast<float>(Block::HeightContent), 0 }, m_BlockColor.GetNormalized(), { 0.0f, 0.0f });
	vertices[5] = Vertex({ static_cast<float>(ARG_REAL_DECORE_WIDTH), static_cast<float>(Block::HeightContent), 0 }, m_BlockColor.GetNormalized(), { 0.0f, 0.0f });

	vertices[6] = Vertex({ otherSide, 0, 0 }, m_BlockColor.GetNormalized(), { 0.0f, 0.0f });
	vertices[7] = Vertex({ otherSide + static_cast<float>(ARG_REAL_DECORE_WIDTH), 0, 0 }, m_BlockColor.GetNormalized(), { 0.0f, 0.0f });
	vertices[8] = Vertex({ otherSide, static_cast<float>(Block::HeightContent / 2), 0 }, m_BlockColor.GetNormalized(), { 0.0f, 0.0f });

	vertices[9] = Vertex({ otherSide, static_cast<float>(Block::HeightContent / 2), 0 }, m_BlockColor.GetNormalized(), { 0.0f, 0.0f });
	vertices[10] = Vertex({ otherSide + static_cast<float>(ARG_REAL_DECORE_WIDTH), static_cast<float>(Block::HeightContent), 0 }, m_BlockColor.GetNormalized(), { 0.0f, 0.0f });
	vertices[11] = Vertex({ otherSide, static_cast<float>(Block::HeightContent), 0 }, m_BlockColor.GetNormalized(), { 0.0f, 0.0f });

	m_DecorLeftTop->UpdateVertices(vertices + 0, 3, true);
	m_DecorLeftBottom->UpdateVertices(vertices + 3, 3, true);
	m_DecorRightTop->UpdateVertices(vertices + 6, 3, true);
	m_DecorRightBottom->UpdateVertices(vertices + 9, 3, true);
}

void ArgumentReal::OnReloadSwapChain()
{
	m_Text->ReloadSwapChain();

	m_DecorLeftTop->ReloadSwapChain();
	m_DecorLeftBottom->ReloadSwapChain();
	m_DecorRightTop->ReloadSwapChain();
	m_DecorRightBottom->ReloadSwapChain();
}

void ArgumentReal::OnFrameUpdate(double deltaTime)
{

}

void ArgumentReal::PostPositionUpdate()
{
	m_Text->SetSuperOffset(m_Position + m_SuperOffset);
	m_Text->SetPosition({ ARG_REAL_DECORE_WIDTH, 0 });

	glm::vec<2, double> pos = m_Position + m_SuperOffset;
	m_DecorLeftTop->SetSuperOffset({ pos.x, pos.y, Renderer::depth_argument });
	m_DecorLeftBottom->SetSuperOffset({ pos.x, pos.y, Renderer::depth_argument });
	m_DecorRightTop->SetSuperOffset({ pos.x, pos.y, Renderer::depth_argument });
	m_DecorRightBottom->SetSuperOffset({ pos.x, pos.y, Renderer::depth_argument });

	MarkDirty();
}

void ArgumentReal::OnSetData()
{
	m_Text->SetText(m_Data);
}
