#pragma once
#include "config.h"

#include "vertices/vertex.hpp"
#include "descriptor_set.hpp"
#include "renderpass.hpp"

#include <latte/utils.hpp>

namespace mac::pipeline
{
///
struct state
{
	///
	std::string name;

	///
	vk::pipeline pipeline;

	///
	vk::pipeline_layout pipeline_layout;
};

///
mac::pipeline::state* create(
	vk::device& logical_device,
	const std::vector<mac::descriptor_set::state*>& descriptor_set_states,
	mac::renderpass::state* renderpass_state,
	mac::vertex::info* vertex_info,
	vk::extent_2d& extent,
	const std::string& name,
	const std::string& shader,
	vk::cull_mode_flags cull_mode_flags
);

///
void destroy(mac::pipeline::state* pipeline_state, vk::device& logical_device);
}