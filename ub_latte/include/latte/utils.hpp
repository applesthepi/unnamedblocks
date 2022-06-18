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
#include <stack>

#include <x86intrin.h>

#include <stb/stb_image.h>
#include <zstd.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES

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
#include <vk_mem_alloc.h>

#if LINUX_TOOLCHAIN
#define TIME_POINT std::chrono::system_clock::time_point
#else
#define TIME_POINT std::chrono::steady_clock::time_point
#endif

#define TIME_POINT_NOW std::chrono::high_resolution_clock::now()

#include "cutils.h"

#define BLOCK_PADDING 2
#define BLOCK_HEIGHT 20
#define BLOCK_HEIGHT_CONTENT (BLOCK_HEIGHT - (BLOCK_PADDING * 2))
#define BLOCK_ARG_DIFF 0.25

//namespace util { template <typename T> std::string to_string(const T& t) {     std::string str{std::to_string (t)};     int offset{1};     if (str.find_last_not_of('0') == str.find('.')) {         offset = 0;     }     str.erase(str.find_last_not_of('0') + offset, std::string::npos);      return str; } }

///
void STRING_DOUBLE_ZERO(std::string& str);

/// Reads file bytes into a char vector allocated on heap.
std::vector<char>* READ_FILE_BYTES_VC(const std::string& file_name);

// https://stackoverflow.com/a/9864472
#define FF_TMP	template<typename... Args>
#define FF_FARG std::forward<Args>(args)...
#define FF_ARG	Args&&... args

namespace vma
{
// API CREATION
FF_TMP auto create_buffer(FF_ARG) -> decltype(vmaCreateBuffer(FF_FARG))
{
	return vmaCreateBuffer(FF_FARG);
}
FF_TMP auto create_image(FF_ARG) -> decltype(vmaCreateImage(FF_FARG))
{
	return vmaCreateImage(FF_FARG);
}
FF_TMP auto create_allocator(FF_ARG) -> decltype(vmaCreateAllocator(FF_FARG))
{
	return vmaCreateAllocator(FF_FARG);
}

// API MEMORY
FF_TMP auto map_memory(FF_ARG) -> decltype(vmaMapMemory(FF_FARG))
{
	return vmaMapMemory(FF_FARG);
}
FF_TMP auto unmap_memory(FF_ARG) -> decltype(vmaUnmapMemory(FF_FARG))
{
	return vmaUnmapMemory(FF_FARG);
}

// API BUFFERS
FF_TMP auto destroy_buffer(FF_ARG) -> decltype(vmaDestroyBuffer(FF_FARG))
{
	return vmaDestroyBuffer(FF_FARG);
}

typedef VmaAllocatorCreateInfo allocator_create_info;
typedef VmaAllocator allocator;
typedef VmaAllocationCreateInfo allocation_create_info;
typedef VmaAllocation allocation;
}

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
FF_TMP auto create_shader_module(FF_ARG) -> decltype(vkCreateShaderModule(FF_FARG))
{
	return vkCreateShaderModule(FF_FARG);
}
FF_TMP auto create_pipeline_layout(FF_ARG) -> decltype(vkCreatePipelineLayout(FF_FARG))
{
	return vkCreatePipelineLayout(FF_FARG);
}
FF_TMP auto create_graphics_pipelines(FF_ARG) -> decltype(vkCreateGraphicsPipelines(FF_FARG))
{
	return vkCreateGraphicsPipelines(FF_FARG);
}

