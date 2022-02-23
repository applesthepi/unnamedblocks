#pragma once
#include "config.h"

#include <latte/utils.hpp>

namespace espresso::mod::block
{
///
class flags
{
public:
	///
	enum class type
	{
		FLAG_CHECK_HEAP,
		FLAG_CHECK_STACK,
		SIZE,
		NONE
	};

	///
	class iterator
	{
	public:
		iterator(const u64* flag);

		///
		type pull_next();

		///
		void reset();

	private:
		///
		u64 m_iterator;

		///
		const u64* m_flag;
	};

	flags();

	///
	espresso::mod::block::flags::iterator use_iterator() const;

	///
	flags& check_heap();

	///
	flags& check_stack();

private:
	///
	espresso::mod::block::flags::iterator m_iterator;

	///
	u64 m_flag;
};
} // namespace espresso::mod::block