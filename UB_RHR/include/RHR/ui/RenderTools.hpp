#pragma once
#include "config.h"

#include <Cappuccino/Utils.hpp>

#define MAX_FRAMES_IN_FLIGHT 2

struct QueueFamilyIndices
{
	std::optional<uint32_t> GraphicsFamily;
	std::optional<uint32_t> PresentFamily;

	bool IsComplete()
	{
		return GraphicsFamily.has_value() && PresentFamily.has_value();
	}
};

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR Capabilities;
	std::vector<VkSurfaceFormatKHR> Formats;
	std::vector<VkPresentModeKHR> PresentModes;
};

struct UniformBufferObject
{
	glm::mat4 Model;
	glm::mat4 View;
	glm::mat4 Projection;

	glm::vec3 Color;
};

class RenderTools
{
public:
	static void Initialization();
	static std::vector<char>* ReadFileBytes(const std::string& filename);
	static bool CheckValidationLayerSupport();
	static VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
	static void CreateAuxCommandBuffer();
	static void ResetAuxCommandBuffer();
	static void CleanAuxCommandBuffer();
	static VkCommandBuffer BeginSingleTimeCommand();
	static void EndSingleTimeCommand(VkCommandBuffer commandBuffer);
	static VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	static VkFormat FindDepthFormat();
	static bool HasStencilComponent(VkFormat format);
	static void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	static uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	static void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
	static void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	static void DeleteBuffer(VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	static void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	static void CopyBuffer(VkDeviceMemory srcMemory, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	static bool CheckDeviceExtensionSupport(VkPhysicalDevice* device);
	static void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
	static VkImage CreateTextureImage(const std::string& texturePath, VkDeviceMemory* textureImageMemory);
	static std::vector<const char*> GetRequiredExtensions();
	static VkResult CreateDebugUtilsMessengerEXT(VkInstance* instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
	static void DestroyDebugUtilsMessengerEXT(VkInstance* instance, VkDebugUtilsMessengerEXT* debugMessenger, const VkAllocationCallbacks* pAllocator);
	static void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	static SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice* device, VkSurfaceKHR* surface);
	static QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice* device, VkSurfaceKHR* surface);
	static VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	static VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	static VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	static VkShaderModule CreateShaderModule(const std::vector<char>& code, VkDevice* device);
	static bool IsDeviceSuitable(VkPhysicalDevice* physicalDevice, VkSurfaceKHR* surface);

	static std::vector<const char*> ValidationLayers;
	static std::vector<const char*> DeviceExtensions;

	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
	    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	    VkDebugUtilsMessageTypeFlagsEXT messageType,
	    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	    void* pUserData)
	{

		if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
			Logger::Warn(pCallbackData->pMessage);
		else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
			Logger::Error(pCallbackData->pMessage);

		return VK_FALSE;
	}
};