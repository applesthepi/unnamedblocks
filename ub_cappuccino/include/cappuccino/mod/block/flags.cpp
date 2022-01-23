#include "flags.hpp"

cap::mod::block::flags::iterator::iterator(const u64* flag)
	: m_iterator(0)
	, m_flag(flag)
{}

void cap::mod::block::flags::iterator::reset() { m_iterator = 0; }

cap::mod::block::flags::type cap::mod::block::flags::iterator::pull_next()
{
	if (m_iterator < (u32)cap::mod::block::flags::type::SIZE)
		return cap::mod::block::flags::type(*m_flag << m_iterator++);
	else
	{
		reset();
		return cap::mod::block::flags::type::NONE;
	}
}

cap::mod::block::flags::flags()
	: m_iterator(cap::mod::block::flags::iterator(&m_flag))
{
	m_flag = 0;
}

cap::mod::block::flags::iterator cap::mod::block::flags::use_iterator() const { return m_iterator; }

cap::mod::block::flags& cap::mod::block::flags::check_heap()
{
	m_flag |= (u64)cap::mod::block::flags::type::FLAG_CHECK_HEAP;
	return *this;
}

cap::mod::block::flags& cap::mod::block::flags::check_stack()
{
	m_flag |= (u64)cap::mod::block::flags::type::FLAG_CHECK_STACK;
	return *this;
}
