#include "config.h"

#include "ModLoader.hpp"
#include "ui/Renderer.hpp"
#include "ui/RenderTools.hpp"
#include "handlers/CategoryHandler.hpp"
#include "registries/UIRegistry.hpp"
// #include "ui/ButtonText.hpp"
#include "imgui/imgui.h"

#if LINUX
#include <dlfcn.h>
#else
#include <windows.h>
#endif

// #include <RHR/RHR.h>
#include <Espresso/InputHandler.hpp>
#include <Cappuccino/Logger.hpp>
#include <Cappuccino/Intrinsics.hpp>
#include <Cappuccino/Utils.hpp>

#include <iostream>
#include <cstring>
#include <cmath>
#include <vector>
#include <math.h>
#include <string>
#include <chrono>

// Include last, has defines that conflict with enums
#if LINUX
#include <X11/Xlib.h>
#endif

/*
static Plane* toolbarPlane;
static unsigned char toolbarCatagory = 0;
static unsigned short toolbarStackCount = 0;

static std::atomic<bool> returnFinished;

void ReturnFinished()
{
	returnFinished = true;
}
*/

// void zoomViewAt(sf::Vector2i pixel, sf::RenderWindow &window, sf::View *view, float zoom)
// {
// 	const sf::Vector2f beforeCoord = window.mapPixelToCoords(pixel, *view);
// 	view->zoom(zoom);
// 	const sf::Vector2f afterCoord = window.mapPixelToCoords(pixel, *view);
// 	const sf::Vector2f offsetCoords = beforeCoord - afterCoord;
// 	view->move(offsetCoords);
// }

static std::atomic<bool> setupFinished;
static std::atomic<bool> basicSetupFinished;

static void AsyncSetup()
{
	RenderTools::Initialization();
	Renderer::Initialization();

	//std::shared_ptr<vui::RenderLayer> setupLayer = std::make_shared<vui::RenderLayer>();
	//std::shared_ptr<vui::RenderFrame> setupFrame = std::make_shared<vui::RenderFrame>();
	//setupFrame->SetWeak(setupFrame);

	//setupFrame->SetPosition({ 0, 0 });
	//setupFrame->SetSize(Renderer::WindowSize);

	//std::shared_ptr<vui::RenderRectangle> setupBackground = std::make_shared<vui::RenderRectangle>();
	//setupBackground->SetWeak(setupBackground);

	//setupBackground->SetColor(COLOR_BACKGROUND_BASE);
	//setupBackground->SetPosition({ 0, 0 });
	//setupBackground->SetSize(Renderer::WindowSize);
	//setupBackground->SetEnabled(true);

	//setupFrame->AddContent(setupBackground);

	//std::shared_ptr<vui::ProgressBar> progressStage = std::make_shared<vui::ProgressBar>(-1, vui::VerticalAlignment::CENTER);
	//std::shared_ptr<vui::ProgressBar> progressMods = std::make_shared<vui::ProgressBar>(0, vui::VerticalAlignment::CENTER);

	basicSetupFinished = true;

	//{
	//	progressStage->SetWeak(progressStage);
	//	progressMods->SetWeak(progressMods);

	//	progressStage->SetEnabled(true);
	//	progressMods->SetEnabled(true);

	//	setupFrame->AddContent(progressStage);
	//	setupFrame->AddContent(progressMods);

	//	setupLayer->AddFrame(setupFrame);
	//	setupLayer->SetEnabled(true);

	//	Renderer::AddLayer(setupLayer);

	//	ModLoaderData* loaderData = new ModLoaderData();
	//	std::thread threadModLoader(ThreadModLoader, loaderData);

	//	while (!loaderData->GetRunning())
	//		std::this_thread::sleep_for(std::chrono::milliseconds(1));

	//	while (!loaderData->GetDone())
	//	{
	//		std::this_thread::sleep_for(std::chrono::milliseconds(5));

	//		progressStage->SetProgress(static_cast<float>(loaderData->GetStage() + 1) / static_cast<float>(loaderData->GetStageCount()));
	//		progressMods->SetProgress(static_cast<float>(loaderData->GetMod() + 1) / static_cast<float>(loaderData->GetModCount()));
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

	setupFinished = true;
}

