#include "ProgressBar.hpp"

#include "ui/Renderer.hpp"

vui::ProgressBar::ProgressBar(int32_t stackOffset, VerticalAlignment vertical, HorizontalAlignment colorGauge, HorizontalAlignment horizontal, float horizontalSizeOverride)
	: IDiColorable(Color().FromU8({ 150, 150, 150, 255 }), Color().FromU8({ 100, 100, 100, 255 }))
	, IEnableable(true)
	, m_RenderRectangleCompleted(std::make_shared<RenderRectangle>())
	, m_RenderRectangleIncompleted(std::make_shared<RenderRectangle>())
	, m_RenderRectangleExtra(std::make_shared<RenderRectangle>())
	, m_ColorGauge(colorGauge)
	, m_Horizontal(horizontal)
	, m_Vertical(vertical)
	, m_StackOffset(stackOffset)
	, m_Progress(0.0f)
	, m_ProgressExtra(0.0f)
	, m_HorizontalSizeOverride(horizontalSizeOverride)
	, m_BoundsAccepted(false)
	, m_UseExtra(false)
	, m_Depth(8)
{
	m_RenderRectangleCompleted->SetWeak(m_RenderRectangleCompleted);
	m_RenderRectangleIncompleted->SetWeak(m_RenderRectangleIncompleted);
	m_RenderRectangleExtra->SetWeak(m_RenderRectangleExtra);

	m_RenderRectangleCompleted->SetEnabled(true);
	m_RenderRectangleIncompleted->SetEnabled(true);
	m_RenderRectangleExtra->SetEnabled(true);

	m_RenderRectangleCompleted->SetDepth(m_Depth);
	m_RenderRectangleIncompleted->SetDepth(m_Depth);
	m_RenderRectangleExtra->SetDepth(m_Depth);
}

void vui::ProgressBar::SetWeak(std::weak_ptr<ProgressBar>&& weak)
{
	m_Weak = std::move(weak);
}

void vui::ProgressBar::SetDepth(uint32_t depth)
{
	m_Depth = depth;

	m_RenderRectangleCompleted->SetDepth(m_Depth);
	m_RenderRectangleIncompleted->SetDepth(m_Depth);
	m_RenderRectangleExtra->SetDepth(m_Depth);

	MarkDirty();
}

void vui::ProgressBar::SetStackOffset(int32_t stackOffset)
{
	m_StackOffset = stackOffset;
	MarkDirty();
}

//void vui::ProgressBar::SetEnabled(bool enabled)
//{
//	m_Enabled = enabled;
//	// dont need to mark dirty
//}

//void vui::ProgressBar::SetColor(glm::vec<3, float> light, glm::vec<3, float> dark)
//{
//	m_ColorLight = light;
//	m_ColorDark = dark;
//}

void vui::ProgressBar::SetProgress(float progress)
{
	m_Progress = progress;
	m_ProgressExtra = progress;
	m_UseExtra = false;

	MarkDirty();
}

void vui::ProgressBar::SetProgress(float progress, float extra)
{
	m_Progress = progress;
	m_ProgressExtra = extra;
	m_UseExtra = true;

	MarkDirty();
}

//void vui::ProgressBar::SetParent(glm::vec<2, int32_t> position, glm::vec<2, int32_t> size)
//{
//	m_SuperOffset = position;
//	m_SuperBounds = size;
//
//	m_RenderRectangleCompleted->SetParent(position, size);
//	m_RenderRectangleIncompleted->SetParent(position, size);
//	m_RenderRectangleExtra->SetParent(position, size);
//
//	MarkDirty();
//}

void vui::ProgressBar::OnRender()
{
	if (m_Enabled && m_BoundsAccepted)
	{
		m_RenderRectangleCompleted->Render();
		m_RenderRectangleIncompleted->Render();

		if (m_UseExtra)
			m_RenderRectangleExtra->Render();
	}
}

#define UI_VERTICAL_STACK 20
#define PROGRESS_BAR_HEIGHT 5
#define PROGRESS_BAR_WIDTH 100

