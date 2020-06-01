#include "ModLoader.h"
#include "RHR/handlers/runtime/PreProcessor.h"
#include "handlers/CategoryHandler.h"
#include "registries/UIRegistry.h"
#include "ui/ButtonText.h"

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
	window.create(sf::VideoMode(1280, 720, 32), UnnamedBlocksVersion, sf::Style::Default);
	window.setVerticalSyncEnabled(false);
	window.setFramerateLimit(200);

	// Initialization

	Global::LoadDefaults();//must be first
	MessageHandler::Initialize();
	InputHandler::Initialization();
	PreProcessor::Initialize();

	// ==============================================================================================================================
	// ============== Intro Animation
	// ==============================================================================================================================
	
	{
		sf::Http http("kikoho.ddns.net");

		sf::Http::Response responseApple = http.sendRequest(sf::Http::Request("applesthepi.png"));
		const std::string& bodyApple = responseApple.getBody();// expecting 128
		sf::Image imgApples;
		imgApples.loadFromMemory(bodyApple.c_str(), bodyApple.length());

		sf::Http::Response responseEmp = http.sendRequest(sf::Http::Request("The-Emperor10.png"));
		const std::string& bodyEmp = responseEmp.getBody();// expecting 128
		sf::Image imgEmp;
		imgEmp.loadFromMemory(bodyEmp.c_str(), bodyEmp.length());

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

	// ==============================================================================================================================
	// ============== Program Initialization
	// ==============================================================================================================================

	window.setFramerateLimit(200);

	run();

	// Setup

	Plane::PrimaryPlane = new Plane();
	Plane::ToolbarPlane = new Plane();

	Plane::PrimaryPlane->setPosition(sf::Vector2f(10, HEADER_HEIGHT + 5));
	Plane::PrimaryPlane->setSize(sf::Vector2u(window.getSize().x - Plane::PrimaryPlane->getPosition().x - 5, window.getSize().y - Plane::PrimaryPlane->getPosition().y - 5));

	CategoryHandler::GetHandler().RegisterHeader();

#ifdef POSIX
	std::chrono::time_point<std::chrono::system_clock> lastVanityReload = std::chrono::high_resolution_clock::now();
#else
	std::chrono::time_point<std::chrono::steady_clock> lastVanityReload = std::chrono::high_resolution_clock::now();
#endif

	bool wasDownLeft = false;
	bool wasDownMiddle = false;
	bool wasDownRight = false;

	sf::Clock cl;
	sf::Clock clTrip;

	sf::Time lastClTrip;
	double deltaTime = 0.0;

	sf::Text frameRate = sf::Text("fps: 0", Global::Font, 12);
	frameRate.setFillColor(MOD_BUTTON_TEXT_FG);

	clTrip.restart();

	// ==============================================================================================================================
	// ============== Window Main Loop
	// ==============================================================================================================================

	while (window.isOpen())
	{
		sf::Event ev;
		while (window.pollEvent(ev))
		{
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

		if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - lastVanityReload).count() > 10)
		{
			lastVanityReload = std::chrono::high_resolution_clock::now();
			// TODO reload vanity
		}

		deltaTime = (double)lastClTrip.asMicroseconds() * 0.0000001;

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
		window.draw(*Plane::ToolbarPlane);
		window.draw(*Plane::PrimaryPlane);
		window.draw(UIRegistry::GetRegistry());

		// ==============================================================================================================================
		// ============== FPS
		// ==============================================================================================================================

		sf::Time tm = cl.getElapsedTime();
		cl.restart();

		if (clTrip.getElapsedTime().asSeconds() >= 1.0f)
		{
			frameRate.setString("fps: " + std::to_string((uint64_t)floor(1.0 / deltaTime)));
			lastClTrip = clTrip.getElapsedTime();
			clTrip.restart();
		}

		frameRate.setPosition(sf::Vector2f(window.getSize().x - 70, 0));

		window.draw(frameRate);
		window.display();
	}

	MessageHandler::Finish();

	return 0;
}
