#include "config.h"

#include "mod_loader.hpp"
#include "mocha/handlers/build.hpp"
#include "mocha/handlers/category.hpp"
#include "lungo/handlers/context.hpp"
#include "lungo/handlers/panel.hpp"
#include "lungo/renderer.hpp"
#include "lungo/tools.hpp"
#include "latte/serializer.hpp"
#include "mocha/handlers/project.hpp"

//#if LINUX
//#include <dlfcn.h>
//#else
//#include <windows.h>
//#endif

// include last, has defines that conflict with enums
//#if LINUX
//#include <X11/Xlib.h>
//#endif

static void async_setup()
{
	rhr::render::renderer::get()->initialize();
}

static void button_callback_build_debug(void* data)
{
	rhr::handler::build::execute(cap::build_system::method::QUICK_BUILD, cap::build_system::type::DEBUG);
}

i32 main()
{
	// clip::set_text("Hello World");

	//	espresso::color testColor = espresso::color().FromU8({ 10, 10, 10, 255 });
	//	auto testColorN = testColor.GetNormalized();
	//	std::cout << testColorN.r << ", " << testColorN.g << ", " <<
	// testColorN.b << ", " << testColorN.a << std::endl;
	//
	//	 latte::logger::Info("CLIENT  - " std::string(VER_CLIENT));
	//	 latte::logger::Info("SERVER  - " std::string(VER_SERVER));

	// #if MODS
	// 	latte::logger::Info("MOD_VIN - " std::string(VER_MOD_VIN));
	// #endif

	// #if BETA
	// 	latte::logger::Warn("this is a beta build! There is likely bugs. Please be
	// careful and save often. Report any issues to the github page
	// https://github.com/applesthepi/unnamedblocks"); #endif

#if LINUX
	// TODO: Proper wayland support
	XInitThreads();
#endif

	//
	// APPLICATION SETUP
	//

	esp::registry::set(new esp::registry());
	rhr::registry::char_texture::set(new rhr::registry::char_texture());
	rhr::render::renderer::set(new rhr::render::renderer());

	latte::logger::initialize();
	rhr::render::tools::initialize();

	rhr::render::renderer::get()->initialize_window(
		[](i16 p0, i32 p1) {
			rhr::handler::input::fire_key(p0, p1);
		},
		[](i32 p0, i32 p1) {
			rhr::handler::input::fire_mouse_button(p0, p1);
		},
		[](f32 p0) {
			rhr::handler::input::fire_mouse_scroll(p0);
		},
		[](i32 p0, i32 p1) {
			rhr::handler::input::fire_mouse_move({ p0, p1 });
		}
	);

	async_setup();
	lungo::handler::panel::initialize();
	// std::future<void> asyncSetup = std::async(std::launch::async,
	// AsyncSetup);

	rhr::handler::input::initialize();
	rhr::handler::build::initialize();
	rhr::handler::context::initialize();
	//rhr::registry::block::create_block_registry();
	// rhr::handler::field::initialize();

	//
	// CREATE PANELS
	//

	lungo::handler::panel::create_panel(
		"plane_primary",
		[](lungo::handler::panel::data& data, double delta_time)
		{
			rhr::stack::plane::primary_plane->frame_update(delta_time);
		},
		[](lungo::handler::panel::data& data)
		{
			rhr::stack::plane::primary_plane->render();
		},
		[](lungo::handler::panel::data& data)
		{
			rhr::stack::plane::primary_plane->reload_swap_chain();
		},
		[](lungo::handler::panel::data& data)
		{
			rhr::stack::plane::primary_plane->update_buffers();
		},
		[](lungo::handler::panel::data& data)
		{
			rhr::stack::plane::primary_plane->render_master_pass();
		},
		[](lungo::handler::panel::data& data)
		{
			rhr::stack::plane::primary_plane->set_position_parent_virtual_offset(
				data.panel_last_position - rhr::render::renderer::get()->get_window_primary()->get_window_position(), true);
		},
		[](lungo::handler::panel::data& data)
		{
			rhr::stack::plane::primary_plane->set_size_parent(data.panel_last_size, false);
			rhr::stack::plane::primary_plane->set_size_max(true);
		});

	lungo::handler::panel::create_panel(
		"plane_toolbar",
		[](lungo::handler::panel::data& data, double delta_time)
		{
			rhr::stack::plane::toolbar_plane->frame_update(delta_time);
		},
		[](lungo::handler::panel::data& data)
		{
			rhr::stack::plane::toolbar_plane->render();
		},
		[](lungo::handler::panel::data& data)
		{
			rhr::stack::plane::toolbar_plane->reload_swap_chain();
		},
		[](lungo::handler::panel::data& data)
		{
			rhr::stack::plane::toolbar_plane->update_buffers();
		},
		[](lungo::handler::panel::data& data)
		{
			rhr::stack::plane::toolbar_plane->render_master_pass();
		},
		[](lungo::handler::panel::data& data)
		{
			rhr::stack::plane::toolbar_plane->set_position_parent_virtual_offset(
				data.panel_last_position - rhr::render::renderer::get()->get_window_primary()->get_window_position(), true);
		},
		[](lungo::handler::panel::data& data)
		{
			rhr::stack::plane::toolbar_plane->set_size_parent(data.panel_last_size, false);
			rhr::stack::plane::toolbar_plane->set_size_max(true);
		}
	);

	lungo::handler::panel::create_panel(
		"console",
		[](lungo::handler::panel::data& data)
		{
			// List of logs that gets appended to when new messages are present in the logger.
			static std::vector<std::string> logs_system;
			static std::vector<std::string> logs_editor;
			static std::vector<std::string> logs_runtime;

			// Whether to auto scroll when at the bottom of the logs.
			static bool auto_scroll = true;

			// Flag to jump to the end of logs.
			static bool scroll_to_bottom = false;

			// Footer button index.
			static u8 button_idx   = 0;
			static u8 n_button_idx = 0;

			n_button_idx = button_idx;

			latte::logger::stream* system_stream	= latte::logger::get_stream_system();
			latte::logger::stream* editor_stream	= latte::logger::get_stream_editor();
			latte::logger::stream* runtime_stream = latte::logger::get_stream_runtime();

			if (system_stream->log_update)
			{
				system_stream->mutex.lock();
				std::vector<std::string>& cached_logs = system_stream->cached_logs;

				for (auto& cached_log : cached_logs)
					logs_system.push_back(cached_log);

				cached_logs.clear();
				system_stream->log_update = false;
				system_stream->mutex.unlock();
			}

			if (editor_stream->log_update)
			{
				editor_stream->mutex.lock();
				std::vector<std::string>& cached_logs = editor_stream->cached_logs;

				for (auto& cached_log : cached_logs)
					logs_editor.push_back(cached_log);

				cached_logs.clear();
				editor_stream->log_update = false;
				editor_stream->mutex.unlock();
			}

			if (runtime_stream->log_update)
			{
				runtime_stream->mutex.lock();
				std::vector<std::string>& cached_logs = runtime_stream->cached_logs;

				for (auto& cached_log : cached_logs)
					logs_runtime.push_back(cached_log);

				cached_logs.clear();
				runtime_stream->log_update = false;
				runtime_stream->mutex.unlock();
			}

			ImGui::Checkbox("AS", &auto_scroll);
			ImGui::Separator();

			const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
			ImGui::BeginChild(
				"ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);

			switch (button_idx)
			{
			case 0:
				for (auto& log : logs_system)
					ImGui::TextUnformatted(log.c_str());
				break;
			case 1:
				for (auto& log : logs_editor)
					ImGui::TextUnformatted(log.c_str());
				break;
			case 2:
				for (auto& log : logs_runtime)
					ImGui::TextUnformatted(log.c_str());
				break;
			}

			if (scroll_to_bottom || (auto_scroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
				ImGui::SetScrollHereY(1.0f);

			ImGui::EndChild();
			ImGui::Separator();

			constexpr glm::vec4 button_active_color = {0.15f, 0.15f, 0.15f, 1.0f};

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {2, 2});

			if (button_idx == 0)
				ImGui::PushStyleColor(
					ImGuiCol_Button,
					{button_active_color.x, button_active_color.y, button_active_color.z, button_active_color.w});
			if (ImGui::Button("system"))
				n_button_idx = 0;
			if (button_idx == 0)
				ImGui::PopStyleColor();

			ImGui::SameLine();

			if (button_idx == 1)
				ImGui::PushStyleColor(
					ImGuiCol_Button,
					{button_active_color.x, button_active_color.y, button_active_color.z, button_active_color.w});
			if (ImGui::Button("editor"))
				n_button_idx = 1;
			if (button_idx == 1)
				ImGui::PopStyleColor();

			ImGui::SameLine();

			if (button_idx == 2)
				ImGui::PushStyleColor(
					ImGuiCol_Button,
					{button_active_color.x, button_active_color.y, button_active_color.z, button_active_color.w});
			if (ImGui::Button("runtime"))
				n_button_idx = 2;
			if (button_idx == 2)
				ImGui::PopStyleColor();

			ImGui::PopStyleVar();

			button_idx		 = n_button_idx;
			scroll_to_bottom = false;
		}
	);

	//
	// EDITOR SETUP
	//

	latte::logger::info(latte::logger::level::EDITOR, "all unsaved progress will be lost if this window is closed");

	rhr::stack::plane::primary_plane = std::make_shared<rhr::stack::plane>(false);
	rhr::stack::plane::toolbar_plane = std::make_shared<rhr::stack::plane>(true);
	rhr::stack::plane::primary_plane->initialize();
	rhr::stack::plane::toolbar_plane->initialize();

	run();// TODO: overhaul mod loader
	rhr::handler::category::populate();

	rhr::stack::plane::primary_plane->set_size_parent(
		rhr::render::renderer::get()->get_window_primary()->get_window_size(), false);
	rhr::stack::plane::primary_plane->set_size_max(true);

	rhr::stack::plane::toolbar_plane->set_size_parent(
		rhr::render::renderer::get()->get_window_primary()->get_window_size(), false);
	rhr::stack::plane::toolbar_plane->set_size_max(true);

	//
	// DEFAULT BLOCKS
	//

	std::shared_ptr<rhr::stack::collection> testCollection =
		std::make_shared<rhr::stack::collection>(rhr::stack::plane::primary_plane->get_static_offset());
	testCollection->initialize();
	testCollection->set_position_local_physical({200, 200}, true);
	testCollection->set_size_local({500, 300}, true);

	std::shared_ptr<rhr::stack::stack> testStack1 =
		std::make_shared<rhr::stack::stack>(rhr::stack::plane::primary_plane->get_static_offset());
	testStack1->initialize();
	testStack1->set_position_local_physical({0, 0}, true);

	std::shared_ptr<rhr::stack::block> testBlock1 =
		std::make_shared<rhr::stack::block>("b_essentials_log_real");
	testBlock1->set_static_offset(rhr::stack::plane::primary_plane->get_static_offset());
	testBlock1->initialize();
	testStack1->add_block(testBlock1);

	std::shared_ptr<rhr::stack::block> testBlock2 =
		std::make_shared<rhr::stack::block>("b_essentials_log_real");
	testBlock2->set_static_offset(rhr::stack::plane::primary_plane->get_static_offset());
	testBlock2->initialize();
	testStack1->add_block(testBlock2);

	testCollection->add_stack(testStack1);
	rhr::stack::plane::primary_plane->add_collection(testCollection, true);

	auto testBlock2State = testBlock2->get_arguments()[0].get_esp_argument_state();
	testBlock2State->mode = esp::argument::mode::VAR;
	testBlock2->get_arguments()[0].get_esp_argument()->on_set_mode(testBlock2State);

	//
	// LOOP SETUP
	//

	TIME_POINT capture			= std::chrono::high_resolution_clock::now();
	TIME_POINT last				= std::chrono::high_resolution_clock::now();
	TIME_POINT begin			= std::chrono::high_resolution_clock::now();
	TIME_POINT begin_fps		= std::chrono::high_resolution_clock::now();
	TIME_POINT sleep_time		= std::chrono::high_resolution_clock::now();
	TIME_POINT diagnostics_time = std::chrono::high_resolution_clock::now();

	std::vector<f64> fps_average;

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	static glm::vec<2, i32> last_plane_size		= {0, 0};
	static glm::vec<2, i32> last_plane_position = {0, 0};
	static glm::vec<2, i32> window_position		= {0, 0};

	glfwGetWindowPos(rhr::render::renderer::get()->get_window_primary()->get_window(), &window_position.x, &window_position.y);
	rhr::render::renderer::get()->get_window_primary()->set_window_position(window_position);

	/*rhr::handler::serializer::node child_node_1;
	child_node_1.data_names = { "name1", "name2" };
	child_node_1.data_values = { "value1", "value2" };

	rhr::handler::serializer::node base_node;
	base_node.data_names = { "name1", "name2" };
	base_node.data_values = { "value1", "value2" };

	base_node.children.push_back(child_node_1);

	rhr::handler::serializer serial(base_node);
	auto& serial_output = serial.serialize();
	LOG_DEBUG(serial_output);*/

	//
	// LOOP
	//

	while (!glfwWindowShouldClose(rhr::render::renderer::get()->get_window_primary()->get_window()))
	{
		// TODO: config
		//		std::this_thread::sleep_for(std::chrono::milliseconds(6 /*
		// 144fps
		//*/));

		glfwPollEvents();
		glfwGetWindowPos(
			rhr::render::renderer::get()->get_window_primary()->get_window(), &window_position.x, &window_position.y);

		if (rhr::render::renderer::get()->get_window_primary()->get_swapchain_recreation_flag())
			rhr::render::renderer::get()->reload_swapchain();

		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
#if 1
		{
			static bool opt_fullscreen_persistant	  = true;
			bool opt_fullscreen						  = opt_fullscreen_persistant;
			static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

			// We are using the ImGuiWindowFlags_NoDocking flag to make the
			// parent window not dockable into, because it would be confusing to
			// have two docking targets within each others.
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

			// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace()
			// will render our background and handle the pass-thru hole, so we
			// ask Begin() to not render a background.
			if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
				window_flags |= ImGuiWindowFlags_NoBackground;

			// Important: note that we proceed even if Begin() returns false
			// (aka window is collapsed). This is because we want to keep our
			// DockSpace() active. If a DockSpace() is inactive, all active
			// windows docked into it will lose their parent and become
			// undocked. We cannot preserve the docking relationship between an
			// active window and an inactive docking, otherwise any change of
			// dockspace/settings would lead to windows being stuck in limbo and
			// never being visible.
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			static bool p_open = true;
			ImGui::Begin("###DockSpace", &p_open, window_flags);
			ImGui::PopStyleVar();

			if (ImGui::Button("run"))
				rhr::handler::build::execute(cap::build_system::method::QUICK_BUILD, cap::build_system::type::DEBUG);

			ImGui::SameLine();

			if (ImGui::Button("save"))
				rhr::handler::project::save_project("test.ub");

			ImGui::SameLine();

			if (ImGui::Button("load"))
				rhr::handler::project::load_project("test.ub");

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
		// latte::logger::info("rhr::render::renderer::render_pass_setup();");
		rhr::render::renderer::get()->render_pass_setup();

		// if
		// (rhr::render::renderer::get()->get_window_primary()->get_swapchain_recreation_flag())
		//	continue;

		lungo::handler::panel::run_imgui();
		rhr::handler::category::render();
		rhr::handler::context::run_imgui();

		ImGui::Render();
		ImDrawData* main_draw_data = ImGui::GetDrawData();
		bool main_is_minimized	   = (main_draw_data->DisplaySize.x <= 0.0f || main_draw_data->DisplaySize.y <= 0.0f);

		rhr::render::renderer::get()->imgui_local->data.ClearValue.color.float32[0] = clear_color.x * clear_color.w;
		rhr::render::renderer::get()->imgui_local->data.ClearValue.color.float32[1] = clear_color.y * clear_color.w;
		rhr::render::renderer::get()->imgui_local->data.ClearValue.color.float32[2] = clear_color.z * clear_color.w;
		rhr::render::renderer::get()->imgui_local->data.ClearValue.color.float32[3] = clear_color.w;

		if (!main_is_minimized)
		{
			rhr::render::renderer::get()->imgui_draw_data = main_draw_data;
			rhr::render::renderer::get()->render_pass_master();
		}

		if (rhr::render::renderer::get()->imgui_local->io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}

		if (main_is_minimized)
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		else
			rhr::render::renderer::get()->frame_present();
	}

	return 0;
}