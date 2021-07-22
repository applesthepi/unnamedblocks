#pragma once
#include "config.h"

#include <Cappuccino/Color.hpp>
#include <Cappuccino/Utils.hpp>

/// Interface for setting a color

class IColorable
{
public:
	/// Sets default color
	IColorable(const Color& color);

	/// Sets color.
	/// \param Color.
	void SetColor(const Color& color);

	/// Gets color.
	/// \return Color.
	const Color& GetColor();
protected:
	/// Color.
	Color m_Color;

	/// Gets called when the color is changed.
	virtual void PostColorUpdate();
};