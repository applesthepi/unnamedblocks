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

static Plane* toolbarPlane;
static unsigned char toolbarCatagory = 0;
static unsigned short toolbarStackCount = 0;

static std::atomic<bool> returnFinished;

void ReturnFinished()
{
	returnFinished = true;
}

int main()
{
	Logger::Info(UnnamedBlocksVersion);
	if (UnnamedBlocksBeta)
		Logger::Warn("this is a beta build! There is likely tons of bugs and some critical bugs. Please be careful and save often. Report any issues to the github page https://github.com/applesthepi/unnamedblocks");

#ifdef LINUX
	// TODO: Proper wayland support
	XInitThreads();
#endif
	Logger::Info("all unsaved progress will be lost if this window is closed");

	ShaderRegistry::Initialize();
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
	
	{
		//
		// intro
		//

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

		sf::Text title("Unnamed Blocks", *Global::Font, 48);
		title.setPosition((window.getSize().x / 2.0) - (title.getLocalBounds().width / 2.0), 40);
		title.setFillColor(sf::Color(MOD_BUTTON_TEXT_FG));

		sf::Sprite spApples(txApples);
		spApples.setPosition((window.getSize().x / 3.0) - 64, (window.getSize().y / 2.0) - 64);

		sf::Sprite spEmp(txEmp);
		spEmp.setPosition(((window.getSize().x * 2.0) / 3.0) - 64, (window.getSize().y / 2.0) - 64);

		double txtStartHeight = (window.getSize().y / 2.0) + 200;
		double txtEndHeight = (window.getSize().y / 2.0) + 70;

		sf::Text txtApples("applesthepi", *Global::Font, 24);
		int32_t xposApples = (window.getSize().x / 3.0) - (txtApples.getLocalBounds().width / 2.0);
		txtApples.setPosition(xposApples, (window.getSize().y / 2.0) + 70);
		txtApples.setFillColor(sf::Color(MOD_BUTTON_TEXT_FG));

		sf::Text txtEmp("The-Emperor10", *Global::Font, 24);
		int32_t xposEmp = ((window.getSize().x * 2) / 3.0) - (txtEmp.getLocalBounds().width / 2.0);
		txtEmp.setPosition(xposEmp, (window.getSize().y / 2.0) + 70);
		txtEmp.setFillColor(sf::Color(MOD_BUTTON_TEXT_FG));

		sf::Text titleGit("github.com/applesthepi/unnamedblocks", *Global::Font, 24);
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

	window.setFramerateLimit(200);

	BlockRegistry* pRegistry = new BlockRegistry();
	run(pRegistry);

	// Setup

	Plane::Planes = new std::vector<Plane*>();

	Plane* primaryPlane = new Plane(sf::Vector2u(0, 0), sf::Vector2u(100, 100));
	Plane::Planes->push_back(primaryPlane);

	toolbarPlane = new Plane(sf::Vector2u(0, 0), sf::Vector2u(100, 100), true);
	Plane::Planes->push_back(toolbarPlane);

	primaryPlane->SetPosition(sf::Vector2u(Global::ToolbarWidth + 10, HEADER_HEIGHT + 5));
	primaryPlane->SetSize(sf::Vector2u(window.getSize().x - primaryPlane->GetPosition().x - 5, window.getSize().y - primaryPlane->GetPosition().y - 5));

	CategoryHandler::Initialize(pRegistry, toolbarPlane);
	CategoryHandler::RegisterHeader(pRegistry, primaryPlane);

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

	sf::Text frameRate;
	frameRate.setString("fps: 0");
	frameRate.setFillColor(MOD_BUTTON_TEXT_FG);
	frameRate.setFont(*Global::Font);
	frameRate.setCharacterSize(12);

	clTrip.restart();

	while (window.isOpen())
	{
		sf::Event ev;
		while (window.pollEvent(ev))
		{
			if (ev.type == sf::Event::Closed)
			{
				std::function<void(const bool&)> cb = [&window](const bool& result)
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

				primaryPlane->SetPosition(sf::Vector2u(Global::ToolbarWidth + 10, HEADER_HEIGHT + 5));
				primaryPlane->SetSize(sf::Vector2u(window.getSize().x - primaryPlane->GetPosition().x - 5, window.getSize().y - primaryPlane->GetPosition().y - 5));
			}
			else if (ev.type == sf::Event::MouseWheelScrolled)
			{
				int32_t delta = ev.mouseWheelScroll.delta * -200;

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
				{
					primaryPlane->AddInnerPosition(sf::Vector2i(delta, 0));
					
					if (Global::Dragging)
						((Stack*)Global::DraggingStack)->AddPosition(sf::Vector2i(delta, 0));
				}
				else
				{
					primaryPlane->AddInnerPosition(sf::Vector2i(0, delta));
					
					if (Global::Dragging)
						((Stack*)Global::DraggingStack)->AddPosition(sf::Vector2i(0, delta));
				}
			}
			else if (ev.type == sf::Event::EventType::KeyPressed)
				InputHandler::FireKeyEvent(ev.key);
			else if (ev.type == sf::Event::EventType::TextEntered)
				InputHandler::FireTextEvent(ev.text);
			else if (ev.type == sf::Event::EventType::MouseButtonPressed)
			{
				if (!UIRegistry::GetRegistry().mouseButton(true, Global::MousePosition, ev.mouseButton.button))
				{
					primaryPlane->MouseButton(true, Global::MousePosition, ev.mouseButton.button);
					toolbarPlane->MouseButton(true, Global::MousePosition, ev.mouseButton.button);
				}
			}
			else if (ev.type == sf::Event::EventType::MouseButtonReleased)
			{
				if (!UIRegistry::GetRegistry().mouseButton(false, Global::MousePosition, ev.mouseButton.button))
				{
					primaryPlane->MouseButton(false, Global::MousePosition, ev.mouseButton.button);
					toolbarPlane->MouseButton(false, Global::MousePosition, ev.mouseButton.button);
				}
			}
		}

		window.clear(MOD_BACKGROUND_HIGH);
		CategoryHandler::Render(&window, toolbarPlane);


		if (window.hasFocus())
			Global::MousePosition = sf::Mouse::getPosition(window);
		else
			Global::MousePosition = sf::Vector2i();


		if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - lastVanityReload).count() > 10)
		{
			lastVanityReload = std::chrono::high_resolution_clock::now();
			primaryPlane->ReloadVanity();
		}

		UIRegistry::GetRegistry().frameUpdate(0.0);

		toolbarPlane->FrameUpdate(pRegistry);
		primaryPlane->FrameUpdate(pRegistry);

		if (Global::Dragging)
			((Stack*)Global::DraggingStack)->FrameUpdate(false);

		CategoryHandler::FrameUpdate(&window, pRegistry, toolbarPlane, primaryPlane);

		//render

		toolbarPlane->Render(&window);
		primaryPlane->Render(&window);

		//render connections
		toolbarPlane->RenderConnection(&window);
		primaryPlane->RenderConnection(&window);

		window.draw(UIRegistry::GetRegistry());

		if (Global::Dragging)
			((Stack*)Global::DraggingStack)->Render(nullptr, &window);

		// execution

		if (Global::SkipFrame)
		{
			Global::SkipFrame = false;
			continue;
		}

		sf::Time tm = cl.getElapsedTime();
		cl.restart();

		if (clTrip.getElapsedTime().asSeconds() >= 1.0f)
		{
			frameRate.setString("fps: " + std::to_string((uint64_t)floor(1.0 / (tm.asMicroseconds() * 0.000001))));
			clTrip.restart();
		}

		frameRate.setPosition(sf::Vector2f(window.getSize().x - 70, 0));

		CategoryHandler::RenderPost(&window);

		window.draw(frameRate);
		window.display();
	}

	Global::ApplicationRunning = false;
	MessageHandler::Finish();

	return 0;
}
