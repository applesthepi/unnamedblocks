#include "ModLoader.h"
#include "RHR/handlers/runtime/PreProcessor.h"
#include "handlers/CategoryHandler.h"
#include "registries/UIRegistry.h"
#include "ui/ButtonText.h"

#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"

#include <Espresso/InputHandler.h>
#include <SFML/Window/Event.hpp>

#ifdef POSIX
#include "config.h"
#include <dlfcn.h>
#else
#include <windows.h>
#endif

#include <RHR/RHR.h>
#include <SFML/Graphics.hpp>
#include <Cappuccino/Logger.h>
#include <Cappuccino/Intrinsics.h>
#include <chrono>

#include <GL/glew.h>
#include <SFML/Network.hpp>

#include <iostream>
#include <cstring>
#include <vector>
#include <math.h>
#include <string>

// Include last, has defines that conflict with enums
#ifdef LINUX
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

int main()
{
	// ==============================================================================================================================
	// ============== Initialization
	// ==============================================================================================================================

	Logger::Info(UnnamedBlocksVersion);
	if (UnnamedBlocksBeta)
		Logger::Warn("this is a beta build! There is likely tons of bugs and some critical bugs. Please be careful and save often. Report any issues to the github page https://github.com/applesthepi/unnamedblocks");

#ifdef LINUX
	// TODO: Proper wayland support
	XInitThreads();
#endif
	Logger::Info("all unsaved progress will be lost if this window is closed");

	ShaderRegistry::ReloadAllShaders();

	// Window Setup

	sf::RenderWindow window;
	window.create(sf::VideoMode(1280, 720, 32), UnnamedBlocksVersion, sf::Style::Default, sf::ContextSettings(0, 0, 4));
	window.setFramerateLimit(60);

	// Initialization

	Global::LoadDefaults();//must be first
	MessageHandler::Initialize();
	InputHandler::Initialization();
	PreProcessor::Initialize();
	Intrinsics::Initialize();

	// ==============================================================================================================================
	// ============== Intro Animation
	// ==============================================================================================================================
#if TRUE
	{
		sf::Http http("kikoho.ddns.net");
		sf::Http::Response responseApple = http.sendRequest(sf::Http::Request("applesthepi.png"), sf::milliseconds(200));
		
		sf::Image imgApples;
		if (responseApple.getStatus() == sf::Http::Response::Status::Ok)
		{
			const std::string& bodyApple = responseApple.getBody();// expecting 128
			imgApples.loadFromMemory(bodyApple.c_str(), bodyApple.length());
			imgApples.saveToFile("res/applesthepi.png");
		}
		else
			imgApples.loadFromFile("res/applesthepi.png");

		sf::Http::Response responseEmp = http.sendRequest(sf::Http::Request("The-Emperor10.png"), sf::milliseconds(200));

		sf::Image imgEmp;
		if (responseApple.getStatus() == sf::Http::Response::Status::Ok)
		{
			const std::string& bodyEmp = responseEmp.getBody();// expecting 128
			imgEmp.loadFromMemory(bodyEmp.c_str(), bodyEmp.length());
			imgEmp.saveToFile("res/emp.png");
		}
		else
			imgEmp.loadFromFile("res/emp.png");

		sf::Texture txApples;
		txApples.loadFromImage(imgApples);

		sf::Texture txEmp;
		txEmp.loadFromImage(imgEmp);

		sf::Text title("Unnamed Blocks", Global::Font, 48);
		title.setPosition((window.getSize().x / 2.0) - (title.getLocalBounds().width / 2.0), 40);
		title.setFillColor(sf::Color(MOD_BUTTON_TEXT_FG));

		sf::Sprite spApples(txApples);
		spApples.setPosition((window.getSize().x / 3.0) - 64, (window.getSize().y / 2.0) - 64);

		sf::Sprite spEmp(txEmp);
		spEmp.setPosition(((window.getSize().x * 2.0) / 3.0) - 64, (window.getSize().y / 2.0) - 64);

		double txtStartHeight = (window.getSize().y / 2.0) + 200;
		double txtEndHeight = (window.getSize().y / 2.0) + 70;

		sf::Text txtApples("applesthepi", Global::Font, 24);
		int32_t xposApples = (window.getSize().x / 3.0) - (txtApples.getLocalBounds().width / 2.0);
		txtApples.setPosition(xposApples, (window.getSize().y / 2.0) + 70);
		txtApples.setFillColor(sf::Color(MOD_BUTTON_TEXT_FG));

		sf::Text txtEmp("The-Emperor10", Global::Font, 24);
		int32_t xposEmp = ((window.getSize().x * 2) / 3.0) - (txtEmp.getLocalBounds().width / 2.0);
		txtEmp.setPosition(xposEmp, (window.getSize().y / 2.0) + 70);
		txtEmp.setFillColor(sf::Color(MOD_BUTTON_TEXT_FG));

		sf::Text titleGit("github.com/applesthepi/unnamedblocks", Global::Font, 24);
		titleGit.setPosition((window.getSize().x / 2.0) - (titleGit.getLocalBounds().width / 2.0), window.getSize().y - 50);
		titleGit.setFillColor(sf::Color(MOD_BUTTON_TEXT_FG));

		std::chrono::high_resolution_clock::time_point begin = std::chrono::high_resolution_clock::now();

		bool fading = false;
		double endTime = 1200;
		double fadeOffset = 1000;

		while (true)
		{
			sf::Event ev;

			while (window.pollEvent(ev)) {}

			std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();

			if (std::chrono::duration_cast<std::chrono::milliseconds>(now - begin).count() >= endTime)
				break;

			if (std::chrono::duration_cast<std::chrono::milliseconds>(now - begin).count() >= fadeOffset)
				fading = true;

			double t = (std::chrono::duration_cast<std::chrono::milliseconds>(now - begin).count() - fadeOffset) / (endTime - fadeOffset);

			// update

			if (fading)
			{
				uint8_t alpha = (1.0 - t) * 253;

				title.setFillColor(sf::Color(MOD_BUTTON_TEXT_FG_C, MOD_BUTTON_TEXT_FG_C, MOD_BUTTON_TEXT_FG_C, alpha));
				titleGit.setFillColor(sf::Color(MOD_BUTTON_TEXT_FG_C, MOD_BUTTON_TEXT_FG_C, MOD_BUTTON_TEXT_FG_C, alpha));

				txtApples.setFillColor(sf::Color(MOD_BUTTON_TEXT_FG_C, MOD_BUTTON_TEXT_FG_C, MOD_BUTTON_TEXT_FG_C, alpha));
				txtEmp.setFillColor(sf::Color(MOD_BUTTON_TEXT_FG_C, MOD_BUTTON_TEXT_FG_C, MOD_BUTTON_TEXT_FG_C, alpha));

				spApples.setColor(sf::Color(255, 255, 255, (1.0 - t) * 255));
				spEmp.setColor(sf::Color(255, 255, 255, (1.0 - t) * 255));
			}

			// render

			window.clear(MOD_BACKGROUND_LOW);

			window.draw(title);
			window.draw(spApples);
			window.draw(spEmp);
			window.draw(txtApples);
			window.draw(txtEmp);
			window.draw(titleGit);

			window.display();
		}
	}
#endif
	// ==============================================================================================================================
	// ============== Program Initialization
	// ==============================================================================================================================

	//window.setFramerateLimit(200);

	run();

	// Setup

	Plane::PrimaryPlane = new Plane(false);
	Plane::ToolbarPlane = new Plane(true);

	Plane::PrimaryPlane->setPosition(sf::Vector2f(10, HEADER_HEIGHT + 5));
	Plane::PrimaryPlane->setSize(sf::Vector2u(window.getSize().x - Plane::PrimaryPlane->getPosition().x - 5, window.getSize().y - Plane::PrimaryPlane->getPosition().y - 5));

	CategoryHandler::CreateHandler();
	CategoryHandler::GetHandler().RegisterHeader();

//#ifdef POSIX
//	std::chrono::time_point<std::chrono::system_clock> lastVanityReload = std::chrono::high_resolution_clock::now();
//#else
//	std::chrono::time_point<std::chrono::steady_clock> lastVanityReload = std::chrono::high_resolution_clock::now();
//#endif

	bool wasDownLeft = false;
	bool wasDownMiddle = false;
	bool wasDownRight = false;

	sf::Clock clDeltaDisplay;
	sf::Clock clDeltaTime;

	double deltaTime = 0.0;
	sf::Time sfmlDeltaTime(sf::Time::Zero);

	int fps = 240;
	int lastFps = 0;
	
	// inverted
	bool vSync = true;

	bool fpsLimiter = true;

	sf::Text frameRate = sf::Text("fps: 0", Global::Font, 12);
	frameRate.setFillColor(MOD_BUTTON_TEXT_FG);

	clDeltaDisplay.restart();
	clDeltaTime.restart();

#ifdef _DEBUG
	ImGui::SFML::Init(window);
#endif

	// ==============================================================================================================================
	// ============== Window Main Loop
	// ==============================================================================================================================

	while (window.isOpen())
	{
		sf::Event ev;
		while (window.pollEvent(ev))
		{
#ifdef _DEBUG
			ImGui::SFML::ProcessEvent(ev);
#endif

			if (ev.type == sf::Event::Closed)
			{
				std::function<void(bool)> cb = [&window](bool result)
				{
					if (result)
						window.close();
				};

				MessageHandler::RegisterMessage(new MessageConfirm("unsaved progress will be lost", &cb), true);
			}
			else if (ev.type == sf::Event::Resized)
			{
				sf::FloatRect visibleArea(0, 0, ev.size.width, ev.size.height);
				window.setView(sf::View(visibleArea));

				Plane::PrimaryPlane->setPosition(sf::Vector2f(CategoryHandler::GetHandler().GetToolbarWidth() + 10, HEADER_HEIGHT + 5));
				Plane::PrimaryPlane->setSize(sf::Vector2u(window.getSize().x - Plane::PrimaryPlane->getPosition().x - 5, window.getSize().y - Plane::PrimaryPlane->getPosition().y - 5));
			}
			else if (ev.type == sf::Event::MouseWheelScrolled)
			{
				if (window.hasFocus())
				{
					int32_t delta = ev.mouseWheelScroll.delta * -200;

					if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
						Plane::PrimaryPlane->TranslateInnerPosition(sf::Vector2i(delta, 0));
					else
						Plane::PrimaryPlane->TranslateInnerPosition(sf::Vector2i(0, delta));
				}
			}
			else if (ev.type == sf::Event::EventType::KeyPressed)
			{
				if (window.hasFocus())
					InputHandler::FireKeyEvent(ev.key);
			}
			else if (ev.type == sf::Event::EventType::TextEntered)
			{
				if (window.hasFocus())
					InputHandler::FireTextEvent(ev.text);
			}
			else if (ev.type == sf::Event::EventType::MouseButtonPressed)
			{
				if (window.hasFocus())
				{
					Global::MousePosition = sf::Vector2i(ev.mouseButton.x, ev.mouseButton.y);

					if (!UIRegistry::GetRegistry().mouseButton(true, Global::MousePosition, ev.mouseButton.button))
					{
						if (!Plane::PrimaryPlane->mouseButton(true, Global::MousePosition, ev.mouseButton.button))
							Plane::ToolbarPlane->mouseButton(true, Global::MousePosition, ev.mouseButton.button);
					}
				}
			}
			else if (ev.type == sf::Event::EventType::MouseButtonReleased)
			{
				if (window.hasFocus())
				{
					Global::MousePosition = sf::Vector2i(ev.mouseButton.x, ev.mouseButton.y);

					if (!UIRegistry::GetRegistry().mouseButton(false, Global::MousePosition, ev.mouseButton.button))
					{
						if (!Plane::PrimaryPlane->mouseButton(false, Global::MousePosition, ev.mouseButton.button))
							Plane::ToolbarPlane->mouseButton(false, Global::MousePosition, ev.mouseButton.button);
					}
				}
			}
		}

		// ==============================================================================================================================
		// ============== Global
		// ==============================================================================================================================

		if (window.hasFocus())
			Global::MousePosition = sf::Mouse::getPosition(window);
		else
			Global::MousePosition = sf::Vector2i();

		Global::WindowSize = window.getSize();

		//if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - lastVanityReload).count() > 10)
		//{
		//	lastVanityReload = std::chrono::high_resolution_clock::now();
		//	// TODO reload vanity
		//}

		// ==============================================================================================================================
		// ============== FPS
		// ==============================================================================================================================

		deltaTime = static_cast<double>(clDeltaTime.getElapsedTime().asMicroseconds()) * 0.000001;
		sfmlDeltaTime = clDeltaTime.getElapsedTime();

		if (clDeltaDisplay.getElapsedTime().asMilliseconds() >= 100)
		{
			frameRate.setString("fps: " + std::to_string((uint64_t)floor(1.0 / deltaTime)));
			clDeltaDisplay.restart();
		}

		clDeltaTime.restart();

		frameRate.setPosition(sf::Vector2f(window.getSize().x - 100, 0));

		// ==============================================================================================================================
		// ============== Frame Update
		// ==============================================================================================================================

		UIRegistry::GetRegistry().frameUpdate(deltaTime);
		CategoryHandler::GetHandler().frameUpdate(deltaTime);

		Plane::ToolbarPlane->frameUpdate(deltaTime);
		Plane::PrimaryPlane->frameUpdate(deltaTime);

		// ==============================================================================================================================
		// ============== Render
		// ==============================================================================================================================

		window.clear(MOD_BACKGROUND_HIGH);
		
		window.draw(CategoryHandler::GetHandler());
		Plane::ToolbarPlane->render(window);
		Plane::PrimaryPlane->render(window);
		window.draw(UIRegistry::GetRegistry());

		Plane::ToolbarPlane->snapRender(window);
		Plane::PrimaryPlane->snapRender(window);

		CategoryHandler::GetHandler().PostRender(&window);
		Plane::ToolbarPlane->postRender(window);
		Plane::PrimaryPlane->postRender(window);

		// ==============================================================================================================================
		// ============== ImGui
		// ==============================================================================================================================

#ifdef _DEBUG

		ImGui::SFML::Update(window, sfmlDeltaTime);

		{
			ImGui::Begin("Debugging");

			if (ImGui::TreeNode("FPS"))
			{
				if (ImGui::Checkbox("Disable VSync", &vSync))
				{
					if (!vSync)
					{
						window.setVerticalSyncEnabled(true);
						window.setFramerateLimit(0);

						fpsLimiter = false;
						fps = 0;
					}
					else
					{
						window.setVerticalSyncEnabled(false);
						window.setFramerateLimit(fps);

						fpsLimiter = false;
						fps = 0;
					}
				}

				if (vSync)
				{
					if (ImGui::Checkbox("Enable FPS Limiter", &fpsLimiter))
					{
						if (fpsLimiter)
							fps = 240;
						else
							fps = 0;
					}

					if (fpsLimiter)
					{
						ImGui::SliderInt("FPS", &fps, 5, 500);

						if (ImGui::Button("60", sf::Vector2f(50, 20)))
							fps = 60;

						ImGui::SameLine();
						if (ImGui::Button("75", sf::Vector2f(50, 20)))
							fps = 75;

						ImGui::SameLine();
						if (ImGui::Button("120", sf::Vector2f(50, 20)))
							fps = 120;

						ImGui::SameLine();
						if (ImGui::Button("144", sf::Vector2f(50, 20)))
							fps = 144;

						ImGui::SameLine();
						if (ImGui::Button("240", sf::Vector2f(50, 20)))
							fps = 240;
					}

					if (fps != lastFps)
					{
						lastFps = fps;
						window.setFramerateLimit(fps);
					}
				}

				ImGui::TreePop();
			}

			ImGui::End();
		}

		ImGui::SFML::Render(window);

#endif
		
		window.draw(frameRate);
		window.display();
	}

	ImGui::SFML::Shutdown();
	MessageHandler::Finish();

	return 0;
}
