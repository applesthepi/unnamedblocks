#pragma once
#include "config.h"

#include "cappuccino/logger.hpp"

#include <bit>
#include <cinttypes>
#include <cmath>
#include <array>
#include <vector>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <functional>
#include <atomic>
#include <unordered_map>
#include <fstream>
#include <string_view>
#include <string>
#include <cstring>
#include <cassert>
#include <cstdint>
#include <random>
#include <set>
#include <memory>
#include <optional>
#include <future>
#include <thread>

#include <stb_image.h>
#include <zstd.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

//#include "cappuccino/imgui/imgui.h"
//#include "cappuccino/imgui/imgui_impl_glfw.h"
//#include "cappuccino/imgui/imgui_impl_vulkan.h"
//
//#define GLFW_INCLUDE_NONE
//#define GLFW_INCLUDE_VULKAN

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN

#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#if LINUX_TOOLCHAIN
#define TIME_POINT std::chrono::system_clock::time_point
#else
#define TIME_POINT std::chrono::steady_clock::time_point
#endif

#include "cutils.h"

// TODO: add vulkan functions
// TODO: add rest of typdefs used in rhr::render::renderer
// Maintain code style standards.
namespace vk
{
typedef VkBool32 bool32;
typedef VkClearValue clear_value;
typedef VkApplicationInfo application_info;
typedef VkInstanceCreateInfo instance_create_info;

typedef VkDevice device;
typedef VkDeviceQueueCreateInfo device_queue_create_info;
typedef VkDeviceMemory device_memory;
typedef VkDeviceCreateInfo device_create_info;
typedef VkDeviceSize device_size;

typedef VkBuffer buffer;
typedef VkBufferImageCopy buffer_image_copy;
typedef VkBufferCreateInfo buffer_create_info;
typedef VkBufferCopy buffer_copy;

typedef VkMemoryRequirements memory_requirements;
typedef VkMemoryPropertyFlags memory_property_flags;
typedef VkMemoryAllocateInfo memory_allocate_info;

typedef VkImage image;
typedef VkImageView image_view;
typedef VkImageTiling image_tiling;
typedef VkImageLayout image_layout;
typedef VkImageUsageFlags image_usage_flags;
typedef VkImageAspectFlags image_aspect_flags;
typedef VkImageViewCreateInfo image_view_create_info;
typedef VkImageMemoryBarrier image_memory_barrier;

typedef VkCommandBuffer command_buffer;
typedef VkCommandBufferAllocateInfo command_buffer_allocate_info;
typedef VkCommandBufferBeginInfo command_buffer_begin_info;

typedef VkFormat format;
typedef VkFormatFeatureFlags format_feature_flags;

typedef VkDescriptorSet descriptor_set;

typedef VkPhysicalDeviceMemoryProperties physical_device_memory_properties;
typedef VkPhysicalDevice physical_device;
typedef VkPhysicalDeviceFeatures physical_device_features;

typedef VkSurfaceCapabilitiesKHR surface_capabilities_khr;
typedef VkSurfaceFormatKHR surface_format_khr;
typedef VkSurfaceKHR surface_khr;
typedef VkPresentModeKHR present_mode_khr;

typedef VkSwapchainCreateInfoKHR swap_chain_create_info_khr;

typedef VkLayerProperties layer_properties;
typedef VkSubmitInfo submit_info;
typedef VkPipelineStageFlags pipeline_stage_flags;
typedef VkExtensionProperties extension_properties;
typedef VkQueueFamilyProperties queue_family_properties;
typedef VkExtent2D extent_2d;
typedef VkShaderModule shader_module;
typedef VkRenderPassBeginInfo render_pass_begin_info;
}

#undef VK_MAKE_API_VERSION
#define VK_MAKE_API_VERSION(variant, major, minor, patch) \
    ((((u32)(variant)) << 29) | (((u32)(major)) << 22) | (((u32)(minor)) << 12) | ((u32)(patch)))

// TODO: add FT functions
// Maintain code style standards.
namespace ft
{
typedef FT_Library library;
typedef FT_Face face;
typedef FT_Error error;
}

namespace cap::endianness
{
	constexpr bool is_le = std::endian::native == std::endian::little;
	template <usize T>
	std::array<char, T> reverse(std::array<char, T> bytes);
	std::array<char, 2> to_ne_bytes(u16 x);
	std::array<char, 4> to_ne_bytes(u32 x);
	std::array<char, 8> to_ne_bytes(u64 x);

	std::array<char, 2> to_le_bytes(u16 x);
	std::array<char, 4> to_le_bytes(u32 x);
	std::array<char, 8> to_le_bytes(u64 x);

	std::array<char, 2> to_be_bytes(u16 x);
	std::array<char, 4> to_be_bytes(u32 x);
	std::array<char, 8> to_be_bytes(u64 x);

	std::array<char, 4> to_ne_bytes(f32 x);
	std::array<char, 8> to_ne_bytes(f64 x);
	std::array<char, 2> to_ne_bytes(i16 x);
	std::array<char, 4> to_ne_bytes(i32 x);
	std::array<char, 8> to_ne_bytes(i64 x);

	std::array<char, 4> to_le_bytes(f32 x);
	std::array<char, 8> to_le_bytes(f64 x);
	std::array<char, 2> to_le_bytes(i16 x);
	std::array<char, 4> to_le_bytes(i32 x);
	std::array<char, 8> to_le_bytes(i64 x);

	std::array<char, 4> to_be_bytes(f32 x);
	std::array<char, 8> to_be_bytes(f64 x);
	std::array<char, 2> to_be_bytes(i16 x);
	std::array<char, 4> to_be_bytes(i32 x);
	std::array<char, 8> to_be_bytes(i64 x);

	u16 u16_from_ne_bytes(std::array<char, 2> bytes);
	u32 u32_from_ne_bytes(std::array<char, 4> bytes);
	u64 u64_from_ne_bytes(std::array<char, 8> bytes);

	u16 u16_from_le_bytes(std::array<char, 2> bytes);
	u32 u32_from_le_bytes(std::array<char, 4> bytes);
	u64 u64_from_le_bytes(std::array<char, 8> bytes);

	u16 u16_from_be_bytes(std::array<char, 2> bytes);
	u32 u32_from_be_bytes(std::array<char, 4> bytes);
	u64 u64_from_be_bytes(std::array<char, 8> bytes);

	i16 i16_from_ne_bytes(std::array<char, 2> bytes);
	i32 i32_from_ne_bytes(std::array<char, 4> bytes);
	i64 i64_from_ne_bytes(std::array<char, 8> bytes);
	f32 float_from_ne_bytes(std::array<char, 4> bytes);
	f64 double_from_ne_bytes(std::array<char, 8> bytes);

	i16 i16_from_le_bytes(std::array<char, 2> bytes);
	i32 i32_from_le_bytes(std::array<char, 4> bytes);
	i64 i64_from_le_bytes(std::array<char, 8> bytes);
	f32 float_from_le_bytes(std::array<char, 4> bytes);
	f64 double_from_le_bytes(std::array<char, 8> bytes);

	i16 i16_from_be_bytes(std::array<char, 2> bytes);
	i32 i32_from_be_bytes(std::array<char, 4> bytes);
	i64 i64_from_be_bytes(std::array<char, 8> bytes);
	f32 float_from_be_bytes(std::array<char, 4> bytes);
	f64 double_from_be_bytes(std::array<char, 8> bytes);
}