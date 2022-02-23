#include "i_dicolorable.hpp"

rhr::render::interfaces::i_dicolorable::i_dicolorable(
	const espresso::color& primary_color, const espresso::color& secondary_color)
	: m_color_primary(primary_color)
	, m_color_secondary(secondary_color)
{}

void rhr::render::interfaces::i_dicolorable::set_color_primary(const espresso::color& primary)
{
	m_color_primary = primary;
	post_color_update();
}

void rhr::render::interfaces::i_dicolorable::set_color_secondary(const espresso::color& secondary)
{
	m_color_secondary = secondary;
	post_color_update();
}

const espresso::color& rhr::render::interfaces::i_dicolorable::get_color_primary()
{
	return m_color_primary;
}

const espresso::color& rhr::render::interfaces::i_dicolorable::get_color_secondary()
{
	return m_color_secondary;
}

void rhr::render::interfaces::i_dicolorable::post_color_update()
{}
