#include "i_dicolorable.hpp"

rhr::render::interfaces::i_dicolorable::i_dicolorable(const cap::color& primary_color, const cap::color& secondary_color)
	: m_color_primary(primary_color)
	, m_color_secondary(secondary_color)
{

}

void rhr::render::interfaces::i_dicolorable::set_color_primary(const cap::color& primary)
{
	m_color_primary = primary;
	post_color_update();
}

void rhr::render::interfaces::i_dicolorable::set_color_secondary(const cap::color& secondary)
{
	m_color_secondary = secondary;
	post_color_update();
}

const cap::color& rhr::render::interfaces::i_dicolorable::get_color_primary()
{
	return m_color_primary;
}

const cap::color& rhr::render::interfaces::i_dicolorable::get_color_secondary()
{
	return m_color_secondary;
}

void rhr::render::interfaces::i_dicolorable::post_color_update()
{
	
}
