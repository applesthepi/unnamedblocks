#include "ArgumentReal.hpp"

#include "ui/Renderer.hpp"
#include "stacking/Block.hpp"

#define ARG_REAL_DECORE_WIDTH 10

ArgumentReal::ArgumentReal(const Color& color)
	: Argument(color)
	, m_Text(std::make_shared<vui::RenderText>())
	, m_DecorLeft(std::make_shared<RenderObject>(true))
	, m_DecorRight(std::make_shared<RenderObject>(true))
{
	m_DecorLeft->SetWeak(m_DecorLeft);
	m_DecorRight->SetWeak(m_DecorRight);

	m_Text->SetWeak(m_Text);
	m_Text->SetDepth(Renderer::DepthArgumentText);
	m_Text->SetColorSecondary(color);
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

	m_DecorLeft->Render();
	m_DecorRight->Render();
}

void ArgumentReal::OnUpdateBuffers()
{
	m_Text->UpdateBuffers();

	int32_t fullWidth = GetWidth();

	Vertex* verticesLeft = (Vertex*)alloca(sizeof(Vertex) * 3);
	Vertex* verticesRight = (Vertex*)alloca(sizeof(Vertex) * 3);

	verticesLeft[0] = Vertex({ static_cast<float>(ARG_REAL_DECORE_WIDTH), 0.0f, 0 }, m_BlockColor.GetNormalized(), { 0.0f, 0.0f });
	verticesLeft[1] = Vertex({ static_cast<float>(0.0f), static_cast<float>(Block::HeightContent / 2), 0 }, m_BlockColor.GetNormalized(), { 1.0f, 0.0f });
	verticesLeft[2] = Vertex({ static_cast<float>(ARG_REAL_DECORE_WIDTH), static_cast<float>(Block::HeightContent), 0 }, m_BlockColor.GetNormalized(), { 1.0f, 1.0f });

	verticesRight[0] = Vertex({ static_cast<float>(fullWidth - ARG_REAL_DECORE_WIDTH), 0.0f, 0 }, m_BlockColor.GetNormalized(), { 0.0f, 0.0f });
	verticesRight[1] = Vertex({ static_cast<float>(fullWidth), static_cast<float>(Block::HeightContent / 2), 0 }, m_BlockColor.GetNormalized(), { 1.0f, 0.0f });
	verticesRight[2] = Vertex({ static_cast<float>(fullWidth - ARG_REAL_DECORE_WIDTH), static_cast<float>(Block::HeightContent), 0 }, m_BlockColor.GetNormalized(), { 1.0f, 1.0f });

	m_DecorLeft->UpdateVertices(verticesLeft, 3, true);
	m_DecorRight->UpdateVertices(verticesRight, 3, true);
}

void ArgumentReal::OnReloadSwapChain()
{
	m_Text->ReloadSwapChain();

	m_DecorLeft->ReloadSwapChain();
	m_DecorRight->ReloadSwapChain();
}

void ArgumentReal::OnFrameUpdate(double deltaTime)
{

}

void ArgumentReal::PostPositionUpdate()
{
	m_Text->SetSuperOffset(m_Position + m_SuperOffset);
	m_Text->SetPosition({ ARG_REAL_DECORE_WIDTH, 0 });

	glm::vec<2, double> pos = m_Position + m_SuperOffset;
	m_DecorLeft->SetSuperOffset({ pos.x, pos.y, Renderer::DepthArgument});
	m_DecorRight->SetSuperOffset({ pos.x, pos.y, Renderer::DepthArgument });
}

void ArgumentReal::OnSetData()
{
	m_Text->SetText(m_Data);
}
