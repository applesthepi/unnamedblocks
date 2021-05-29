#pragma once
#include "RenderFrame.h"

namespace vui
{

class RenderLayer
{
public:
	RenderLayer();

	void SetEnabled(bool enabled);
	void AddFrame(std::weak_ptr<RenderFrame>&& frame);

	void Render();
	void ReloadSwapChain();

private:
	SHARED_MUTEX m_Mutex;
	std::vector<std::weak_ptr<RenderFrame>> m_Frames;
	bool m_Enabled;
};

}