void vui::ProgressBar::OnUpdateBuffers()
{
	m_Dirty = false;

	glm::vec<2, int32_t> parentHigh = (m_SuperOffset + m_SuperBounds);
	glm::vec<2, int32_t> center = parentHigh / glm::vec<2, int32_t>(2, 2);
	int32_t ypos = 0;

	if (m_Vertical == VerticalAlignment::TOP)
		ypos = UI_VERTICAL_STACK + ((UI_VERTICAL_STACK + PROGRESS_BAR_HEIGHT) * m_StackOffset);
	else if (m_Vertical == VerticalAlignment::CENTER)
		ypos = center.y + ((UI_VERTICAL_STACK + PROGRESS_BAR_HEIGHT) * m_StackOffset);
	else if (m_Vertical == VerticalAlignment::BOTTOM)
		ypos = (parentHigh.y - (UI_VERTICAL_STACK + PROGRESS_BAR_HEIGHT)) + ((UI_VERTICAL_STACK + PROGRESS_BAR_HEIGHT) * m_StackOffset);

	ypos += m_SuperOffset.y;

	float horizontalSize = PROGRESS_BAR_WIDTH;

	if (m_HorizontalSizeOverride > 0.0f && m_HorizontalSizeOverride <= 1.0f)
		horizontalSize = m_HorizontalSizeOverride;

	m_Size = { m_SuperBounds.x * horizontalSize, PROGRESS_BAR_HEIGHT };
	m_Position = { m_SuperOffset.x + (m_SuperBounds.x - m_Size.x) / 2, ypos };

	if (m_Position.y + m_Size.y > parentHigh.y || m_Position.y < m_SuperOffset.y || m_Position.x < m_SuperOffset.x || m_Position.x + m_Size.x > parentHigh.x || m_Progress < 0.0f || m_Progress > 1.0f)
	{
		Logger::Error("ProgressBar's dimensions are out of range");
		m_BoundsAccepted = false;
	}
	else
		m_BoundsAccepted = true;

	if (m_BoundsAccepted)
	{
		m_RenderRectangleCompleted->SetEnabled(true);
		m_RenderRectangleIncompleted->SetEnabled(true);
		m_RenderRectangleExtra->SetEnabled(true);

		m_RenderRectangleCompleted->SetPosition({ m_Position.x, m_Position.y });
		m_RenderRectangleCompleted->SetSize({ m_Size.x * m_Progress, m_Size.y });
		m_RenderRectangleCompleted->UpdateBuffers();

		if (m_UseExtra)
		{
			m_RenderRectangleIncompleted->SetPosition({ m_Position.x + (m_Size.x * m_Progress), m_Position.y });
			m_RenderRectangleIncompleted->SetSize({ (m_Size.x * m_ProgressExtra) - (m_Size.x * m_Progress), m_Size.y });
			m_RenderRectangleIncompleted->UpdateBuffers();

			m_RenderRectangleExtra->SetPosition({ m_Position.x + (m_Size.x * m_ProgressExtra), m_Position.y });
			m_RenderRectangleExtra->SetSize({ m_Size.x - (m_Size.x * m_ProgressExtra), m_Size.y });
			m_RenderRectangleExtra->UpdateBuffers();

			if (m_ColorGauge == HorizontalAlignment::LEFT)
			{
				m_RenderRectangleCompleted->SetColor(m_ColorPrimary);
				m_RenderRectangleIncompleted->SetColor(m_ColorSecondary);
				m_RenderRectangleExtra->SetColor(Color().FromU8({ 255, 255, 255, 255 }));
			}
			else if (m_ColorGauge == HorizontalAlignment::CENTER)
			{
				m_RenderRectangleCompleted->SetColor(m_ColorSecondary);
				m_RenderRectangleIncompleted->SetColor(m_ColorPrimary);
				m_RenderRectangleExtra->SetColor(m_ColorSecondary);
			}
			else if (m_ColorGauge == HorizontalAlignment::RIGHT)
			{
				m_RenderRectangleCompleted->SetColor(Color().FromU8({ 255, 255, 255, 255 }));
				m_RenderRectangleIncompleted->SetColor(m_ColorSecondary);
				m_RenderRectangleExtra->SetColor(m_ColorPrimary);
			}
		}
		else
		{
			m_RenderRectangleIncompleted->SetPosition({ m_Position.x + (m_Size.x * m_Progress), m_Position.y });
			m_RenderRectangleIncompleted->SetSize({ m_Size.x * (1.0f - m_Progress), m_Size.y });
			m_RenderRectangleIncompleted->UpdateBuffers();

			if (m_ColorGauge == HorizontalAlignment::LEFT || m_ColorGauge == HorizontalAlignment::CENTER)
			{
				m_RenderRectangleCompleted->SetColor(m_ColorPrimary);
				m_RenderRectangleIncompleted->SetColor(m_ColorSecondary);
			}
			else if (m_ColorGauge == HorizontalAlignment::RIGHT)
			{
				m_RenderRectangleCompleted->SetColor(m_ColorSecondary);
				m_RenderRectangleIncompleted->SetColor(m_ColorPrimary);
			}
		}
	}
}

void vui::ProgressBar::OnReloadSwapChain()
{
	m_RenderRectangleCompleted->ReloadSwapChain();
	m_RenderRectangleIncompleted->ReloadSwapChain();

	if (m_UseExtra)
		m_RenderRectangleExtra->ReloadSwapChain();
}

//void vui::ProgressBar::MarkDirty()
//{
//	if (m_Dirty)
//		return;
//
//	m_Dirty = true;
//	Renderer::AddDirtyUI(m_Weak);
//}