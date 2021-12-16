#include "tools.hpp"

#include "rhr/rendering/renderer.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void rhr::render::tools::initialize()
{
	validation_layers = {
		"VK_LAYER_NV_optimus",
		"VK_LAYER_KHRONOS_synchronization2",
		"VK_LAYER_KHRONOS_validation",
		"VK_LAYER_LUNARG_monitor",
		"VK_LAYER_LUNARG_screenshot"
	};

	device_extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
}

std::vector<char>* rhr::render::tools::read_file_bytes(const std::string& file_name)
{
	std::ifstream file(file_name, std::ios::ate | std::ios::binary);

	if (!file.is_open())
		cap::logger::fatal(cap::logger::level::SYSTEM, "failed to open file \"" + file_name + "\"");

	usize file_size = (usize)file.tellg();
	std::vector<char>* buffer = new std::vector<char>(file_size);

	file.seekg(0);
	file.read(buffer->data(), file_size);
	file.close();

	return buffer;
}

bool rhr::render::tools::check_validation_layer_support()
{
	u32 layer_count;
	vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

	available_validation_layers.clear();
	available_validation_layers.resize(layer_count);
	vkEnumerateInstanceLayerProperties(&layer_count, available_validation_layers.data());

#if 1
	for (const char* layer_name : validation_layers)
	{
		bool layer_found = false;

		for (const auto& layer_properties : available_validation_layers)
		{
			if (strcmp(layer_name, layer_properties.layerName) == 0)
			{
				layer_found = true;
				break;
			}
		}

		if (!layer_found)
		{
			cap::logger::warn(cap::logger::level::SYSTEM, "layer \"" + std::string(layer_name) + "\" not supported");
			return false;
		}
	}
#else
	validation_layers.clear();

	for (const auto& layer_properties : available_validation_layers)
		validation_layers.push_back(layer_properties.layerName);
#endif

	for (const auto& validation_layer : validation_layers)
		cap::logger::info(cap::logger::level::SYSTEM, "loading validation layer " + std::string(validation_layer));

	return true;
}

vk::image_view rhr::render::tools::create_image_view(vk::image image, vk::format format, vk::image_aspect_flags aspect_flags)
{
	vk::image_view_create_info view_info {};
	view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	view_info.image = image;
	view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
	view_info.format = format;
	view_info.subresourceRange.aspectMask = aspect_flags;
	view_info.subresourceRange.baseMipLevel = 0;
	view_info.subresourceRange.levelCount = 1;
	view_info.subresourceRange.baseArrayLayer = 0;
	view_info.subresourceRange.layerCount = 1;

	vk::image_view image_view;

	if (vkCreateImageView(*rhr::render::renderer::get_window_primary()->get_device(), &view_info, nullptr, &image_view) != VK_SUCCESS)
		cap::logger::fatal(cap::logger::level::SYSTEM, "failed to create image view");

	return image_view;
}

void rhr::render::tools::create_aux_command_buffer()
{
	return;
	//std::unique_lock<SHARED_MUTEX> lock(rhr::render::renderer::AuxCommandBufferMutex);
	//
	//vk::command_buffer_allocate_info allocInfo {};
	//allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	//allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	//allocInfo.commandPool = rhr::render::renderer::CommandPool;
	//allocInfo.commandBufferCount = 1;
	//
	//vkAllocateCommandBuffers(*rhr::render::renderer::get_window_primary()->get_device(), &allocInfo, &rhr::render::renderer::AuxCommandBuffer);
	//
	//vk::command_buffer_begin_info beginInfo {};
	//beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	//beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	//
	//vkBeginCommandBuffer(rhr::render::renderer::AuxCommandBuffer, &beginInfo);
}

void rhr::render::tools::reset_aux_command_buffer()
{
	return;
	//std::unique_lock<SHARED_MUTEX> lock(rhr::render::renderer::AuxCommandBufferMutex);
	//
	//vkEndCommandBuffer(rhr::render::renderer::AuxCommandBuffer);
	//
	//vk::submit_info submitInfo {};
	//submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	//submitInfo.commandBufferCount = 1;
	//submitInfo.pCommandBuffers = &rhr::render::renderer::AuxCommandBuffer;
	//
	//vkQueueSubmit(rhr::render::renderer::GraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	//vkQueueWaitIdle(rhr::render::renderer::GraphicsQueue);
	//CleanAuxCommandBuffer();
	//
	//vk::command_buffer_begin_info beginInfo {};
	//beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	//beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	//
	//vkBeginCommandBuffer(rhr::render::renderer::AuxCommandBuffer, &beginInfo);
}

