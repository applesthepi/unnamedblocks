#include "UIRegistry.hpp"

#include "ui/RenderTools.hpp"

#include <Cappuccino/Color.hpp>
// TODO: remove debug
#include <iostream>

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

	std::vector<uint8_t*> char_images;
	std::vector<glm::vec<2, uint16_t>> char_sizes;
	std::vector<char> chars;

	std::string test = "abcdefghijklmnopqrstuvwxyz1234567890";

	for (char i = 32; i <= 126; i++)
	{
		FT_UInt glyph_index = FT_Get_Char_Index(m_ft_face, i);

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
		
#if 0
		for (size_t y = 0; y < map.rows; y++)
		{
			for (size_t x = 0; x < map.width; x++)
				std::cout << static_cast<uint16_t>(map.buffer[y * map.width + x]) << ", ";

			std::cout << '\n';
		}
		
		std::cout << std::flush;
#endif

		uint8_t* char_image = (uint8_t*)malloc(map.width * map.rows * 4);
		glm::vec<4, uint8_t> fill_color_bytes;

		for (uint16_t y = 0; y < map.rows; y++)
		{
			for (uint16_t x = 0; x < map.width; x++)
			{
				uint8_t pixel = map.buffer[y * map.width + x];
				fill_color_bytes = { 255, 255, 255, pixel };
				memcpy(char_image + (y * map.width * 4) + (x * 4), &fill_color_bytes, 4);
			}
		}

		char_images.push_back(char_image);
		char_sizes.push_back({map.width, map.rows});
		chars.push_back(i);
	}

	uint16_t highest_width = 0;
	uint16_t highest_height = 0;

	for (size_t i = 0; i < char_sizes.size(); i++)
	{
		if (char_sizes[i].x > highest_width)
			highest_width = char_sizes[i].x;

		if (char_sizes[i].y > highest_height)
			highest_height = char_sizes[i].y;
	}

	uint16_t image_side_length = glm::ceil(glm::sqrt(static_cast<double>(char_images.size())));
	size_t texture_sheet_total_size = glm::pow(image_side_length, 2) * highest_width * highest_height * 4;
	uint8_t* texture_sheet = (uint8_t*)malloc(texture_sheet_total_size);
	memset(texture_sheet, 0x0, texture_sheet_total_size);

	for (uint16_t y = 0; y < image_side_length; y++)
	{
		for (uint16_t x = 0; x < image_side_length; x++)
		{
			uint8_t* texture_sheet_char = texture_sheet + (x * highest_width * 4) + (y * image_side_length * highest_width * highest_height * 4);
			uint16_t char_idx = y * image_side_length + x;
			
			if (char_idx >= char_sizes.size())
				continue;

			glm::vec<2, float> first = { static_cast<float>(x * highest_width) / static_cast<float>(image_side_length * highest_width), static_cast<float>(y * highest_height) / static_cast<float>(image_side_length * highest_height) };
			glm::vec<2, float> second = { static_cast<float>((x + 1) * highest_width) / static_cast<float>(image_side_length * highest_width), static_cast<float>((y + 1) * highest_height) / static_cast<float>(image_side_length * highest_height) };
			m_char_coords[chars[char_idx]] = { first, second, char_sizes[char_idx] };

			for (uint16_t cy = 0; cy < char_sizes[char_idx].y; cy++)
				memcpy(texture_sheet_char + (cy * image_side_length * highest_width * 4), char_images[char_idx] + (cy * char_sizes[char_idx].x * 4), char_sizes[char_idx].x * 4);
		}
	}

	for (auto char_image : char_images)
		free(char_image);

#if 0
	for (size_t i = 0; i < image_side_length * highest_height; i++)
	{
		for (size_t a = 0; a < image_side_length * highest_width * 4; a++)
		{
			std::cout << ((texture_sheet[(i * image_side_length * highest_width * 4) + a] == 255) ? 1 : 0);
			//texture_sheet[(i * image_side_length * highest_width * 4) + a] = 128;
		}

		std::cout << '\n';
	}

	std::cout << std::flush;
#endif

	m_texture_sheet_image = RenderTools::CreateTextureImage({ image_side_length * highest_width, image_side_length * highest_height }, texture_sheet, &m_texture_sheet_memory);
}

VkImage& UIRegistry::GetTextureSheetImage()
{
	return m_texture_sheet_image;
}

UIRegistry::CharTextureData UIRegistry::GetCharTextureCoords(char character)
{
	return { m_char_coords[character] };
}

FT_Library UIRegistry::m_ft_library;

FT_Face UIRegistry::m_ft_face;

VkImage UIRegistry::m_texture_sheet_image;

VkDeviceMemory UIRegistry::m_texture_sheet_memory;

std::unordered_map<char, UIRegistry::CharTextureData> UIRegistry::m_char_coords;
