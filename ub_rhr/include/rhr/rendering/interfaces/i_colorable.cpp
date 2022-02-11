#include "i_colorable.hpp"

rhr::render::interfaces::i_colorable::i_colorable(const cap::color& color)
	: m_color(color)
{}

void rhr::render::interfaces::i_colorable::set_color(const cap::color& color)
{
	m_color = color;
	post_color_update();
}

const cap::color& rhr::render::interfaces::i_colorable::get_color()
{
	return m_color;
}

void rhr::render::interfaces::i_colorable::post_color_update()
{}
