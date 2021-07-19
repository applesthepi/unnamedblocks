#pragma once
#include "config.h"

#include "ui/interfaces/IUI.hpp"
#include "ui/interfaces/IPositionable.hpp"
#include "ui/interfaces/ISizeable.hpp"
#include "ui/interfaces/IBoundedParent.hpp"
#include "ui/interfaces/IEnableable.hpp"
#include "ui/interfaces/IDiColorable.hpp"
#include "ui/RenderRectangle.hpp"

#include <Cappuccino/Utils.hpp>
#include <Espresso/InputHandler.hpp>

namespace vui
{

	class RenderButton : public IRenderable, public IPositionable<2, int32_t>, public ISizeable<int32_t>, public IEnableable, public IDiColorable
	{
	public:
		RenderButton(const Color& primaryColor, const Color& secondaryColor);
		~RenderButton();

		/// Sets Z depth.
		/// \param Z depth.
		void SetDepth(uint32_t depth);

		/// Callback called when button is clicked.
		/// \param Callback.
		void SetCallback(void(*callback)(void*), void* data);

		void MouseUpdate(glm::vec<2, int32_t> position, float scroll, MouseOperation operation);

		void EnableFillWidth();
	private:
		void OnRender() override;

		void OnUpdateBuffers() override;

		void OnReloadSwapChain() override;

		void PostPositionUpdate() override;

		void PostSizeUpdate() override;

		void PostColorUpdate() override;

		/// Rectangle.
		std::shared_ptr<RenderRectangle> m_RenderRectangle;

		/// Callback information.
		void(*m_Callback)(void*);
		void* m_CallbackData;

		bool m_EnableFillWidth;
	};

}