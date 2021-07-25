#include "ArgumentBoolean.hpp"

#include "ui/Renderer.hpp"
#include "stacking/Block.hpp"

#define ARG_BOOL_DECORE_WIDTH 10

ArgumentBoolean::ArgumentBoolean(const Color& color)
	: Argument(color)
	, m_Text(std::make_shared<vui::RenderText>())
	, m_DecorLeft(std::make_shared<RenderObject>(true))
	, m_DecorRight(std::make_shared<RenderObject>(true))
{
	m_DecorLeft->SetWeak(m_DecorLeft);
	m_DecorRight->SetWeak(m_DecorRight);

	m_Text->SetWeak(m_Text);
	m_Text->SetDepth(Renderer::DepthArgumentText);
	m_Text->SetColorPrimary(Color::TextPrimaryColor);
	m_Text->SetColorSecondary(color);
	m_Text->SetPadding(Argument::Padding);
}

BlockArgumentType ArgumentBoolean::GetType()
{
	return BlockArgumentType::REAL;
}

uint32_t ArgumentBoolean::GetWidth()
{
	return m_Text->GetSize().x + (2 * ARG_BOOL_DECORE_WIDTH);
}

bool ArgumentBoolean::HasData()
{
	return true;
}

void ArgumentBoolean::OnRender()
{
	m_Text->Render();

	m_DecorLeft->Render();
	m_DecorRight->Render();
}

void ArgumentBoolean::OnUpdateBuffers()
{
	m_Text->UpdateBuffers();

	int32_t fullWidth = GetWidth();

	Vertex vertices[6];

	vertices[0] = Vertex({ static_cast<float>(ARG_BOOL_DECORE_WIDTH), 0.0f, 0 }, m_BlockColor.GetNormalized(), { 0.0f, 0.0f });
	vertices[1] = Vertex({ static_cast<float>(0.0f), 0.0f/*static_cast<float>(Block::HeightContent / 2)*/, 0 }, m_BlockColor.GetNormalized(), { 1.0f, 0.0f });
	vertices[2] = Vertex({ static_cast<float>(ARG_BOOL_DECORE_WIDTH), static_cast<float>(Block::HeightContent), 0 }, m_BlockColor.GetNormalized(), { 1.0f, 1.0f });

	vertices[3] = Vertex({ static_cast<float>(fullWidth - ARG_BOOL_DECORE_WIDTH), 0.0f, 0 }, m_BlockColor.GetNormalized(), { 0.0f, 0.0f });
	vertices[4] = Vertex({ static_cast<float>(fullWidth), static_cast<float>(Block::HeightContent/* / 2*/), 0 }, m_BlockColor.GetNormalized(), { 1.0f, 0.0f });
	vertices[5] = Vertex({ static_cast<float>(fullWidth - ARG_BOOL_DECORE_WIDTH), static_cast<float>(Block::HeightContent), 0 }, m_BlockColor.GetNormalized(), { 1.0f, 1.0f });

	m_DecorLeft->UpdateVertices(vertices + 0, 3, true);
	m_DecorRight->UpdateVertices(vertices + 3, 3, true);
}

void ArgumentBoolean::OnReloadSwapChain()
{
	m_Text->ReloadSwapChain();

	m_DecorLeft->ReloadSwapChain();
	m_DecorRight->ReloadSwapChain();
}

void ArgumentBoolean::OnFrameUpdate(double deltaTime)
{

}

void ArgumentBoolean::PostPositionUpdate()
{
	m_Text->SetSuperOffset(m_Position + m_SuperOffset);
	m_Text->SetPosition({ ARG_BOOL_DECORE_WIDTH, 0 });

	glm::vec<2, double> pos = m_Position + m_SuperOffset;
	m_DecorLeft->SetSuperOffset({ pos.x, pos.y, Renderer::depth_argument });
	m_DecorRight->SetSuperOffset({ pos.x, pos.y, Renderer::depth_argument });

	MarkDirty();
}

void ArgumentBoolean::OnSetData()
{
	m_Text->SetText(m_Data);
}
