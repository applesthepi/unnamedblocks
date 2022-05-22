#pragma once
#include "config.h"

#include "ubos/ubo.hpp"

#include <latte/utils.hpp>

namespace mac::descriptor_set
{
///
struct descriptor_buffer
{
	///
	vk::buffer buffer;

	///
	vma::allocation allocation;
};

///
struct descriptor_image
{
	///
	vk::image_view image_view;

	///
	vk::sampler* image_sampler;
};

///
struct instance
{
	///
	vk::descriptor_set descriptor_set;

	///
	std::vector<mac::descriptor_set::descriptor_buffer*> descriptor_buffers;

	///
	std::vector<mac::descriptor_set::descriptor_image*> descriptor_images;
};

///
struct state
{
	///
	vk::descriptor_set_layout descriptor_set_layout;

	///
	vk::descriptor_pool* descriptor_pool;

	///
	std::vector<mac::ubo*> descriptor_buffer_ubos;

	///
	u32 descriptor_image_count;
};

///
mac::descriptor_set::state* create(vk::device& logical_device, const std::vector<mac::ubo*>& descriptor_buffer_ubos, u32 descriptor_image_count);

///
void destroy(mac::descriptor_set::state* descriptor_set_state);

///
mac::descriptor_set::instance* create_instance(
	mac::descriptor_set::state* descriptor_set_state,
	vma::allocator& allocator,
	vk::device& logical_device,
	const std::vector<mac::descriptor_set::descriptor_image*>& descriptor_images
);

///
mac::descriptor_set::descriptor_buffer* create_descriptor_buffer(vma::allocator& allocator, mac::ubo* ubo);

///
mac::descriptor_set::descriptor_image* create_descriptor_image(vk::image_view& image_view, vk::sampler* sampler);

///


///
void set_descriptor_pool(mac::descriptor_set::state* descriptor_set_state, vk::descriptor_pool* descriptor_pool);

///
void simplify(const std::vector<mac::descriptor_set::state*>& descriptor_set_states, std::vector<vk::descriptor_set_layout>& descriptor_sets);

///
vk::descriptor_pool create_pool(vk::device& logical_device);

///

}