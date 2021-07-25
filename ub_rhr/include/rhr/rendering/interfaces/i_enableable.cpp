#include "i_enableable.hpp"

rhr::render::interfaces::i_enableable::i_enableable(bool enabled)
	: m_enabled(enabled)
{

}

void rhr::render::interfaces::i_enableable::set_enabled(bool enabled)
{
	m_enabled = enabled;
}

bool rhr::render::interfaces::i_enableable::get_enabled()
{
	return m_enabled;
}