void rhr::render::tools::clean_aux_command_buffer()
{
	return;
	//for (u32 i = 0; i < rhr::render::renderer::AuxBufferMemory.size(); i++)
	//	vkDestroyBuffer(*rhr::render::renderer::get_window_primary()->get_device(), rhr::render::renderer::AuxBufferMemory[i], nullptr);
	//
	//for (u32 i = 0; i < rhr::render::renderer::AuxDeviceMemory.size(); i++)
	//	vkFreeMemory(*rhr::render::renderer::get_window_primary()->get_device(), rhr::render::renderer::AuxDeviceMemory[i], nullptr);
	//
	//rhr::render::renderer::AuxBufferMemory.clear();
	//rhr::render::renderer::AuxDeviceMemory.clear();
}

vk::command_buffer rhr::render::tools::begin_single_time_command()
{
	vk::command_buffer command_buffer;

	vk::command_buffer_allocate_info allocate_info {};
	allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocate_info.commandPool = *rhr::render::renderer::get_window_primary()->get_command_pool();
	allocate_info.commandBufferCount = 1;

	vkAllocateCommandBuffers(*rhr::render::renderer::get_window_primary()->get_device(), &allocate_info, &command_buffer);

	vk::command_buffer_begin_info begin_info {};
	begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(command_buffer, &begin_info);
	return command_buffer;
}

void rhr::render::tools::end_single_time_command(vk::command_buffer command_buffer)
{
	vkEndCommandBuffer(command_buffer);

	vk::submit_info submit_info {};
	submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit_info.commandBufferCount = 1;
	submit_info.pCommandBuffers = &command_buffer;

	vkQueueSubmit(*rhr::render::renderer::get_window_primary()->get_graphics_queue(), 1, &submit_info, VK_NULL_HANDLE);
	vkQueueWaitIdle(*rhr::render::renderer::get_window_primary()->get_graphics_queue());
	vkFreeCommandBuffers(*rhr::render::renderer::get_window_primary()->get_device(), *rhr::render::renderer::get_window_primary()->get_command_pool(), 1, &command_buffer);
}

vk::format rhr::render::tools::find_supported_format(const std::vector<vk::format>& candidates, vk::image_tiling tiling, vk::format_feature_flags features)
{
	for (vk::format format : candidates)
	{
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(*rhr::render::renderer::get_window_primary()->get_physical_device(), format, &props);

		if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
		{
			return format;
		}
		else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
		{
			return format;
		}
	}

	cap::logger::fatal(cap::logger::level::SYSTEM, "failed to find supported format");
	return candidates.front();
}

