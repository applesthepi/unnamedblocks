#pragma once
#include "config.h"

#include <latte/utils.hpp>

namespace espresso
{
/// Used to standardize color with glm.
class color
{
public:
	/// Initializes color to opaque black (0.0f, 0.0f, 0.0f, 1.0f).
	color();

	/// Sets color to normalized 4 component normalized (0.0 - 1.0) vector.
	/// \param Color.
	/// \return Color instance.
	color& from_normalized(const glm::vec<4, f32>& color);

	/// Sets color to 4 component u8 (0 - 255) vector.
	/// \param Color.
	/// \return Color instance.
	color& from_u8(const glm::vec<4, u8>& color);

	/// Gets color as an f32 (0.0 - 1.0).
	/// \return Color.
	const glm::vec<4, f32>& get_normalized() const;

	///
	glm::vec<4, f32> get_normalized_scaled(f32 scale, bool scale_alpha) const;

	///
	color get_scaled(f32 scale, bool scale_alpha) const;

	/// Gets color as a 4 component u8 (0 - 255) vector.
	/// \return Color.
	glm::vec<4, u8> get_u8() const;

	static color background_color_1;
	static color background_color_2;
	static color background_color_3;
	static color text_primary_color;
	static color text_secondary_color;
	static color white;
	static color black;
	static color alpha;

private:
	/// Color stored as a 4 component normalized (0.0 - 1.0) vector.
	glm::vec<4, f32> m_color;
};
} // namespace cap