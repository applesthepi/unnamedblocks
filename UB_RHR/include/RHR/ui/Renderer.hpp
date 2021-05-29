#pragma once
#include "config.h"

#include "ui/RenderLayer.hpp"

#include <Cappuccino/Utils.hpp>

/*

To make a new pipeline:
	- add var to Render class
	- add creation to InitPipelines()
	- add destruction to CleanupSwapChain()

*/

class Renderer
{
public:
	static void InitializeWindow();
	static void Initialization();
	static void AddDirtyObject(std::weak_ptr<RenderObject> object);
	static void AddDirtyFrame(std::weak_ptr<vui::RenderFrame> frame);
	static void AddDirtyUI(std::weak_ptr<vui::RenderUI> ui);
	static void ProcessDirty();
	static void Render(size_t idx, double deltaTime, bool setup, TIME_POINT& diagnosticsTime);
	static void CleanupSwapChain();
	static void Cleanup();
	static void RecreateSwapChain();
	static void AddLayer(std::weak_ptr<vui::RenderLayer> layer);
	static void ReloadLayerSwapChains();

	static VkInstance Instance;
	static VkDebugUtilsMessengerEXT DebugMessenger;
	static VkPhysicalDevice PhysicalDevice;
	static VkDevice Device;
	static VkQueue GraphicsQueue;
	static VkQueue PresentQueue;
	static VkSurfaceKHR Surface;
	static VkSwapchainKHR SwapChain;

	static GLFWwindow* Window;

	static std::vector<VkImage> SwapChainImages;
	static std::vector<VkImageView> SwapChainImageViews;
	static VkFormat SwapChainImageFormat;
	static VkExtent2D SwapChainExtent;
	static std::vector<VkFramebuffer> SwapChainFramebuffers;
	static std::vector<VkCommandBuffer> CommandBuffers;
	static VkCommandBuffer ActiveCommandBuffer;
	//static VkCommandBuffer AuxCommandBuffer;
	//static std::vector<VkBuffer> AuxBufferMemory;
	//static std::vector<VkDeviceMemory> AuxDeviceMemory;
	//static std::shared_mutex AuxCommandBufferMutex;
	static std::vector<VkDeviceQueueCreateInfo> QueueCreateInfos;

	static VkRenderPass RenderPass;
	static VkDescriptorSetLayout DescriptorSetLayout;
	static VkCommandPool CommandPool;
	static VkDescriptorPool DescriptorPool;
	static VkSampler TextureSampler;
	static VkImageView DepthImageView;

	static std::vector<VkSemaphore> ImageAvailableSemaphores;
	static std::vector<VkSemaphore> RenderFinishedSemaphores;
	static std::vector<VkFence> InFlightFences;
	static std::vector<VkFence> ImagesInFlight;
	static bool FramebufferResized;
	static glm::mat4 ViewMatrix;
	static glm::mat4 ProjectionMatrix;
	static glm::mat4 UIProjectionMatrix;
	static bool VsyncEnabled;
	static glm::vec<2, uint32_t> WindowSize;
	static std::shared_ptr<vui::RenderFrame> DebugFrame;

	static VkPipelineLayout BlocksPipelineLayout;
	static VkPipelineLayout UIPipelineLayout;
	static VkPipelineLayout UITexturePipelineLayout;

	static VkPipeline BlocksPipeline;
	static VkPipeline UIPipeline;
	static VkPipeline UITexturePipeline;

#ifdef NDEBUG
	static const bool EnableValidationLayers = false;
#else
	static const bool EnableValidationLayers = true;
#endif

private:
	static void InitInstance();
	static void InitDebug();
	static void InitDevice();
	static void InitLogicalDevice();
	static void InitSwapChain();
	static void InitImageViews();
	static void InitRenderPass();
	static void InitDescriptorSetLayout();
	static void CreatePipeline(const std::string& shader, VkPipeline* pipeline, VkPipelineLayout* layout);
	static void InitPipelines();
	static void InitCommandPool();
	static void InitFrameBuffers();
	static void InitDepthResources();
	static void InitTextureSampler();
	static void InitDescriptorPool();
	static void InitCommandBuffers();
	static void InitSyncObjects();

	static std::vector<std::weak_ptr<RenderObject>> m_DirtyObjects;
	static std::vector<std::weak_ptr<vui::RenderFrame>> m_DirtyFrames;
	static std::vector<std::weak_ptr<vui::RenderUI>> m_DirtyUI;
	static std::vector<std::weak_ptr<vui::RenderLayer>> m_Layers;
	static SHARED_MUTEX m_DirtyMutex;
};