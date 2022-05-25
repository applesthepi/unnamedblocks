#pragma once
#include "config.h"

#include "object.hpp"
#include "macchiato/window.hpp"
#include "macchiato/ubos/ubo_cam.hpp"
#include "macchiato/ubos/ubo_obj.hpp"

#include <latte/utils.hpp>

namespace mac
{
class object_texture : public mac::object
{
public:
	object_texture(mac::window::state* window_state, vk::image_view& image_view);

	///
	mac::ubo_obj::uniform& ubo_obj();

	void initial_update_buffers(vma::allocator& allocator, vk::command_buffer& command_buffer) override;
protected:
	void update_ubos(vma::allocator& allocator) override;

	///
	void create_ubos(mac::window::state* window_state);

	///
	void destroy_ubos();
private:
	///
	mac::ubo_obj m_ubo_obj;

	///
	vk::image_view& m_image_view;
};
}