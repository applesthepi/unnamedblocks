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

	///
	std::vector<vk::descriptor_buffer_info> descriptor_buffer_infos;

	///
	std::vector<vk::descriptor_image_info> descriptor_image_infos;

	///
	std::vector<vk::write_descriptor_set> write_descriptor_sets;
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
void update_descriptor_buffer(mac::descriptor_set::descriptor_buffer* descriptor_buffer, vma::allocator& allocator, mac::ubo* ubo);

///
void set_descriptor_pool(mac::descriptor_set::state* descriptor_set_state, vk::descriptor_pool* descriptor_pool);

///
void simplify_descriptor_set_layouts(const std::vector<mac::descriptor_set::state*>& descriptor_set_states, std::vector<vk::descriptor_set_layout>& descriptor_sets);

///
void simplify_descriptor_sets(const std::vector<mac::descriptor_set::instance*>& instances, std::vector<vk::descriptor_set>& descriptor_sets);

///
vk::descriptor_pool create_pool(vk::device& logical_device);
}