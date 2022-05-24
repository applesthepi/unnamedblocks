#include "descriptor_set.hpp"

mac::descriptor_set::state* mac::descriptor_set::create(vk::device& logical_device, const std::vector<mac::ubo*>& descriptor_buffer_ubos, u32 descriptor_image_count)
{
	auto descriptor_set_state = new mac::descriptor_set::state {
		.descriptor_pool = nullptr,
		.descriptor_buffer_ubos = descriptor_buffer_ubos,
		.descriptor_image_count = descriptor_image_count,
	};

	std::vector<vk::descriptor_set_layout_binding> descriptor_set_layout_bindings;
	descriptor_set_layout_bindings.reserve(descriptor_buffer_ubos.size() + descriptor_image_count);

	for (u32 i = 0; i < descriptor_buffer_ubos.size(); i++)
	{
		auto& descriptor_set_layout_binding = descriptor_set_layout_bindings.emplace_back();
		descriptor_set_layout_binding = {};

		descriptor_set_layout_binding.binding = i;
		descriptor_set_layout_binding.descriptorCount = 1;
		descriptor_set_layout_binding.pImmutableSamplers = nullptr;

		descriptor_set_layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptor_set_layout_binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	}

	for (u32 i = 0; i < descriptor_image_count; i++)
	{
		auto& descriptor_set_layout_binding = descriptor_set_layout_bindings.emplace_back();
		descriptor_set_layout_binding = {};

		descriptor_set_layout_binding.binding = i + descriptor_buffer_ubos.size();
		descriptor_set_layout_binding.descriptorCount = 1;
		descriptor_set_layout_binding.pImmutableSamplers = nullptr;

		descriptor_set_layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptor_set_layout_binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	}

	vk::descriptor_set_layout_create_info descriptor_set_layout_create_info = {};

	descriptor_set_layout_create_info.sType		   = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descriptor_set_layout_create_info.bindingCount = static_cast<u32>(descriptor_set_layout_bindings.size());
	descriptor_set_layout_create_info.pBindings	   = descriptor_set_layout_bindings.data();

	if (vk::create_descriptor_set_layout(
		logical_device, &descriptor_set_layout_create_info, nullptr, &descriptor_set_state->descriptor_set_layout)
		!= VK_SUCCESS)
	{
		latte::logger::fatal(latte::logger::level::SYSTEM, "failed to create descriptor set layout");
		return nullptr;
	}

	return descriptor_set_state;
}

void mac::descriptor_set::destroy(mac::descriptor_set::state* descriptor_set_state)
{
	delete descriptor_set_state;
}

mac::descriptor_set::instance* mac::descriptor_set::create_instance(
	mac::descriptor_set::state* descriptor_set_state,
	vma::allocator& allocator,
	vk::device& logical_device,
	const std::vector<mac::descriptor_set::descriptor_image*>& descriptor_images
)
{
	if (descriptor_images.size() != descriptor_set_state->descriptor_image_count)
	{
		latte::logger::fatal(latte::logger::level::SYSTEM, "the provided resources does not cover the descriptor set image count");
		return nullptr;
	}

	auto instance = new mac::descriptor_set::instance {};

	for (auto descriptor_buffer_ubo : descriptor_set_state->descriptor_buffer_ubos)
	{
		instance->descriptor_buffers.emplace_back(create_descriptor_buffer(
			allocator,
			descriptor_buffer_ubo
		));
	}

	instance->descriptor_images = descriptor_images;

	vk::descriptor_set_allocate_info descriptor_set_allocate_info = {};

	descriptor_set_allocate_info.sType			    = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	descriptor_set_allocate_info.descriptorSetCount = 1;
	descriptor_set_allocate_info.pSetLayouts		= &descriptor_set_state->descriptor_set_layout;
	descriptor_set_allocate_info.descriptorPool	    = *descriptor_set_state->descriptor_pool;

	if (vk::allocate_descriptor_sets(logical_device, &descriptor_set_allocate_info, &instance->descriptor_set) != VK_SUCCESS)
	{
		latte::logger::fatal(latte::logger::level::SYSTEM, "failed to allocate descriptor set for instance");
		return nullptr;
	}

	instance->descriptor_buffer_infos.reserve(instance->descriptor_buffers.size());
	instance->descriptor_image_infos.reserve(instance->descriptor_images.size());

	for (u32 i = 0; i < instance->descriptor_buffers.size(); i++)
	{
		auto& descriptor_buffer_info = instance->descriptor_buffer_infos.emplace_back(vk::descriptor_buffer_info{});

		descriptor_buffer_info.buffer = instance->descriptor_buffers[i]->buffer;
		descriptor_buffer_info.offset = 0;
		descriptor_buffer_info.range  = descriptor_set_state->descriptor_buffer_ubos[i]->size();
	}

	for (auto descriptor_image : instance->descriptor_images)
	{
		auto& descriptor_image_info = instance->descriptor_image_infos.emplace_back(vk::descriptor_image_info{});

		descriptor_image_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		descriptor_image_info.imageView   = descriptor_image->image_view;
		descriptor_image_info.sampler     = *descriptor_image->image_sampler;
	}

	instance->write_descriptor_sets.reserve(instance->descriptor_buffer_infos.size() + instance->descriptor_image_infos.size());

	for (u32 i = 0; i < instance->descriptor_buffer_infos.size(); i++)
	{
		auto& write_descriptor_set = instance->write_descriptor_sets.emplace_back(vk::write_descriptor_set{});

		write_descriptor_set.sType			 = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write_descriptor_set.dstSet			 = instance->descriptor_set;
		write_descriptor_set.dstBinding		 = i;
		write_descriptor_set.dstArrayElement = 0;
		write_descriptor_set.descriptorType	 = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		write_descriptor_set.descriptorCount = 1;
		write_descriptor_set.pBufferInfo	 = &instance->descriptor_buffer_infos[i];
	}

	for (u32 i = 0; i < instance->descriptor_image_infos.size(); i++)
	{
		auto& write_descriptor_set = instance->write_descriptor_sets.emplace_back(vk::write_descriptor_set{});

		write_descriptor_set.sType			 = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write_descriptor_set.dstSet			 = instance->descriptor_set;
		write_descriptor_set.dstBinding		 = i + instance->descriptor_buffer_infos.size();
		write_descriptor_set.dstArrayElement = 0;
		write_descriptor_set.descriptorType	 = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		write_descriptor_set.descriptorCount = 1;
		write_descriptor_set.pImageInfo		 = &instance->descriptor_image_infos[i];
	}

	vk::update_descriptor_sets(
		logical_device,
		static_cast<u32>(instance->write_descriptor_sets.size()),
		instance->write_descriptor_sets.data(),
		0,
		nullptr
	);

	return instance;
}

