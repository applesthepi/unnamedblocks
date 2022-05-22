#pragma once
#include "config.h"

#include "macchiato/vertex.hpp"
#include "macchiato/descriptor_set.hpp"

#include <latte/utils.hpp>

namespace mac
{
class object
{
public:
	object(vk::device& logical_device);
	~object();

	///
	virtual void set_data(mac::vertex* vertices, u32* indices) = 0;

	///
	virtual void update_buffers() = 0;

	///
	virtual void render() = 0;
protected:
	///
	virtual void create_ubos() = 0;

	///
	virtual void destroy_ubos() = 0;

	///
	virtual void update_ubos() = 0;

	///
	vk::device& m_logical_device;

	///
	std::vector<mac::descriptor_set::state*> m_descriptor_set_state;

	///
	std::vector<mac::descriptor_set::instance*> m_descriptor_set_instances;
};
}