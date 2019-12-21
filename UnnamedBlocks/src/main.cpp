#include <iostream>
#include <cstring>

#include <RHR/RHR.h>
//#include <SFML/Graphics.hpp>

//#include "content/ContentLoader.h"
#include <GL/glew.h>
//#include <stdio.h> 
//typedef int(__cdecl* MYPROC)(LPWSTR);

#include "ModLoader.h"

#ifdef LINUX
#include <X11/Xlib.h>
#endif

std::vector<Button*> contextButtons;
std::vector<Button*> toolbarButtons;
std::vector<Button*> catButtons;

ContextSystem sys;

static Plane* toolbarPlane;
static unsigned char toolbarCatagory = 0;
static unsigned short toolbarStackCount = 0;

static void ReloadCatagory(unsigned index)
{
	toolbarPlane->DeleteAllBlocks();

	unsigned int idx = 0;
	unsigned int widest = 0;

	for (unsigned int a = 0; a < BlockRegistry::GetBlocks()->size(); a++)
	{
		if ((*BlockRegistry::GetBlocks())[a].Catagory == (*BlockRegistry::GetCatagories())[index].UnlocalizedName)
		{
			Stack* stack = new Stack(sf::Vector2i(5, 5 + (idx * (Global::BlockHeight + 5))));
			Block* block = new Block((*BlockRegistry::GetBlocks())[a].UnlocalizedName);

			toolbarPlane->AddStack(stack);
			stack->AddBlock(block);

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
	run();
	
#ifdef LINUX
	XInitThreads();
	Logger::Info("Linux build");
#endif
	Logger::Info("all unsaved progress will be lost if this window is closed");
	
	ShaderRegistry::Initialize();

	// Window Setup
	
	sf::RenderWindow window;
	window.create(sf::VideoMode(1280, 720, 32), "Unnamed Blocks", sf::Style::Default);

	const char* gpuVendor = (const char*)glGetString(GL_VENDOR);

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
	
	ShaderRegistry::ReloadAllShaders();

	// Default Settings

	window.setVerticalSyncEnabled(false);
	window.setFramerateLimit(150);

	// Initialization

	Global::LoadDefaults();//must be first

	BlockRegistry::Initialize();
	MessageHandler::Initialize();
	TypingSystem::Initialization();
	ButtonRegistry::Initialize();

	Plane::Planes = new std::vector<Plane*>();

	// Setup

	//ContentLoader::LoadContent();

	Plane* primaryPlane = new Plane(sf::Vector2u(110, 16 + 10), sf::Vector2u(800, 500));
	Plane::Planes->push_back(primaryPlane);

	for (unsigned int i = 0; i < BlockRegistry::GetCatagories()->size(); i++)
	{
		std::function<void()>* callback = new std::function<void()>();
		*callback = [i]()
		{
			if (Global::Dragging)
				return;

			ReloadCatagory(i);
		};

		Button* cat = new Button(sf::Vector2i(5, 5 + (i * (16 + 5))), sf::Vector2u(100, 16), callback);
		cat->SetButtonModeText((*BlockRegistry::GetCatagories())[i].DisplayName, (*BlockRegistry::GetCatagories())[i].Color, 12);

		catButtons.push_back(cat);
		ButtonRegistry::AddButton(cat);
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
		*function = [primaryPlane]()
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
			ProjectHandler::LoadProject(*result, primaryPlane);
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
		*function = [primaryPlane]()
		{
			if (ProjectHandler::CurrentPath == "")
				Logger::Info("running unsaved project");
			else
				Logger::Info("running \"" + ProjectHandler::CurrentPath + "\"");

			Plane* planeCopy = new Plane(sf::Vector2u(0, 0), sf::Vector2u(0, 0));
			planeCopy->CopyEverything(primaryPlane);

			RuntimeHandler::Run(planeCopy);
		};

		Button* button = new Button(sf::Vector2i(Global::ToolbarWidth + (105 * 4) + 10, 5), sf::Vector2u(100, 16), function);
		button->SetButtonModeText("run", sf::Color(200, 200, 200), 12);

		toolbarButtons.push_back(button);
		ButtonRegistry::AddButton(button);
	}

	toolbarPlane = new Plane(sf::Vector2u(5, (catButtons.size() * (16 + 5)) + 5), sf::Vector2u(Global::ToolbarWidth, (window.getSize().y - ((catButtons.size() * (16 + 5)) + 5)) - 5), true);
	Plane::Planes->push_back(toolbarPlane);

	{
		unsigned int idx = 0;

		for (unsigned int i = 0; i < BlockRegistry::GetBlocks()->size(); i++)
		{
			if ((*BlockRegistry::GetBlocks())[i].Catagory == (*BlockRegistry::GetCatagories())[0].UnlocalizedName)
			{
				Stack* stack = new Stack(sf::Vector2i(5, 5 + (idx * (Global::BlockHeight + 5))));
				Block* block = new Block((*BlockRegistry::GetBlocks())[i].UnlocalizedName);

				toolbarPlane->AddStack(stack);
				stack->AddBlock(block);

				idx++;
			}
		}
	}

	bool wasDownLeft = false;
	bool wasDownMiddle = false;
	bool wasDownRight = false;

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

		if (Global::ContextActive)
		{
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

					contextButtons[0]->SetButtonModeText("duplicate stack", sf::Color(70, 70, 70), 12);
					contextButtons[1]->SetButtonModeText("delete stack", sf::Color(70, 70, 70), 12);
					contextButtons[2]->SetButtonModeText("duplicate block", sf::Color(70, 70, 70), 12);
					contextButtons[3]->SetButtonModeText("delete block", sf::Color(70, 70, 70), 12);

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

		toolbarPlane->FrameUpdate(&window);
		primaryPlane->FrameUpdate(&window);

		if (Global::Dragging)
			((Stack*)Global::DraggingStack)->FrameUpdate(&window);
		
		if (toolbarPlane->GetStackCount() != toolbarStackCount)
			ReloadCatagory(toolbarCatagory);

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
		
		if (Global::SkipFrame)
		{
			Global::SkipFrame = false;
			continue;
		}

		window.display();
	}

	Global::ApplicationRunning = false;

	return 0;
}
