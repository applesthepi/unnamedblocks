#include "ModLoader.h"
#include "RHR/handlers/runtime/PreProcessor.h"


//#define POSIX
// TODO fix config on windows
#ifdef POSIX
#include "config.h"
#include <dlfcn.h>
#else
#include "RHR/config.h"
#include <windows.h>
#endif

#include <libtcc.h>
#include <RHR/RHR.h>
#include <SFML/Graphics.hpp>
#include <Espresso/Logger.h>
#include <chrono>
#include <boost/filesystem.hpp>

//#include "content/ContentLoader.h"
#include <GL/glew.h>
//#include <stdio.h> 
//typedef int(__cdecl* MYPROC)(LPWSTR);
#include <RHR/RHR.h>

#ifdef POSIX
#include <X11/Xlib.h>
#endif

#include <iostream>
#include <cstring>
#include <vector>
#include <math.h>

#define UB_VERSION "Unnamed Blocks v0.0b2"
#define UB_BETA_BUILD true

#define CONTEXT_COLOR 180, 180, 180, 200

std::vector<Button*> contextButtons;
std::vector<Button*> toolbarButtons;
std::vector<Button*> catButtons;

ContextSystem sys;

static Plane* toolbarPlane;
static unsigned char toolbarCatagory = 0;
static unsigned short toolbarStackCount = 0;

static std::atomic<bool> returnFinished;

void ReturnFinished()
{
	returnFinished = true;
}

static void ReloadCatagory(uint16_t index, BlockRegistry* registry)
{
	toolbarPlane->DeleteAllBlocks();

	unsigned int idx = 0;
	unsigned int widest = 0;

	for (unsigned int a = 0; a < registry->GetBlocks().size(); a++)
	{
		if (registry->GetBlocks()[a]->GetCategory() == registry->GetCategories()[index]->GetUnlocalizedName())
		{
			Stack* stack = new Stack(sf::Vector2i(5, 5 + (idx * (Global::BlockHeight + 5))), registry);
			Block* block = new Block(registry->GetBlocks()[a]->GetUnlocalizedName(), registry, stack->GetFunctionUpdate(), stack->GetFunctionSelect());

			toolbarPlane->AddStack(stack);
			stack->AddBlock(block);

			stack->ReRender();
			stack->ReloadVanity();

			if (block->GetWidth() > widest)
			{
				widest = block->GetWidth();
			}

			idx++;
		}
	}

	if (widest == 0)
		Global::ToolbarWidth = 100;
	else
		Global::ToolbarWidth = widest + 10;

	toolbarCatagory = index;
	toolbarStackCount = toolbarPlane->GetStackCount();
}

