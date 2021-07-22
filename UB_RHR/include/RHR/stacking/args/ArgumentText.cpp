#include "ArgumentText.hpp"

#include "ui/Renderer.hpp"

ArgumentText::ArgumentText(const Color& color)
	: Argument(color)
	, m_Text(std::make_shared<vui::RenderText>())
{
	m_Text->SetWeak(m_Text);
	m_Text->SetDepth(Renderer::DepthArgumentText);
	m_Text->SetColorPrimary(Color::Black);
	m_Text->SetColorSecondary(color);
	m_Text->SetPadding(Argument::Padding);
	m_Text->EnableBackground(false);
}

BlockArgumentType ArgumentText::GetType()
{
	return BlockArgumentType::TEXT;
}

uint32_t ArgumentText::GetWidth()
{
	return m_Text->GetSize().x;
}

bool ArgumentText::HasData()
{
	return true;
}

void ArgumentText::OnRender()
{
	m_Text->Render();
}

void ArgumentText::OnUpdateBuffers()
{
	m_Text->UpdateBuffers();
}

void ArgumentText::OnReloadSwapChain()
{
	m_Text->ReloadSwapChain();
}

void ArgumentText::OnFrameUpdate(double deltaTime)
{
	
}

void ArgumentText::PostPositionUpdate()
{
	m_Text->SetSuperOffset(m_Position + m_SuperOffset);
}

void ArgumentText::OnSetData()
{
	m_Text->SetText(m_Data);
}
