#include "ArgumentString.hpp"

#include "ui/Renderer.hpp"
#include "stacking/Block.hpp"

ArgumentString::ArgumentString(const Color& color)
	: Argument(color)
	, m_Text(std::make_shared<vui::RenderText>())
{
	m_Text->SetWeak(m_Text);
	m_Text->SetDepth(Renderer::depth_argument_text);
	m_Text->SetColorPrimary(Color::TextPrimaryColor);
	m_Text->SetColorSecondary(color);
	m_Text->SetPadding(Argument::Padding);
}

BlockArgumentType ArgumentString::GetType()
{
	return BlockArgumentType::STRING;
}

uint32_t ArgumentString::GetWidth()
{
	return m_Text->GetSize().x;
}

bool ArgumentString::HasData()
{
	return true;
}

void ArgumentString::OnRender()
{
	m_Text->Render();
}

void ArgumentString::OnUpdateBuffers()
{
	m_Text->UpdateBuffers();
}

void ArgumentString::OnReloadSwapChain()
{
	m_Text->ReloadSwapChain();
}

void ArgumentString::OnFrameUpdate(double deltaTime)
{

}

void ArgumentString::PostPositionUpdate()
{
	m_Text->SetSuperOffset(m_Position + m_SuperOffset);
}

void ArgumentString::OnSetData()
{
	m_Text->SetText(m_Data);
}
