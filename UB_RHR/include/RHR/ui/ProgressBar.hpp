#pragma once
#include "config.h"

#include "ui/RenderObject.hpp"
#include "ui/interfaces/IDiColorable.hpp"
#include "ui/interfaces/IPositionable.hpp"
#include "ui/interfaces/ISizeable.hpp"
#include "ui/interfaces/IEnableable.hpp"
#include "ui/interfaces/IBoundedParent.hpp"
#include "ui/RenderRectangle.hpp"

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

class ProgressBar : public IRenderable, public IUpdatable, public IDiColorable, public IEnableable, public IPositionable<int32_t>, public ISizeable<int32_t>
{
public:
	ProgressBar(int32_t stackOffset, VerticalAlignment vertical, HorizontalAlignment colorGauge = HorizontalAlignment::LEFT, HorizontalAlignment horizontal = HorizontalAlignment::CENTER, float horizontalSizeOverride = 0.0f);

	void SetWeak(std::weak_ptr<ProgressBar>&& weak);
	void SetDepth(uint32_t depth);
	void SetStackOffset(int32_t stackOffset);
	void SetProgress(float progress);
	void SetProgress(float progress, float extra);
protected:
	/// Add draw calls to cmd buffer prebound by Renderer.
	void OnRender() override;

	/// Updates mesh on cpu side.
	void OnUpdateBuffers() override;

	/// Regenerates descriptor sets including uniforms.
	void OnReloadSwapChain() override;
private:
	std::shared_ptr<RenderRectangle> m_RenderRectangleCompleted;
	std::shared_ptr<RenderRectangle> m_RenderRectangleIncompleted;
	std::shared_ptr<RenderRectangle> m_RenderRectangleExtra;

	HorizontalAlignment m_ColorGauge;
	HorizontalAlignment m_Horizontal;
	VerticalAlignment m_Vertical;

	int32_t m_StackOffset;
	float m_Progress;
	float m_ProgressExtra;
	float m_HorizontalSizeOverride;

	bool m_Dirty;
	bool m_BoundsAccepted;
	bool m_UseExtra;

	uint32_t m_Depth;

	std::weak_ptr<ProgressBar> m_Weak;
};

}