int main()
{
	/*
	typedef void(*f_initialize)(int*);
	system("tcc comp.c -shared -o comp.so");
	
	int* re = new int();
	*re = 5;

	void* so = dlopen("./comp.so", RTLD_NOW);
	if(so == nullptr) {
		printf("%s\n", dlerror());
		static_assert("killme");
	}
	f_initialize initialize = (f_initialize)dlsym(so, "re");
	initialize(re);
	
	Logger::Debug(std::to_string(*re));

	system("pause");

	return 0;
	*/

	/*
	typedef void(*f_initialize)(int*);
	system("call compile.bat");

	int* re = new int();
	*re = 5;

	HINSTANCE dll = LoadLibrary("./comp.dll");
	if (dll == nullptr) {
		Logger::Error("REEE 1");
		system("pause");
		return 0;
	}
	f_initialize initialize = (f_initialize)GetProcAddress(dll, "re");
	if (initialize == nullptr) {
		Logger::Error("REEE 2");
		system("pause");
		return 0;
	}

	initialize(re);

	Logger::Debug(std::to_string(*re));

	system("pause");
	return 0;
	*/


	Logger::Info(UB_VERSION);
	if (UB_BETA_BUILD)
		Logger::Warn("this is a beta build! There is likely tons of bugs and some critical bugs. Please be careful and save often. Report any issues to the github page https://github.com/applesthepi/unnamedblocks");

#ifdef LINUX
	// TODO: Proper wayland support
	XInitThreads();
#endif
	Logger::Info("all unsaved progress will be lost if this window is closed");

	ShaderRegistry::Initialize();

	// Window Setup

	sf::RenderWindow window;
	window.create(sf::VideoMode(1280, 720, 32), UB_VERSION, sf::Style::Default);
	/*
	const char* gpuVendor = (const char*)glGetString(GL_VENDOR);
	Logger::Debug("gpu vendor \"" + std::string(gpuVendor) + "\"");

	if (strcmp(gpuVendor, "Intel Open Source Technology Center") == 0)
	{
		Logger::Info("loading intel shaders");
		Global::GpuType = GpuMan::INTEL;
	}
	else if (strcmp(gpuVendor, "NVIDIA Corporation") == 0)
	{
		Logger::Info("loading nvidia shaders");
		Global::GpuType = GpuMan::NVIDIA;
	}
	else
		Logger::Warn("gpu not detected; using default shaders");
	*/
	ShaderRegistry::ReloadAllShaders();
	// Default Settings

	window.setVerticalSyncEnabled(false);
	window.setFramerateLimit(250);

	// Initialization

	Global::LoadDefaults();//must be first
	MessageHandler::Initialize();
	TypingSystem::Initialization();
	ButtonRegistry::Initialize();
	BlockRegistry* pRegistry;

	pRegistry = new BlockRegistry();

	run(pRegistry);

	Plane::Planes = new std::vector<Plane*>();

	// Setup

	//ContentLoader::LoadContent();

	Plane* primaryPlane = new Plane(sf::Vector2u(110, 16 + 10), sf::Vector2u(800, 500));
	Plane::Planes->push_back(primaryPlane);

	uint16_t useCount = 0;

	for (unsigned int i = 0; i < pRegistry->GetCategories().size(); i++)
	{
		if (pRegistry->GetCategories()[i]->GetDisplayName() == "")
			continue;

		std::function<void()>* callback = new std::function<void()>();
		*callback = [i, &pRegistry]()
		{
			if (Global::Dragging)
				return;

			ReloadCatagory(i, pRegistry);
		};

		Button* cat = new Button(sf::Vector2i(5, 5 + (useCount * (16 + 5))), sf::Vector2u(100, 16), callback);
		cat->SetButtonModeText(pRegistry->GetCategories()[i]->GetDisplayName(), pRegistry->GetCategories()[i]->GetColor(), 12);

		catButtons.push_back(cat);
		ButtonRegistry::AddButton(cat);

		useCount++;
	}
	{
		std::function<void()>* function = new std::function<void()>();
		*function = [primaryPlane]()
		{
			bool* result = new bool(false);

			MessageHandler::RegisterMessageSync(new Message("all unsaved progress will be lost if you continue", result));

			if (*result)
			{
				primaryPlane->DeleteAllBlocks();
			}
		};

		Button* button = new Button(sf::Vector2i(Global::ToolbarWidth + (105 * 0) + 10, 5), sf::Vector2u(100, 16), function);
		button->SetButtonModeText("new", sf::Color(200, 200, 200), 12);

		toolbarButtons.push_back(button);
		ButtonRegistry::AddButton(button);
	}
	{
		std::function<void()>* function = new std::function<void()>();
		*function = [primaryPlane, &pRegistry]()
		{
			std::string* result = new std::string();

			MessageHandler::RegisterMessageSync(new Message("load file path:", result));
			if (*result == "")
			{
				delete result;
				return;
			}
			*result += ".ub";

			Logger::Info("loading project from \"" + *result + "\"");
			ProjectHandler::LoadProject(*result, primaryPlane, pRegistry);
			ProjectHandler::CurrentPath = *result;

			delete result;
		};

		Button* button = new Button(sf::Vector2i(Global::ToolbarWidth + (105 * 1) + 10, 5), sf::Vector2u(100, 16), function);
		button->SetButtonModeText("open", sf::Color(200, 200, 200), 12);

		toolbarButtons.push_back(button);
		ButtonRegistry::AddButton(button);
	}
	{
		std::function<void()>* function = new std::function<void()>();
		*function = [primaryPlane]()
		{
			if (ProjectHandler::CurrentPath == "")
			{
				std::string* result = new std::string();

				MessageHandler::RegisterMessageSync(new Message("save file path:", result));
				if (*result == "")
				{
					delete result;
					return;
				}
				*result += ".ub";

				Logger::Info("saveas to \"" + *result + "\"");
				ProjectHandler::SaveProject(*result, primaryPlane);
				ProjectHandler::CurrentPath = *result;

				delete result;
			}
			else
			{
				ProjectHandler::SaveProject(ProjectHandler::CurrentPath, primaryPlane);
				Logger::Info("save to \"" + ProjectHandler::CurrentPath + "\"");
			}
		};

		Button* button = new Button(sf::Vector2i(Global::ToolbarWidth + (105 * 2) + 10, 5), sf::Vector2u(100, 16), function);
		button->SetButtonModeText("save", sf::Color(200, 200, 200), 12);

		toolbarButtons.push_back(button);
		ButtonRegistry::AddButton(button);
	}
	{
	std::function<void()>* function = new std::function<void()>();
	*function = [primaryPlane]()
	{
		std::string* result = new std::string();

		MessageHandler::RegisterMessageSync(new Message("save file path:", result));
		if (*result == "")
		{
			delete result;
			return;
		}
		*result += ".ub";

		Logger::Info("saveas to \"" + *result + "\"");
		ProjectHandler::SaveProject(*result, primaryPlane);
		ProjectHandler::CurrentPath = *result;

		delete result;
	};

	Button* button = new Button(sf::Vector2i(Global::ToolbarWidth + (105 * 3) + 10, 5), sf::Vector2u(100, 16), function);
	button->SetButtonModeText("saveas", sf::Color(200, 200, 200), 12);

	toolbarButtons.push_back(button);
	ButtonRegistry::AddButton(button);
	}
	{
		std::function<void()>* function = new std::function<void()>();
		*function = [&primaryPlane, &pRegistry]()
		{
			if (ProjectHandler::CurrentPath == "")
				Logger::Info("running unsaved project");
			else
				Logger::Info("running \"" + ProjectHandler::CurrentPath + "\"");

			Plane* planeCopy = new Plane(sf::Vector2u(0, 0), sf::Vector2u(0, 0));
			planeCopy->CopyEverything(primaryPlane, pRegistry);

			PreProcessor::Cleanup();

			std::vector<uint64_t> units;

			for (uint64_t i = 0; i < planeCopy->GetStackCount(); i++)
				units.push_back(PreProcessor::InitializeTranslationUnit(planeCopy->GetStack(i), pRegistry));

			bool waiting = true;
			bool error = false;

			while (waiting)
			{
				Logger::Debug("...waiting for translation units to build...");
				std::this_thread::sleep_for(std::chrono::milliseconds(100));

				for (uint64_t i = 0; i < units.size(); i++)
				{
					if (PreProcessor::GetTranslationUnitStatus(units[i]) == PreProcessorTranslationUnitStatus::DONE)
					{
						waiting = false;
						break;
					}
					else if (PreProcessor::GetTranslationUnitStatus(units[i]) == PreProcessorTranslationUnitStatus::DONE_ERROR)
					{
						Logger::Error("there was an error building the transcript units");
						waiting = false;
						error = true;
						break;
					}
				}
			}

			if (error)
				return;

			Logger::Debug("...completed translation unit build, building...");

			PreProcessor::BeginBuild(true);

			waiting = true;
			error = false;

			while (waiting)
			{
				Logger::Debug("...waiting for final build...");
				std::this_thread::sleep_for(std::chrono::milliseconds(100));

				if (PreProcessor::GetStatus() == PreProcessorStatus::DONE)
				{
					waiting = false;
					continue;
				}
				else if (PreProcessor::GetStatus() == PreProcessorStatus::DONE_ERROR)
				{
					Logger::Error("there was an error building");
					waiting = false;
					error = true;
					continue;
				}
			}

			if (error)
				return;

			Logger::Debug("...completed build, compiling...");

			PreProcessor::Start();
		};

		Button* button = new Button(sf::Vector2i(Global::ToolbarWidth + (105 * 4) + 10, 5), sf::Vector2u(100, 16), function);
		button->SetButtonModeText("run", sf::Color(200, 200, 200), 12);

		toolbarButtons.push_back(button);
		ButtonRegistry::AddButton(button);
	}

	toolbarPlane = new Plane(sf::Vector2u(5, (catButtons.size() * (16 + 5)) + 5), sf::Vector2u(Global::ToolbarWidth, (window.getSize().y - ((catButtons.size() * (16 + 5)) + 5)) - 5), true);
	Plane::Planes->push_back(toolbarPlane);
	{
		int16_t useCat = 0;
		for (uint16_t i = 0; i < pRegistry->GetCategories().size(); i++)
		{
			if (pRegistry->GetCategories()[i]->GetDisplayName() != "")
			{
				useCat = i;
				break;
			}
		}

		toolbarCatagory = useCat;

		if (useCat != -1)
		{
			unsigned int idx = 0;

			for (unsigned int i = 0; i < pRegistry->GetBlocks().size(); i++)
			{
				if (pRegistry->GetBlocks()[i]->GetCategory() == pRegistry->GetCategories()[useCat]->GetUnlocalizedName())
				{
					Stack* stack = new Stack(sf::Vector2i(5, 5 + (idx * (Global::BlockHeight + 5))), pRegistry);
					Block* block = new Block(pRegistry->GetBlocks()[i]->GetUnlocalizedName(), pRegistry, stack->GetFunctionUpdate(), stack->GetFunctionSelect());

					toolbarPlane->AddStack(stack);
					stack->AddBlock(block);

					stack->ReloadVanity();
					stack->ReRender();

					idx++;
				}
			}
		}
	}

#ifdef POSIX
	std::chrono::time_point<std::chrono::system_clock> lastVanityReload = std::chrono::high_resolution_clock::now();
#else
	std::chrono::time_point<std::chrono::steady_clock> lastVanityReload = std::chrono::high_resolution_clock::now();
#endif

	bool wasDownLeft = false;
	bool wasDownMiddle = false;
	bool wasDownRight = false;
	bool wasContextOpen = false;

	//sf::View zoomedView(sf::FloatRect(0, 0, primaryPlane->GetSize().x, primaryPlane->GetSize().y));

	//Global::ZoomAspect = (sf::Vector2f)primaryPlane->GetSize();

	sf::Clock cl;
	sf::Clock clTrip;

	sf::Text frameRate;
	frameRate.setString("fps: 0");
	frameRate.setFillColor(sf::Color(128, 128, 128, 255));
	frameRate.setFont(*Global::Font);
	frameRate.setCharacterSize(18);

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
				//float aspect = (float)primaryPlane->GetSize().y / (float)primaryPlane->GetSize().x;

				//Global::ZoomAspect.x += ev.mouseWheelScroll.delta * 50;
				//Global::ZoomAspect.y += ev.mouseWheelScroll.delta * 50 * aspect;

				//zoomedView.setSize(sf::Vector2f(Global::ZoomAspect.x, Global::ZoomAspect.y));
			}
		}

		window.clear();

		//engine frame update

		toolbarPlane->SetSize(sf::Vector2u(Global::ToolbarWidth, (window.getSize().y - ((catButtons.size() * (16 + 5)) + 5)) - 5));
		primaryPlane->SetPosition(sf::Vector2u(Global::ToolbarWidth + 10, 16 + 10));
		primaryPlane->SetSize(sf::Vector2u(window.getSize().x - primaryPlane->GetPosition().x - 5, window.getSize().y - primaryPlane->GetPosition().y - 5));

		//frame update

		if (window.hasFocus())
		{
			Global::MousePosition = sf::Mouse::getPosition(window);

			TypingSystem::Update();
			ButtonRegistry::FrameUpdateUI(&window);

			for (unsigned int i = 0; i < catButtons.size(); i++)
			{
				catButtons[i]->SetSize(sf::Vector2u(Global::ToolbarWidth, 16));
			}

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

			for (unsigned int i = 0; i < toolbarButtons.size(); i++)
			{
				toolbarButtons[i]->SetPosition(sf::Vector2i(Global::ToolbarWidth + (105 * i) + 10, 5));
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
					*callback0 = [&pRegistry]()
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

					contextButtons[0]->SetButtonModeText("duplicate stack", sf::Color(CONTEXT_COLOR), 12);
					contextButtons[1]->SetButtonModeText("delete stack", sf::Color(CONTEXT_COLOR), 12);
					contextButtons[2]->SetButtonModeText("duplicate block", sf::Color(CONTEXT_COLOR), 12);
					contextButtons[3]->SetButtonModeText("delete block", sf::Color(CONTEXT_COLOR), 12);

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

		if (toolbarPlane->GetStackCount() != toolbarStackCount)
			ReloadCatagory(toolbarCatagory, pRegistry);

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

		frameRate.setPosition(sf::Vector2f(window.getSize().x - 200, 0));
		window.draw(frameRate);
		window.display();
	}

	Global::ApplicationRunning = false;

	return 0;
}