FF_TMP auto create_image_view(FF_ARG) -> decltype(vkCreateImageView(FF_FARG))
{
	return vkCreateImageView(FF_FARG);
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
FF_TMP auto get_instance_proc_addr(FF_ARG) -> decltype(vkGetInstanceProcAddr(FF_FARG))
{
	return vkGetInstanceProcAddr(FF_FARG);
}
FF_TMP auto get_physical_device_queue_family_properties(FF_ARG) -> decltype(vkGetPhysicalDeviceQueueFamilyProperties(FF_FARG))
{
	return vkGetPhysicalDeviceQueueFamilyProperties(FF_FARG);
}
FF_TMP auto get_physical_device_surface_support_khr(FF_ARG) -> decltype(vkGetPhysicalDeviceSurfaceSupportKHR(FF_FARG))
{
	return vkGetPhysicalDeviceSurfaceSupportKHR(FF_FARG);
}
FF_TMP auto get_physical_device_surface_capabilities_khr(FF_ARG) -> decltype(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(FF_FARG))
{
	return vkGetPhysicalDeviceSurfaceCapabilitiesKHR(FF_FARG);
}
FF_TMP auto get_physical_device_surface_formats_khr(FF_ARG) -> decltype(vkGetPhysicalDeviceSurfaceFormatsKHR(FF_FARG))
{
	return vkGetPhysicalDeviceSurfaceFormatsKHR(FF_FARG);
}
FF_TMP auto get_physical_device_surface_present_modes_khr(FF_ARG) -> decltype(vkGetPhysicalDeviceSurfacePresentModesKHR(FF_FARG))
{
	return vkGetPhysicalDeviceSurfacePresentModesKHR(FF_FARG);
}
FF_TMP auto get_physical_device_features(FF_ARG) -> decltype(vkGetPhysicalDeviceFeatures(FF_FARG))
{
	return vkGetPhysicalDeviceFeatures(FF_FARG);
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
FF_TMP auto enumerate_instance_layer_properties(FF_ARG) -> decltype(vkEnumerateInstanceLayerProperties(FF_FARG))
{
	return vkEnumerateInstanceLayerProperties(FF_FARG);
}
FF_TMP auto enumerate_device_extension_properties(FF_ARG) -> decltype(vkEnumerateDeviceExtensionProperties(FF_FARG))
{
	return vkEnumerateDeviceExtensionProperties(FF_FARG);
}

// API Allocation.
FF_TMP auto allocate_command_buffers(FF_ARG) -> decltype(vkAllocateCommandBuffers(FF_FARG))
{
	return vkAllocateCommandBuffers(FF_FARG);
}
FF_TMP auto allocate_descriptor_sets(FF_ARG) -> decltype(vkAllocateDescriptorSets(FF_FARG))
{
	return vkAllocateDescriptorSets(FF_FARG);
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

// API UPDATE
FF_TMP auto update_descriptor_sets(FF_ARG) -> decltype(vkUpdateDescriptorSets(FF_FARG))
{
	return vkUpdateDescriptorSets(FF_FARG);
}

// API DESTROY
FF_TMP auto destroy_framebuffer(FF_ARG) -> decltype(vkDestroyFramebuffer(FF_FARG))
{
	return vkDestroyFramebuffer(FF_FARG);
}
FF_TMP auto destroy_image_view(FF_ARG) -> decltype(vkDestroyImageView(FF_FARG))
{
	return vkDestroyImageView(FF_FARG);
}
FF_TMP auto destroy_pipeline(FF_ARG) -> decltype(vkDestroyPipeline(FF_FARG))
{
	return vkDestroyPipeline(FF_FARG);
}
FF_TMP auto destroy_pipeline_layout(FF_ARG) -> decltype(vkDestroyPipelineLayout(FF_FARG))
{
	return vkDestroyPipelineLayout(FF_FARG);
}
FF_TMP auto destroy_renderpass(FF_ARG) -> decltype(vkDestroyRenderPass(FF_FARG))
{
	return vkDestroyRenderPass(FF_FARG);
}
FF_TMP auto destroy_swapchain(FF_ARG) -> decltype(vkDestroySwapchainKHR(FF_FARG))
{
	return vkDestroySwapchainKHR(FF_FARG);
}

// API OTHER
FF_TMP auto wait_for_fences(FF_ARG) -> decltype(vkWaitForFences(FF_FARG))
{
	return vkWaitForFences(FF_FARG);
}
FF_TMP auto acquire_next_image(FF_ARG) -> decltype(vkAcquireNextImageKHR(FF_FARG))
{
	return vkAcquireNextImageKHR(FF_FARG);
}
FF_TMP auto reset_fences(FF_ARG) -> decltype(vkResetFences(FF_FARG))
{
	return vkResetFences(FF_FARG);
}
FF_TMP auto queue_submit(FF_ARG) -> decltype(vkQueueSubmit(FF_FARG))
{
	return vkQueueSubmit(FF_FARG);
}
FF_TMP auto queue_present(FF_ARG) -> decltype(vkQueuePresentKHR(FF_FARG))
{
	return vkQueuePresentKHR(FF_FARG);
}
FF_TMP auto queue_wait_idle(FF_ARG) -> decltype(vkQueueWaitIdle(FF_FARG))
{
	return vkQueueWaitIdle(FF_FARG);
}
FF_TMP auto device_wait_idle(FF_ARG) -> decltype(vkDeviceWaitIdle(FF_FARG))
{
	return vkDeviceWaitIdle(FF_FARG);
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
FF_TMP auto copy_buffer(FF_ARG) -> decltype(vkCmdCopyBuffer(FF_FARG))
{
	return vkCmdCopyBuffer(FF_FARG);
}
FF_TMP auto bind_descriptor_sets(FF_ARG) -> decltype(vkCmdBindDescriptorSets(FF_FARG))
{
	return vkCmdBindDescriptorSets(FF_FARG);
}
FF_TMP auto pipeline_barrier(FF_ARG) -> decltype(vkCmdPipelineBarrier(FF_FARG))
{
	return vkCmdPipelineBarrier(FF_FARG);
}
FF_TMP auto copy_buffer_to_image(FF_ARG) -> decltype(vkCmdCopyBufferToImage(FF_FARG))
{
	return vkCmdCopyBufferToImage(FF_FARG);
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
typedef VkBufferUsageFlagBits buffer_usage_flag_bits;
typedef VkBufferUsageFlags buffer_usage_flags;

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
typedef VkImageCreateInfo image_create_info;

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
typedef VkDescriptorSetAllocateInfo descriptor_set_allocate_info;
typedef VkDescriptorBufferInfo descriptor_buffer_info;
typedef VkDescriptorImageInfo descriptor_image_info;
typedef VkWriteDescriptorSet write_descriptor_set;

typedef VkPhysicalDeviceMemoryProperties physical_device_memory_properties;
typedef VkPhysicalDevice physical_device;
typedef VkPhysicalDeviceFeatures physical_device_features;
typedef VkPhysicalDeviceProperties physical_device_properties;

typedef VkSurfaceCapabilitiesKHR surface_capabilities_khr;
typedef VkSurfaceFormatKHR surface_format_khr;
typedef VkSurfaceKHR surface_khr;
typedef VkPresentModeKHR present_mode_khr;
typedef VkPresentInfoKHR present_info;

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
typedef VkShaderModuleCreateInfo shader_module_create_info;
typedef VkShaderModule shader_module;
typedef VkQueue queue;
typedef VkResult result;
typedef VkCullModeFlags cull_mode_flags;
typedef VkViewport viewport;
typedef VkDynamicState dynamic_state;
typedef VkFilter filter;
typedef VkSamplerAddressMode sampler_address_mode;

typedef VkExtent2D extent_2d;
typedef VkRect2D rect_2d;
typedef VkVertexInputBindingDescription vertex_input_binding_description;
typedef VkVertexInputAttributeDescription vertex_input_attribute_description;

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
FF_TMP auto set_window_focus_callback(FF_ARG) -> decltype(glfwSetWindowFocusCallback(FF_FARG))
{
	return glfwSetWindowFocusCallback(FF_FARG);
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
FF_TMP auto get_framebuffer_size(FF_ARG) -> decltype(glfwGetFramebufferSize(FF_FARG))
{
	return glfwGetFramebufferSize(FF_FARG);
}

// API OTHER
FF_TMP auto window_should_close(FF_ARG) -> decltype(glfwWindowShouldClose(FF_FARG))
{
	return glfwWindowShouldClose(FF_FARG);
}
FF_TMP auto poll_events(FF_ARG) -> decltype(glfwPollEvents(FF_FARG))
{
	return glfwPollEvents(FF_FARG);
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
