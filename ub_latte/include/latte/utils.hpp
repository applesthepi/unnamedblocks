#pragma once
#include "config.h"

#include "logger.hpp"

#include <array>
#include <atomic>
#include <bit>
#include <cassert>
#include <cinttypes>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <optional>
#include <random>
#include <set>
#include <shared_mutex>
#include <string>
#include <string_view>
#include <thread>
#include <unordered_map>
#include <vector>
#include <sstream>

#include <stb/stb_image.h>
#include <zstd.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/rapidjson.h>

//#include <imgui.h>
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

#include <GLFW/glfw3.h>

#if LINUX_TOOLCHAIN
#define TIME_POINT std::chrono::system_clock::time_point
#else
#define TIME_POINT std::chrono::steady_clock::time_point
#endif

#include "cutils.h"

#define BLOCK_PADDING 2
#define BLOCK_HEIGHT 20
#define BLOCK_HEIGHT_CONTENT (BLOCK_HEIGHT - (BLOCK_PADDING * 2))

// https://stackoverflow.com/a/9864472
#define FF_TMP	template<typename... Args>
#define FF_FARG std::forward<Args>(args)...
#define FF_ARG	Args&&... args

namespace vk
{
// API Creation.
FF_TMP auto create_instance(FF_ARG) -> decltype(vkCreateInstance(FF_FARG))
{
	return vkCreateInstance(FF_FARG);
}
FF_TMP auto create_device(FF_ARG) -> decltype(vkCreateDevice(FF_FARG))
{
	return vkCreateDevice(FF_FARG);
}
FF_TMP auto create_swapchain_khr(FF_ARG) -> decltype(vkCreateSwapchainKHR(FF_FARG))
{
	return vkCreateSwapchainKHR(FF_FARG);
}
FF_TMP auto create_framebuffer(FF_ARG) -> decltype(vkCreateFramebuffer(FF_FARG))
{
	return vkCreateFramebuffer(FF_FARG);
}
FF_TMP auto create_semaphore(FF_ARG) -> decltype(vkCreateSemaphore(FF_FARG))
{
	return vkCreateSemaphore(FF_FARG);
}
FF_TMP auto create_fence(FF_ARG) -> decltype(vkCreateFence(FF_FARG))
{
	return vkCreateFence(FF_FARG);
}
FF_TMP auto create_render_pass(FF_ARG) -> decltype(vkCreateRenderPass(FF_FARG))
{
	return vkCreateRenderPass(FF_FARG);
}
FF_TMP auto create_descriptor_set_layout(FF_ARG) -> decltype(vkCreateDescriptorSetLayout(FF_FARG))
{
	return vkCreateDescriptorSetLayout(FF_FARG);
}
FF_TMP auto create_command_pool(FF_ARG) -> decltype(vkCreateCommandPool(FF_FARG))
{
	return vkCreateCommandPool(FF_FARG);
}
FF_TMP auto create_descriptor_pool(FF_ARG) -> decltype(vkCreateDescriptorPool(FF_FARG))
{
	return vkCreateDescriptorPool(FF_FARG);
}
FF_TMP auto create_sampler(FF_ARG) -> decltype(vkCreateSampler(FF_FARG))
{
	return vkCreateSampler(FF_FARG);
}

// API Getters.
FF_TMP auto get_device_queue(FF_ARG) -> decltype(vkGetDeviceQueue(FF_FARG))
{
	return vkGetDeviceQueue(FF_FARG);
}
FF_TMP auto get_swapchain_images_khr(FF_ARG) -> decltype(vkGetSwapchainImagesKHR(FF_FARG))
{
	return vkGetSwapchainImagesKHR(FF_FARG);
}
FF_TMP auto get_physical_device_properties(FF_ARG) -> decltype(vkGetPhysicalDeviceProperties(FF_FARG))
{
	return vkGetPhysicalDeviceProperties(FF_FARG);
}

// API Enumeration.
FF_TMP auto enumerate_instance_extension_properties(FF_ARG) -> decltype(vkEnumerateInstanceExtensionProperties(FF_FARG))
{
	return vkEnumerateInstanceExtensionProperties(FF_FARG);
}
FF_TMP auto enumerate_physical_device(FF_ARG) -> decltype(vkEnumeratePhysicalDevices(FF_FARG))
{
	return vkEnumeratePhysicalDevices(FF_FARG);
}

// API Allocation.
FF_TMP auto allocate_command_buffers(FF_ARG) -> decltype(vkAllocateCommandBuffers(FF_FARG))
{
	return vkAllocateCommandBuffers(FF_FARG);
}

// API Begin & End.
FF_TMP auto begin_command_buffer(FF_ARG) -> decltype(vkBeginCommandBuffer(FF_FARG))
{
	return vkBeginCommandBuffer(FF_FARG);
}
FF_TMP auto end_command_buffer(FF_ARG) -> decltype(vkEndCommandBuffer(FF_FARG))
{
	return vkEndCommandBuffer(FF_FARG);
}
FF_TMP auto map_memory(FF_ARG) -> decltype(vkMapMemory(FF_FARG))
{
	return vkMapMemory(FF_FARG);
}
FF_TMP auto unmap_memory(FF_ARG) -> decltype(vkUnmapMemory(FF_FARG))
{
	return vkUnmapMemory(FF_FARG);
}

namespace cmd
{
// API CMD.
FF_TMP auto begin_render_pass(FF_ARG) -> decltype(vkCmdBeginRenderPass(FF_FARG))
{
	return vkCmdBeginRenderPass(FF_FARG);
}
FF_TMP auto end_render_pass(FF_ARG) -> decltype(vkCmdEndRenderPass(FF_FARG))
{
	return vkCmdEndRenderPass(FF_FARG);
}
FF_TMP auto bind_pipeline(FF_ARG) -> decltype(vkCmdBindPipeline(FF_FARG))
{
	return vkCmdBindPipeline(FF_FARG);
}
FF_TMP auto bind_vertex_buffers(FF_ARG) -> decltype(vkCmdBindVertexBuffers(FF_FARG))
{
	return vkCmdBindVertexBuffers(FF_FARG);
}
FF_TMP auto bind_index_buffer(FF_ARG) -> decltype(vkCmdBindIndexBuffer(FF_FARG))
{
	return vkCmdBindIndexBuffer(FF_FARG);
}
FF_TMP auto draw_indexed(FF_ARG) -> decltype(vkCmdDrawIndexed(FF_FARG))
{
	return vkCmdDrawIndexed(FF_FARG);
}
FF_TMP auto draw(FF_ARG) -> decltype(vkCmdDraw(FF_FARG))
{
	return vkCmdDraw(FF_FARG);
}
} // namespace cmd

typedef VkInstance instance;

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

typedef VkCommandBufferBeginInfo command_buffer_begin_info;
typedef VkCommandBuffer command_buffer;
typedef VkCommandBufferAllocateInfo command_buffer_allocate_info;

typedef VkCommandPoolCreateInfo command_pool_create_info;
typedef VkCommandPool command_pool;

typedef VkDescriptorPoolCreateInfo descriptor_pool_create_info;
typedef VkDescriptorPool descriptor_pool;
typedef VkDescriptorPoolSize descriptor_pool_size;

typedef VkFormat format;
typedef VkFormatFeatureFlags format_feature_flags;

typedef VkDescriptorSetLayoutBinding descriptor_set_layout_binding;
typedef VkDescriptorSetLayoutCreateInfo descriptor_set_layout_create_info;
typedef VkDescriptorSetLayout descriptor_set_layout;
typedef VkDescriptorSet descriptor_set;

typedef VkPhysicalDeviceMemoryProperties physical_device_memory_properties;
typedef VkPhysicalDevice physical_device;
typedef VkPhysicalDeviceFeatures physical_device_features;
typedef VkPhysicalDeviceProperties physical_device_properties;

typedef VkSurfaceCapabilitiesKHR surface_capabilities_khr;
typedef VkSurfaceFormatKHR surface_format_khr;
typedef VkSurfaceKHR surface_khr;
typedef VkPresentModeKHR present_mode_khr;

typedef VkSwapchainCreateInfoKHR swapchain_create_info_khr;
typedef VkSwapchainKHR swapchain_khr;

typedef VkSemaphoreCreateInfo semaphore_create_info;
typedef VkSemaphore semaphore;
typedef VkFenceCreateInfo fence_create_info;
typedef VkFence fence;
typedef VkRenderPassCreateInfo render_pass_create_info;
typedef VkRenderPassBeginInfo render_pass_begin_info;
typedef VkRenderPass render_pass;
typedef VkFramebufferCreateInfo framebuffer_create_info;
typedef VkSamplerCreateInfo sampler_create_info;
typedef VkSampler sampler;

typedef VkFramebuffer frame_buffer;
typedef VkPipeline pipeline;
typedef VkPipelineLayoutCreateInfo pipeline_layout_create_info;
typedef VkPipelineLayout pipeline_layout;
typedef VkPipelineShaderStageCreateInfo pipeline_shader_stage_create_info;
typedef VkPipelineVertexInputStateCreateInfo pipeline_vertex_input_state_create_info;
typedef VkPipelineStageFlags pipeline_stage_flags;
typedef VkPipelineViewportStateCreateInfo pipeline_viewport_state_create_info;
typedef VkPipelineRasterizationStateCreateInfo pipeline_rasterization_state_create_info;
typedef VkPipelineMultisampleStateCreateInfo pipeline_multisample_state_create_info;
typedef VkPipelineColorBlendAttachmentState pipeline_color_blend_attachment_state;
typedef VkPipelineColorBlendStateCreateInfo pipeline_color_blend_state_create_info;

typedef VkPipelineDynamicStateCreateInfo pipeline_dynamic_state_create_info;

typedef VkGraphicsPipelineCreateInfo graphics_pipeline_create_info;
typedef VkPipelineInputAssemblyStateCreateInfo pipeline_input_assembly_state_create_info;
typedef VkLayerProperties layer_properties;
typedef VkSubmitInfo submit_info;
typedef VkExtensionProperties extension_properties;
typedef VkQueueFamilyProperties queue_family_properties;
typedef VkShaderModule shader_module;
typedef VkQueue queue;
typedef VkResult result;
typedef VkCullModeFlags cull_mode_flags;
typedef VkViewport viewport;
typedef VkDynamicState dynamic_state;

typedef VkExtent2D extent_2d;
typedef VkRect2D rect_2d;

typedef VkSubpassDependency subpass_dependency;
typedef VkAttachmentDescription attachment_description;
typedef VkAttachmentReference attachment_reference;
typedef VkSubpassDescription subpass_description;

typedef VkDebugUtilsMessengerCreateInfoEXT debug_utils_messenger_create_info;
typedef VkDebugUtilsMessengerEXT debug_utils_messenger_ext;
} // namespace vk

