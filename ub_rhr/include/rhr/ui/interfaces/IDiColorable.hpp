#pragma once
#include "config.h"

#include <Cappuccino/Color.hpp>
#include <Cappuccino/Utils.hpp>

/// Interface for setting two colors.

class IDiColorable
{
public:
	/// Sets default colors.
	IDiColorable(const Color& primary, const Color& secondary);

	/// Sets primary color.
	/// \param Primary color.
	void SetColorPrimary(const Color& primary);

	/// Sets secondary color.
	/// \param Secondary color.
	void SetColorSecondary(const Color& secondary);

	/// Gets primary color.
	/// \return Primary color.
	const Color& GetColorPrimary();

	/// Gets secondary color.
	/// \return Secondary color.
	const Color& GetColorSecondary();
protected:
	/// Gets called when the color is changed.
	virtual void PostColorUpdate();

	/// Color.
	Color m_ColorPrimary;
	Color m_ColorSecondary;
};