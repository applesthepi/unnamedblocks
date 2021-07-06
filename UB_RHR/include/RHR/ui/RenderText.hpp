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
	class RenderText : public IRenderable, public IPositionable<int32_t>, public ISizeable<int32_t>, public IDiColorable, public IEnableable
	{
	public:
		RenderText();

		/// REQUIRED Sets weak.
		/// \param Weak ptr of itself.
		void SetWeak(const std::weak_ptr<RenderText>&& weak);

		/// Sets text.
		/// \param Text.
		void SetText(const std::string& text);

		/// Sets Z depth.
		/// \param Z depth.
		void SetDepth(uint32_t depth);

		/// Gets the weak reference.
		/// \return Weak reference to itself.
		std::weak_ptr<RenderText>& GetWeak();
	private:
		/// Check for weak.
		bool IsWeak();

		/// Add draw calls to cmd buffer prebound by Renderer.
		void OnRender() override;

		/// Updates mesh on cpu side.
		void OnUpdateBuffers() override;

		/// Regenerates descriptor sets including uniforms.
		void OnReloadSwapChain() override;

		/// Abstracted RenderObject to render the rectangle.
		std::shared_ptr<RenderObject> m_RenderObject;

		/// Weak reference.
		std::weak_ptr<RenderText> m_Weak;

		/// Flag if weak is set.
		bool m_WeakSet;

		std::string m_Text;
		uint32_t m_Depth;
	};

}