#include "Color.hpp"

Color::Color()
	:m_Color({ 0.0f, 0.0f, 0.0f, 1.0f })
{

}

Color& Color::FromNormalized(const glm::vec<4, float>& color)
{
	m_Color = color;
	return *this;
}

Color& Color::FromU8(const glm::vec<4, uint8_t>& color)
{
	m_Color = { static_cast<float>(color.r) / 255.0f, static_cast<float>(color.g) / 255.0f, static_cast<float>(color.b) / 255.0f, static_cast<float>(color.a) / 255.0f };
	return *this;
}

const glm::vec<4, float>& Color::GetNormalized() const
{
	return m_Color;
}

glm::vec<4, uint8_t> Color::GetU8() const
{
	return glm::vec<4, uint8_t>(static_cast<uint8_t>(m_Color.r * 255.0f), static_cast<uint8_t>(m_Color.g * 255.0f), static_cast<uint8_t>(m_Color.b * 255.0f), static_cast<uint8_t>(m_Color.a * 255.0f));
}

Color Color::BackgroundColor1 = Color().FromU8({ 20, 20, 20, 255 });

Color Color::BackgroundColor2 = Color().FromU8({ 25, 25, 25, 255 });

Color Color::BackgroundColor3 = Color().FromU8({ 32, 32, 32, 255 });

Color Color::TextPrimaryColor = Color().FromU8({ 255, 255, 255, 255 });

Color Color::TextSecondaryColor = Color().FromU8({ 120, 120, 120, 255 });

Color Color::White = Color().FromU8({ 255, 255, 255, 255 });

Color Color::Black = Color().FromU8({ 0, 0, 0, 255 });
