#include "color.hpp"

espresso::color::color()
	: m_color({0.0f, 0.0f, 0.0f, 1.0f})
{}

espresso::color& espresso::color::from_normalized(const glm::vec<4, f32>& color)
{
	m_color = color;
	return *this;
}

espresso::color& espresso::color::from_u8(const glm::vec<4, u8>& color)
{
	m_color = {
		static_cast<f32>(color.r) / 255.0f,
		static_cast<f32>(color.g) / 255.0f,
		static_cast<f32>(color.b) / 255.0f,
		static_cast<f32>(color.a) / 255.0f};

	return *this;
}

const glm::vec<4, f32>& espresso::color::get_normalized() const
{
	return m_color;
}

glm::vec<4, f32> espresso::color::get_normalized_scaled(f32 scale, bool scale_alpha) const
{
	if (scale_alpha)
		return {m_color.r * scale, m_color.g * scale, m_color.b * scale, m_color.a * scale};
	else
		return {m_color.r * scale, m_color.g * scale, m_color.b * scale, m_color.a};
}

glm::vec<4, u8> espresso::color::get_u8() const
{
	return glm::vec<4, u8>(
		static_cast<u8>(m_color.r * 255.0f),
		static_cast<u8>(m_color.g * 255.0f),
		static_cast<u8>(m_color.b * 255.0f),
		static_cast<u8>(m_color.a * 255.0f));
}

espresso::color espresso::color::background_color_1 = espresso::color().from_u8({20, 20, 20, 255});

espresso::color espresso::color::background_color_2 = espresso::color().from_u8({25, 25, 25, 255});

espresso::color espresso::color::background_color_3 = espresso::color().from_u8({32, 32, 32, 255});

espresso::color espresso::color::text_primary_color = espresso::color().from_u8({255, 255, 255, 255});

espresso::color espresso::color::text_secondary_color = espresso::color().from_u8({120, 120, 120, 255});

espresso::color espresso::color::white = espresso::color().from_u8({255, 255, 255, 255});

espresso::color espresso::color::black = espresso::color().from_u8({0, 0, 0, 255});

espresso::color espresso::color::alpha = espresso::color().from_u8({0, 0, 0, 0});