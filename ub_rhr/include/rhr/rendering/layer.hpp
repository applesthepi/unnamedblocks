#if 1
#pragma once
#include "config.h"

#include "rhr/rendering/frame.hpp"
#include "rhr/rendering/interfaces/i_enableable.hpp"

#include <cappuccino/utils.hpp>

namespace rhr::render
{
///
class layer : public rhr::render::interfaces::i_enableable
{
public:
	///
	layer();

	///
	void add_frame(std::weak_ptr<rhr::render::frame>&& frame);

	///
	void render();

	///
	void reload_swap_chain();
private:
	///
	std::shared_mutex m_mutex;

	///
	std::vector<std::weak_ptr<rhr::render::frame>> m_frames;
};
}
#endif