vk::format rhr::render::tools::find_depth_format()
{
	return find_supported_format(
	    { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
	    VK_IMAGE_TILING_OPTIMAL,
	    VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

bool rhr::render::tools::has_stencil_component(vk::format format)
{
	return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

void rhr::render::tools::transition_image_layout(vk::image image, vk::format format, vk::image_layout old_layout, vk::image_layout new_layout)
{
	vk::command_buffer command_buffer = begin_single_time_command();

	vk::image_memory_barrier barrier {};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = old_layout;
	barrier.newLayout = new_layout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = image;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;
	barrier.srcAccessMask = 0; // TODO
	barrier.dstAccessMask = 0; // TODO

	vk::pipeline_stage_flags source_stage;
	vk::pipeline_stage_flags destination_stage;

	if (new_layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
	{
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

		if (has_stencil_component(format))
		{
			barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
		}
	}
	else
	{
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	}

	if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED && new_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
	{
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destination_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (old_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && new_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
	{
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		source_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		destination_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}
	else if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED && new_layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
	{
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destination_stage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	}
	else
		cap::logger::fatal(cap::logger::level::SYSTEM, "unsupported layout transition");

	vkCmdPipelineBarrier(
	    command_buffer,
	    source_stage, destination_stage,
	    0,
	    0, nullptr,
	    0, nullptr,
	    1, &barrier);

	end_single_time_command(command_buffer);
}

u32 rhr::render::tools::find_memory_type(u32 type_filter, vk::memory_property_flags properties)
{
	vk::physical_device_memory_properties memory_properties;
	vkGetPhysicalDeviceMemoryProperties(*rhr::render::renderer::get_window_primary()->get_physical_device(), &memory_properties);

	for (u32 i = 0; i < memory_properties.memoryTypeCount; i++)
	{
		if ((type_filter & (1 << i)) && (memory_properties.memoryTypes[i].propertyFlags & properties) == properties)
			return i;
	}

	cap::logger::fatal(cap::logger::level::SYSTEM, "failed to find suitable memory type");
	return 0;
}

void rhr::render::tools::copy_buffer_to_image(vk::buffer buffer, vk::image image, u32 width, u32 height)
{
	vk::command_buffer command_buffer = begin_single_time_command();

	vk::buffer_image_copy region {};
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;

	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;

	region.imageOffset = { 0, 0, 0 };
	region.imageExtent = {
		width,
		height,
		1
	};

	vkCmdCopyBufferToImage(
	    command_buffer,
	    buffer,
	    image,
	    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
	    1,
	    &region);

	end_single_time_command(command_buffer);
}

void rhr::render::tools::create_buffer(vk::device_size size, VkBufferUsageFlags usage, vk::memory_property_flags properties, vk::buffer& buffer, vk::device_memory& buffer_memory)
{
	vk::buffer_create_info buffer_info {};
	buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	buffer_info.size = size;
	buffer_info.usage = usage;
	buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(*rhr::render::renderer::get_window_primary()->get_device(), &buffer_info, nullptr, &buffer) != VK_SUCCESS)
	{
		cap::logger::error(cap::logger::level::SYSTEM, "failed to create buffer");
		return;
	}

	vk::memory_requirements memory_requirements;
	vkGetBufferMemoryRequirements(*rhr::render::renderer::get_window_primary()->get_device(), buffer, &memory_requirements);

	vk::memory_allocate_info allocate_info {};
	allocate_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocate_info.allocationSize = memory_requirements.size;
	allocate_info.memoryTypeIndex = find_memory_type(memory_requirements.memoryTypeBits, properties);

	if (vkAllocateMemory(*rhr::render::renderer::get_window_primary()->get_device(), &allocate_info, nullptr, &buffer_memory) != VK_SUCCESS)
	{
		cap::logger::error(cap::logger::level::SYSTEM, "failed to allocate buffer memory");
		return;
	}

	vkBindBufferMemory(*rhr::render::renderer::get_window_primary()->get_device(), buffer, buffer_memory, 0);
}

void rhr::render::tools::delete_buffer(vk::buffer& buffer, vk::device_memory& buffer_memory)
{
	vkDestroyBuffer(*rhr::render::renderer::get_window_primary()->get_device(), buffer, nullptr);
	vkFreeMemory(*rhr::render::renderer::get_window_primary()->get_device(), buffer_memory, nullptr);
}

void rhr::render::tools::copy_buffer(vk::buffer src_buffer, vk::buffer dst_buffer, vk::device_size size)
{
	vk::command_buffer command_buffer = begin_single_time_command();

	vk::buffer_copy copy_region = {};
	copy_region.size = size;
	vkCmdCopyBuffer(command_buffer, src_buffer, dst_buffer, 1, &copy_region);

	end_single_time_command(command_buffer);
}

void rhr::render::tools::copy_buffer(vk::device_memory src_memory, vk::buffer src_buffer, vk::buffer dst_buffer, vk::device_size size)
{
	//vk::buffer_copy copy_region = {};
	//copy_region.size = size;
	//vkCmdCopyBuffer(*rhr::render::command::active_command_buffer, src_buffer, dst_buffer, 1, &copy_region);
}

bool rhr::render::tools::check_device_extension_support(vk::physical_device* device)
{
	u32 extension_count;
	vkEnumerateDeviceExtensionProperties(*device, nullptr, &extension_count, nullptr);

	std::vector<vk::extension_properties> available_extensions(extension_count);
	vkEnumerateDeviceExtensionProperties(*device, nullptr, &extension_count, available_extensions.data());

	std::set<std::string> required_extensions(device_extensions.begin(), device_extensions.end());

	for (const auto& extension : available_extensions)
		required_extensions.erase(extension.extensionName);

	return required_extensions.empty();
}

void rhr::render::tools::create_image(u32 width, u32 height, vk::format format, vk::image_tiling tiling, vk::image_usage_flags usage, vk::memory_property_flags properties, vk::image& image, vk::device_memory& image_memory)
{
	VkImageCreateInfo image_info {};
	image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	image_info.imageType = VK_IMAGE_TYPE_2D;
	image_info.extent.width = width;
	image_info.extent.height = height;
	image_info.extent.depth = 1;
	image_info.mipLevels = 1;
	image_info.arrayLayers = 1;
	image_info.format = format;
	image_info.tiling = tiling;
	image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	image_info.usage = usage;
	image_info.samples = VK_SAMPLE_COUNT_1_BIT;
	image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateImage(*rhr::render::renderer::get_window_primary()->get_device(), &image_info, nullptr, &image) != VK_SUCCESS)
	{
		cap::logger::error(cap::logger::level::SYSTEM, "failed to create image");
		return;
	}

	vk::memory_requirements memory_requirements;
	vkGetImageMemoryRequirements(*rhr::render::renderer::get_window_primary()->get_device(), image, &memory_requirements);

	vk::memory_allocate_info allocation_info {};
	allocation_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocation_info.allocationSize = memory_requirements.size;
	allocation_info.memoryTypeIndex = find_memory_type(memory_requirements.memoryTypeBits, properties);

	if (vkAllocateMemory(*rhr::render::renderer::get_window_primary()->get_device(), &allocation_info, nullptr, &image_memory) != VK_SUCCESS)
	{
		cap::logger::error(cap::logger::level::SYSTEM, "failed to allocate image memory");
		return;
	}

	vkBindImageMemory(*rhr::render::renderer::get_window_primary()->get_device(), image, image_memory, 0);
}

vk::image rhr::render::tools::create_texture_image(const std::string& texture_path, vk::device_memory* texture_image_memory)
{
	i32 texture_width;
	i32 texture_height;
	i32 texture_channels;
	u8* pixels;

	pixels = stbi_load(texture_path.c_str(), &texture_width, &texture_height, &texture_channels, 4);

	vk::device_size image_size = texture_width * texture_height * 4;

	if (!pixels)
		cap::logger::fatal(cap::logger::level::SYSTEM, "failed to load texture \"" + texture_path + "\"");

	vk::buffer staging_buffer;
	vk::device_memory staging_buffer_memory;
	create_buffer(image_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, staging_buffer, staging_buffer_memory);

	void* data;
	vkMapMemory(*rhr::render::renderer::get_window_primary()->get_device(), staging_buffer_memory, 0, image_size, 0, &data);
	memcpy(data, pixels, static_cast<usize>(image_size));
	vkUnmapMemory(*rhr::render::renderer::get_window_primary()->get_device(), staging_buffer_memory);

	vk::image image;
	create_image(texture_width, texture_height, *rhr::render::renderer::get_window_primary()->get_swapchain_format(), VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, image, *texture_image_memory);

	transition_image_layout(image, *rhr::render::renderer::get_window_primary()->get_swapchain_format(), VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	copy_buffer_to_image(staging_buffer, image, static_cast<u32>(texture_width), static_cast<u32>(texture_height));
	transition_image_layout(image, *rhr::render::renderer::get_window_primary()->get_swapchain_format(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	vkDestroyBuffer(*rhr::render::renderer::get_window_primary()->get_device(), staging_buffer, nullptr);
	vkFreeMemory(*rhr::render::renderer::get_window_primary()->get_device(), staging_buffer_memory, nullptr);

	return image;
}

vk::image rhr::render::tools::create_texture_image(glm::vec<2, u32> size, u8* pixels, vk::device_memory* texture_image_memory)
{
// 	u8* pixels = (u8*)malloc(size.x * size.y * 4);

// 	if (pixels == nullptr)
// 		cap::logger::Fatal("failed to allocate texture in ram.");

// 	glm::vec<4, u8> fill_color = color.GetU8();
// 	
// 	for (u32 y = 0; y < size.y; y++)
// 	{
// 		for (u32 x = 0; x < size.x; x++)
// 			memcpy(pixels + (y * (size.x * 4)) + (x * 4), &fill_color, 4);
// 	}

	vk::device_size image_size = size.x * size.y * 4;

	vk::buffer staging_buffer;
	vk::device_memory staging_buffer_memory;
	create_buffer(image_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, staging_buffer, staging_buffer_memory);

	void* data;
	vkMapMemory(*rhr::render::renderer::get_window_primary()->get_device(), staging_buffer_memory, 0, image_size, 0, &data);
	memcpy(data, pixels, static_cast<usize>(image_size));
	vkUnmapMemory(*rhr::render::renderer::get_window_primary()->get_device(), staging_buffer_memory);

	vk::image image;
	create_image(size.x, size.y, *rhr::render::renderer::get_window_primary()->get_swapchain_format(), VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, image, *texture_image_memory);

	transition_image_layout(image, *rhr::render::renderer::get_window_primary()->get_swapchain_format(), VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	copy_buffer_to_image(staging_buffer, image, size.x, size.y);
	transition_image_layout(image, *rhr::render::renderer::get_window_primary()->get_swapchain_format(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	vkDestroyBuffer(*rhr::render::renderer::get_window_primary()->get_device(), staging_buffer, nullptr);
	vkFreeMemory(*rhr::render::renderer::get_window_primary()->get_device(), staging_buffer_memory, nullptr);

	return image;
}

std::vector<const char*> rhr::render::tools::get_required_extensions()
{
	u32 glfw_extension_count = 0;
	const char** glfw_extensions;
	glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

	std::vector<const char*> extensions(glfw_extensions, glfw_extensions + glfw_extension_count);

	if (rhr::render::renderer::enable_validation_layers)
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

	return extensions;
}

VkResult rhr::render::tools::create_debug_utils_message_ext(VkInstance* instance, const VkDebugUtilsMessengerCreateInfoEXT* create_info, const VkAllocationCallbacks* allocator, VkDebugUtilsMessengerEXT* debug_message)
{
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(*instance, "vkCreateDebugUtilsMessengerEXT");

	if (func != nullptr)
		return func(*instance, create_info, allocator, debug_message);
	else
		return VK_ERROR_EXTENSION_NOT_PRESENT;
}

void rhr::render::tools::destroy_debug_utils_message_ext(VkInstance* instance, VkDebugUtilsMessengerEXT* debug_message, const VkAllocationCallbacks* allocator)
{
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(*instance, "vkDestroyDebugUtilsMessengerEXT");

	if (func != nullptr)
		func(*instance, *debug_message, allocator);
}

void rhr::render::tools::populate_debug_messenge_create_info(VkDebugUtilsMessengerCreateInfoEXT& create_info)
{
	create_info = {};
	create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	create_info.pfnUserCallback = debug_callback;
}

rhr::render::tools::swap_chain_support_details rhr::render::tools::query_swap_chain_support(vk::physical_device* device, vk::surface_khr* surface)
{
	swap_chain_support_details details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(*device, *surface, &details.capabilities);

	u32 format_count;
	vkGetPhysicalDeviceSurfaceFormatsKHR(*device, *surface, &format_count, nullptr);

	if (format_count != 0)
	{
		details.formats.resize(format_count);
		vkGetPhysicalDeviceSurfaceFormatsKHR(*device, *surface, &format_count, details.formats.data());
	}

	u32 present_mode_count;
	vkGetPhysicalDeviceSurfacePresentModesKHR(*device, *surface, &present_mode_count, nullptr);

	if (present_mode_count != 0)
	{
		details.present_modes.resize(present_mode_count);
		vkGetPhysicalDeviceSurfacePresentModesKHR(*device, *surface, &present_mode_count, details.present_modes.data());
	}

	return details;
}

rhr::render::tools::queue_family_indices rhr::render::tools::find_queue_families(vk::physical_device* device, vk::surface_khr* surface)
{
	queue_family_indices indices;

	u32 queue_family_count = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(*device, &queue_family_count, nullptr);

	std::vector<vk::queue_family_properties> queue_families(queue_family_count);
	vkGetPhysicalDeviceQueueFamilyProperties(*device, &queue_family_count, queue_families.data());

	u32 i = 0;
	for (const auto& queue_family : queue_families)
	{
		vk::bool32 present_support = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(*device, i, *surface, &present_support);

		if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			indices.graphics_family.emplace(i);

		if (present_support)
			indices.present_family.emplace(i);

		if (indices.complete())
			break;

		i++;
	}

	return indices;
}

vk::present_mode_khr rhr::render::tools::choose_swap_present_mode(const std::vector<vk::present_mode_khr>& available_present_modes)
{
	for (const auto& available_present_mode : available_present_modes)
	{
		if (available_present_mode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			//rhr::render::renderer::vsync_enabled = true;
			return available_present_mode;
		}
	}

	for (const auto& available_present_mode : available_present_modes)
	{
		if (available_present_mode == VK_PRESENT_MODE_IMMEDIATE_KHR)
		{
			//rhr::render::renderer::vsync_enabled = false;
			return available_present_mode;
		}
	}

	//rhr::render::renderer::vsync_enabled = true;
	return VK_PRESENT_MODE_FIFO_KHR;
}

vk::surface_format_khr rhr::render::tools::choose_swap_surface_format(const std::vector<vk::surface_format_khr>& available_formats)
{
	for (const auto& available_format : available_formats)
	{
		if (available_format.format == VK_FORMAT_B8G8R8A8_UNORM)// && */available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			return available_format;
	}

	return available_formats[0];
}

vk::extent_2d rhr::render::tools::choose_swap_extent(const vk::surface_capabilities_khr& capabilities)
{
	if (capabilities.currentExtent.width != UINT32_MAX)
	{
		return capabilities.currentExtent;
	}
	else
	{
		i32 width;
		i32 height;
		glfwGetFramebufferSize(rhr::render::renderer::get_window_primary()->get_window(), &width, &height);

		vk::extent_2d actual_extent = {
			static_cast<u32>(width),
			static_cast<u32>(height)
		};

		actual_extent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actual_extent.width));
		actual_extent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actual_extent.height));

		return actual_extent;
	}
}

vk::shader_module rhr::render::tools::create_shader_module(const std::vector<char>& code, vk::device* device)
{
	VkShaderModuleCreateInfo create_info {};
	create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	create_info.codeSize = code.size();
	create_info.pCode = reinterpret_cast<const u32*>(code.data());

	vk::shader_module shader_module;

	if (vkCreateShaderModule(*device, &create_info, nullptr, &shader_module) != VK_SUCCESS)
		cap::logger::error(cap::logger::level::SYSTEM, "failed to create shader modules");

	return shader_module;
}

bool rhr::render::tools::is_device_suitable(vk::physical_device* physical_device, vk::surface_khr* surface)
{
	/*
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);

	vk::physical_device_features deviceFeatures;
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader;
	*/

	queue_family_indices indices = find_queue_families(physical_device, surface);
	bool extensions_supported = check_device_extension_support(physical_device);

	bool swap_chain_adequate = false;
	if (extensions_supported)
	{
		swap_chain_support_details swap_chain_support = query_swap_chain_support(physical_device, surface);
		swap_chain_adequate = !swap_chain_support.formats.empty() && !swap_chain_support.present_modes.empty();
	}

	vk::physical_device_features supported_features;
	vkGetPhysicalDeviceFeatures(*physical_device, &supported_features);

	return indices.complete() && extensions_supported && swap_chain_adequate && supported_features.samplerAnisotropy;
}

VKAPI_ATTR vk::bool32 VKAPI_CALL rhr::render::tools::debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, VkDebugUtilsMessageTypeFlagsEXT message_type, const VkDebugUtilsMessengerCallbackDataEXT* callback_data, void* user_data)
{
	if (message_severity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
		cap::logger::warn(cap::logger::level::SYSTEM, std::string(callback_data->pMessage));
	else if (message_severity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
		cap::logger::error(cap::logger::level::SYSTEM, std::string(callback_data->pMessage));

	return VK_FALSE;
}

std::vector<const char*> rhr::render::tools::validation_layers;
std::vector<vk::layer_properties> rhr::render::tools::available_validation_layers;
std::vector<const char*> rhr::render::tools::device_extensions;