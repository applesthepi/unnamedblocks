#pragma once
#include "config.h"

#include "ui/RenderUI.hpp"
#include "ui/RenderObject.hpp"

#include <Cappuccino/Utils.hpp>

namespace vui
{

class RenderRectangle : public RenderUI
{
public:
	RenderRectangle(bool disableDirty = false);

	void SetWeak(std::weak_ptr<RenderRectangle>&& weak);
	void SetPosition(glm::vec<2, int32_t> position);
	void SetSize(glm::vec<2, int32_t> size);
	void SetColor(glm::vec<3, float> color);
	void SetTexture(const std::string& texture);
	void SetDepth(uint32_t depth);
	void SetEnabled(bool enabled);

	void SetParent(glm::vec<2, int32_t> position, glm::vec<2, int32_t> size) override;
	void Render() override;
	void UpdateBuffers() override;
	void ReloadSwapChain() override;

private:
	void MarkDirty();

	std::shared_ptr<RenderObject> m_RenderObject;
	std::weak_ptr<RenderRectangle> m_Weak;

	glm::vec<2, int32_t> m_Position;
	glm::vec<2, int32_t> m_Size;
	glm::vec<2, int32_t> m_ParentPosition;
	glm::vec<2, int32_t> m_ParentSize;

	bool m_HasColor;
	bool m_HasTexture;
	bool m_Enabled;
	bool m_Dirty;
	bool m_DisableDirty;

	glm::vec<3, float> m_Color;
	std::string m_Texture;
	uint32_t m_Depth;
};

}