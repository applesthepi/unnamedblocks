#pragma once
#include "config.h"

#include "cappuccino/utils.hpp"

#if !LINUX
#ifdef __CAP
#define CAP_DLL __declspec(dllexport)
#else
#define CAP_DLL __declspec(dllimport)
#endif
#else
#define CAP_DLL
#endif

namespace cap::mod::block
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
		CAP_DLL iterator(const u64* flag);

		///
		CAP_DLL type pull_next();

		///
		CAP_DLL void reset();
	private:
		///
		u64 m_iterator;

		///
		const u64* m_flag;
	};

	CAP_DLL flags();

	///
	CAP_DLL cap::mod::block::flags::iterator use_iterator() const;

	///
	CAP_DLL flags& check_heap();

	///
	CAP_DLL flags& check_stack();
private:
	///
	cap::mod::block::flags::iterator m_iterator;

	///
	u64 m_flag;
};
}