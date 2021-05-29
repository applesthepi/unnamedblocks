#pragma once
#include <MccCommon/Utils.h>

namespace vui
{

#define PROGRESS_BAR_WIDTH 0.9f /* (%) */
#define PROGRESS_BAR_HEIGHT 10 /* (px) */
#define UI_VERTICAL_STACK 20 /* (px) */

#define COLOR_BACKGROUND_BASE glm::vec<3, float>(0.008f, 0.008f, 0.008f)
#define COLOR_BACKGROUND_MIN glm::vec<3, float>(0.006f, 0.006f, 0.006f)
#define COLOR_BACKGROUND_MAX glm::vec<3, float>(0.01f, 0.012f, 0.01f)

// Orange
#define COLOR_BACKGROUND_ACCENT_1_LIGHT glm::vec<3, float>(1.0f, 0.2f, 0.0f)
#define COLOR_BACKGROUND_ACCENT_1_DARK glm::vec<3, float>(0.1f, 0.025f, 0.0f)

// Cyan
#define COLOR_BACKGROUND_ACCENT_2_LIGHT glm::vec<3, float>(0.0f, 0.4f, 0.8f)
#define COLOR_BACKGROUND_ACCENT_2_DARK glm::vec<3, float>(0.0f, 0.03f, 0.05f)

// Purple
#define COLOR_BACKGROUND_ACCENT_3_LIGHT glm::vec<3, float>(0.35f, 0.0f, 0.7f)
#define COLOR_BACKGROUND_ACCENT_3_DARK glm::vec<3, float>(0.03f, 0.0f, 0.05f)

// Magenta
#define COLOR_BACKGROUND_ACCENT_4_LIGHT glm::vec<3, float>(0.95f, 0.0f, 0.9f)
#define COLOR_BACKGROUND_ACCENT_4_DARK glm::vec<3, float>(0.1f, 0.0f, 0.08f)

// Blue
#define COLOR_BACKGROUND_ACCENT_5_LIGHT glm::vec<3, float>(0.0f, 0.1f, 0.9f)
#define COLOR_BACKGROUND_ACCENT_5_DARK glm::vec<3, float>(0.0f, 0.01f, 0.08f)


class RenderUI
{
public:
	virtual void SetParent(glm::vec<2, int32_t> position, glm::vec<2, int32_t> size) = 0;
	virtual void Render() = 0;
	virtual void UpdateBuffers() = 0;
	virtual void ReloadSwapChain() = 0;
};

}