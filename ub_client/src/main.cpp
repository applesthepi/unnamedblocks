#include "config.h"

#include "mod_loader.hpp"
#include "rhr/rendering/renderer.hpp"
#include "rhr/rendering/tools.hpp"
#include "rhr/handlers/category.hpp"
#include "rhr/handlers/field.hpp"
#include "rhr/registries/char_texture.hpp"

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

/*
static rhr::stack::plane* toolbarPlane;
static unsigned char toolbarCatagory = 0;
static unsigned short toolbarStackCount = 0;

static std::atomic<bool> returnFinished;

void ReturnFinished()
{
	returnFinished = true;
}
*/

// void zoomViewAt(sf::Vector2i pixel, sf::RenderWindow &window, sf::View *view, f32 zoom)
// {
// 	const sf::Vector2f beforeCoord = window.mapPixelToCoords(pixel, *view);
// 	view->zoom(zoom);
// 	const sf::Vector2f afterCoord = window.mapPixelToCoords(pixel, *view);
// 	const sf::Vector2f offsetCoords = beforeCoord - afterCoord;
// 	view->move(offsetCoords);
// }

//static std::atomic<bool> setupFinished;
//static std::atomic<bool> basicSetupFinished;

static void AsyncSetup()
{
	rhr::render::tools::initialize();
	rhr::render::renderer::initialize();
	rhr::registry::char_texture::process_fonts();

	//std::shared_ptr<rhr::render::layer> setupLayer = std::make_shared<rhr::render::layer>();
	//std::shared_ptr<rhr::render::frame> setupFrame = std::make_shared<rhr::render::frame>();
	//setupFrame->set_weak(setupFrame);

	//setupFrame->set_position({ 0, 0 });
	//setupFrame->set_size(rhr::render::renderer::WindowSize);

	//std::shared_ptr<rhr::render::object::rectangle> setupBackground = std::make_shared<rhr::render::object::rectangle>();
	//setupBackground->set_weak(setupBackground);

	//setupBackground->set_color(COLOR_BACKGROUND_BASE);
	//setupBackground->set_position({ 0, 0 });
	//setupBackground->set_size(rhr::render::renderer::WindowSize);
	//setupBackground->SetEnabled(true);

	//setupFrame->add_content(setupBackground);

	//std::shared_ptr<vui::ProgressBar> progressStage = std::make_shared<vui::ProgressBar>(-1, vui::VerticalAlignment::CENTER);
	//std::shared_ptr<vui::ProgressBar> progressMods = std::make_shared<vui::ProgressBar>(0, vui::VerticalAlignment::CENTER);

	//basicSetupFinished = true;

	//{
	//	progressStage->set_weak(progressStage);
	//	progressMods->set_weak(progressMods);

	//	progressStage->SetEnabled(true);
	//	progressMods->SetEnabled(true);

	//	setupFrame->add_content(progressStage);
	//	setupFrame->add_content(progressMods);

	//	setupLayer->add_frame(setupFrame);
	//	setupLayer->SetEnabled(true);

	//	rhr::render::renderer::add_layer(setupLayer);

	//	ModLoaderData* loaderData = new ModLoaderData();
	//	std::thread threadModLoader(ThreadModLoader, loaderData);

	//	while (!loaderData->GetRunning())
	//		std::this_thread::sleep_for(std::chrono::milliseconds(1));

	//	while (!loaderData->GetDone())
	//	{
	//		std::this_thread::sleep_for(std::chrono::milliseconds(5));

	//		progressStage->SetProgress(static_cast<f32>(loaderData->GetStage() + 1) / static_cast<f32>(loaderData->GetStageCount()));
	//		progressMods->SetProgress(static_cast<f32>(loaderData->GetMod() + 1) / static_cast<f32>(loaderData->GetModCount()));
	//	}

	//	if (threadModLoader.joinable())
	//		threadModLoader.join();
	//}

	//if (a_singleplayer)
	//{
	//	Server::Instance->StartSectorManager();
	//	Server::Instance->StartNetworking();
	//}

	//Logger::Debug(SIDE::CLIENT, "waiting...");
	//std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	//Logger::Debug(SIDE::CLIENT, "starting...");
	//Client::Instance->StartChunkManager();
	//Client::Instance->StartNetworking();

	//Logger::Debug(SIDE::CLIENT, "...started");
	//Client::Instance->TestRequests();

	//Client::Instance->GetDiagnostics()->SetEnabled(true);

	//setupFinished = true;
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

#if 0
	std::shared_ptr<rhr::render::frame> frameBase = std::make_shared<rhr::render::frame>();
	frameBase->set_weak(frameBase);
	frameBase->set_size({ 1280, 720 });
	std::shared_ptr<rhr::render::frame> frameBackground = std::make_shared<rhr::render::frame>();
	frameBackground->set_weak(frameBackground);
	frameBackground->set_size({ 1280, 720 });
	frameBackground->set_padding(0);

	std::shared_ptr<rhr::render::object::rectangle> rectBackground = std::make_shared<rhr::render::object::rectangle>();
	rectBackground->set_weak(rectBackground);
	rectBackground->set_color(cap::color().FromU8({128, 128, 128, 255}));
	rectBackground->set_depth(11);

	frameBackground->add_content(rectBackground, std::weak_ptr<rhr::render::interfaces::i_updateable>(), rectBackground, rectBackground, rhr::render::cardinal::local::RIGHT);
	rectBackground->set_size_max();

	std::shared_ptr<rhr::render::layer> layer = std::make_shared<rhr::render::layer>();
	layer->add_frame(frameBackground);
	layer->add_frame(frameBase);

	std::shared_ptr<rhr::render::frame> frameA0 = std::make_shared<rhr::render::frame>();
	frameA0->set_weak(frameA0);
	std::shared_ptr<rhr::render::frame> frameA1 = std::make_shared<rhr::render::frame>();
	frameA1->set_weak(frameA1);

	std::shared_ptr<rhr::render::frame> frameB0 = std::make_shared<rhr::render::frame>();
	frameB0->set_weak(frameB0);
	std::shared_ptr<rhr::render::frame> frameB1 = std::make_shared<rhr::render::frame>();
	frameB1->set_weak(frameB1);

	std::shared_ptr<rhr::render::frame> frameC0 = std::make_shared<rhr::render::frame>();
	frameC0->set_weak(frameC0);
	std::shared_ptr<rhr::render::frame> frameC1 = std::make_shared<rhr::render::frame>();
	frameC1->set_weak(frameC1);

	std::shared_ptr<rhr::render::frame> frameD0 = std::make_shared<rhr::render::frame>();
	frameD0->set_weak(frameD0);
	std::shared_ptr<rhr::render::frame> frameD1 = std::make_shared<rhr::render::frame>();
	frameD1->set_weak(frameD1);

	std::shared_ptr<rhr::render::frame> frameE0 = std::make_shared<rhr::render::frame>();
	frameE0->set_weak(frameE0);
	std::shared_ptr<rhr::render::frame> frameE1 = std::make_shared<rhr::render::frame>();
	frameE1->set_weak(frameE1);

	frameBase->add_frame(frameA0, rhr::render::cardinal::local::RIGHT);
	frameBase->add_frame(frameA1, rhr::render::cardinal::local::RIGHT);

	frameA0->set_size_max();
	frameA1->set_size_max();

	frameA0->add_frame(frameB0, rhr::render::cardinal::local::DOWN);
	frameA0->add_frame(frameB1, rhr::render::cardinal::local::DOWN);

	frameB0->set_size_max();
	frameB1->set_size_max();

	frameB1->add_frame(frameC0, rhr::render::cardinal::local::RIGHT);
	frameB1->add_frame(frameC1, rhr::render::cardinal::local::RIGHT);

	frameC0->set_size_max();
	frameC1->set_size_max();

	frameC1->add_frame(frameD0, rhr::render::cardinal::local::DOWN);
	frameC1->add_frame(frameD1, rhr::render::cardinal::local::DOWN);

	frameD0->set_size_max();
	frameD1->set_size_max();

	frameD0->add_frame(frameE0, rhr::render::cardinal::local::DOWN);
	frameD0->add_frame(frameE1, rhr::render::cardinal::local::DOWN);

	frameE0->set_size_max();
	frameE1->set_size_max();

	std::shared_ptr<rhr::render::object::rectangle> rectA1_1 = std::make_shared<rhr::render::object::rectangle>();
	rectA1_1->set_weak(rectA1_1);
	std::shared_ptr<rhr::render::object::rectangle> rectA1_2 = std::make_shared<rhr::render::object::rectangle>();
	rectA1_2->set_weak(rectA1_2);

	std::shared_ptr<rhr::render::object::rectangle> rectB0_1 = std::make_shared<rhr::render::object::rectangle>();
	rectB0_1->set_weak(rectB0_1);
	std::shared_ptr<rhr::render::object::rectangle> rectB0_2 = std::make_shared<rhr::render::object::rectangle>();
	rectB0_2->set_weak(rectB0_2);
	std::shared_ptr<rhr::render::object::rectangle> rectB0_3 = std::make_shared<rhr::render::object::rectangle>();
	rectB0_3->set_weak(rectB0_3);
	std::shared_ptr<rhr::render::object::rectangle> rectB0_4 = std::make_shared<rhr::render::object::rectangle>();
	rectB0_4->set_weak(rectB0_4);

	std::shared_ptr<rhr::render::object::rectangle> rectC0_1 = std::make_shared<rhr::render::object::rectangle>();
	rectC0_1->set_weak(rectC0_1);
	std::shared_ptr<rhr::render::object::rectangle> rectC0_2 = std::make_shared<rhr::render::object::rectangle>();
	rectC0_2->set_weak(rectC0_2);

	std::shared_ptr<rhr::render::object::rectangle> rectD1_1 = std::make_shared<rhr::render::object::rectangle>();
	rectD1_1->set_weak(rectD1_1);
	std::shared_ptr<rhr::render::object::rectangle> rectD1_2 = std::make_shared<rhr::render::object::rectangle>();
	rectD1_2->set_weak(rectD1_2);
	std::shared_ptr<rhr::render::object::rectangle> rectD1_3 = std::make_shared<rhr::render::object::rectangle>();
	rectD1_3->set_weak(rectD1_3);

	std::shared_ptr<rhr::render::object::rectangle> rectE0_1 = std::make_shared<rhr::render::object::rectangle>();
	rectE0_1->set_weak(rectE0_1);

	std::shared_ptr<rhr::render::object::rectangle> rectE1_1 = std::make_shared<rhr::render::object::rectangle>();
	rectE1_1->set_weak(rectE1_1);
	std::shared_ptr<rhr::render::object::rectangle> rectE1_2 = std::make_shared<rhr::render::object::rectangle>();
	rectE1_2->set_weak(rectE1_2);

	frameA1->add_content(rectA1_1, std::weak_ptr<rhr::render::interfaces::i_updateable>(), rectA1_1, rectA1_1, rhr::render::cardinal::local::RIGHT);
	frameA1->add_content(rectA1_2, std::weak_ptr<rhr::render::interfaces::i_updateable>(), rectA1_2, rectA1_2, rhr::render::cardinal::local::RIGHT);

	frameB0->add_content(rectB0_1, std::weak_ptr<rhr::render::interfaces::i_updateable>(), rectB0_1, rectB0_1, rhr::render::cardinal::local::DOWN);
	frameB0->add_content(rectB0_2, std::weak_ptr<rhr::render::interfaces::i_updateable>(), rectB0_2, rectB0_2, rhr::render::cardinal::local::DOWN);
	frameB0->add_content(rectB0_3, std::weak_ptr<rhr::render::interfaces::i_updateable>(), rectB0_3, rectB0_3, rhr::render::cardinal::local::DOWN);
	frameB0->add_content(rectB0_4, std::weak_ptr<rhr::render::interfaces::i_updateable>(), rectB0_4, rectB0_4, rhr::render::cardinal::local::DOWN);

	frameC0->add_content(rectC0_1, std::weak_ptr<rhr::render::interfaces::i_updateable>(), rectC0_1, rectC0_1, rhr::render::cardinal::local::RIGHT);
	frameC0->add_content(rectC0_2, std::weak_ptr<rhr::render::interfaces::i_updateable>(), rectC0_2, rectC0_2, rhr::render::cardinal::local::RIGHT);

	frameD1->add_content(rectD1_1, std::weak_ptr<rhr::render::interfaces::i_updateable>(), rectD1_1, rectD1_1, rhr::render::cardinal::local::RIGHT);
	frameD1->add_content(rectD1_2, std::weak_ptr<rhr::render::interfaces::i_updateable>(), rectD1_2, rectD1_2, rhr::render::cardinal::local::RIGHT);
	frameD1->add_content(rectD1_3, std::weak_ptr<rhr::render::interfaces::i_updateable>(), rectD1_3, rectD1_3, rhr::render::cardinal::local::RIGHT);

	frameE0->add_content(rectE0_1, std::weak_ptr<rhr::render::interfaces::i_updateable>(), rectE0_1, rectE0_1, rhr::render::cardinal::local::RIGHT);

	frameE1->add_content(rectE1_1, std::weak_ptr<rhr::render::interfaces::i_updateable>(), rectE1_1, rectE1_1, rhr::render::cardinal::local::RIGHT);
	frameE1->add_content(rectE1_2, std::weak_ptr<rhr::render::interfaces::i_updateable>(), rectE1_2, rectE1_2, rhr::render::cardinal::local::RIGHT);

	rectA1_1->set_size_max();
	rectA1_2->set_size_max();

	rectB0_1->set_size_max();
	rectB0_2->set_size_max();
	rectB0_3->set_size_max();
	rectB0_4->set_size_max();

	rectC0_1->set_size_max();
	rectC0_2->set_size_max();

	rectD1_1->set_size_max();
	rectD1_2->set_size_max();
	rectD1_3->set_size_max();

	rectE0_1->set_size_max();

	rectE1_1->set_size_max();
	rectE1_2->set_size_max();

	rhr::render::renderer::add_layer(layer);
#endif

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

	frameBackground->add_content(rectBackground, std::weak_ptr<rhr::render::interfaces::i_updateable>(), rectBackground, rectBackground, rhr::render::cardinal::local::RIGHT);
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
	std::shared_ptr<rhr::render::frame> frameCategories = std::make_shared<rhr::render::frame>();
	frameCategories->set_weak(frameCategories);
	std::shared_ptr<rhr::render::frame> frameToolbar = std::make_shared<rhr::render::frame>();
	frameToolbar->set_weak(frameToolbar);
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

	frameOptions->add_content(rectOptions, std::weak_ptr<rhr::render::interfaces::i_updateable>(), rectOptions, rectOptions, rhr::render::cardinal::local::RIGHT);
	rectOptions->set_size_max();
	
	//frameCategories->add_content(rectCategories, std::weak_ptr<rhr::render::interfaces::i_updateable>(), rectCategories, rectCategories, rhr::render::cardinal::local::RIGHT);
	//rectCategories->set_size_max();

	framePrimary->add_content(rhr::stack::plane::primary_plane, rhr::stack::plane::primary_plane, rhr::stack::plane::primary_plane, rhr::stack::plane::primary_plane, rhr::render::cardinal::local::RIGHT);
	rhr::stack::plane::primary_plane->set_size_max();
	frameToolbar->add_content(rhr::stack::plane::toolbar_plane, rhr::stack::plane::toolbar_plane, rhr::stack::plane::toolbar_plane, rhr::stack::plane::toolbar_plane, rhr::render::cardinal::local::RIGHT);
	rhr::stack::plane::toolbar_plane->set_size_max();

	rhr::render::renderer::add_layer(layer);

	// Critical Setup

	rhr::render::renderer::initialize_window();
	AsyncSetup();
	//std::future<void> asyncSetup = std::async(std::launch::async, AsyncSetup);

	InputHandler::Initialization();
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
	
	std::shared_ptr<rhr::stack::stack> testStack2 = std::make_shared<rhr::stack::stack>();
	testStack2->set_weak(testStack2);
	testStack2->set_position({ 300, 300 });

	for (u8 i = 0; i < 5; i++)
	{
		std::shared_ptr<rhr::stack::block> testBlock = std::make_shared<rhr::stack::block>("vin_main");
		testBlock->set_weak(testBlock);
		testStack1->add_block(testBlock);
	}

	for (u8 i = 0; i < 5; i++)
	{
		std::shared_ptr<rhr::stack::block> testBlock = std::make_shared<rhr::stack::block>("vin_main");
		testBlock->set_weak(testBlock);
		testStack2->add_block(testBlock);
	}

	testCollection->add_stack(testStack1);
	testCollection->add_stack(testStack2);
	rhr::stack::plane::primary_plane->add_collection(testCollection, true);

	usize currentFrame = 0;
	f64 deltaTime = 0.0f;

	TIME_POINT capture = std::chrono::high_resolution_clock::now();
	TIME_POINT last = std::chrono::high_resolution_clock::now();
	TIME_POINT begin = std::chrono::high_resolution_clock::now();
	TIME_POINT beginFps = std::chrono::high_resolution_clock::now();
	TIME_POINT sleepTime = std::chrono::high_resolution_clock::now();
	TIME_POINT diagnosticsTime = std::chrono::high_resolution_clock::now();

	std::vector<f64> fpsAdverage;
	bool reloadRenderObjects = false;

	//for (auto block : rhr::registry::block::GetRegistry().GetBlocks())
	//{
	//	rhr::stack::plane::toolbar_plane
	//}


	// Create Client
	//Client::Instance = new Client(username, password, a_singleplayer);

#if DEBUG_ALLOCATIONS
	LogTotalMemoryConsumedInit();
#endif

	//while (!glfwWindowShouldClose(rhr::render::renderer::Window) && !basicSetupFinished)
	//{
	//	std::this_thread::sleep_for(std::chrono::milliseconds(16 /* 60fps */));
	//	glfwPollEvents();
	//}

	while (!glfwWindowShouldClose(rhr::render::renderer::window))
	{

#if DEBUG_ALLOCATIONS
		LogTotalMemoryConsumed();
#endif

		//if (!rhr::render::renderer::vsync_enabled)
			std::this_thread::sleep_for(std::chrono::milliseconds(6 /* 144fps */));

		glfwPollEvents();

		vkWaitForFences(rhr::render::renderer::device, 1, &rhr::render::renderer::in_flight_fences[currentFrame], VK_TRUE, UINT64_MAX);
		vkResetFences(rhr::render::renderer::device, 1, &rhr::render::renderer::in_flight_fences[currentFrame]);

		u32 imageIndex;
		VkResult result = vkAcquireNextImageKHR(rhr::render::renderer::device, rhr::render::renderer::swap_chain, UINT64_MAX, rhr::render::renderer::image_available_semaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);
		rhr::render::renderer::active_command_buffer = rhr::render::renderer::command_buffers[imageIndex];

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			rhr::render::renderer::recreate_swap_chain();
			currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
			reloadRenderObjects = true;
			continue;
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
			Logger::Fatal("failed to acquire swap chain image");

		//std::cout << imageIndex << std::endl;
		// Check if a previous frame is using this image (i.e. there is its fence to wait on)
		if (rhr::render::renderer::images_in_flight[imageIndex] != VK_NULL_HANDLE)
			vkWaitForFences(rhr::render::renderer::device, 1, &rhr::render::renderer::images_in_flight[imageIndex], VK_TRUE, UINT64_MAX);

		// Mark the image as now being in use by this frame
		rhr::render::renderer::images_in_flight[imageIndex] = rhr::render::renderer::in_flight_fences[currentFrame];

		capture = std::chrono::high_resolution_clock::now();
		deltaTime = static_cast<f64>(std::chrono::duration_cast<std::chrono::microseconds>(capture - last).count()) / 1000000.0;
		last = capture;

		if (reloadRenderObjects)
		{
			rhr::stack::plane::primary_plane->reload_swap_chain();
			rhr::stack::plane::toolbar_plane->reload_swap_chain();

			rhr::render::renderer::reload_layer_swap_chains();
			reloadRenderObjects = false;
		}

		rhr::render::renderer::render(imageIndex, deltaTime, /*!setupFinished*/ false, diagnosticsTime);

		if (std::chrono::duration_cast<std::chrono::milliseconds>(capture - beginFps).count() > 200)
		{
			beginFps = capture;

			if (fpsAdverage.size() == 5)
			{
				f64 adverage = 0.0;

				for (u8 i = 0; i < fpsAdverage.size(); i++)
					adverage += fpsAdverage[i];

				adverage /= static_cast<f64>(fpsAdverage.size());
				fpsAdverage.clear();

				// Logger::Info(SIDE::CLIENT, "FPS: " + std::to_string(static_cast<u64>(adverage)));
			}
			else
				fpsAdverage.push_back(1.0 / deltaTime);
		}

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { rhr::render::renderer::image_available_semaphores[currentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &rhr::render::renderer::command_buffers[imageIndex];

		VkSemaphore signalSemaphores[] = { rhr::render::renderer::render_finished_semaphores[currentFrame] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		vkResetFences(rhr::render::renderer::device, 1, &rhr::render::renderer::in_flight_fences[currentFrame]);

		if (vkQueueSubmit(rhr::render::renderer::graphics_queue, 1, &submitInfo, rhr::render::renderer::in_flight_fences[currentFrame]) != VK_SUCCESS)
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
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = nullptr; // Optional

		result = vkQueuePresentKHR(rhr::render::renderer::present_queue, &presentInfo);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || rhr::render::renderer::frame_buffer_resized)
		{
			rhr::render::renderer::frame_buffer_resized = false;
			rhr::render::renderer::recreate_swap_chain();
			currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
			reloadRenderObjects = true;
			continue;
		}
		else if (result != VK_SUCCESS)
		{
			Logger::Error("failed to present swap chain image");
			return -1;
		}

		vkQueueWaitIdle(rhr::render::renderer::present_queue);
		currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

		//sleepTime = std::chrono::high_resolution_clock::now();
		//while (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - sleepTime).count() < (60.0 - deltaTime))
		//{
		//	std::this_thread::sleep_for(std::chrono::milliseconds(0));
		//}
	}

	// rhr::stack::plane::primary_plane->setPosition(sf::Vector2f(10, HEADER_HEIGHT + 5));
	// rhr::stack::plane::primary_plane->setSize(sf::Vector2u(window.getSize().x - rhr::stack::plane::primary_plane->getPosition().x - 5, window.getSize().y - rhr::stack::plane::primary_plane->getPosition().y - 5));

	// CategoryHandler::CreateHandler();
	// CategoryHandler::GetHandler().RegisterHeader();

	//#ifdef POSIX
	//	std::chrono::time_point<std::chrono::system_clock> lastVanityReload = std::chrono::high_resolution_clock::now();
	//#else
	//	std::chrono::time_point<std::chrono::steady_clock> lastVanityReload = std::chrono::high_resolution_clock::now();
	//#endif

	// bool wasDownLeft = false;
	// bool wasDownMiddle = false;
	// bool wasDownRight = false;

	// sf::Clock clDeltaDisplay;
	// sf::Clock clDeltaTime;

	// f64 deltaTime = 0.0;
	// sf::Time sfmlDeltaTime(sf::Time::Zero);

	// int fps = 240;
	// int lastFps = 0;

	// window.setFramerateLimit(fps);

	// inverted
	// bool vSync = true;

	// bool fpsLimiter = true;

	// sf::Text frameRate = sf::Text("fps: 0", Global::Font, 12);
	// frameRate.setFillColor(MOD_BUTTON_TEXT_FG);

	// clDeltaDisplay.restart();
	// clDeltaTime.restart();

	// *rhr::stack::plane::toolbar_plane->GetView() = window.getDefaultView();
	// *rhr::stack::plane::primary_plane->GetView() = window.getDefaultView();

	// #ifndef NDEBUG
	// 	//auto killme = ImGui::CreateContext();
	// 	//ImGui::SetCurrentContext(killme);
	// 	ImGui::SFML::Init(window);
	// #endif

		//////////////////////////////////////////////////////////
		// Window Main Loop
		//////////////////////////////////////////////////////////

	// while (window.isOpen())
	// {
		// 		sf::Event ev;
		// 		while (window.pollEvent(ev))
		// 		{
		// #ifndef NDEBUG
		// 			ImGui::SFML::ProcessEvent(ev);
		// #endif

		// 			if (ev.type == sf::Event::Closed)
		// 			{
		// 				std::function<void(bool)> cb = [&window](bool result)
		// 				{
		// 					if (result)
		// 						window.close();
		// 				};

		// 				MessageHandler::RegisterMessage(new MessageConfirm("unsaved progress will be lost", &cb), true);
		// 			}
		// 			else if (ev.type == sf::Event::Resized)
		// 			{
		// 				sf::FloatRect visibleArea(0, 0, ev.size.width, ev.size.height);
		// 				window.setView(sf::View(visibleArea));

		// 				rhr::stack::plane::primary_plane->setPosition(sf::Vector2f(CategoryHandler::GetHandler().GetToolbarWidth() + 10, HEADER_HEIGHT + 5));
		// 				rhr::stack::plane::primary_plane->setSize(sf::Vector2u(window.getSize().x - rhr::stack::plane::primary_plane->getPosition().x - 5, window.getSize().y - rhr::stack::plane::primary_plane->getPosition().y - 5));
		// 			}
		// 			else if (ev.type == sf::Event::MouseWheelScrolled)
		// 			{
		// 				if (window.hasFocus())
		// 				{
		// 					i32 delta = ev.mouseWheelScroll.delta * -200;

		// 					if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
		// 					{
		// 						sf::View* primaryView = rhr::stack::plane::primary_plane->GetView();
		// 						primaryView->setCenter(primaryView->getCenter() + sf::Vector2f(delta, 0.0f));
		// 					}
		// 					else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
		// 					{
		// 						if (ev.mouseWheelScroll.delta > 0)
		// 						{
		// 							zoomViewAt(sf::Vector2i(ev.mouseWheelScroll.x, ev.mouseWheelScroll.y), window, rhr::stack::plane::primary_plane->GetView(), 1.0f / 1.2f);

		// 							if (rhr::stack::plane::primary_plane->CalculateZoom().x > 5.0)
		// 								zoomViewAt(sf::Vector2i(ev.mouseWheelScroll.x, ev.mouseWheelScroll.y), window, rhr::stack::plane::primary_plane->GetView(), 1.2f);
		// 						}
		// 						else
		// 						{
		// 							zoomViewAt(sf::Vector2i(ev.mouseWheelScroll.x, ev.mouseWheelScroll.y), window, rhr::stack::plane::primary_plane->GetView(), 1.2f);

		// 							if (rhr::stack::plane::primary_plane->CalculateZoom().x < 0.2)
		// 								zoomViewAt(sf::Vector2i(ev.mouseWheelScroll.x, ev.mouseWheelScroll.y), window, rhr::stack::plane::primary_plane->GetView(), 1.0f / 1.2f);
		// 						}
		// 					}
		// 					else
		// 					{
		// 						sf::View* primaryView = rhr::stack::plane::primary_plane->GetView();
		// 						primaryView->setCenter(primaryView->getCenter() + sf::Vector2f(0.0f, delta));
		// 					}
		// 				}
		// 			}
		// 			else if (ev.type == sf::Event::EventType::KeyPressed)
		// 			{
		// 				if (window.hasFocus())
		// 					InputHandler::FireKeyEvent(ev.key);
		// 			}
		// 			else if (ev.type == sf::Event::EventType::TextEntered)
		// 			{
		// 				if (window.hasFocus())
		// 					InputHandler::FireTextEvent(ev.text);
		// 			}
		// 			else if (ev.type == sf::Event::EventType::MouseButtonPressed)
		// 			{
		// 				if (window.hasFocus())
		// 				{
		// 					Global::MousePosition = sf::Vector2i(ev.mouseButton.x, ev.mouseButton.y);

		// 					if (!rhr::registry::char_texture::GetRegistry().mouseButton(true, Global::MousePosition, ev.mouseButton.button))
		// 					{
		// 						if (!rhr::stack::plane::primary_plane->mouseButton(true, Global::MousePosition, ev.mouseButton.button))
		// 							rhr::stack::plane::toolbar_plane->mouseButton(true, Global::MousePosition, ev.mouseButton.button);
		// 					}
		// 				}
		// 			}
		// 			else if (ev.type == sf::Event::EventType::MouseButtonReleased)
		// 			{
		// 				if (window.hasFocus())
		// 				{
		// 					Global::MousePosition = sf::Vector2i(ev.mouseButton.x, ev.mouseButton.y);

		// 					if (!rhr::registry::char_texture::GetRegistry().mouseButton(false, Global::MousePosition, ev.mouseButton.button))
		// 					{
		// 						if (!rhr::stack::plane::primary_plane->mouseButton(false, Global::MousePosition, ev.mouseButton.button))
		// 							rhr::stack::plane::toolbar_plane->mouseButton(false, Global::MousePosition, ev.mouseButton.button);
		// 					}
		// 				}
		// 			}
		// 		}

				//////////////////////////////////////////////////////////
				// Global
				//////////////////////////////////////////////////////////

				// if (window.hasFocus())
				// 	Global::MousePosition = sf::Mouse::getPosition(window);
				// else
				// 	Global::MousePosition = sf::Vector2i();

				// Global::WindowSize = window.getSize();

				//////////////////////////////////////////////////////////
				// FPS
				//////////////////////////////////////////////////////////

				// deltaTime = static_cast<f64>(clDeltaTime.getElapsedTime().asMicroseconds()) * 0.000001;
				// sfmlDeltaTime = clDeltaTime.getElapsedTime();

				// if (clDeltaDisplay.getElapsedTime().asMilliseconds() >= 100)
				// {
				// 	frameRate.setString("fps: " + std::to_string((u64)floor(1.0 / deltaTime)));
				// 	clDeltaDisplay.restart();
				// }

				// clDeltaTime.restart();

				// frameRate.setPosition(sf::Vector2f(window.getSize().x - 100, 0));

				//////////////////////////////////////////////////////////
				// Frame Update
				//////////////////////////////////////////////////////////

				// rhr::registry::char_texture::GetRegistry().frameUpdate(deltaTime);
				// CategoryHandler::GetHandler().frameUpdate(deltaTime);

				// rhr::stack::plane::toolbar_plane->frameUpdate(deltaTime);
				// rhr::stack::plane::primary_plane->frameUpdate(deltaTime);

				//////////////////////////////////////////////////////////
				// render
				//////////////////////////////////////////////////////////

				// window.clear(MOD_BACKGROUND_HIGH);

				// window.draw(CategoryHandler::GetHandler());
				// rhr::stack::plane::toolbar_plane->render(window);
				// rhr::stack::plane::primary_plane->render(window);
				// window.draw(rhr::registry::char_texture::GetRegistry());

				// rhr::stack::plane::toolbar_plane->snapRender(window);
				// rhr::stack::plane::primary_plane->snapRender(window);

				// CategoryHandler::GetHandler().PostRender(&window);
				// rhr::stack::plane::toolbar_plane->postRender(window);
				// rhr::stack::plane::primary_plane->postRender(window);

				//////////////////////////////////////////////////////////
				// ImGui
				//////////////////////////////////////////////////////////

				// #ifndef NDEBUG
				// 		ImGui::SFML::Update(window, sfmlDeltaTime);

				// 		{
				// 			ImGui::Begin("Debugging");

				// 			if (ImGui::TreeNode("FPS"))
				// 			{
				// 				if (ImGui::Checkbox("Disable VSync", &vSync))
				// 				{
				// 					if (!vSync)
				// 					{
				// 						window.setVerticalSyncEnabled(true);
				// 						window.setFramerateLimit(0);

				// 						fpsLimiter = false;
				// 						fps = 0;
				// 					}
				// 					else
				// 					{
				// 						window.setVerticalSyncEnabled(false);
				// 						window.setFramerateLimit(fps);

				// 						fpsLimiter = false;
				// 						fps = 0;
				// 					}
				// 				}

				// 				if (vSync)
				// 				{
				// 					if (ImGui::Checkbox("Enable FPS Limiter", &fpsLimiter))
				// 					{
				// 						if (fpsLimiter)
				// 							fps = 240;
				// 						else
				// 							fps = 0;
				// 					}

				// 					if (fpsLimiter)
				// 					{
				// 						ImGui::SliderInt("FPS", &fps, 5, 500);

				// 						if (ImGui::Button("60", sf::Vector2f(50, 20)))
				// 							fps = 60;

				// 						ImGui::SameLine();
				// 						if (ImGui::Button("75", sf::Vector2f(50, 20)))
				// 							fps = 75;

				// 						ImGui::SameLine();
				// 						if (ImGui::Button("120", sf::Vector2f(50, 20)))
				// 							fps = 120;

				// 						ImGui::SameLine();
				// 						if (ImGui::Button("144", sf::Vector2f(50, 20)))
				// 							fps = 144;

				// 						ImGui::SameLine();
				// 						if (ImGui::Button("240", sf::Vector2f(50, 20)))
				// 							fps = 240;
				// 					}

				// 					if (fps != lastFps)
				// 					{
				// 						lastFps = fps;
				// 						window.setFramerateLimit(fps);
				// 					}
				// 				}

				// 				ImGui::TreePop();
				// 			}

				// 			ImGui::End();
				// 		}

				// 		ImGui::SFML::render(window);

				// #endif

				// 		window.draw(frameRate);
				// 		window.display();
// }

// 	ImGui::SFML::Shutdown();
// 	MessageHandler::Finish();

	return 0;
}
