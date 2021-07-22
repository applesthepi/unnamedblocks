#pragma once
#include "config.h"

#include "ui/Cardinal.hpp"
#include "ui/interfaces/IRenderable.hpp"
#include "ui/interfaces/IUpdatable.hpp"
#include "ui/interfaces/IPositionable.hpp"
#include "ui/interfaces/ISizeable.hpp"
#include "ui/RenderRectangle.hpp"

#include <Cappuccino/Utils.hpp>
#include <Espresso/InputHandler.hpp>

namespace vui
{
	class RenderFrame : public IRenderable, public IUpdatable, public IPositionable<2, int32_t>, public ISizeable<int32_t>
	{
	public:
		RenderFrame();

		void SetPadding(uint8_t padding);

		void SetFrame(std::shared_ptr<RenderFrame>& frame);
		void AddFrame(std::shared_ptr<RenderFrame>& frame, LocalCardinal cardinal);
		void AddContent(std::weak_ptr<IRenderable>&& renderable, std::weak_ptr<IUpdatable>&& updatable, std::weak_ptr<IPositionable>&& positionable, std::weak_ptr<ISizeable>&& sizeable, LocalCardinal cardinal);
		void MouseButton(glm::vec<2, int32_t> position, float scroll, MouseOperation operation);
		void Link(LocalCardinal cardinal);
		void Unlink(LocalCardinal cardinal);
		void UpdateLinks();
		void SetBar(size_t idx, int32_t offset);
		void EnableBackground(const Color& color);
		void DisableBarMovement();
	protected:
		void OnRender() override;
		void OnUpdateBuffers() override;
		void OnReloadSwapChain() override;
		void OnSetWeak() override;
		void PostPositionUpdate() override;
		void PostSizeUpdate() override;
	private:
		struct ContentPair
		{
			std::weak_ptr<IRenderable> Renderable;
			std::weak_ptr<IUpdatable> Updatable;
			std::weak_ptr<IPositionable> Positionable;
			std::weak_ptr<ISizeable> Sizeable;
		};

		struct Bar
		{
			Bar(int32_t offset)
				: Offset(offset)
			{

			}

			int32_t Offset;
			glm::vec<2, int32_t> AbsolutePosition;
			glm::vec<2, int32_t> Size;
		};

		struct RenderFrameMouseButtonData
		{
			RenderFrameMouseButtonData()
				: Ptr(nullptr)
			{

			}

			std::weak_ptr<IRenderable> Weak;
			vui::RenderFrame* Ptr;
		};

		static void RenderFrameMouseButton(glm::vec<2, int32_t> position, float scroll, MouseOperation operation, void* data);
		void UpdateContentDimentions();
		void SubmitNewBarPosition(LocalCardinal cardinal);
		void EqualizeBars(bool sizeToContent);
		void PushLinks(std::shared_ptr<RenderFrame>& frame);
		void UpdateMouseButtonStatus(bool enabled);
		void UpdateBarsFromRelative();
		void UpdateBarsFromAbsolute();
		void ResetDrag();
		void UpdateBackground();

		bool m_HasFrame;
		bool m_HasSpace;
		bool m_HasContent;
		bool m_LinkUp, m_LinkDown, m_LinkLeft, m_LinkRight;
		bool m_MouseButtonRegistered;
		bool m_Dragging;
		size_t m_DraggingBarIdx;
		glm::vec<2, int32_t> m_DraggingMouseBegin;
		glm::vec<2, int32_t> m_DraggingObjectBegin;

		PlaneSpace m_Space;
		std::vector<std::shared_ptr<RenderFrame>> m_Frames;
		std::vector<ContentPair> m_Content;
		std::vector<Bar> m_Bars;

		uint8_t m_Padding;
		RenderFrameMouseButtonData* m_MouseButtonData;
		bool m_BackgroundEnabled;
		bool m_DisableBarMovement;
		std::shared_ptr<vui::RenderRectangle> m_Background;
	};
}