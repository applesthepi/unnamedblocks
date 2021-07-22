#pragma once
#include "config.h"

#include "ui/RenderFrame.hpp"
#include "ui/interfaces/IEnableable.hpp"

#include <Cappuccino/Utils.hpp>

namespace vui
{

	class RenderLayer : public IEnableable
	{
	public:
		RenderLayer();

		void AddFrame(std::weak_ptr<RenderFrame>&& frame);

		void Render();
		void ReloadSwapChain();

	private:
		std::shared_mutex m_Mutex;
		std::vector<std::weak_ptr<RenderFrame>> m_Frames;
	};

}