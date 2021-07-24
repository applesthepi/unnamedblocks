#include "char_texture.hpp"

#include "rhr/rendering/tools.hpp"
#include "rhr/stacking/block.hpp"

#include <cappuccino/color.hpp>

void rhr::registries::char_texture::process_fonts()
{
	ft::error error;

	error = FT_Init_FreeType(&m_library);
	if (error)
	{
		Logger::Error("failed to init freetype \"" + std::to_string(error) + "\"");
		return;
	}

	process_font("res/CascadiaMono-Light.ttf", texture_type::LIGHT_NORMAL);
	process_font("res/CascadiaMono-Bold.ttf", texture_type::BOLD_NORMAL);
	process_font("res/CascadiaMonoItalic-LightItalic.ttf", texture_type::LIGHT_ITALIC);
	process_font("res/CascadiaMonoItalic-BoldItalic.ttf", texture_type::BOLD_ITALIC);
}

void rhr::registries::char_texture::process_font(const std::string& font_path, texture_type type)
{
	texture_data font_texture_data;
	ft::error error;

	error = FT_Init_FreeType(&m_library);
	if (error)
	{
		Logger::Error("failed to init freetype \"" + std::to_string(error) + "\"");
		return;
	}

	error = FT_New_Face(m_library, font_path.c_str(), 0, &font_texture_data.face);
	if (error)
	{
		Logger::Error("failed to init freetype font \"" + std::to_string(error) + "\"");
		return;
	}

	error = FT_Set_Pixel_Sizes(font_texture_data.face, 0, Block::Height - (Block::Padding * 2));
	if (error)
	{
		Logger::Error("failed to set pixel size of freetype font \"" + std::to_string(error) + "\"");
		return;
	}

	std::vector<u8*> char_images;
	std::vector<glm::vec<2, u16>> char_sizes;
	std::vector<char> chars;
	std::vector<glm::vec<2, u16>> char_offsets;
	std::vector<glm::vec<2, u16>> char_advances;

	for (char i = 32; i <= 126; i++)
	{
		FT_UInt glyph_index = FT_Get_Char_Index(font_texture_data.face, i);

		error = FT_Load_Glyph(font_texture_data.face, glyph_index, FT_LOAD_DEFAULT);
		if (error)
		{
			Logger::Error("failed load glyph \"" + std::to_string(error) + "\"");
			continue;
		}

		error = FT_Render_Glyph(font_texture_data.face->glyph, FT_RENDER_MODE_NORMAL);
		if (error)
		{
			Logger::Error("failed render glyph \"" + std::to_string(error) + "\"");
			continue;
		}

		FT_Bitmap map = font_texture_data.face->glyph->bitmap;

#if 0
		for (size_t y = 0; y < map.rows; y++)
		{
			for (size_t x = 0; x < map.width; x++)
				std::cout << static_cast<u16>(map.buffer[y * map.width + x]) << ", ";

			std::cout << '\n';
		}

		std::cout << std::flush;
#endif

		u8* char_image = (u8*)malloc(map.width * map.rows * 4);
		glm::vec<4, u8> fill_color_bytes;

		for (u16 y = 0; y < map.rows; y++)
		{
			for (u16 x = 0; x < map.width; x++)
			{
				u8 pixel = map.buffer[y * map.width + x];
				fill_color_bytes = { 255, 255, 255, pixel };
				memcpy(char_image + (y * map.width * 4) + (x * 4), &fill_color_bytes, 4);
			}
		}

		char_images.push_back(char_image);
		char_sizes.push_back({ map.width, map.rows });
		chars.push_back(i);
		char_offsets.push_back({ font_texture_data.face->glyph->bitmap_left, font_texture_data.face->glyph->bitmap_top });
		char_advances.push_back({ font_texture_data.face->glyph->advance.x, font_texture_data.face->glyph->advance.y });
	}

	u16 highest_width = 0;
	u16 highest_height = 0;

	for (usize i = 0; i < char_sizes.size(); i++)
	{
		if (char_sizes[i].x > highest_width)
			highest_width = char_sizes[i].x;

		if (char_sizes[i].y > highest_height)
			highest_height = char_sizes[i].y;
	}

	u16 image_side_length = glm::ceil(glm::sqrt(static_cast<double>(char_images.size())));
	usize texture_sheet_total_size = glm::pow(image_side_length, 2) * highest_width * highest_height * 4;
	u8* texture_sheet = (u8*)malloc(texture_sheet_total_size);
	memset(texture_sheet, 0x0, texture_sheet_total_size);

	for (u16 y = 0; y < image_side_length; y++)
	{
		for (u16 x = 0; x < image_side_length; x++)
		{
			u8* texture_sheet_char = texture_sheet + (x * highest_width * 4) + (y * image_side_length * highest_width * highest_height * 4);
			u16 char_idx = y * image_side_length + x;

			if (char_idx >= char_sizes.size())
				continue;

			glm::vec<2, float> first = { static_cast<float>(x * highest_width) / static_cast<float>(image_side_length * highest_width), static_cast<float>(y * highest_height) / static_cast<float>(image_side_length * highest_height) };
			glm::vec<2, float> second = { static_cast<float>(x * highest_width + char_sizes[char_idx].x) / static_cast<float>(image_side_length * highest_width), static_cast<float>(y * highest_height + char_sizes[char_idx].y) / static_cast<float>(image_side_length * highest_height) };
			font_texture_data.char_map[chars[char_idx]] = { first, second, char_sizes[char_idx], char_offsets[char_idx], char_advances[char_idx] };

			for (u16 cy = 0; cy < char_sizes[char_idx].y; cy++)
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

	font_texture_data.image = RenderTools::create_texture_image({ image_side_length * highest_width, image_side_length * highest_height }, texture_sheet, &font_texture_data.memory);
	texture_map[type] = font_texture_data;
}

ft::library rhr::registries::char_texture::m_library;

std::unordered_map<rhr::registries::char_texture::texture_type, rhr::registries::char_texture::texture_data> rhr::registries::char_texture::texture_map;
