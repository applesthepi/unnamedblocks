#pragma once
#include "config.h"

#include "stacking/args/Argument.hpp"
#include "registries/UIRegistry.hpp"

#include <Cappuccino/Utils.hpp>
#include <Espresso/InputHandler.hpp>

class ArgumentText : public Argument
{
public:
	/// TODO
	ArgumentText();

	/// How to interpret the data.
	/// \return Type of argument.
	BlockArgumentType GetType() override;

	/// Get width for surrounding argument spacing.
	/// \return Width of argument.
	uint32_t GetWidth() override;

	/// Whether or not the argument contains data for storing.
	/// \return Has data.
	bool HasData() override;
private:
	std::string m_Text;
	uint32_t m_Width;
	bool m_WidthNeedsUpdate;
};
