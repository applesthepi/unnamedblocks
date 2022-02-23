#include "char_texture.hpp"

#include "lungo/tools.hpp"
#include "mocha/stacking/block.hpp"

#include <espresso/color.hpp>
// TODO: remove
#include <iostream>

void rhr::registry::char_texture::process_fonts()
{
	//	ft::error error;
	//
	//	error = FT_Init_FreeType(&m_library);
	//	if (error)
	//	{
	//		cap::logger::error("failed to init freetype \"" + std::to_string(error) + "\"");
	//		return;
	//	}
}

void rhr::registry::char_texture::process_font(const std::string& font_path, texture_type type, u16 size)
{
	if (size < 2 || size > 1000)
	{
		cap::logger::error(
			cap::logger::level::SYSTEM,
			"size to big; error processing font \"" + font_path + "\" of size \"" + std::to_string(size) + "\"");
		return;
	}

	rhr::registry::char_texture::texture_map* local_map = nullptr;
	bool found											= false;
	ft::error error;

	for (auto font : m_texture_maps)
	{
		if (font->texture_size == size)
		{
			local_map = font;
			found	  = true;
			break;
		}
	}

	if (!local_map)
	{
		local_map = new rhr::registry::char_texture::texture_map(size);
	}

	texture_data font_texture_data;

	error = FT_New_Face(local_map->library, font_path.c_str(), 0, &font_texture_data.face);
	if (error)
	{
		cap::logger::error(
			cap::logger::level::SYSTEM, "failed to init freetype font \"" + std::to_string(error) + "\"");
		return;
	}

	error = FT_Set_Pixel_Sizes(font_texture_data.face, 0, size);
	if (error)
	{
		cap::logger::error(
			cap::logger::level::SYSTEM, "failed to set pixel size of freetype font \"" + std::to_string(error) + "\"");
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
			cap::logger::error(cap::logger::level::SYSTEM, "failed load glyph \"" + std::to_string(error) + "\"");
			continue;
		}

		error = FT_Render_Glyph(font_texture_data.face->glyph, FT_RENDER_MODE_NORMAL);
		if (error)
		{
			cap::logger::error(cap::logger::level::SYSTEM, "failed render glyph \"" + std::to_string(error) + "\"");
			continue;
		}

		FT_Bitmap map = font_texture_data.face->glyph->bitmap;

#if 0
		for (usize y = 0; y < map.rows; y++)
		{
			for (usize x = 0; x < map.width; x++)
				std::cout << static_cast<u16>(map.buffer[y * map.width + x]) << ", ";

			std::cout << '\n';
		}

		std::cout << std::flush;
#endif

		if (i == 'A')
			cap::logger::info(cap::logger::level::SYSTEM, std::to_string(map.width) + ", " + std::to_string(map.rows));

		u8* char_image = (u8*)malloc(map.width * map.rows * 4);
		glm::vec<4, u8> fill_color_bytes;

		for (u16 y = 0; y < map.rows; y++)
		{
			for (u16 x = 0; x < map.width; x++)
			{
				u8 pixel		 = map.buffer[y * map.width + x];
				fill_color_bytes = {255, 255, 255, pixel};
				memcpy(char_image + (y * map.width * 4) + (x * 4), &fill_color_bytes, 4);
			}
		}

		char_images.push_back(char_image);
		char_sizes.push_back({map.width, map.rows});
		chars.push_back(i);
		char_offsets.push_back({font_texture_data.face->glyph->bitmap_left, font_texture_data.face->glyph->bitmap_top});
		char_advances.push_back({font_texture_data.face->glyph->advance.x, font_texture_data.face->glyph->advance.y});
	}

	u16 highest_width  = 0;
	u16 highest_height = 0;

	for (usize i = 0; i < char_sizes.size(); i++)
	{
		if (char_sizes[i].x > highest_width)
			highest_width = char_sizes[i].x;

		if (char_sizes[i].y > highest_height)
			highest_height = char_sizes[i].y;
	}

	u16 image_side_length		   = glm::ceil(glm::sqrt(static_cast<f64>(char_images.size())));
	usize texture_sheet_total_size = glm::pow(image_side_length, 2) * highest_width * highest_height * 4;
	u8* texture_sheet			   = (u8*)malloc(texture_sheet_total_size);
	memset(texture_sheet, 0x0, texture_sheet_total_size);

	for (u16 y = 0; y < image_side_length; y++)
	{
		for (u16 x = 0; x < image_side_length; x++)
		{
			u8* texture_sheet_char =
				texture_sheet + (x * highest_width * 4) + (y * image_side_length * highest_width * highest_height * 4);
			u16 char_idx = y * image_side_length + x;

			if (char_idx >= char_sizes.size())
				continue;

			glm::vec<2, f32> first = {
				static_cast<f32>(x * highest_width) / static_cast<f32>(image_side_length * highest_width),
				static_cast<f32>(y * highest_height) / static_cast<f32>(image_side_length * highest_height)};
			glm::vec<2, f32> second = {
				static_cast<f32>(x * highest_width + char_sizes[char_idx].x)
					/ static_cast<f32>(image_side_length * highest_width),
				static_cast<f32>(y * highest_height + char_sizes[char_idx].y)
					/ static_cast<f32>(image_side_length * highest_height)};
			font_texture_data.char_map[chars[char_idx]] = {
				first, second, char_sizes[char_idx], char_offsets[char_idx], char_advances[char_idx]};

			for (u16 cy = 0; cy < char_sizes[char_idx].y; cy++)
				memcpy(
					texture_sheet_char + (cy * image_side_length * highest_width * 4),
					char_images[char_idx] + (cy * char_sizes[char_idx].x * 4),
					char_sizes[char_idx].x * 4);
		}
	}

	//	for (usize i = 0; i < texture_sheet_total_size; i++)
	//		std::cout << (u16)(texture_sheet[i]) << " ";
	//
	//	std::cout << std::flush;

	for (auto char_image : char_images)
		free(char_image);

