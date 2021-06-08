#pragma once
#include "config.h"

#include "ui/Cardinal.hpp"
#include "ui/interfaces/IUI.hpp"
#include "ui/interfaces/IPositionable.hpp"
#include "ui/interfaces/ISizeable.hpp"

#include <Cappuccino/Utils.hpp>

namespace vui
{
	class RenderFrame : public IUI, public IPositionable<int32_t>, public ISizeable<int32_t>
	{
	public:
		RenderFrame();

		void SetPadding(uint8_t padding);

		void SetFrame(std::unique_ptr<RenderFrame>& frame);
		void AddFrame(std::unique_ptr<RenderFrame>& frame, LocalCardinal cardinal);
		void AddContent(std::weak_ptr<IUI>&& content);
	protected:
		void OnRender() override;
		void OnUpdateBuffers() override;
		void OnReloadSwapChain() override;

		bool OnSetPosition(const glm::vec<2, int32_t>&& position) override;
		bool OnSetSize(const glm::vec<2, int32_t>&& size) override;
	private:
		void UpdateContentDimentions();

		bool m_HasFrame;
		bool m_HasSpace;
		bool m_HasContent;

		PlaneSpace m_Space;
		std::vector<std::unique_ptr<RenderFrame>> m_Frames;
		std::vector<std::weak_ptr<IUI>> m_Content;

		uint8_t m_Padding;
	};
}