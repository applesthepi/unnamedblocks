#include "config.h"

#include "mod_loader.hpp"
#include "rhr/rendering/renderer.hpp"
#include "rhr/rendering/tools.hpp"
#include "rhr/handlers/category.hpp"
#include "rhr/handlers/field.hpp"
#include "rhr/registries/char_texture.hpp"
#include "rhr/handlers/build.hpp"
#include "rhr/rendering/objects/button_image.hpp"
//#include "imgui/imgui.h"

#if LINUX
#include <dlfcn.h>
#else
#include <windows.h>
#endif

#include <espresso/input_handler.hpp>
#include <cappuccino/logger.hpp>
#include <cappuccino/utils.hpp>
#include <iostream>

// Include last, has defines that conflict with enums
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
	//cap::color testColor = cap::color().FromU8({ 10, 10, 10, 255 });
	//auto testColorN = testColor.GetNormalized();
	//std::cout << testColorN.r << ", " << testColorN.g << ", " << testColorN.b << ", " << testColorN.a << std::endl;

	// Logger::Info("CLIENT  - " std::string(VER_CLIENT));
	// Logger::Info("SERVER  - " std::string(VER_SERVER));

// #if MODS
// 	Logger::Info("MOD_VIN - " std::string(VER_MOD_VIN));
// #endif

// #if BETA
// 	Logger::Warn("this is a beta build! There is likely bugs. Please be careful and save often. Report any issues to the github page https://github.com/applesthepi/unnamedblocks");
// #endif

#if LINUX
	// TODO: Proper wayland support
	XInitThreads();
#endif

	Logger::Info("all unsaved progress will be lost if this window is closed");
	
	// Frames and Layers

	rhr::stack::plane::primary_plane = std::make_shared<rhr::stack::plane>(false);
	rhr::stack::plane::primary_plane->set_weak(rhr::stack::plane::primary_plane);

	rhr::stack::plane::toolbar_plane = std::make_shared<rhr::stack::plane>(true);
	rhr::stack::plane::toolbar_plane->set_weak(rhr::stack::plane::toolbar_plane);

	std::shared_ptr<rhr::render::frame> frameBase = std::make_shared<rhr::render::frame>();
	frameBase->set_weak(frameBase);
	frameBase->set_size({ 1280, 720 });

	std::shared_ptr<rhr::render::frame> frameBackground = std::make_shared<rhr::render::frame>();
	frameBackground->set_weak(frameBackground);
	frameBackground->set_size({ 1280, 720 });
	frameBackground->set_padding(0);

	std::shared_ptr<rhr::render::object::rectangle> rectBackground = std::make_shared<rhr::render::object::rectangle>();
	rectBackground->set_weak(rectBackground);
	rectBackground->set_color(cap::color::background_color_1);
	rectBackground->set_depth(rhr::render::renderer::depth_background);

	frameBackground->add_content(rectBackground, std::weak_ptr<rhr::render::interfaces::i_updateable>(), rectBackground, rectBackground, rectBackground, rhr::render::cardinal::local::RIGHT);
	rectBackground->set_size_max();

	std::shared_ptr<rhr::render::layer> layer = std::make_shared<rhr::render::layer>();
	layer->add_frame(frameBackground);
	layer->add_frame(frameBase);

	std::shared_ptr<rhr::render::frame> frameOptionsContent = std::make_shared<rhr::render::frame>();
	frameOptionsContent->set_weak(frameOptionsContent);
	std::shared_ptr<rhr::render::frame> frameSidebarPrimary = std::make_shared<rhr::render::frame>();
	frameSidebarPrimary->set_weak(frameSidebarPrimary);
	std::shared_ptr<rhr::render::frame> frameSidebarCategories = std::make_shared<rhr::render::frame>();
	frameSidebarCategories->set_weak(frameSidebarCategories);

	std::shared_ptr<rhr::render::frame> frameOptions = std::make_shared<rhr::render::frame>();
	frameOptions->set_weak(frameOptions);
	frameOptions->enable_background(cap::color::background_color_2);
	std::shared_ptr<rhr::render::frame> frameCategories = std::make_shared<rhr::render::frame>();
	frameCategories->set_weak(frameCategories);
	std::shared_ptr<rhr::render::frame> frameToolbar = std::make_shared<rhr::render::frame>();
	frameToolbar->set_weak(frameToolbar);
	frameToolbar->enable_background(cap::color::background_color_2);
	std::shared_ptr<rhr::render::frame> framePrimary = std::make_shared<rhr::render::frame>();
	framePrimary->set_weak(framePrimary);

	std::shared_ptr<rhr::render::object::rectangle> rectOptions = std::make_shared<rhr::render::object::rectangle>();
	rectOptions->set_weak(rectOptions);
	std::shared_ptr<rhr::render::object::rectangle> rectCategories = std::make_shared<rhr::render::object::rectangle>();
	rectCategories->set_weak(rectCategories);

	//frameBase->add_content(rectOptions, std::weak_ptr<rhr::render::interfaces::i_updateable>(), rectOptions, rectOptions, rhr::render::cardinal::local::RIGHT);
	//rectOptions->set_size_max();

	frameBase->add_frame(frameOptionsContent, rhr::render::cardinal::local::RIGHT);
	frameOptionsContent->set_size_max();

	frameOptionsContent->add_frame(frameOptions, rhr::render::cardinal::local::DOWN);
	frameOptionsContent->add_frame(frameSidebarPrimary, rhr::render::cardinal::local::DOWN);
	frameOptionsContent->set_bar(0, 50);

	frameOptions->set_size_max();
	frameSidebarPrimary->set_size_max();

	frameSidebarPrimary->add_frame(frameSidebarCategories, rhr::render::cardinal::local::RIGHT);
	frameSidebarPrimary->add_frame(framePrimary, rhr::render::cardinal::local::RIGHT);
	frameSidebarPrimary->set_bar(0, 200);

	frameSidebarCategories->set_size_max();
	framePrimary->set_size_max();

	frameSidebarCategories->add_frame(frameCategories, rhr::render::cardinal::local::DOWN);
	frameSidebarCategories->add_frame(frameToolbar, rhr::render::cardinal::local::DOWN);
	frameSidebarCategories->set_bar(0, 200);

	frameCategories->set_size_max();
	frameToolbar->set_size_max();

