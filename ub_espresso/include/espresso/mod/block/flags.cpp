#include "flags.hpp"

espresso::mod::block::flags::iterator::iterator(const u64* flag)
	: m_iterator(0)
	, m_flag(flag)
{}

void espresso::mod::block::flags::iterator::reset()
{
	m_iterator = 0;
}

espresso::mod::block::flags::type espresso::mod::block::flags::iterator::pull_next()
{
	if (m_iterator < (u32)espresso::mod::block::flags::type::SIZE)
		return espresso::mod::block::flags::type(*m_flag << m_iterator++);
	else
	{
		reset();
		return espresso::mod::block::flags::type::NONE;
	}
}

espresso::mod::block::flags::flags()
	: m_iterator(espresso::mod::block::flags::iterator(&m_flag))
{
	m_flag = 0;
}

espresso::mod::block::flags::iterator espresso::mod::block::flags::use_iterator() const
{
	return m_iterator;
}

espresso::mod::block::flags& espresso::mod::block::flags::check_heap()
{
	m_flag |= (u64)espresso::mod::block::flags::type::FLAG_CHECK_HEAP;
	return *this;
}

espresso::mod::block::flags& espresso::mod::block::flags::check_stack()
{
	m_flag |= (u64)espresso::mod::block::flags::type::FLAG_CHECK_STACK;
	return *this;
}
