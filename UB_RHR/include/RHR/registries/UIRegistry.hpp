#pragma once
#include "config.h"

#include <Cappuccino/Utils.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

class UIRegistry
{
public:
	static void Initialize();
	static VkImage& GetTextureSheetImage();
private:
	static FT_Library m_ft_library;
	static FT_Face m_ft_face;
	static VkImage m_texture_sheet_image;
	static VkDeviceMemory m_texture_sheet_memory;
};