//	frameOptions->add_content(rectOptions, std::weak_ptr<rhr::render::interfaces::i_updateable>(), rectOptions, rectOptions, rhr::render::cardinal::local::RIGHT);
//	rectOptions->set_size_max();
	
//	frameCategories->add_content(rectCategories, std::weak_ptr<rhr::render::interfaces::i_updateable>(), rectCategories, rectCategories, rhr::render::cardinal::local::RIGHT);
//	rectCategories->set_size_max();

	std::shared_ptr<rhr::render::object::button_image> button_debug = std::make_shared<rhr::render::object::button_image>("res/deb_run_debug.png");
    button_debug->set_weak(button_debug);
    button_debug->set_size({16, 16});
    button_debug->set_callback(button_callback_build_debug, nullptr);
    frameOptions->add_content(button_debug, std::weak_ptr<rhr::render::interfaces::i_updateable>(), button_debug, button_debug, button_debug, rhr::render::cardinal::local::LEFT);

	framePrimary->add_content(rhr::stack::plane::primary_plane, rhr::stack::plane::primary_plane, rhr::stack::plane::primary_plane, rhr::stack::plane::primary_plane, std::weak_ptr<rhr::render::interfaces::i_enableable>(), rhr::render::cardinal::local::RIGHT);
	rhr::stack::plane::primary_plane->set_size_max();
	frameToolbar->add_content(rhr::stack::plane::toolbar_plane, rhr::stack::plane::toolbar_plane, rhr::stack::plane::toolbar_plane, rhr::stack::plane::toolbar_plane, std::weak_ptr<rhr::render::interfaces::i_enableable>(), rhr::render::cardinal::local::RIGHT);
	rhr::stack::plane::toolbar_plane->set_size_max();

	rhr::render::renderer::add_layer(layer);

	// Critical Setup

	rhr::render::renderer::initialize_window();
    async_setup();
	//std::future<void> asyncSetup = std::async(std::launch::async, AsyncSetup);

	InputHandler::Initialization();
	rhr::handler::build::initialize();
	rhr::registry::block::create_block_registry();
	//rhr::handler::field::initialize();

	run();
	rhr::handler::category::populate(frameCategories);

	// Debug

	std::shared_ptr<rhr::stack::collection> testCollection = std::make_shared<rhr::stack::collection>();
	testCollection->set_weak(testCollection);
	testCollection->set_position({ 200, 200 });
	testCollection->set_size({ 500, 300 });

	std::shared_ptr<rhr::stack::stack> testStack1 = std::make_shared<rhr::stack::stack>();
	testStack1->set_weak(testStack1);
	testStack1->set_position({ 0, 0 });

	std::shared_ptr<rhr::stack::block> testBlock1 = std::make_shared<rhr::stack::block>("vin_main");
	testBlock1->set_weak(testBlock1);
	testStack1->add_block(testBlock1);

	std::shared_ptr<rhr::stack::block> testBlock2 = std::make_shared<rhr::stack::block>("vin_string_log");
	testBlock2->set_weak(testBlock2);
	testStack1->add_block(testBlock2);

	testCollection->add_stack(testStack1);
	rhr::stack::plane::primary_plane->add_collection(testCollection, true);

	usize current_frame = 0;
	f64 delta_time = 0.0;

	TIME_POINT capture = std::chrono::high_resolution_clock::now();
	TIME_POINT last = std::chrono::high_resolution_clock::now();
	TIME_POINT begin = std::chrono::high_resolution_clock::now();
	TIME_POINT begin_fps = std::chrono::high_resolution_clock::now();
	TIME_POINT sleep_time = std::chrono::high_resolution_clock::now();
	TIME_POINT diagnostics_time = std::chrono::high_resolution_clock::now();

	std::vector<f64> fps_average;
	bool reload_render_objects = false;

	while (!glfwWindowShouldClose(rhr::render::renderer::window))
	{
		// temporary
		std::this_thread::sleep_for(std::chrono::milliseconds(6 /* 144fps */));

		glfwPollEvents();

		vkWaitForFences(rhr::render::renderer::device, 1, &rhr::render::renderer::in_flight_fences[current_frame], VK_TRUE, UINT64_MAX);
		vkResetFences(rhr::render::renderer::device, 1, &rhr::render::renderer::in_flight_fences[current_frame]);

		u32 image_index;
		VkResult result = vkAcquireNextImageKHR(rhr::render::renderer::device, rhr::render::renderer::swap_chain, UINT64_MAX, rhr::render::renderer::image_available_semaphores[current_frame], VK_NULL_HANDLE, &image_index);
		rhr::render::renderer::active_command_buffer = rhr::render::renderer::command_buffers[image_index];

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			rhr::render::renderer::recreate_swap_chain();
            current_frame = (current_frame + 1) % MAX_FRAMES_IN_FLIGHT;
            reload_render_objects = true;
			continue;
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
			Logger::Fatal("failed to acquire swap chain image");

		//std::cout << imageIndex << std::endl;
		// Check if a previous frame is using this image (i.e. there is its fence to wait on)
		if (rhr::render::renderer::images_in_flight[image_index] != VK_NULL_HANDLE)
			vkWaitForFences(rhr::render::renderer::device, 1, &rhr::render::renderer::images_in_flight[image_index], VK_TRUE, UINT64_MAX);

		// Mark the image as now being in use by this frame
		rhr::render::renderer::images_in_flight[image_index] = rhr::render::renderer::in_flight_fences[current_frame];

		capture = std::chrono::high_resolution_clock::now();
        delta_time = static_cast<f64>(std::chrono::duration_cast<std::chrono::microseconds>(capture - last).count()) / 1000000.0;
		last = capture;

		if (reload_render_objects)
		{
			rhr::stack::plane::primary_plane->reload_swap_chain();
			rhr::stack::plane::toolbar_plane->reload_swap_chain();

			rhr::render::renderer::reload_layer_swap_chains();
            reload_render_objects = false;
		}

		rhr::render::renderer::render(image_index, delta_time, false, diagnostics_time);

		if (std::chrono::duration_cast<std::chrono::milliseconds>(capture - begin_fps).count() > 200)
		{
            begin_fps = capture;

			if (fps_average.size() == 5)
			{
			    f64 average = 0.0;

				for (double i : fps_average)
                    average += i;

                average /= static_cast<f64>(fps_average.size());
				fps_average.clear();

				Logger::Debug("FPS: " + std::to_string(static_cast<u64>(average)));
			}
			else
				fps_average.push_back(1.0 / delta_time);
		}

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { rhr::render::renderer::image_available_semaphores[current_frame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &rhr::render::renderer::command_buffers[image_index];

		VkSemaphore signalSemaphores[] = { rhr::render::renderer::render_finished_semaphores[current_frame] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		vkResetFences(rhr::render::renderer::device, 1, &rhr::render::renderer::in_flight_fences[current_frame]);

		if (vkQueueSubmit(rhr::render::renderer::graphics_queue, 1, &submitInfo, rhr::render::renderer::in_flight_fences[current_frame]) != VK_SUCCESS)
		{
			Logger::Error("failed to submit draw call to command buffer");
			return -1;
		}

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { rhr::render::renderer::swap_chain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &image_index;
		presentInfo.pResults = nullptr; // Optional

		result = vkQueuePresentKHR(rhr::render::renderer::present_queue, &presentInfo);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || rhr::render::renderer::frame_buffer_resized)
		{
			rhr::render::renderer::frame_buffer_resized = false;
			rhr::render::renderer::recreate_swap_chain();
            current_frame = (current_frame + 1) % MAX_FRAMES_IN_FLIGHT;
            reload_render_objects = true;
			continue;
		}
		else if (result != VK_SUCCESS)
		{
			Logger::Error("failed to present swap chain image");
			return -1;
		}

		vkQueueWaitIdle(rhr::render::renderer::present_queue);
        current_frame = (current_frame + 1) % MAX_FRAMES_IN_FLIGHT;
	}

	return 0;
}
