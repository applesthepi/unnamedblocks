#include "descriptor_set.hpp"

mac::descriptor_set::state* mac::descriptor_set::create(vk::device& logical_device, const std::vector<mac::descriptor_set::binding_type>& bindings)
{
	auto descriptor_set_state = new mac::descriptor_set::state {
		.descriptor_pool = nullptr,
	};

	std::vector<vk::descriptor_set_layout_binding> descriptor_set_layout_bindings;
	descriptor_set_layout_bindings.reserve(bindings.size());

	for (u32 i = 0; i < bindings.size(); i++)
	{
		auto& descriptor_set_layout_binding = descriptor_set_layout_bindings.emplace_back();
		descriptor_set_layout_binding = {};

		descriptor_set_layout_binding.binding			 = i;
		descriptor_set_layout_binding.descriptorCount	 = 1;
		descriptor_set_layout_binding.pImmutableSamplers = nullptr;

		switch (bindings[i])
		{
		case mac::descriptor_set::binding_type::UNIFORM_BUFFER:
			descriptor_set_layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptor_set_layout_binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
			break;
		case mac::descriptor_set::binding_type::IMAGE_SAMPLER:
			descriptor_set_layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptor_set_layout_binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
			break;
		default:
			break;
		}
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

void mac::descriptor_set::instance(mac::descriptor_set::state* descriptor_set_state, vk::device& logical_device, vk::descriptor_set& descriptor_set_instance)
{
	vk::descriptor_set_allocate_info descriptor_set_allocate_info = {};
	descriptor_set_allocate_info.sType				 = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	descriptor_set_allocate_info.descriptorSetCount = 1;
	descriptor_set_allocate_info.pSetLayouts		 = &descriptor_set_state->descriptor_set_layout;
	descriptor_set_allocate_info.descriptorPool	 = *descriptor_set_state->descriptor_pool;

	if (vk::allocate_descriptor_sets(logical_device, &descriptor_set_allocate_info, &descriptor_set_instance) != VK_SUCCESS)
	{
		latte::logger::fatal(latte::logger::level::SYSTEM, "failed to allocate descriptor set for instance");
		return;
	}
}

void mac::descriptor_set::set_descriptor_pool(
	mac::descriptor_set::state* descriptor_set_state, vk::descriptor_pool* descriptor_pool
)
{
	descriptor_set_state->descriptor_pool = descriptor_pool;
}

void mac::descriptor_set::simplify(
	const std::vector<mac::descriptor_set::state*>& descriptor_set_states,
	std::vector<vk::descriptor_set_layout>& descriptor_sets
)
{
	descriptor_sets.reserve(descriptor_set_states.size());

	for (const auto& descriptor_set_state : descriptor_set_states)
		descriptor_sets.emplace_back(descriptor_set_state->descriptor_set_layout);
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
