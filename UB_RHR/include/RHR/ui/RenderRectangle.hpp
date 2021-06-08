#pragma once
#include "config.h"

#include "ui/interfaces/IUI.hpp"
#include "ui/interfaces/IPositionable.hpp"
#include "ui/interfaces/ISizeable.hpp"
#include "ui/interfaces/IBoundedParent.hpp"
#include "ui/interfaces/IEnableable.hpp"
#include "ui/interfaces/IWeak.hpp"
#include "ui/interfaces/IColorable.hpp"
#include "ui/RenderObject.hpp"

#include <Cappuccino/Utils.hpp>

namespace vui
{

class RenderRectangle : public IUI, public IPositionable<int32_t>, public ISizeable<int32_t>, public IBoundedParent, public IEnableable, public IWeak<RenderRectange>, public IColorable
{
public:
	RenderRectangle(bool disableDirty = false);

	void SetTexture(const std::string& texture);
	void SetDepth(uint32_t depth);

private:
	/// Add draw calls to cmd buffer prebound by Renderer.
	void OnRender() override;

	/// Updates mesh on cpu side.
	void OnUpdateBuffers() override;

	/// Regenerates descriptor sets including uniforms.
	void OnReloadSwapChain() override;

	std::shared_ptr<RenderObject> m_RenderObject;

	bool m_HasColor;
	bool m_HasTexture;
	bool m_DisableDirty;

	std::string m_Texture;
	uint32_t m_Depth;
};

}