#include "i_enableable.hpp"

rhr::render::interfaces::i_enableable::i_enableable(bool enabled)
	: m_enabled(enabled)
{}

void rhr::render::interfaces::i_enableable::set_enabled(bool enabled)
{
	m_enabled = enabled;
	post_enable_update(enabled);
}

bool rhr::render::interfaces::i_enableable::get_enabled()
{
	return m_enabled;
}

void rhr::render::interfaces::i_enableable::post_enable_update(bool enabled)
{}
