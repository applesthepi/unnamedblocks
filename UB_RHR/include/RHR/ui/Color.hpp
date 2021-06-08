#pragma once
#include "config.h"

#include <Cappuccino/Utils.hpp>

/// Used to standardize color with glm.

class Color
{
public:
	/// Initializes color to opaque black (0.0, 0.0, 0.0, 1.0).
	Color();

	/// Sets color to normalized 4 component normalized (0.0 - 1.0) vector.
	/// \param Color.
	/// \return Color instance.
	Color& FromNormalized(const glm::vec<4, float>&& color);

	/// Sets color to 4 component uint8_t (0 - 255) vector.
	/// \param Color.
	/// \return Color instance.
	Color& FromU8(const glm::vec<4, uint8_t>&& color);

	/// Gets color as float (0.0 - 1.0).
	/// \return Color.
	const glm::vec<4, float>& GetNormalized();

	/// Gets color as a 4 component uint8_t (0 - 255) vector.
	/// \return Color.
	const glm::vec<4, uint8_t>&& GetU8();
private:
	/// Color stored as a 4 component normalized (0.0 - 1.0) vector.
	glm::vec<4, float> m_Color;
}