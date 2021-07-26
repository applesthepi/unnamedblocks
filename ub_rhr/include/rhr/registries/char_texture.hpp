#pragma once
#include "config.h"

#include <cappuccino/utils.hpp>

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

	/// Loads and processes fonts.
	static void process_fonts();

	/// Map of font texture's data.
	static std::unordered_map<texture_type, texture_data> texture_map;
private:
	///
	static void process_font(const std::string& font_path, texture_type type);

	/// Used to load ttf fonts.
	static ft::library m_library;
};
}