mac::descriptor_set::descriptor_buffer* mac::descriptor_set::create_descriptor_buffer(vma::allocator& allocator, mac::ubo* ubo)
{
	auto descriptor_buffer = new mac::descriptor_set::descriptor_buffer {};

	vk::buffer_create_info buffer_create_info = {};
	vma::allocation_create_info allocation_create_info = {};

	buffer_create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	buffer_create_info.size	 = ubo->size();
	buffer_create_info.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

	allocation_create_info.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;
	allocation_create_info.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;

	if (vma::create_buffer(
		allocator,
		&buffer_create_info,
		&allocation_create_info,
		&descriptor_buffer->buffer,
		&descriptor_buffer->allocation,
		nullptr) != VK_SUCCESS)
	{
		latte::logger::fatal(latte::logger::level::SYSTEM, "failed to allocate a buffer for a descriptor buffer");
		return nullptr;
	}

	return descriptor_buffer;
}

mac::descriptor_set::descriptor_image*
mac::descriptor_set::create_descriptor_image(vk::image_view& image_view, vk::sampler* sampler)
{
	auto descriptor_image = new mac::descriptor_set::descriptor_image {
		.image_view = image_view,
		.image_sampler = sampler,
	};

	return descriptor_image;
}

void
mac::descriptor_set::update_descriptor_buffer(mac::descriptor_set::descriptor_buffer* descriptor_buffer, vma::allocator& allocator, mac::ubo* ubo)
{
	void* data;
	vma::map_memory(allocator, descriptor_buffer->allocation, &data);
	memcpy(data, ubo->data(), ubo->size());
	vma::unmap_memory(allocator, descriptor_buffer->allocation);
}

void mac::descriptor_set::set_descriptor_pool(
	mac::descriptor_set::state* descriptor_set_state, vk::descriptor_pool* descriptor_pool
)
{
	descriptor_set_state->descriptor_pool = descriptor_pool;
}

void mac::descriptor_set::simplify_descriptor_set_layouts(
	const std::vector<mac::descriptor_set::state*>& descriptor_set_states,
	std::vector<vk::descriptor_set_layout>& descriptor_sets
)
{
	descriptor_sets.reserve(descriptor_set_states.size());

	for (const auto& descriptor_set_state : descriptor_set_states)
		descriptor_sets.emplace_back(descriptor_set_state->descriptor_set_layout);
}

void mac::descriptor_set::simplify_descriptor_sets(
	const std::vector<mac::descriptor_set::instance*>& instances,
	std::vector<vk::descriptor_set>& descriptor_sets
)
{
	for (auto instance : instances)
		descriptor_sets.emplace_back(instance->descriptor_set);
}

vk::descriptor_pool mac::descriptor_set::create_pool(vk::device& logical_device)
{
	std::array<vk::descriptor_pool_size, 2> descriptor_pool_sizes = {};
	vk::descriptor_pool_create_info descriptor_pool_create_info	  = {};

	descriptor_pool_sizes[0].type			 = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptor_pool_sizes[0].descriptorCount = static_cast<u32>(10000);

	descriptor_pool_sizes[1].type			 = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	descriptor_pool_sizes[1].descriptorCount = static_cast<u32>(10000);

	descriptor_pool_create_info.sType		  = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descriptor_pool_create_info.poolSizeCount = static_cast<u32>(descriptor_pool_sizes.size());
	descriptor_pool_create_info.pPoolSizes	  = descriptor_pool_sizes.data();
	descriptor_pool_create_info.maxSets		  = 10000;
	descriptor_pool_create_info.flags		  = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

	vk::descriptor_pool descriptor_pool = {};

	if (vk::create_descriptor_pool(logical_device, &descriptor_pool_create_info, nullptr, &descriptor_pool)
		!= VK_SUCCESS)
		latte::logger::fatal(latte::logger::level::SYSTEM, "failed to create descriptor pool");

	return descriptor_pool;
}
