#pragma once
#include "config.h"
/*
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

class ProgressBar : public IRenderable, public IUpdatable, public IDiColorable, public IEnableable, public IPositionable<2, i32>, public ISizeable<i32>
{
public:
	ProgressBar(i32 stackOffset, VerticalAlignment vertical, HorizontalAlignment colorGauge = HorizontalAlignment::LEFT, HorizontalAlignment horizontal = HorizontalAlignment::CENTER, f32 horizontalSizeOverride = 0.0f);

	void SetWeak(std::weak_ptr<ProgressBar>&& weak);
	void SetDepth(u32 depth);
	void SetStackOffset(i32 stackOffset);
	void SetProgress(f32 progress);
	void SetProgress(f32 progress, f32 extra);
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

	i32 m_StackOffset;
	f32 m_Progress;
	f32 m_ProgressExtra;
	f32 m_HorizontalSizeOverride;

	bool m_Dirty;
	bool m_BoundsAccepted;
	bool m_UseExtra;

	u32 m_Depth;

	std::weak_ptr<ProgressBar> m_Weak;
};

}
*/