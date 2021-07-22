#pragma once
#include "config.h"

#include <Cappuccino/Utils.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

class UIRegistry
{
public:
	struct CharTextureData
	{
		glm::vec<2, float> First;
		glm::vec<2, float> Second;
		glm::vec<2, uint16_t> Size;
		glm::vec<2, uint16_t> Offset;
		glm::vec<2, uint16_t> Advance;
	};
	static void Initialize();
	static VkImage& GetTextureSheetImage();
	static CharTextureData GetCharTextureCoords(char character);
private:
	static FT_Library m_ft_library;
	static FT_Face m_ft_face;
	static VkImage m_texture_sheet_image;
	static VkDeviceMemory m_texture_sheet_memory;
	static std::unordered_map<char, CharTextureData> m_char_coords;
};
