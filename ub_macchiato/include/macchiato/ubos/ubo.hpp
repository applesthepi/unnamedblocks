#pragma once
#include "config.h"

#include <latte/utils.hpp>

namespace mac
{
///
class ubo
{
public:
	///
	virtual u32 size() = 0;

	///
	virtual void* data() = 0;
};
}
