#pragma once
#include "config.h"

#include "object.hpp"
#include "macchiato/window.hpp"
#include "macchiato/ubos/ubo_cam.hpp"
#include "macchiato/ubos/ubo_obj.hpp"

#include <latte/utils.hpp>

namespace mac
{
class object_color : public mac::object
{
public:
	object_color(mac::window::state* window_state, vk::device& logical_device, mac::ubo_cam* ubo_cam);

	///
	mac::ubo_obj& ubo_obj();

	void initial_update_buffers(vma::allocator& allocator, vk::command_buffer& command_buffer) override;
protected:
	///
	void create_ubos(mac::window::state* window_state);

	///
	void destroy_ubos();

	void update_ubos(vma::allocator& allocator) override;
private:
	///
	mac::ubo_cam* m_ubo_cam;
	mac::ubo_obj m_ubo_obj;

	///
	mac::window::state* m_window_state;
};
}