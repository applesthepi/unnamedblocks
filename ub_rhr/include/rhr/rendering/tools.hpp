#pragma once
#include "config.h"

#include <cappuccino/color.hpp>
#include <cappuccino/utils.hpp>

//#define MAX_FRAMES_IN_FLIGHT 5

namespace rhr::render
{
///
class tools
{
public:
	///
	struct queue_family_indices
	{
		///
		std::optional<u32> graphics_family;

		///
		std::optional<u32> present_family;

		///
		bool complete() { return graphics_family.has_value() && present_family.has_value(); }
	};

	///
	struct swap_chain_support_details
	{
		///
		vk::surface_capabilities_khr capabilities;

		///
		std::vector<vk::surface_format_khr> formats;

		///
		std::vector<vk::present_mode_khr> present_modes;
	};

	///
	struct uniform_buffer_object
	{
		///
		glm::mat4 model;

		///
		glm::mat4 view;

		///
		glm::mat4 projection;

		///
		glm::vec<3, f32> color;
	};

	///
	static void initialize();

	///
	static std::vector<char>* read_file_bytes(const std::string& file_name);

	///
	static bool check_validation_layer_support();

	///
	static vk::image_view create_image_view(vk::image image, vk::format format, vk::image_aspect_flags aspect_flags);

	///
	static void create_aux_command_buffer();

	///
	static void reset_aux_command_buffer();

	///
	static void clean_aux_command_buffer();

	///
	static vk::command_buffer begin_single_time_command();

	///
	static void end_single_time_command(vk::command_buffer command_buffer);

	///
	static vk::format find_supported_format(const std::vector<vk::format>& candidates, vk::image_tiling tiling, vk::format_feature_flags features);

	///
	static vk::format find_depth_format();

	///
	static bool has_stencil_component(vk::format format);

	///
	static void transition_image_layout(vk::image image, vk::format format, vk::image_layout old_layout, vk::image_layout new_layout);

	///
	static u32 find_memory_type(u32 type_filter, vk::memory_property_flags properties);

	///
	static void copy_buffer_to_image(vk::buffer buffer, vk::image image, u32 width, u32 height);

	///
	static void create_buffer(vk::device_size size, VkBufferUsageFlags usage, vk::memory_property_flags properties, vk::buffer& buffer, vk::device_memory& buffer_memory);

	///
	static void delete_buffer(vk::buffer& buffer, vk::device_memory& buffer_memory);

	///
	static void copy_buffer(vk::buffer src_buffer, vk::buffer dst_buffer, vk::device_size size);

	///
	static void copy_buffer(vk::device_memory src_memory, vk::buffer src_buffer, vk::buffer dst_buffer, vk::device_size size);

	///
	static bool check_device_extension_support(vk::physical_device* device);

	///
	static void create_image(
		u32 width,
		u32 height,
		vk::format format,
		vk::image_tiling tiling,
		vk::image_usage_flags usage,
		vk::memory_property_flags properties,
		vk::image& image,
		vk::device_memory& image_memory);

	///
	static vk::image create_texture_image(const std::string& texturePath, vk::device_memory* textureImageMemory);

	///
	static vk::image create_texture_image(glm::vec<2, u32> size, u8* pixels, vk::device_memory* textureImageMemory);

	///
	static std::vector<const char*> get_required_extensions();

	///
	static VkResult create_debug_utils_message_ext(
		VkInstance* instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);

	///
	static void destroy_debug_utils_message_ext(VkInstance* instance, VkDebugUtilsMessengerEXT* debugMessenger, const VkAllocationCallbacks* pAllocator);

	///
	static void populate_debug_messenge_create_info(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

	///
	static swap_chain_support_details query_swap_chain_support(vk::physical_device* device, VkSurfaceKHR* surface);

	///
	static queue_family_indices find_queue_families(vk::physical_device* device, VkSurfaceKHR* surface);

	///
	static VkPresentModeKHR choose_swap_present_mode(const std::vector<VkPresentModeKHR>& availablePresentModes);

	///
	static VkSurfaceFormatKHR choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR>& availableFormats);

	///
	static VkExtent2D choose_swap_extent(const VkSurfaceCapabilitiesKHR& capabilities);

	///
	static VkShaderModule create_shader_module(const std::vector<char>& code, VkDevice* device);

	///
	static bool is_device_suitable(vk::physical_device* physicalDevice, VkSurfaceKHR* surface);

	///
	static std::vector<const char*> validation_layers;

	///
	static std::vector<vk::layer_properties> available_validation_layers;

	///
	static std::vector<const char*> device_extensions;

	///
	static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);
};
} // namespace rhr::render