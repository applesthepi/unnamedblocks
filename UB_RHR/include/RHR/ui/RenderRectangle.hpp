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

class RenderRectangle : public IRenderable, public IPositionable<2, int32_t>, public ISizeable<int32_t>, public IEnableable, public IColorable
{
public:
	RenderRectangle();

	/// Sets texture of rectangle.
	/// \param Texture path.
	void SetTexture(const std::string& texture);

	/// Sets Z depth.
	/// \param Z depth.
	void SetDepth(uint32_t depth);
private:
	void OnRender() override;

	void OnUpdateBuffers() override;

	void OnReloadSwapChain() override;

	void PostPositionUpdate() override;

	void PostColorUpdate() override;

	/// Abstracted RenderObject to render the rectangle.
	std::shared_ptr<RenderObject> m_RenderObject;

	bool m_HasColor;
	bool m_HasTexture;

	std::string m_Texture;
	uint32_t m_Depth;
};

}