#include "ModLoader.h"
#include "RHR/handlers/runtime/PreProcessor.h"
#include "handlers/InputHandler.h"
#include "handlers/CategoryHandler.h"
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
#include <RHR/RHR.h>

#ifdef LINUX
#include <X11/Xlib.h>
#endif

#include <iostream>
#include <cstring>
#include <vector>
#include <math.h>
#include <string>

#define CONTEXT_COLOR 180, 180, 180, 200

std::vector<Button*> contextButtons;

ContextSystem sys;

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
	ButtonRegistry::Initialize();
	PreProcessor::Initialize();

	BlockRegistry* pRegistry = new BlockRegistry();
	run(pRegistry);

	// Setup

	Plane::Planes = new std::vector<Plane*>();

	Plane* primaryPlane = new Plane(sf::Vector2u(0, 0), sf::Vector2u(100, 100));
	Plane::Planes->push_back(primaryPlane);

	toolbarPlane = new Plane(sf::Vector2u(0, 0), sf::Vector2u(100, 100), true);
	Plane::Planes->push_back(toolbarPlane);

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
	bool wasContextOpen = false;

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
				bool result = false;

				MessageHandler::RegisterMessageSync(new Message("all unsaved progress will be lost if you continue", &result));

				if (result)
				{
					window.close();
				}
			}
			else if (ev.type == sf::Event::Resized)
			{
				sf::FloatRect visibleArea(0, 0, ev.size.width, ev.size.height);
				window.setView(sf::View(visibleArea));
			}
			else if (ev.type == sf::Event::MouseWheelScrolled)
			{
				int32_t delta = ev.mouseWheelScroll.delta * -200;

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
				{
					primaryPlane->AddInnerPosition(sf::Vector2i(delta, 0));
					std::cout << Global::Dragging << std::endl;
					if (Global::Dragging)
						((Stack*)Global::DraggingStack)->AddPosition(sf::Vector2i(delta, 0));
				}
				else
				{
					primaryPlane->AddInnerPosition(sf::Vector2i(0, delta));
					std::cout << Global::Dragging << std::endl;
					if (Global::Dragging)
						((Stack*)Global::DraggingStack)->AddPosition(sf::Vector2i(0, delta));
				}
			}
			else if (ev.type == sf::Event::EventType::KeyPressed) {
				InputHandler::FireKeyEvent(ev.key);
			}
			else if (ev.type == sf::Event::EventType::TextEntered) {
				InputHandler::FireTextEvent(ev.text);
			}
		}

		window.clear(MOD_BACKGROUND_HIGH);
		CategoryHandler::Render(&window, toolbarPlane);

		//engine frame update

		primaryPlane->SetPosition(sf::Vector2u(Global::ToolbarWidth + 10, HEADER_HEIGHT + 5));
		primaryPlane->SetSize(sf::Vector2u(window.getSize().x - primaryPlane->GetPosition().x - 5, window.getSize().y - primaryPlane->GetPosition().y - 5));

		//frame update

		if (window.hasFocus())
		{
			Global::MousePosition = sf::Mouse::getPosition(window);

			ButtonRegistry::FrameUpdateUI(&window);

			if (!wasDownLeft && sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				wasDownLeft = true;

				if (!ButtonRegistry::MouseUpdateButtons(sf::Mouse::Left, true))
				{
					primaryPlane->MouseButton(true, Global::MousePosition, sf::Mouse::Left);
					toolbarPlane->MouseButton(true, Global::MousePosition, sf::Mouse::Left);
				}
			}
			else if (wasDownLeft && !sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				wasDownLeft = false;

				if (!ButtonRegistry::MouseUpdateButtons(sf::Mouse::Left, false))
				{
					primaryPlane->MouseButton(false, Global::MousePosition, sf::Mouse::Left);
					toolbarPlane->MouseButton(false, Global::MousePosition, sf::Mouse::Left);
				}
			}

			if (!wasDownMiddle && sf::Mouse::isButtonPressed(sf::Mouse::Middle))
			{
				wasDownMiddle = true;

				if (!ButtonRegistry::MouseUpdateButtons(sf::Mouse::Middle, true))
				{
					primaryPlane->MouseButton(true, Global::MousePosition, sf::Mouse::Middle);
					toolbarPlane->MouseButton(true, Global::MousePosition, sf::Mouse::Middle);
				}
			}
			else if (wasDownMiddle && !sf::Mouse::isButtonPressed(sf::Mouse::Middle))
			{
				wasDownMiddle = false;

				if (!ButtonRegistry::MouseUpdateButtons(sf::Mouse::Middle, false))
				{
					primaryPlane->MouseButton(false, Global::MousePosition, sf::Mouse::Middle);
					toolbarPlane->MouseButton(false, Global::MousePosition, sf::Mouse::Middle);
				}
			}

			if (!wasDownRight && sf::Mouse::isButtonPressed(sf::Mouse::Right))
			{
				wasDownRight = true;

				if (!ButtonRegistry::MouseUpdateButtons(sf::Mouse::Right, true))
				{
					primaryPlane->MouseButton(true, Global::MousePosition, sf::Mouse::Right);
					toolbarPlane->MouseButton(true, Global::MousePosition, sf::Mouse::Right);
				}
			}
			else if (wasDownRight && !sf::Mouse::isButtonPressed(sf::Mouse::Right))
			{
				wasDownRight = false;

				if (!ButtonRegistry::MouseUpdateButtons(sf::Mouse::Right, false))
				{
					primaryPlane->MouseButton(false, Global::MousePosition, sf::Mouse::Right);
					toolbarPlane->MouseButton(false, Global::MousePosition, sf::Mouse::Right);
				}
			}
		}

		if ((Global::ContextActive && !wasContextOpen) || Global::ContextUpdate)
		{
			Global::ContextUpdate = false;
			wasContextOpen = true;

			if (Global::Context.Position != sys.Position || Global::Context.Type != sys.Type)
			{
				sys = Global::Context;

				for (unsigned int i = 0; i < contextButtons.size(); i++)
				{
					ButtonRegistry::RemoveButton(contextButtons[i]);
					delete contextButtons[i];
				}

				contextButtons.clear();

				if (sys.Type == ContextType::BLOCK)
				{
					std::function<void()>* callback0 = new std::function<void()>();
					*callback0 = []()
					{
						(*Global::Context.Callback)(0);
					};

					std::function<void()>* callback1 = new std::function<void()>();
					*callback1 = []()
					{
						(*Global::Context.Callback)(1);
					};

					std::function<void()>* callback2 = new std::function<void()>();
					*callback2 = []()
					{
						(*Global::Context.Callback)(2);
					};

					std::function<void()>* callback3 = new std::function<void()>();
					*callback3 = []()
					{
						(*Global::Context.Callback)(3);
					};

					contextButtons.push_back(new Button(sf::Vector2i(sys.Position.x, sys.Position.y), sf::Vector2u(300, 16), callback0));
					contextButtons.push_back(new Button(sf::Vector2i(sys.Position.x, sys.Position.y + (1 * 16)), sf::Vector2u(300, 16), callback1));
					contextButtons.push_back(new Button(sf::Vector2i(sys.Position.x, sys.Position.y + (2 * 16)), sf::Vector2u(300, 16), callback2));
					contextButtons.push_back(new Button(sf::Vector2i(sys.Position.x, sys.Position.y + (3 * 16)), sf::Vector2u(300, 16), callback3));

					contextButtons[0]->SetButtonModeText("duplicate stack", sf::Color(CONTEXT_COLOR), sf::Color::Black, 12);
					contextButtons[1]->SetButtonModeText("delete stack", sf::Color(CONTEXT_COLOR), sf::Color::Black, 12);
					contextButtons[2]->SetButtonModeText("duplicate block", sf::Color(CONTEXT_COLOR), sf::Color::Black, 12);
					contextButtons[3]->SetButtonModeText("delete block", sf::Color(CONTEXT_COLOR), sf::Color::Black, 12);

					ButtonRegistry::AddButton(contextButtons[0]);
					ButtonRegistry::AddButton(contextButtons[1]);
					ButtonRegistry::AddButton(contextButtons[2]);
					ButtonRegistry::AddButton(contextButtons[3]);
				}
				else if (sys.Type == ContextType::NONE)
				{
					Global::ContextActive = false;
				}
			}
		}
		else if (!Global::ContextActive && wasContextOpen)
		{
			wasContextOpen = false;

			for (unsigned int i = 0; i < contextButtons.size(); i++)
			{
				ButtonRegistry::RemoveButton(contextButtons[i]);
				delete contextButtons[i];
			}

			contextButtons.clear();
		}

		if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - lastVanityReload).count() > 10)
		{
			lastVanityReload = std::chrono::high_resolution_clock::now();
			primaryPlane->ReloadVanity();
		}

		toolbarPlane->FrameUpdate();
		primaryPlane->FrameUpdate();

		if (Global::Dragging)
			((Stack*)Global::DraggingStack)->FrameUpdate(false);

		CategoryHandler::FrameUpdate(pRegistry, toolbarPlane);

		//messages
		MessageHandler::RunSyncMessages();

		//render

		toolbarPlane->Render(&window);
		primaryPlane->Render(&window);

		//render connections
		toolbarPlane->RenderConnection(&window);
		primaryPlane->RenderConnection(&window);

		if (Global::Dragging)
			((Stack*)Global::DraggingStack)->Render(nullptr, &window);

		ButtonRegistry::RenderUI(&window);

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

	return 0;
}
