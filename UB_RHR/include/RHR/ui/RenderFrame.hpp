#pragma once
#include "config.h"

#include "ui/Cardinal.hpp"
#include "ui/interfaces/IRenderable.hpp"
#include "ui/interfaces/IUpdatable.hpp"
#include "ui/interfaces/IPositionable.hpp"
#include "ui/interfaces/ISizeable.hpp"

#include <Cappuccino/Utils.hpp>

namespace vui
{
	class RenderFrame : public IRenderable, public IUpdatable, public IPositionable<int32_t>, public ISizeable<int32_t>
	{
	public:
		RenderFrame();

		void SetPadding(uint8_t padding);

		void SetFrame(std::unique_ptr<RenderFrame>& frame);
		void AddFrame(std::unique_ptr<RenderFrame>& frame, LocalCardinal cardinal);
		void AddContent(std::weak_ptr<IRenderable>&& renderable, std::weak_ptr<IUpdatable>&& updatable, std::weak_ptr<IPositionable>&& positionable, std::weak_ptr<ISizeable>&& sizeable);
	protected:
		void OnRender() override;
		void OnUpdateBuffers() override;
		void OnReloadSwapChain() override;

		bool OnPositionUpdate(const glm::vec<2, int32_t>&& position, const glm::vec<2, int32_t>&& offset) override;
		bool OnSizeUpdate(const glm::vec<2, int32_t>&& size, const glm::vec<2, int32_t>&& bounds) override;
	private:
		struct ContentPair
		{
			std::weak_ptr<IRenderable> Renderable;
			std::weak_ptr<IUpdatable> Updatable;
			std::weak_ptr<IPositionable> Positionable;
			std::weak_ptr<ISizeable> Sizeable;
		};

		void UpdateContentDimentions();

		bool m_HasFrame;
		bool m_HasSpace;
		bool m_HasContent;

		PlaneSpace m_Space;
		std::vector<std::unique_ptr<RenderFrame>> m_Frames;
		std::vector<ContentPair> m_Content;

		uint8_t m_Padding;
	};
}