int main()
{
	// ==============================================================================================================================
	// ============== Initialization
	// ==============================================================================================================================

	// Logger::Info("CLIENT  - " std::string(VER_CLIENT));
	// Logger::Info("SERVER  - " std::string(VER_SERVER));

#if MODS
	Logger::Info("MOD_VIN - " std::string(VER_MOD_VIN));
#endif

#if BETA
	Logger::Warn("this is a beta build! There is likely tons of bugs and some critical bugs. Please be careful and save often. Report any issues to the github page https://github.com/applesthepi/unnamedblocks");
#endif

#if LINUX
	// TODO: Proper wayland support
	XInitThreads();
#endif
	Logger::Info("all unsaved progress will be lost if this window is closed");

	//ShaderRegistry::ReloadAllShaders();

	// Window Setup

	// sf::RenderWindow window;
	// window.create(sf::VideoMode(1280, 720, 32), UnnamedBlocksVersion, sf::Style::Default, sf::ContextSettings(0, 0, 4));
	// window.setFramerateLimit(60);

	// Initialization

	// Global::LoadDefaults(); //must be first
	// MessageHandler::Initialize();
	InputHandler::Initialization();
	// PreProcessor::Initialize();
	// Intrinsics::Initialize();

	// ==============================================================================================================================
	// ============== Intro Animation
	// ==============================================================================================================================
	// #if TRUE
	// 	{
	// 		sf::Http http("kikoho.ddns.net");
	// 		sf::Http::Response responseApple = http.sendRequest(sf::Http::Request("applesthepi.png"), sf::milliseconds(200));

	// 		sf::Image imgApples;
	// 		if (responseApple.getStatus() == sf::Http::Response::Status::Ok)
	// 		{
	// 			const std::string &bodyApple = responseApple.getBody(); // expecting 128
	// 			imgApples.loadFromMemory(bodyApple.c_str(), bodyApple.length());
	// 			imgApples.saveToFile("res/applesthepi.png");
	// 		}
	// 		else
	// 			imgApples.loadFromFile("res/applesthepi.png");

	// 		sf::Http::Response responseEmp = http.sendRequest(sf::Http::Request("The-Emperor10.png"), sf::milliseconds(200));

	// 		sf::Image imgEmp;
	// 		if (responseApple.getStatus() == sf::Http::Response::Status::Ok)
	// 		{
	// 			const std::string &bodyEmp = responseEmp.getBody(); // expecting 128
	// 			imgEmp.loadFromMemory(bodyEmp.c_str(), bodyEmp.length());
	// 			imgEmp.saveToFile("res/emp.png");
	// 		}
	// 		else
	// 			imgEmp.loadFromFile("res/emp.png");

	// 		sf::Texture txApples;
	// 		txApples.loadFromImage(imgApples);

	// 		sf::Texture txEmp;
	// 		txEmp.loadFromImage(imgEmp);

	// 		sf::Text title("Unnamed Blocks", Global::Font, 48);
	// 		title.setPosition((window.getSize().x / 2.0) - (title.getLocalBounds().width / 2.0), 40);
	// 		title.setFillColor(sf::Color(MOD_BUTTON_TEXT_FG));

	// 		sf::Sprite spApples(txApples);
	// 		spApples.setPosition((window.getSize().x / 3.0) - 64, (window.getSize().y / 2.0) - 64);

	// 		sf::Sprite spEmp(txEmp);
	// 		spEmp.setPosition(((window.getSize().x * 2.0) / 3.0) - 64, (window.getSize().y / 2.0) - 64);

	// 		double txtStartHeight = (window.getSize().y / 2.0) + 200;
	// 		double txtEndHeight = (window.getSize().y / 2.0) + 70;

	// 		sf::Text txtApples("applesthepi", Global::Font, 24);
	// 		int32_t xposApples = (window.getSize().x / 3.0) - (txtApples.getLocalBounds().width / 2.0);
	// 		txtApples.setPosition(xposApples, (window.getSize().y / 2.0) + 70);
	// 		txtApples.setFillColor(sf::Color(MOD_BUTTON_TEXT_FG));

	// 		sf::Text txtEmp("The-Emperor10", Global::Font, 24);
	// 		int32_t xposEmp = ((window.getSize().x * 2) / 3.0) - (txtEmp.getLocalBounds().width / 2.0);
	// 		txtEmp.setPosition(xposEmp, (window.getSize().y / 2.0) + 70);
	// 		txtEmp.setFillColor(sf::Color(MOD_BUTTON_TEXT_FG));

	// 		sf::Text titleGit("github.com/applesthepi/unnamedblocks", Global::Font, 24);
	// 		titleGit.setPosition((window.getSize().x / 2.0) - (titleGit.getLocalBounds().width / 2.0), window.getSize().y - 50);
	// 		titleGit.setFillColor(sf::Color(MOD_BUTTON_TEXT_FG));

	// 		std::chrono::high_resolution_clock::time_point begin = std::chrono::high_resolution_clock::now();

	// 		bool fading = false;
	// 		double endTime = 1200;
	// 		double fadeOffset = 1000;

	// 		while (true)
	// 		{
	// 			sf::Event ev;

	// 			while (window.pollEvent(ev))
	// 			{
	// 			}

	// 			std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();

	// 			if (std::chrono::duration_cast<std::chrono::milliseconds>(now - begin).count() >= endTime)
	// 				break;

	// 			if (std::chrono::duration_cast<std::chrono::milliseconds>(now - begin).count() >= fadeOffset)
	// 				fading = true;

	// 			double t = (std::chrono::duration_cast<std::chrono::milliseconds>(now - begin).count() - fadeOffset) / (endTime - fadeOffset);

	// 			// update

	// 			if (fading)
	// 			{
	// 				uint8_t alpha = (1.0 - t) * 253;

	// 				title.setFillColor(sf::Color(MOD_BUTTON_TEXT_FG_C, MOD_BUTTON_TEXT_FG_C, MOD_BUTTON_TEXT_FG_C, alpha));
	// 				titleGit.setFillColor(sf::Color(MOD_BUTTON_TEXT_FG_C, MOD_BUTTON_TEXT_FG_C, MOD_BUTTON_TEXT_FG_C, alpha));

	// 				txtApples.setFillColor(sf::Color(MOD_BUTTON_TEXT_FG_C, MOD_BUTTON_TEXT_FG_C, MOD_BUTTON_TEXT_FG_C, alpha));
	// 				txtEmp.setFillColor(sf::Color(MOD_BUTTON_TEXT_FG_C, MOD_BUTTON_TEXT_FG_C, MOD_BUTTON_TEXT_FG_C, alpha));

	// 				spApples.setColor(sf::Color(255, 255, 255, (1.0 - t) * 255));
	// 				spEmp.setColor(sf::Color(255, 255, 255, (1.0 - t) * 255));
	// 			}

	// 			// render

	// 			window.clear(MOD_BACKGROUND_LOW);

	// 			window.draw(title);
	// 			window.draw(spApples);
	// 			window.draw(spEmp);
	// 			window.draw(txtApples);
	// 			window.draw(txtEmp);
	// 			window.draw(titleGit);

	// 			window.display();
	// 		}

	// 		while (true)
	// 		{
	// 			sf::Event ev;

	// 			while (window.pollEvent(ev))
	// 			{
	// 			}

	// 			std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();

	// 			if (std::chrono::duration_cast<std::chrono::milliseconds>(now - begin).count() >= endTime)
	// 				break;

	// 			if (std::chrono::duration_cast<std::chrono::milliseconds>(now - begin).count() >= fadeOffset)
	// 				fading = true;

	// 			double t = (std::chrono::duration_cast<std::chrono::milliseconds>(now - begin).count() - fadeOffset) / (endTime - fadeOffset);

	// 			// update

	// 			if (fading)
	// 			{
	// 				uint8_t alpha = (1.0 - t) * 253;

	// 				title.setFillColor(sf::Color(MOD_BUTTON_TEXT_FG_C, MOD_BUTTON_TEXT_FG_C, MOD_BUTTON_TEXT_FG_C, alpha));
	// 				titleGit.setFillColor(sf::Color(MOD_BUTTON_TEXT_FG_C, MOD_BUTTON_TEXT_FG_C, MOD_BUTTON_TEXT_FG_C, alpha));

	// 				txtApples.setFillColor(sf::Color(MOD_BUTTON_TEXT_FG_C, MOD_BUTTON_TEXT_FG_C, MOD_BUTTON_TEXT_FG_C, alpha));
	// 				txtEmp.setFillColor(sf::Color(MOD_BUTTON_TEXT_FG_C, MOD_BUTTON_TEXT_FG_C, MOD_BUTTON_TEXT_FG_C, alpha));

	// 				spApples.setColor(sf::Color(255, 255, 255, (1.0 - t) * 255));
	// 				spEmp.setColor(sf::Color(255, 255, 255, (1.0 - t) * 255));
	// 			}

	// 			// render

	// 			window.clear(MOD_BACKGROUND_LOW);

	// 			window.display();
	// 		}
	// 	}
	// #endif
	// ==============================================================================================================================
	// ============== Program Initialization
	// ==============================================================================================================================

	run();
	
	// Setup

	Plane::PrimaryPlane = std::make_shared<Plane>(false);
	Plane::PrimaryPlane->SetWeak(Plane::PrimaryPlane);

	Plane::ToolbarPlane = std::make_shared<Plane>(true);
	Plane::ToolbarPlane->SetWeak(Plane::ToolbarPlane);

	std::shared_ptr<vui::RenderFrame> frameBase = std::make_shared<vui::RenderFrame>();
	frameBase->SetWeak(frameBase);
	frameBase->SetSize({ 1280, 720 });
	std::shared_ptr<vui::RenderFrame> frameBackground = std::make_shared<vui::RenderFrame>();
	frameBackground->SetWeak(frameBackground);
	frameBackground->SetSize({ 1280, 720 });
	frameBackground->SetPadding(0);

	std::shared_ptr<vui::RenderRectangle> rectBackground = std::make_shared<vui::RenderRectangle>();
	rectBackground->SetWeak(rectBackground);
	rectBackground->SetColor(Color().FromU8({128, 128, 128, 255}));
	rectBackground->SetDepth(11);

	frameBackground->AddContent(rectBackground, std::weak_ptr<IUpdatable>(), rectBackground, rectBackground, vui::LocalCardinal::RIGHT);
	rectBackground->SetSizeMax();

	std::shared_ptr<vui::RenderLayer> layer = std::make_shared<vui::RenderLayer>();
	layer->AddFrame(frameBackground);
	layer->AddFrame(frameBase);

	std::shared_ptr<vui::RenderFrame> frameA0 = std::make_shared<vui::RenderFrame>();
	frameA0->SetWeak(frameA0);
	std::shared_ptr<vui::RenderFrame> frameA1 = std::make_shared<vui::RenderFrame>();
	frameA1->SetWeak(frameA1);

	std::shared_ptr<vui::RenderFrame> frameB0 = std::make_shared<vui::RenderFrame>();
	frameB0->SetWeak(frameB0);
	std::shared_ptr<vui::RenderFrame> frameB1 = std::make_shared<vui::RenderFrame>();
	frameB1->SetWeak(frameB1);

	std::shared_ptr<vui::RenderFrame> frameC0 = std::make_shared<vui::RenderFrame>();
	frameC0->SetWeak(frameC0);
	std::shared_ptr<vui::RenderFrame> frameC1 = std::make_shared<vui::RenderFrame>();
	frameC1->SetWeak(frameC1);

	std::shared_ptr<vui::RenderFrame> frameD0 = std::make_shared<vui::RenderFrame>();
	frameD0->SetWeak(frameD0);
	std::shared_ptr<vui::RenderFrame> frameD1 = std::make_shared<vui::RenderFrame>();
	frameD1->SetWeak(frameD1);

	std::shared_ptr<vui::RenderFrame> frameE0 = std::make_shared<vui::RenderFrame>();
	frameE0->SetWeak(frameE0);
	std::shared_ptr<vui::RenderFrame> frameE1 = std::make_shared<vui::RenderFrame>();
	frameE1->SetWeak(frameE1);

	frameBase->AddFrame(frameA0, vui::LocalCardinal::RIGHT);
	frameBase->AddFrame(frameA1, vui::LocalCardinal::RIGHT);

	frameA0->SetSizeMax();
	frameA1->SetSizeMax();

	frameA0->AddFrame(frameB0, vui::LocalCardinal::DOWN);
	frameA0->AddFrame(frameB1, vui::LocalCardinal::DOWN);

	frameB0->SetSizeMax();
	frameB1->SetSizeMax();

	frameB1->AddFrame(frameC0, vui::LocalCardinal::RIGHT);
	frameB1->AddFrame(frameC1, vui::LocalCardinal::RIGHT);

	frameC0->SetSizeMax();
	frameC1->SetSizeMax();

	frameC1->AddFrame(frameD0, vui::LocalCardinal::DOWN);
	frameC1->AddFrame(frameD1, vui::LocalCardinal::DOWN);

	frameD0->SetSizeMax();
	frameD1->SetSizeMax();

	frameD0->AddFrame(frameE0, vui::LocalCardinal::DOWN);
	frameD0->AddFrame(frameE1, vui::LocalCardinal::DOWN);

	frameE0->SetSizeMax();
	frameE1->SetSizeMax();

	std::shared_ptr<vui::RenderRectangle> rectA1_1 = std::make_shared<vui::RenderRectangle>();
	rectA1_1->SetWeak(rectA1_1);
	std::shared_ptr<vui::RenderRectangle> rectA1_2 = std::make_shared<vui::RenderRectangle>();
	rectA1_2->SetWeak(rectA1_2);

	std::shared_ptr<vui::RenderRectangle> rectB0_1 = std::make_shared<vui::RenderRectangle>();
	rectB0_1->SetWeak(rectB0_1);
	std::shared_ptr<vui::RenderRectangle> rectB0_2 = std::make_shared<vui::RenderRectangle>();
	rectB0_2->SetWeak(rectB0_2);
	std::shared_ptr<vui::RenderRectangle> rectB0_3 = std::make_shared<vui::RenderRectangle>();
	rectB0_3->SetWeak(rectB0_3);
	std::shared_ptr<vui::RenderRectangle> rectB0_4 = std::make_shared<vui::RenderRectangle>();
	rectB0_4->SetWeak(rectB0_4);

	std::shared_ptr<vui::RenderRectangle> rectC0_1 = std::make_shared<vui::RenderRectangle>();
	rectC0_1->SetWeak(rectC0_1);
	std::shared_ptr<vui::RenderRectangle> rectC0_2 = std::make_shared<vui::RenderRectangle>();
	rectC0_2->SetWeak(rectC0_2);

	std::shared_ptr<vui::RenderRectangle> rectD1_1 = std::make_shared<vui::RenderRectangle>();
	rectD1_1->SetWeak(rectD1_1);
	std::shared_ptr<vui::RenderRectangle> rectD1_2 = std::make_shared<vui::RenderRectangle>();
	rectD1_2->SetWeak(rectD1_2);
	std::shared_ptr<vui::RenderRectangle> rectD1_3 = std::make_shared<vui::RenderRectangle>();
	rectD1_3->SetWeak(rectD1_3);

	std::shared_ptr<vui::RenderRectangle> rectE0_1 = std::make_shared<vui::RenderRectangle>();
	rectE0_1->SetWeak(rectE0_1);

	std::shared_ptr<vui::RenderRectangle> rectE1_1 = std::make_shared<vui::RenderRectangle>();
	rectE1_1->SetWeak(rectE1_1);
	std::shared_ptr<vui::RenderRectangle> rectE1_2 = std::make_shared<vui::RenderRectangle>();
	rectE1_2->SetWeak(rectE1_2);

	frameA1->AddContent(rectA1_1, std::weak_ptr<IUpdatable>(), rectA1_1, rectA1_1, vui::LocalCardinal::RIGHT);
	frameA1->AddContent(rectA1_2, std::weak_ptr<IUpdatable>(), rectA1_2, rectA1_2, vui::LocalCardinal::RIGHT);

	frameB0->AddContent(rectB0_1, std::weak_ptr<IUpdatable>(), rectB0_1, rectB0_1, vui::LocalCardinal::DOWN);
	frameB0->AddContent(rectB0_2, std::weak_ptr<IUpdatable>(), rectB0_2, rectB0_2, vui::LocalCardinal::DOWN);
	frameB0->AddContent(rectB0_3, std::weak_ptr<IUpdatable>(), rectB0_3, rectB0_3, vui::LocalCardinal::DOWN);
	frameB0->AddContent(rectB0_4, std::weak_ptr<IUpdatable>(), rectB0_4, rectB0_4, vui::LocalCardinal::DOWN);

	frameC0->AddContent(rectC0_1, std::weak_ptr<IUpdatable>(), rectC0_1, rectC0_1, vui::LocalCardinal::RIGHT);
	frameC0->AddContent(rectC0_2, std::weak_ptr<IUpdatable>(), rectC0_2, rectC0_2, vui::LocalCardinal::RIGHT);

	frameD1->AddContent(rectD1_1, std::weak_ptr<IUpdatable>(), rectD1_1, rectD1_1, vui::LocalCardinal::RIGHT);
	frameD1->AddContent(rectD1_2, std::weak_ptr<IUpdatable>(), rectD1_2, rectD1_2, vui::LocalCardinal::RIGHT);
	frameD1->AddContent(rectD1_3, std::weak_ptr<IUpdatable>(), rectD1_3, rectD1_3, vui::LocalCardinal::RIGHT);

	frameE0->AddContent(rectE0_1, std::weak_ptr<IUpdatable>(), rectE0_1, rectE0_1, vui::LocalCardinal::RIGHT);

	frameE1->AddContent(rectE1_1, std::weak_ptr<IUpdatable>(), rectE1_1, rectE1_1, vui::LocalCardinal::RIGHT);
	frameE1->AddContent(rectE1_2, std::weak_ptr<IUpdatable>(), rectE1_2, rectE1_2, vui::LocalCardinal::RIGHT);

	rectA1_1->SetSizeMax();
	rectA1_2->SetSizeMax();

	rectB0_1->SetSizeMax();
	rectB0_2->SetSizeMax();
	rectB0_3->SetSizeMax();
	rectB0_4->SetSizeMax();

	rectC0_1->SetSizeMax();
	rectC0_2->SetSizeMax();

	rectD1_1->SetSizeMax();
	rectD1_2->SetSizeMax();
	rectD1_3->SetSizeMax();

	rectE0_1->SetSizeMax();

	rectE1_1->SetSizeMax();
	rectE1_2->SetSizeMax();

	Renderer::AddLayer(layer);

	//frame->AddContent(Plane::PrimaryPlane, Plane::PrimaryPlane, Plane::PrimaryPlane, Plane::PrimaryPlane, vui::LocalCardinal::RIGHT);
	//frame->AddContent(Plane::ToolbarPlane, Plane::ToolbarPlane, Plane::ToolbarPlane, Plane::ToolbarPlane, vui::LocalCardinal::RIGHT);

	//Plane::PrimaryPlane->SetSizeMax();
	//Plane::ToolbarPlane->SetSizeMax();

	size_t currentFrame = 0;
	double deltaTime = 0.0f;

	TIME_POINT capture = std::chrono::high_resolution_clock::now();
	TIME_POINT last = std::chrono::high_resolution_clock::now();
	TIME_POINT begin = std::chrono::high_resolution_clock::now();
	TIME_POINT beginFps = std::chrono::high_resolution_clock::now();
	TIME_POINT sleepTime = std::chrono::high_resolution_clock::now();
	TIME_POINT diagnosticsTime = std::chrono::high_resolution_clock::now();

	std::vector<double> fpsAdverage;
	bool reloadRenderObjects = false;

	Renderer::InitializeWindow();

	// Create Client
	//Client::Instance = new Client(username, password, a_singleplayer);

	std::future<void> asyncSetup = std::async(std::launch::async, AsyncSetup);

#if DEBUG_ALLOCATIONS
	LogTotalMemoryConsumedInit();
#endif

	while (!glfwWindowShouldClose(Renderer::Window) && !basicSetupFinished)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(16 /* 60fps */));
		glfwPollEvents();
	}

	while (!glfwWindowShouldClose(Renderer::Window))
	{

#if DEBUG_ALLOCATIONS
		LogTotalMemoryConsumed();
#endif

		if (!Renderer::VsyncEnabled)
			std::this_thread::sleep_for(std::chrono::milliseconds(6 /* 144fps */));

		glfwPollEvents();

		vkWaitForFences(Renderer::Device, 1, &Renderer::InFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
		vkResetFences(Renderer::Device, 1, &Renderer::InFlightFences[currentFrame]);

		uint32_t imageIndex;
		VkResult result = vkAcquireNextImageKHR(Renderer::Device, Renderer::SwapChain, UINT64_MAX, Renderer::ImageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);
		Renderer::ActiveCommandBuffer = Renderer::CommandBuffers[imageIndex];

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			Renderer::RecreateSwapChain();
			currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
			reloadRenderObjects = true;
			continue;
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
			Logger::Fatal("failed to acquire swap chain image");

		//std::cout << imageIndex << std::endl;
		// Check if a previous frame is using this image (i.e. there is its fence to wait on)
		if (Renderer::ImagesInFlight[imageIndex] != VK_NULL_HANDLE)
			vkWaitForFences(Renderer::Device, 1, &Renderer::ImagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);

		// Mark the image as now being in use by this frame
		Renderer::ImagesInFlight[imageIndex] = Renderer::InFlightFences[currentFrame];

		capture = std::chrono::high_resolution_clock::now();
		deltaTime = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(capture - last).count()) / 1000000.0;
		last = capture;

		if (reloadRenderObjects)
		{
			Plane::PrimaryPlane->ReloadSwapChain();
			Plane::ToolbarPlane->ReloadSwapChain();

			Renderer::ReloadLayerSwapChains();
			reloadRenderObjects = false;
		}

		Renderer::Render(imageIndex, deltaTime, !setupFinished, diagnosticsTime);

		if (std::chrono::duration_cast<std::chrono::milliseconds>(capture - beginFps).count() > 200)
		{
			beginFps = capture;

			if (fpsAdverage.size() == 5)
			{
				double adverage = 0.0;

				for (uint8_t i = 0; i < fpsAdverage.size(); i++)
					adverage += fpsAdverage[i];

				adverage /= static_cast<double>(fpsAdverage.size());
				fpsAdverage.clear();

				// Logger::Info(SIDE::CLIENT, "FPS: " + std::to_string(static_cast<uint64_t>(adverage)));
			}
			else
				fpsAdverage.push_back(1.0 / deltaTime);
		}

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { Renderer::ImageAvailableSemaphores[currentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &Renderer::CommandBuffers[imageIndex];

		VkSemaphore signalSemaphores[] = { Renderer::RenderFinishedSemaphores[currentFrame] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		vkResetFences(Renderer::Device, 1, &Renderer::InFlightFences[currentFrame]);

		if (vkQueueSubmit(Renderer::GraphicsQueue, 1, &submitInfo, Renderer::InFlightFences[currentFrame]) != VK_SUCCESS)
		{
			Logger::Error("failed to submit draw call to command buffer");
			return -1;
		}

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { Renderer::SwapChain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = nullptr; // Optional

		result = vkQueuePresentKHR(Renderer::PresentQueue, &presentInfo);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || Renderer::FramebufferResized)
		{
			Renderer::FramebufferResized = false;
			Renderer::RecreateSwapChain();
			currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
			reloadRenderObjects = true;
			continue;
		}
		else if (result != VK_SUCCESS)
		{
			Logger::Error("failed to present swap chain image");
			return -1;
		}

		vkQueueWaitIdle(Renderer::PresentQueue);
		currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

		//sleepTime = std::chrono::high_resolution_clock::now();
		//while (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - sleepTime).count() < (60.0 - deltaTime))
		//{
		//	std::this_thread::sleep_for(std::chrono::milliseconds(0));
		//}
	}

	// Plane::PrimaryPlane->setPosition(sf::Vector2f(10, HEADER_HEIGHT + 5));
	// Plane::PrimaryPlane->setSize(sf::Vector2u(window.getSize().x - Plane::PrimaryPlane->getPosition().x - 5, window.getSize().y - Plane::PrimaryPlane->getPosition().y - 5));

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

	// double deltaTime = 0.0;
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

	// *Plane::ToolbarPlane->GetView() = window.getDefaultView();
	// *Plane::PrimaryPlane->GetView() = window.getDefaultView();

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

		// 				Plane::PrimaryPlane->setPosition(sf::Vector2f(CategoryHandler::GetHandler().GetToolbarWidth() + 10, HEADER_HEIGHT + 5));
		// 				Plane::PrimaryPlane->setSize(sf::Vector2u(window.getSize().x - Plane::PrimaryPlane->getPosition().x - 5, window.getSize().y - Plane::PrimaryPlane->getPosition().y - 5));
		// 			}
		// 			else if (ev.type == sf::Event::MouseWheelScrolled)
		// 			{
		// 				if (window.hasFocus())
		// 				{
		// 					int32_t delta = ev.mouseWheelScroll.delta * -200;

		// 					if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
		// 					{
		// 						sf::View* primaryView = Plane::PrimaryPlane->GetView();
		// 						primaryView->setCenter(primaryView->getCenter() + sf::Vector2f(delta, 0.0f));
		// 					}
		// 					else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
		// 					{
		// 						if (ev.mouseWheelScroll.delta > 0)
		// 						{
		// 							zoomViewAt(sf::Vector2i(ev.mouseWheelScroll.x, ev.mouseWheelScroll.y), window, Plane::PrimaryPlane->GetView(), 1.0f / 1.2f);

		// 							if (Plane::PrimaryPlane->CalculateZoom().x > 5.0)
		// 								zoomViewAt(sf::Vector2i(ev.mouseWheelScroll.x, ev.mouseWheelScroll.y), window, Plane::PrimaryPlane->GetView(), 1.2f);
		// 						}
		// 						else
		// 						{
		// 							zoomViewAt(sf::Vector2i(ev.mouseWheelScroll.x, ev.mouseWheelScroll.y), window, Plane::PrimaryPlane->GetView(), 1.2f);

		// 							if (Plane::PrimaryPlane->CalculateZoom().x < 0.2)
		// 								zoomViewAt(sf::Vector2i(ev.mouseWheelScroll.x, ev.mouseWheelScroll.y), window, Plane::PrimaryPlane->GetView(), 1.0f / 1.2f);
		// 						}
		// 					}
		// 					else
		// 					{
		// 						sf::View* primaryView = Plane::PrimaryPlane->GetView();
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

		// 					if (!UIRegistry::GetRegistry().mouseButton(true, Global::MousePosition, ev.mouseButton.button))
		// 					{
		// 						if (!Plane::PrimaryPlane->mouseButton(true, Global::MousePosition, ev.mouseButton.button))
		// 							Plane::ToolbarPlane->mouseButton(true, Global::MousePosition, ev.mouseButton.button);
		// 					}
		// 				}
		// 			}
		// 			else if (ev.type == sf::Event::EventType::MouseButtonReleased)
		// 			{
		// 				if (window.hasFocus())
		// 				{
		// 					Global::MousePosition = sf::Vector2i(ev.mouseButton.x, ev.mouseButton.y);

		// 					if (!UIRegistry::GetRegistry().mouseButton(false, Global::MousePosition, ev.mouseButton.button))
		// 					{
		// 						if (!Plane::PrimaryPlane->mouseButton(false, Global::MousePosition, ev.mouseButton.button))
		// 							Plane::ToolbarPlane->mouseButton(false, Global::MousePosition, ev.mouseButton.button);
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

				// deltaTime = static_cast<double>(clDeltaTime.getElapsedTime().asMicroseconds()) * 0.000001;
				// sfmlDeltaTime = clDeltaTime.getElapsedTime();

				// if (clDeltaDisplay.getElapsedTime().asMilliseconds() >= 100)
				// {
				// 	frameRate.setString("fps: " + std::to_string((uint64_t)floor(1.0 / deltaTime)));
				// 	clDeltaDisplay.restart();
				// }

				// clDeltaTime.restart();

				// frameRate.setPosition(sf::Vector2f(window.getSize().x - 100, 0));

				//////////////////////////////////////////////////////////
				// Frame Update
				//////////////////////////////////////////////////////////

				// UIRegistry::GetRegistry().frameUpdate(deltaTime);
				// CategoryHandler::GetHandler().frameUpdate(deltaTime);

				// Plane::ToolbarPlane->frameUpdate(deltaTime);
				// Plane::PrimaryPlane->frameUpdate(deltaTime);

				//////////////////////////////////////////////////////////
				// Render
				//////////////////////////////////////////////////////////

				// window.clear(MOD_BACKGROUND_HIGH);

				// window.draw(CategoryHandler::GetHandler());
				// Plane::ToolbarPlane->render(window);
				// Plane::PrimaryPlane->render(window);
				// window.draw(UIRegistry::GetRegistry());

				// Plane::ToolbarPlane->snapRender(window);
				// Plane::PrimaryPlane->snapRender(window);

				// CategoryHandler::GetHandler().PostRender(&window);
				// Plane::ToolbarPlane->postRender(window);
				// Plane::PrimaryPlane->postRender(window);

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

				// 		ImGui::SFML::Render(window);

				// #endif

				// 		window.draw(frameRate);
				// 		window.display();
// }

// 	ImGui::SFML::Shutdown();
// 	MessageHandler::Finish();

	return 0;
}
