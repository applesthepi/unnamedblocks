#pragma once
#include "config.h"

#include "ui/RenderUI.hpp"
#include "ui/RenderRectangle.hpp"
#include "ui/RenderObject.hpp"

#include <Cappuccino/Utils.hpp>

namespace vui
{

enum class HorizontalAlignment
{
	LEFT,
	CENTER,
	RIGHT
};

enum class VerticalAlignment
{
	TOP,
	CENTER,
	BOTTOM
};

class ProgressBar : public RenderUI
{
public:
	ProgressBar(int32_t stackOffset, VerticalAlignment vertical, HorizontalAlignment colorGauge = HorizontalAlignment::LEFT, HorizontalAlignment horizontal = HorizontalAlignment::CENTER, float horizontalSizeOverride = 0.0f);

	void SetWeak(std::weak_ptr<ProgressBar>&& weak);
	void SetDepth(uint32_t depth);
    void SetStackOffset(int32_t stackOffset);
	void SetEnabled(bool enabled);
	void SetColor(glm::vec<3, float> light, glm::vec<3, float> dark);
	void SetProgress(float progress);
    void SetProgress(float progress, float extra);

	void SetParent(glm::vec<2, int32_t> position, glm::vec<2, int32_t> size) override;
	void Render() override;
	void UpdateBuffers() override;
	void ReloadSwapChain() override;

private:
	void MarkDirty();

	std::shared_ptr<RenderRectangle> m_RenderRectangleCompleted;
	std::shared_ptr<RenderRectangle> m_RenderRectangleIncompleted;
    std::shared_ptr<RenderRectangle> m_RenderRectangleExtra;
	std::weak_ptr<ProgressBar> m_Weak;

	HorizontalAlignment m_ColorGauge;
	HorizontalAlignment m_Horizontal;
	VerticalAlignment m_Vertical;
	int32_t m_StackOffset;
	float m_Progress;
    float m_ProgressExtra;
	float m_HorizontalSizeOverride;

	glm::vec<2, int32_t> m_ParentPosition;
	glm::vec<2, int32_t> m_ParentSize;

	glm::vec<2, int32_t> m_Position;
	glm::vec<2, int32_t> m_TotalSize;

	bool m_Enabled;
	bool m_Dirty;
	bool m_BoundsAccepted;
    bool m_UseExtra;

	uint32_t m_Depth;
	glm::vec<3, float> m_ColorLight;
	glm::vec<3, float> m_ColorDark;
};

}