#if 0
	for (usize i = 0; i < image_side_length * highest_height; i++)
	{
		for (usize a = 0; a < image_side_length * highest_width * 4; a++)
		{
			std::cout << ((texture_sheet[(i * image_side_length * highest_width * 4) + a] == 255) ? 1 : 0);
			//texture_sheet[(i * image_side_length * highest_width * 4) + a] = 128;
		}

		std::cout << '\n';
	}

	std::cout << std::flush;
#endif

	font_texture_data.image = rhr::render::tools::create_texture_image(
		{image_side_length * highest_width, image_side_length * highest_height},
		texture_sheet,
		&font_texture_data.memory);

	local_map->map[type] = font_texture_data;
	if (!found)
		m_texture_maps.push_back(local_map);
}

rhr::registry::char_texture::texture_map* rhr::registry::char_texture::get_texture_map(u16 size)
{
	//	cap::logger::info("size: " + std::to_string(size));

	for (auto font : m_texture_maps)
	{
		if (font->texture_size == size)
			return font;
	}

	process_font("res/Roboto-Regular.ttf", texture_type::LIGHT_NORMAL, size);
	process_font("res/Roboto-Bold.ttf", texture_type::BOLD_NORMAL, size);
	process_font("res/Roboto-Italic.ttf", texture_type::LIGHT_ITALIC, size);
	process_font("res/Roboto-BoldItalic.ttf", texture_type::BOLD_ITALIC, size);

	for (auto font : m_texture_maps)
	{
		if (font->texture_size == size)
		{
			cap::logger::info(
				cap::logger::level::SYSTEM, "successfuly generated char texture of size: " + std::to_string(size));
			return font;
		}
	}

	cap::logger::error(cap::logger::level::SYSTEM, "failed to generate char texture");
	return nullptr;
}

// ft::library rhr::registry::char_texture::m_library;

std::vector<rhr::registry::char_texture::texture_map*> rhr::registry::char_texture::m_texture_maps;