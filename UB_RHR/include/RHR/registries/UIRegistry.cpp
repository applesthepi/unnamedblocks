#include "UIRegistry.hpp"

void UIRegistry::Initialize()
{
	FT_Error error;

	error = FT_Init_FreeType(&m_ft_library);
	if (error)
	{
		Logger::Error("failed to init freetype \"" + std::to_string(error) + "\"");
		return;
	}

	error = FT_New_Face(m_ft_library, "res/CascadiaMono-Regular.ttf", 0, &m_ft_face);
	if (error)
	{
		Logger::Error("failed to init freetype font \"" + std::to_string(error) + "\"");
		return;
	}

	error = FT_Set_Pixel_Sizes(m_ft_face, 0, 50);
	if (error)
	{
		Logger::Error("failed to set pixel size of freetype font \"" + std::to_string(error) + "\"");
		return;
	}

	int pen_x = 0;
	int pen_y = 0;

	std::string test = "test";

	for (int i = 0; i < test.size(); i++)
	{
		FT_UInt glyph_index = FT_Get_Char_Index(m_ft_face, test[i]);

		error = FT_Load_Glyph(m_ft_face, glyph_index, FT_LOAD_DEFAULT);
		if (error)
		{
			Logger::Error("failed load glyph \"" + std::to_string(error) + "\"");
			continue;
		}

		error = FT_Render_Glyph(m_ft_face->glyph, FT_RENDER_MODE_NORMAL);
		if (error)
		{
			Logger::Error("failed render glyph \"" + std::to_string(error) + "\"");
			continue;
		}

		FT_Bitmap map = m_ft_face->glyph->bitmap;

	}
}

FT_Library UIRegistry::m_ft_library;

FT_Face UIRegistry::m_ft_face;
