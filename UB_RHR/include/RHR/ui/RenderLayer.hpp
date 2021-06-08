#pragma once
#include "config.h"

#include "ui/RenderFrame.hpp"

#include <Cappuccino/Utils.hpp>

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
		std::shared_mutex m_Mutex;
		std::vector<std::weak_ptr<RenderFrame>> m_Frames;
		bool m_Enabled;
	};

}