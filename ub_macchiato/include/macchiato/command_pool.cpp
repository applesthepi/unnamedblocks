#include "command_pool.hpp"

#include "swapchain.hpp"

mac::command_pool::state* mac::command_pool::create(vk::device& logical_device, vk::physical_device& physical_device, vk::surface_khr* surface)
{
	auto command_pool_state = new mac::command_pool::state {

	};

	auto queue_families = mac::swapchain::find_queue_families(physical_device, surface);
	vk::command_pool_create_info command_pool_create_info = {};

	command_pool_create_info.sType			  = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	command_pool_create_info.queueFamilyIndex = queue_families.graphics_family.value();
	command_pool_create_info.flags			  = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

	if (vk::create_command_pool(logical_device, &command_pool_create_info, nullptr, &command_pool_state->command_pool)
		!= VK_SUCCESS)
	{
		latte::logger::fatal(latte::logger::level::SYSTEM, "failed to create command pool");
		return nullptr;
	}
}

void mac::command_pool::destroy(mac::command_pool::state* command_pool_state)
{
	delete command_pool_state;
}
