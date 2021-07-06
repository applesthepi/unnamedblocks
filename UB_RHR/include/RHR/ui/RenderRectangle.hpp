#pragma once
#include "config.h"

#include "ui/interfaces/IUI.hpp"
#include "ui/interfaces/IPositionable.hpp"
#include "ui/interfaces/ISizeable.hpp"
#include "ui/interfaces/IBoundedParent.hpp"
#include "ui/interfaces/IEnableable.hpp"
#include "ui/interfaces/IColorable.hpp"
#include "ui/RenderObject.hpp"

#include <Cappuccino/Utils.hpp>

namespace vui
{

class RenderRectangle : public IRenderable, public IPositionable<int32_t>, public ISizeable<int32_t>, public IEnableable, public IColorable
{
public:
	RenderRectangle();

	/// REQUIRED Sets weak.
	/// \param Weak ptr of itself.
	void SetWeak(const std::weak_ptr<RenderRectangle>&& weak);

	/// Sets texture of rectangle.
	/// \param Texture path.
	void SetTexture(const std::string& texture);

	/// Sets Z depth.
	/// \param Z depth.
	void SetDepth(uint32_t depth);

	/// Gets the weak reference.
	/// \return Weak reference to itself.
	std::weak_ptr<RenderRectangle>& GetWeak();
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
	std::weak_ptr<RenderRectangle> m_Weak;

	/// Flag if weak is set.
	bool m_WeakSet;

	bool m_HasColor;
	bool m_HasTexture;

	std::string m_Texture;
	uint32_t m_Depth;
};

}