namespace glfw
{
// API Creation.
FF_TMP auto create_window(FF_ARG) -> decltype(glfwCreateWindow(FF_FARG))
{
	return glfwCreateWindow(FF_FARG);
}
FF_TMP auto create_window_surface(FF_ARG) -> decltype(glfwCreateWindowSurface(FF_FARG))
{
	return glfwCreateWindowSurface(FF_FARG);
}

// API Setters.
FF_TMP auto set_framebuffer_size_callback(FF_ARG) -> decltype(glfwSetFramebufferSizeCallback(FF_FARG))
{
	return glfwSetFramebufferSizeCallback(FF_FARG);
}
FF_TMP auto set_key_callback(FF_ARG) -> decltype(glfwSetKeyCallback(FF_FARG))
{
	return glfwSetKeyCallback(FF_FARG);
}
FF_TMP auto set_mouse_button_callback(FF_ARG) -> decltype(glfwSetMouseButtonCallback(FF_FARG))
{
	return glfwSetMouseButtonCallback(FF_FARG);
}
FF_TMP auto set_scroll_callback(FF_ARG) -> decltype(glfwSetScrollCallback(FF_FARG))
{
	return glfwSetScrollCallback(FF_FARG);
}
FF_TMP auto set_cursor_position_callback(FF_ARG) -> decltype(glfwSetCursorPosCallback(FF_FARG))
{
	return glfwSetCursorPosCallback(FF_FARG);
}
FF_TMP auto set_window_position_callback(FF_ARG) -> decltype(glfwSetWindowPosCallback(FF_FARG))
{
	return glfwSetWindowPosCallback(FF_FARG);
}
FF_TMP auto set_window_user_pointer(FF_ARG) -> decltype(glfwSetWindowUserPointer(FF_FARG))
{
	return glfwSetWindowUserPointer(FF_FARG);
}
FF_TMP auto set_window_hint(FF_ARG) -> decltype(glfwWindowHint(FF_FARG))
{
	return glfwWindowHint(FF_FARG);
}

// API Getters.
FF_TMP auto get_window_user_pointer(FF_ARG) -> decltype(glfwGetWindowUserPointer(FF_FARG))
{
	return glfwGetWindowUserPointer(FF_FARG);
}
FF_TMP auto get_window_position(FF_ARG) -> decltype(glfwGetWindowPos(FF_FARG))
{
	return glfwGetWindowPos(FF_FARG);
}
FF_TMP auto get_required_instance_extensions(FF_ARG) -> decltype(glfwGetRequiredInstanceExtensions(FF_FARG))
{
	return glfwGetRequiredInstanceExtensions(FF_FARG);
}

// API Types.
using window = GLFWwindow;
} // namespace glfw

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
} // namespace ft

namespace cap::endianness
{
constexpr bool is_le = std::endian::native == std::endian::little;
template<usize T>
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
} // namespace cap::endianness