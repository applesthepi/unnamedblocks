#pragma once
#include "config.h"

#include <latte/utils.hpp>

namespace rhr::registry
{
///
class char_texture
{
public:
	///
	struct char_data
	{
		///
		glm::vec<2, f32> first;

		///
		glm::vec<2, f32> second;

		///
		glm::vec<2, u16> size;

		///
		glm::vec<2, u16> offset;

		///
		glm::vec<2, u16> advance;
	};

	///
	struct texture_data
	{
		///
		ft::face face;

		///
		vk::image image;

		///
		vk::device_memory memory;

		///
		std::unordered_map<i8, char_data> char_map;
	};

	///
	enum class texture_type
	{
		LIGHT_NORMAL,
		BOLD_NORMAL,
		LIGHT_ITALIC,
		BOLD_ITALIC
	};

	///
	struct texture_map
	{
		texture_map(u16 size)
			: texture_size(size)
		{
			ft::error error = FT_Init_FreeType(&library);
			if (error)
			{
				latte::logger::error(
					latte::logger::level::SYSTEM, "failed to init freetype \"" + std::to_string(error) + "\"");
				return;
			}
		}

		///
		u16 texture_size;

		///
		std::unordered_map<texture_type, texture_data> map;

		///
		ft::library library;
	};

	///
	static char_texture* get();

	///
	static void set(char_texture* reg);

	/// Loads and processes fonts.
	void process_fonts();

	///
	texture_map* get_texture_map(u16 size);

private:
	///
	static char_texture* m_registry;

	///
	void process_font(const std::string& font_path, texture_type type, u16 size);

	///
	std::vector<texture_map*> m_texture_maps;
};
} // namespace rhr::registry
