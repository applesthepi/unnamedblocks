#pragma once
#include "config.h"

#include <latte/utils.hpp>

namespace mac::descriptor_set
{
///
enum class binding_type
{
	UNIFORM_BUFFER,
	IMAGE_SAMPLER,
};

///
struct state
{
	///
	vk::descriptor_set_layout descriptor_set_layout;

	///
	vk::descriptor_pool* descriptor_pool;
};

///
mac::descriptor_set::state* create(vk::device& logical_device, const std::vector<mac::descriptor_set::binding_type>& bindings);

///
void destroy(mac::descriptor_set::state* descriptor_set_state);

///
void instance(mac::descriptor_set::state* descriptor_set_state, vk::device& logical_device, vk::descriptor_set& descriptor_set_instance);

///
void set_descriptor_pool(mac::descriptor_set::state* descriptor_set_state, vk::descriptor_pool* descriptor_pool);

///
void simplify(const std::vector<mac::descriptor_set::state*>& descriptor_set_states, std::vector<vk::descriptor_set_layout>& descriptor_sets);

///
vk::descriptor_pool create_pool(vk::device& logical_device);
}