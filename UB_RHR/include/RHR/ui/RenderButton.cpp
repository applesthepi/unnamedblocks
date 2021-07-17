#include "RenderButton.hpp"

static void RenderButtonMouseUpdate(glm::vec<2, int32_t> position, float scroll, MouseOperation operation, void* data)
{
	vui::RenderButton* button = (vui::RenderButton*)data;
	button->MouseUpdate(position, scroll, operation);
}

vui::RenderButton::RenderButton(const Color& primaryColor, const Color& secondaryColor)
	: IEnableable(true)
	, IDiColorable(primaryColor, secondaryColor)
	, m_RenderRectangle(std::make_shared<RenderRectangle>())
	, m_Callback(nullptr)
	, m_CallbackData(nullptr)
{
	m_RenderRectangle->SetWeak(m_RenderRectangle);
	m_RenderRectangle->SetColor(m_ColorSecondary);

	SetPosition({ 0, 0 });
	SetSize({ 100, 20 });

	InputHandler::RegisterMouseCallback(RenderButtonMouseUpdate, this);
}

vui::RenderButton::~RenderButton()
{
	InputHandler::UnregisterMouseCallback(RenderButtonMouseUpdate);
}

void vui::RenderButton::SetDepth(uint32_t depth)
{
	m_RenderRectangle->SetDepth(depth);
}

void vui::RenderButton::SetCallback(void(*callback)(void*), void* data)
{
	m_Callback = callback;
	m_CallbackData = data;
}

void vui::RenderButton::MouseUpdate(glm::vec<2, int32_t> position, float scroll, MouseOperation operation)
{
	if (operation == MouseOperation::Click)
	{
		glm::vec<2, int32_t> buttonPosition = m_Position + m_SuperOffset;

		if (position.x >= buttonPosition.x && position.x < buttonPosition.x + m_Size.x &&
			position.y >= buttonPosition.y && position.y < buttonPosition.y + m_Size.y)
		{
			if (m_Callback == nullptr)
				Logger::Warn("button callback is nullptr");
			else
				m_Callback(m_CallbackData);
		}
	}
}

void vui::RenderButton::OnRender()
{
	m_RenderRectangle->Render();
}

void vui::RenderButton::OnUpdateBuffers()
{
	m_RenderRectangle->UpdateBuffers();
}

void vui::RenderButton::OnReloadSwapChain()
{
	m_RenderRectangle->ReloadSwapChain();
}

void vui::RenderButton::PostPositionUpdate()
{
	m_RenderRectangle->SetSuperOffset(m_Position + m_SuperOffset);
}

void vui::RenderButton::PostSizeUpdate()
{
	m_RenderRectangle->SetSize(m_Size);
}

void vui::RenderButton::PostColorUpdate()
{
	m_RenderRectangle->SetColor(m_ColorSecondary);
}
