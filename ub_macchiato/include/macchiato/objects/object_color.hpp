#pragma once
#include "config.h"

#include "object.hpp"
#include "macchiato/window.hpp"

#include <latte/utils.hpp>

namespace mac
{
class object_color : public mac::object
{
public:
	object_color(mac::window::state* window_state, vk::device& logical_device);
	~object_color();

	///
	void set_data(mac::vertex* vertices, u32* indices) override;

	///
	void update_buffers() override;

	///
	void render() override;
protected:
	///
	void create_ubos(mac::window::state* window_state) override;

	///
	void destroy_ubos() override;

	///
	void update_ubos() override;
};
}