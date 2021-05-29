#pragma once
#include "Cardinal.h"
#include "RenderUI.h"

#include <MccCommon/Utils.h>

namespace vui
{

class RenderFrame
{
public:
	RenderFrame();

	void SetWeak(std::weak_ptr<RenderFrame>&& weak);
	void SetPosition(glm::vec<2, int32_t> position);
	void SetSize(glm::vec<2, int32_t> size);
	void SetPadding(uint8_t padding);

	void SetFrame(std::unique_ptr<RenderFrame>& frame);
	void AddFrame(std::unique_ptr<RenderFrame>& frame, LocalCardinal cardinal);
	void AddContent(std::weak_ptr<RenderUI>&& content);
	void Render();
	void UpdateBuffers();
	void ReloadSwapChain();

private:
	void UpdateContentDimentions();
	void MarkDirty();

	bool m_HasFrame;
	bool m_HasSpace;
	bool m_HasContent;
	bool m_Dirty;

	PlaneSpace m_Space;
	std::vector<std::unique_ptr<RenderFrame>> m_Frames;
	std::vector<std::weak_ptr<RenderUI>> m_Content;

	uint8_t m_Padding;
	glm::vec<2, int32_t> m_Position;
	glm::vec<2, int32_t> m_Size;
	std::weak_ptr<RenderFrame> m_Weak;
};

}