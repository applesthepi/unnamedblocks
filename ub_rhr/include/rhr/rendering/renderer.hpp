#pragma once
#include "config.h"

#include "rhr/rendering/layer.hpp"
#include "rhr/rendering/objects/object.hpp"
#include "rhr/rendering/interfaces/i_renderable.hpp"

#include <cappuccino/utils.hpp>

/*

To make a new pipeline:
	- add var to Render class
	- add creation to InitPipelines()
	- add destruction to CleanupSwapChain()

*/

namespace rhr::render
{
///
class renderer
{
public:
	///
	static void initialize_window();

	///
	static void initialize();

	///
	static void add_dirty(std::weak_ptr<rhr::render::interfaces::i_renderable> renderable);

	///
	static void process_dirty();

	///
	static void render(usize idx, f64 deltaTime, bool setup, TIME_POINT& diagnosticsTime);

	///
	static void clean_up_swap_chain();

	///
	static void clean_up();

	///
	static void recreate_swap_chain();

	///
	static void add_layer(std::weak_ptr<rhr::render::layer> layer);

	///
	static void reload_layer_swap_chains();

	///
	static VkInstance instance;

	///
	static VkDebugUtilsMessengerEXT debug_messenger;

	///
	static VkPhysicalDevice physical_device;

	///
	static VkDevice device;

	///
	static VkQueue graphics_queue;

	///
	static VkQueue present_queue;
	
	///
	static VkSurfaceKHR surface;

	///
	static VkSwapchainKHR swap_chain;

	///
	static GLFWwindow* window;

	///
	static std::vector<VkImage> swap_chain_images;

	///
	static std::vector<VkImageView> swap_chain_image_views;

	///
	static VkFormat swap_chain_image_format;

	///
	static VkExtent2D swap_chain_extent;

	///
	static std::vector<VkFramebuffer> swap_chain_frame_buffers;

	///
	static std::vector<VkCommandBuffer> command_buffers;

	///
	static VkCommandBuffer active_command_buffer;

	///
	static std::vector<VkDeviceQueueCreateInfo> queue_create_infos;

	///
	static VkRenderPass render_pass;

	///
	static VkDescriptorSetLayout descriptor_set_layout;

	///
	static VkCommandPool command_pool;

	///
	static VkDescriptorPool descriptor_pool;

	///
	static VkSampler texture_sampler;

	///
	static VkImageView depth_image_view;

	///
	static std::vector<VkSemaphore> image_available_semaphores;

	///
	static std::vector<VkSemaphore> render_finished_semaphores;

	///
	static std::vector<VkFence> in_flight_fences;

	///
	static std::vector<VkFence> images_in_flight;

	///
	static bool frame_buffer_resized;

	///
	static glm::mat4 view_matrix;

	///
	static glm::mat4 projection_matrix;

	///
	static glm::mat4 ui_projection_matrix;

	///
	static bool vsync_enabled;

	///
	static glm::vec<2, i32> window_size;

	///
	static std::shared_ptr<rhr::render::frame> debug_frame;

	///
	static VkPipelineLayout blocks_pipeline_layout;
	static VkPipelineLayout ui_pipeline_layout;
	static VkPipelineLayout ui_texture_pipeline_layout;

	///
	//static VkPipeline BlocksPipeline;
	static VkPipeline ui_pipeline;
	static VkPipeline ui_texture_pipeline;

	///
	static u32 depth_background;
	static u32 depth_plane;
	static u32 depth_collection;
	static u32 depth_stack;
	static u32 depth_block;
	static u32 depth_argument;
	static u32 depth_argument_text;

	///
#ifdef NDEBUG
	static const bool enable_validation_layers = false;
#else
	static const bool enable_validation_layers = true;
#endif

private:
	///
	static void init_instance();

	///
	static void init_debug();

	///
	static void init_device();

	///
	static void init_logical_device();

	///
	static void init_swap_chain();

	///
	static void init_image_views();

	///
	static void init_render_pass();

	///
	static void init_descriptor_set_layout();

	///
	static void create_pipeline(const std::string& shader, VkPipeline* pipeline, VkPipelineLayout* layout);

	///
	static void init_pipelines();

	///
	static void init_command_pool();

	///
	static void init_frame_buffers();

	///
	static void init_depth_resources();

	///
	static void init_texture_sampler();

	///
	static void init_descriptor_pool();

	///
	static void init_command_buffers();

	///
	static void init_sync_objects();

	///
	static std::vector<std::weak_ptr<rhr::render::interfaces::i_renderable>> m_dirty_renderable;

	///
	static std::vector<std::weak_ptr<rhr::render::layer>> m_layers;

	///
	static std::shared_mutex m_dirty_mutex;
};
}