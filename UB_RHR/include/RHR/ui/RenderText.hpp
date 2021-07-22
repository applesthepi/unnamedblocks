#pragma once
#include "config.h"

#include "ui/interfaces/IUI.hpp"
#include "ui/interfaces/IPositionable.hpp"
#include "ui/interfaces/ISizeable.hpp"
#include "ui/interfaces/IBoundedParent.hpp"
#include "ui/interfaces/IEnableable.hpp"
#include "ui/interfaces/IDiColorable.hpp"
#include "ui/RenderObject.hpp"

#include <Cappuccino/Utils.hpp>

namespace vui
{
	class RenderText : public IRenderable, public IPositionable<2, int32_t>, public ISizeable<int32_t>, public IDiColorable, public IEnableable
	{
	public:
		RenderText();

		/// Sets text.
		/// \param Text.
		void SetText(const std::string& text);

		/// Sets Z depth.
		/// \param Z depth.
		void SetDepth(uint32_t depth);
	private:
		/// Add draw calls to cmd buffer prebound by Renderer.
		void OnRender() override;

		/// Updates mesh on cpu side.
		void OnUpdateBuffers() override;

		/// Regenerates descriptor sets including uniforms.
		void OnReloadSwapChain() override;

		void PostPositionUpdate() override;
		void PostSizeUpdate() override;

		/// RenderObject to render the background rectangle.
		std::shared_ptr<RenderObject> m_RenderObjectBackground;

		/// RenderObject to render the actual text.
		std::shared_ptr<RenderObject> m_RenderObjectText;

		std::string m_Text;
		uint32_t m_Depth;
	};

}