#include "config.h"

#include "mod_loader.hpp"
#include "rhr/rendering/renderer.hpp"
#include "rhr/rendering/tools.hpp"
#include "rhr/rendering/device.hpp"
#include "rhr/rendering/panel.hpp"
#include "rhr/handlers/category.hpp"
#include "rhr/handlers/build.hpp"

#if LINUX
#include <dlfcn.h>
#else
#include <windows.h>
#endif

// include last, has defines that conflict with enums
#if LINUX
#include <X11/Xlib.h>
#endif

static void async_setup()
{
	rhr::render::tools::initialize();
	rhr::render::renderer::initialize();
	rhr::registry::char_texture::process_fonts();
}

static void button_callback_build_debug(void* data)
{
    rhr::handler::build::execute(cap::build_system::method::QUICK_BUILD, cap::build_system::type::DEBUG);
}

int main()
{
//	cap::color testColor = cap::color().FromU8({ 10, 10, 10, 255 });
//	auto testColorN = testColor.GetNormalized();
//	std::cout << testColorN.r << ", " << testColorN.g << ", " << testColorN.b << ", " << testColorN.a << std::endl;
//
//	 cap::logger::Info("CLIENT  - " std::string(VER_CLIENT));
//	 cap::logger::Info("SERVER  - " std::string(VER_SERVER));

// #if MODS
// 	cap::logger::Info("MOD_VIN - " std::string(VER_MOD_VIN));
// #endif

// #if BETA
// 	cap::logger::Warn("this is a beta build! There is likely bugs. Please be careful and save often. Report any issues to the github page https://github.com/applesthepi/unnamedblocks");
// #endif

#if LINUX
	// TODO: Proper wayland support
	XInitThreads();
#endif

	cap::logger::info("all unsaved progress will be lost if this window is closed");

	rhr::stack::plane::primary_plane = std::make_shared<rhr::stack::plane>(false);
	rhr::stack::plane::primary_plane->set_weak(rhr::stack::plane::primary_plane);

	rhr::stack::plane::toolbar_plane = std::make_shared<rhr::stack::plane>(true);
	rhr::stack::plane::toolbar_plane->set_weak(rhr::stack::plane::toolbar_plane);

	// critical setup

	// TODO: async setup

	rhr::render::renderer::initialize_window();
    async_setup();
    rhr::render::panel::initialize_panels();
	//std::future<void> asyncSetup = std::async(std::launch::async, AsyncSetup);

	InputHandler::Initialization();
	rhr::handler::build::initialize();
	rhr::registry::block::create_block_registry();
	//rhr::handler::field::initialize();

	run();
	rhr::handler::category::populate();

	rhr::stack::plane::primary_plane->set_size_parent(rhr::render::renderer::window_size, false);
	rhr::stack::plane::primary_plane->set_size_max();

	rhr::stack::plane::toolbar_plane->set_size_parent(rhr::render::renderer::window_size, false);
	rhr::stack::plane::toolbar_plane->set_size_max();

	// default blocks

	std::shared_ptr<rhr::stack::collection> testCollection = std::make_shared<rhr::stack::collection>();
	testCollection->set_weak(testCollection);
	testCollection->set_position_local_physical({ 200, 200 });
	testCollection->set_size_local({ 500, 300 });

	std::shared_ptr<rhr::stack::stack> testStack1 = std::make_shared<rhr::stack::stack>();
	testStack1->set_weak(testStack1);
	testStack1->set_position_local_physical({ 0, 0 });

	std::shared_ptr<rhr::stack::block> testBlock1 = std::make_shared<rhr::stack::block>("vin_main");
	testBlock1->set_weak(testBlock1);
	testStack1->add_block(testBlock1);

	std::shared_ptr<rhr::stack::block> testBlock2 = std::make_shared<rhr::stack::block>("vin_string_log");
	testBlock2->set_weak(testBlock2);
	testStack1->add_block(testBlock2);

	testCollection->add_stack(testStack1);
	rhr::stack::plane::primary_plane->add_collection(testCollection, true);

	TIME_POINT capture = std::chrono::high_resolution_clock::now();
	TIME_POINT last = std::chrono::high_resolution_clock::now();
	TIME_POINT begin = std::chrono::high_resolution_clock::now();
	TIME_POINT begin_fps = std::chrono::high_resolution_clock::now();
	TIME_POINT sleep_time = std::chrono::high_resolution_clock::now();
	TIME_POINT diagnostics_time = std::chrono::high_resolution_clock::now();

	std::vector<f64> fps_average;

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	static glm::vec<2, i32> last_plane_size = { 0, 0 };
	static glm::vec<2, i32> last_plane_position = { 0, 0 };
	static glm::vec<2, i32> window_position = { 0, 0 };

	glfwGetWindowPos(rhr::render::renderer::window, &rhr::render::renderer::window_position.x, &rhr::render::renderer::window_position.y);

	while (!glfwWindowShouldClose(rhr::render::renderer::window))
	{
		// TODO: config
//		std::this_thread::sleep_for(std::chrono::milliseconds(6 /* 144fps */));

		glfwPollEvents();
		glfwGetWindowPos(rhr::render::renderer::window, &window_position.x, &window_position.y);

		if (rhr::render::renderer::reload_swap_chain_flag)
		{
			int width, height;
			glfwGetFramebufferSize(rhr::render::renderer::window, &width, &height);
			if (width > 0 && height > 0)
			{
				rhr::render::tools::swap_chain_support_details swap_chain_support = rhr::render::tools::query_swap_chain_support(&rhr::render::device::physical_device, &rhr::render::renderer::surface);
				rhr::render::tools::queue_family_indices indices = rhr::render::tools::find_queue_families(&rhr::render::device::physical_device, &rhr::render::renderer::surface);

				ImGui_ImplVulkan_SetMinImageCount(swap_chain_support.capabilities.minImageCount);
				ImGui_ImplVulkanH_CreateOrResizeWindow(
					rhr::render::device::instance,
					rhr::render::device::physical_device,
					rhr::render::device::device_master,
					&rhr::render::renderer::imgui_local->data,
					indices.graphics_family.value(),
					nullptr,
					width,
					height,
					swap_chain_support.capabilities.minImageCount
					);

				rhr::render::renderer::imgui_local->data.FrameIndex = 0;
				rhr::render::renderer::reload_swap_chain_flag = false;

				rhr::stack::plane::primary_plane->reload_swap_chain();
				rhr::stack::plane::toolbar_plane->reload_swap_chain();

				rhr::render::renderer::reload_layer_swap_chains();
			}
		}

		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
#if 1
		{
			static bool opt_fullscreen_persistant = true;
			bool opt_fullscreen = opt_fullscreen_persistant;
			static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

			// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
			// because it would be confusing to have two docking targets within each others.
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

			if (opt_fullscreen)
			{
				ImGuiViewport* viewport = ImGui::GetMainViewport();

				ImGui::SetNextWindowPos(viewport->Pos);
				ImGui::SetNextWindowSize(viewport->Size);
				ImGui::SetNextWindowViewport(viewport->ID);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

				window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
				window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
			}

			// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
			// and handle the pass-thru hole, so we ask Begin() to not render a background.
			if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
				window_flags |= ImGuiWindowFlags_NoBackground;

			// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
			// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
			// all active windows docked into it will lose their parent and become undocked.
			// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
			// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			static bool p_open = true;
			ImGui::Begin("###DockSpace", &p_open, window_flags);
			ImGui::PopStyleVar();

			if (ImGui::Button("run"))
				rhr::handler::build::execute(cap::build_system::method::QUICK_BUILD, cap::build_system::type::DEBUG);

			if (opt_fullscreen)
				ImGui::PopStyleVar(2);

			// DockSpace
			ImGuiIO& io = ImGui::GetIO();
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}

			ImGui::End();
		}
#endif

		rhr::render::renderer::render_pass_setup();

		rhr::render::panel::run_imgui();
		rhr::handler::category::render();

		ImGui::Render();
		ImDrawData* main_draw_data = ImGui::GetDrawData();
		bool main_is_minimized = (main_draw_data->DisplaySize.x <= 0.0f || main_draw_data->DisplaySize.y <= 0.0f);

		rhr::render::renderer::imgui_local->data.ClearValue.color.float32[0] = clear_color.x * clear_color.w;
		rhr::render::renderer::imgui_local->data.ClearValue.color.float32[1] = clear_color.y * clear_color.w;
		rhr::render::renderer::imgui_local->data.ClearValue.color.float32[2] = clear_color.z * clear_color.w;
		rhr::render::renderer::imgui_local->data.ClearValue.color.float32[3] = clear_color.w;

		if (!main_is_minimized)
		{
			rhr::render::renderer::imgui_draw_data = main_draw_data;
			rhr::render::renderer::render_pass_master();
		}

		if (rhr::render::renderer::imgui_local->io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}

		if (main_is_minimized)
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		else
			rhr::render::renderer::frame_present();
	}

	return 0;
}