#pragma once
#include "config.h"

#include <latte/utils.hpp>

namespace mac::texture_sampler
{
///
struct state
{
	///
	vk::sampler texture_sampler;
};

///
mac::texture_sampler::state* create(
	vk::device& logical_device,
	vk::physical_device& physical_device,
	vk::filter magnification,
	vk::filter minification,
	vk::sampler_address_mode mode_u,
	vk::sampler_address_mode mode_v,
	vk::sampler_address_mode mode_w,
	float anisotropy
);

///
void destroy(mac::texture_sampler::state* texture_sampler_state);
}