#include "command_pool.hpp"

#include "lungo/renderer.hpp"
#include "lungo/tools.hpp"

rhr::render::component::command_pool::command_pool()
	: m_active_command_buffer {}
	, m_command_pool {}
	, m_descriptor_pool {}
{
	std::unique_ptr<rhr::render::component::window>& window = rhr::render::renderer::get()->get_window_primary();

	rhr::render::tools::queue_family_indices queue_family_indices =
		rhr::render::tools::find_queue_families(window->get_physical_device(), window->get_surface());
	vk::command_pool_create_info command_pool_create_info = {};

	command_pool_create_info.sType			  = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	command_pool_create_info.queueFamilyIndex = queue_family_indices.graphics_family.value();
	command_pool_create_info.flags			  = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

	if (vk::create_command_pool(*window->get_device(), &command_pool_create_info, nullptr, &m_command_pool)
		!= VK_SUCCESS)
		latte::logger::fatal(latte::logger::level::SYSTEM, "failed to create command pool");
}

rhr::render::component::command_pool::~command_pool()
{}

void rhr::render::component::command_pool::initialize_descriptor_pool()
{
	std::unique_ptr<rhr::render::component::window>& window = rhr::render::renderer::get()->get_window_primary();

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

	if (vk::create_descriptor_pool(*window->get_device(), &descriptor_pool_create_info, nullptr, &m_descriptor_pool)
		!= VK_SUCCESS)
		latte::logger::fatal(latte::logger::level::SYSTEM, "failed to create descriptor pool");
}

void rhr::render::component::command_pool::initialize_command_buffers()
{
	std::unique_ptr<rhr::render::component::window>& window = rhr::render::renderer::get()->get_window_primary();

	if (!m_command_buffer_panels.empty())
	{
		vkFreeCommandBuffers(
			*window->get_device(),
			m_command_pool,
			static_cast<u32>(m_command_buffer_panels.size()),
			m_command_buffer_panels.data());
	}

	if (!m_command_buffer_master.empty())
	{
		vkFreeCommandBuffers(
			*window->get_device(),
			m_command_pool,
			static_cast<u32>(m_command_buffer_master.size()),
			m_command_buffer_master.data());
	}

	m_command_buffer_panels.clear();
	m_command_buffer_master.clear();

	m_command_buffer_panels.resize(static_cast<usize>(window->get_framebuffer_count()));
	m_command_buffer_master.resize(static_cast<usize>(window->get_framebuffer_count()));

	generate_command_buffer(m_command_buffer_panels.size(), m_command_buffer_panels.data());
	generate_command_buffer(m_command_buffer_master.size(), m_command_buffer_master.data());

	for (u8 i = 0; i < window->get_framebuffer_count(); i++)
		setup_command_buffer(&m_command_buffer_panels[i], window->get_framebuffer(i));

	for (u8 i = 0; i < window->get_framebuffer_count(); i++)
		setup_command_buffer(&m_command_buffer_master[i], window->get_framebuffer(i));
}

vk::command_buffer* rhr::render::component::command_pool::get_active_command_buffer()
{
	return m_active_command_buffer;
}

vk::command_buffer* rhr::render::component::command_pool::get_master_command_buffer(u8 idx)
{
	if (idx >= static_cast<u8>(m_command_buffer_master.size()))
	{
		if (!m_command_buffer_master.empty())
		{
			latte::logger::error(
				latte::logger::level::SYSTEM, "failed to fetch master command buffer using idx, using first one instead");
			return &m_command_buffer_master.front();
		}
		else
			latte::logger::fatal(latte::logger::level::SYSTEM, "failed to fetch master command buffer, none registered");
	}

	return &m_command_buffer_master[static_cast<usize>(idx)];
}

vk::command_buffer* rhr::render::component::command_pool::get_panel_command_buffer(u8 idx)
{
	if (idx >= static_cast<u8>(m_command_buffer_panels.size()))
	{
		if (!m_command_buffer_panels.empty())
		{
			latte::logger::error(
				latte::logger::level::SYSTEM, "failed to fetch panel command buffer using idx, using first one instead");
			return &m_command_buffer_panels.front();
		}
		else
			latte::logger::fatal(latte::logger::level::SYSTEM, "failed to fetch panel command buffer, none registered");
	}

	return &m_command_buffer_panels[static_cast<usize>(idx)];
}

void rhr::render::component::command_pool::generate_command_buffer(u32 count, vk::command_buffer* command_buffer)
{
	std::unique_ptr<rhr::render::component::window>& window		  = rhr::render::renderer::get()->get_window_primary();
	vk::command_buffer_allocate_info command_buffer_allocate_info = {};

	command_buffer_allocate_info.sType				= VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	command_buffer_allocate_info.commandPool		= m_command_pool;
	command_buffer_allocate_info.level				= VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	command_buffer_allocate_info.commandBufferCount = count;

	if (vk::allocate_command_buffers(*window->get_device(), &command_buffer_allocate_info, command_buffer)
		!= VK_SUCCESS)
		latte::logger::fatal(latte::logger::level::SYSTEM, "failed to create command buffers");
}

void rhr::render::component::command_pool::setup_command_buffer(
	vk::command_buffer* command_buffer, vk::frame_buffer* frame_buffer)
{
	std::unique_ptr<rhr::render::component::window>& window = rhr::render::renderer::get()->get_window_primary();

	vk::command_buffer_begin_info begin_info	= {};
	vk::render_pass_begin_info render_pass_info = {};
	std::array<vk::clear_value, 1> clear_values = {};

	begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	if (vk::begin_command_buffer(*command_buffer, &begin_info) != VK_SUCCESS)
	{
		latte::logger::fatal(latte::logger::level::SYSTEM, "failed to begin recording the command buffer during setup");
	}

	// render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	// render_pass_info.renderPass = *window->get_render_pass(0);
	// render_pass_info.framebuffer = *frame_buffer;
	// render_pass_info.renderArea.offset = { 0, 0 };
	// render_pass_info.renderArea.extent = *window->get_swapchain_extent();
	//
	// clear_values[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
	//
	// render_pass_info.clearValueCount = static_cast<u32>(clear_values.size());
	// render_pass_info.pClearValues = clear_values.data();
	//
	// vk::cmd::begin_render_pass(*command_buffer, &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);
	// vk::cmd::bind_pipeline(*command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, *window->get_color_pipeline("master"));
	// vk::cmd::end_render_pass(*command_buffer);

	if (vk::end_command_buffer(*command_buffer) != VK_SUCCESS)
		latte::logger::fatal(latte::logger::level::SYSTEM, "failed to stop recording the command buffer during setup");
}

vk::command_pool& rhr::render::component::command_pool::get_command_pool()
{
	return m_command_pool;
}

vk::descriptor_pool& rhr::render::component::command_pool::get_descriptor_pool()
{
	return m_descriptor_pool;
}

void rhr::render::component::command_pool::set_active_command_buffer(u8 idx)
{
	m_active_command_buffer = &m_command_buffer_master[idx];
}
