#include "ArgumentText.hpp"

ArgumentText::ArgumentText()
	: m_Text(std::make_shared<vui::RenderText>())
{
	m_Text->SetWeak(m_Text);
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

void ArgumentText::OnSetData()
{
	m_Text->SetText(m_Data);
}
