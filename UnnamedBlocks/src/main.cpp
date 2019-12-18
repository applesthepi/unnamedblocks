#include "stacking/Plane.h"

#include "registries/ShaderRegistry.h"
#include "registries/BlockRegistry.h"
#include "content/ContentLoader.h"

#include "ui/Button.h"
#include "ui/TypingSystem.h"

#include "Global.h"

#include "handlers/MessageHandler.h"
#include "handlers/ProjectHandler.h"
#include "handlers/Logger.h"
#include "handlers/runtime/RuntimeHandler.h"

#include <iostream>
#include <GL/gl.h>
#include <cstring>

#ifdef LINUX
#include <X11/Xlib.h>
#endif
std::vector<Button*> catagoryButtons;
std::vector<Button*> contextButtons;
ContextSystem sys;

Button* buttonNew;
Button* buttonOpen;
Button* buttonSave;
Button* buttonSaveAs;
Button* buttonRun;

static void FrameUpdateUI(sf::RenderWindow* window)
{
	for (unsigned int i = 0; i < catagoryButtons.size(); i++)
	{
		catagoryButtons[i]->FrameUpdate(window);
	}

	if (Global::ContextActive)
	{
		for (unsigned int i = 0; i < contextButtons.size(); i++)
		{
			contextButtons[i]->FrameUpdate(window);
		}
	}

	buttonNew->SetPosition(sf::Vector2i(Global::ToolbarWidth + (105 * 0) + 10, 5));
	buttonOpen->SetPosition(sf::Vector2i(Global::ToolbarWidth + (105 * 1) + 10, 5));
	buttonSave->SetPosition(sf::Vector2i(Global::ToolbarWidth + (105 * 2) + 10, 5));
	buttonSaveAs->SetPosition(sf::Vector2i(Global::ToolbarWidth + (105 * 3) + 10, 5));
	buttonRun->SetPosition(sf::Vector2i(Global::ToolbarWidth + (105 * 4) + 10, 5));

	buttonNew->FrameUpdate(window);
	buttonOpen->FrameUpdate(window);
	buttonSave->FrameUpdate(window);
	buttonSaveAs->FrameUpdate(window);
	buttonRun->FrameUpdate(window);
}

static void RenderUI(sf::RenderWindow* window)
{
	for (unsigned int i = 0; i < catagoryButtons.size(); i++)
	{
		catagoryButtons[i]->Render(window);
	}

	if (Global::ContextActive)
	{
		for (unsigned int i = 0; i < contextButtons.size(); i++)
		{
			contextButtons[i]->Render(window);
		}
	}

	buttonNew->Render(window);
	buttonOpen->Render(window);
	buttonSave->Render(window);
	buttonSaveAs->Render(window);
	buttonRun->Render(window);
}

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

	window.setVerticalSyncEnabled(true);

	// Initialization

	Global::LoadDefaults();//must be first

	BlockRegistry::Initialize();
	MessageHandler::Initialize();
	TypingSystem::Initialization();

	Plane::Planes = new std::vector<Plane*>();

	// Setup

	ContentLoader::LoadContent();

	Plane* primaryPlane = new Plane(sf::Vector2u(110, 16 + 10), sf::Vector2u(800, 500));
	Plane::Planes->push_back(primaryPlane);

	catagoryButtons = std::vector<Button*>();

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

		catagoryButtons.push_back(cat);
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

		buttonNew = new Button(sf::Vector2i(Global::ToolbarWidth + (105 * 0) + 10, 5), sf::Vector2u(100, 16), function);
		buttonNew->SetButtonModeText("new", sf::Color(200, 200, 200), 12);
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

		buttonOpen = new Button(sf::Vector2i(Global::ToolbarWidth + (105 * 1) + 10, 5), sf::Vector2u(100, 16), function);
		buttonOpen->SetButtonModeText("open", sf::Color(200, 200, 200), 12);
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

		buttonSave = new Button(sf::Vector2i(Global::ToolbarWidth + (105 * 2) + 10, 5), sf::Vector2u(100, 16), function);
		buttonSave->SetButtonModeText("save", sf::Color(200, 200, 200), 12);
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

		buttonSaveAs = new Button(sf::Vector2i(Global::ToolbarWidth + (105 * 3) + 10, 5), sf::Vector2u(100, 16), function);
		buttonSaveAs->SetButtonModeText("saveas", sf::Color(200, 200, 200), 12);
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

		buttonRun = new Button(sf::Vector2i(Global::ToolbarWidth + (105 * 4) + 10, 5), sf::Vector2u(100, 16), function);
		buttonRun->SetButtonModeText("run", sf::Color(200, 200, 200), 12);
	}

	toolbarPlane = new Plane(sf::Vector2u(5, (catagoryButtons.size() * (16 + 5)) + 5), sf::Vector2u(Global::ToolbarWidth, (window.getSize().y - ((catagoryButtons.size() * (16 + 5)) + 5)) - 5), true);
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
	
		toolbarPlane->SetSize(sf::Vector2u(Global::ToolbarWidth, (window.getSize().y - ((catagoryButtons.size() * (16 + 5)) + 5)) - 5));
		primaryPlane->SetPosition(sf::Vector2u(Global::ToolbarWidth + 10, 16 + 10));
		primaryPlane->SetSize(sf::Vector2u(window.getSize().x - primaryPlane->GetPosition().x - 5, window.getSize().y - primaryPlane->GetPosition().y - 5));
		
		//frame update

		if (window.hasFocus())
		{
			TypingSystem::Update();
			FrameUpdateUI(&window);

			for (unsigned int i = 0; i < catagoryButtons.size(); i++)
			{
				catagoryButtons[i]->SetSize(sf::Vector2u(Global::ToolbarWidth, 16));
			}

			if (!wasDownLeft && sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				wasDownLeft = true;
				primaryPlane->MouseButton(true, sf::Mouse::getPosition(window), sf::Mouse::Left);
				toolbarPlane->MouseButton(true, sf::Mouse::getPosition(window), sf::Mouse::Left);
			}
			else if (wasDownLeft && !sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				wasDownLeft = false;
				primaryPlane->MouseButton(false, sf::Mouse::getPosition(window), sf::Mouse::Left);
				toolbarPlane->MouseButton(false, sf::Mouse::getPosition(window), sf::Mouse::Left);
			}

			if (!wasDownMiddle && sf::Mouse::isButtonPressed(sf::Mouse::Middle))
			{
				wasDownMiddle = true;
				primaryPlane->MouseButton(true, sf::Mouse::getPosition(window), sf::Mouse::Middle);
				toolbarPlane->MouseButton(true, sf::Mouse::getPosition(window), sf::Mouse::Middle);
			}
			else if (wasDownMiddle && !sf::Mouse::isButtonPressed(sf::Mouse::Middle))
			{
				wasDownMiddle = false;
				primaryPlane->MouseButton(false, sf::Mouse::getPosition(window), sf::Mouse::Middle);
				toolbarPlane->MouseButton(false, sf::Mouse::getPosition(window), sf::Mouse::Middle);
			}

			if (!wasDownRight && sf::Mouse::isButtonPressed(sf::Mouse::Right))
			{
				wasDownRight = true;
				primaryPlane->MouseButton(true, sf::Mouse::getPosition(window), sf::Mouse::Right);
				toolbarPlane->MouseButton(true, sf::Mouse::getPosition(window), sf::Mouse::Right);
			}
			else if (wasDownRight && !sf::Mouse::isButtonPressed(sf::Mouse::Right))
			{
				wasDownRight = false;
				primaryPlane->MouseButton(false, sf::Mouse::getPosition(window), sf::Mouse::Right);
				toolbarPlane->MouseButton(false, sf::Mouse::getPosition(window), sf::Mouse::Right);
			}
		}

		if (Global::ContextActive)
		{
			if (Global::Context.Position != sys.Position || Global::Context.Type != sys.Type)
			{
				sys = Global::Context;
				for (unsigned int i = 0; i < contextButtons.size(); i++)
					delete contextButtons[i];

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
					
					contextButtons.push_back(new Button(sf::Vector2i(sys.Position.x, sys.Position.y), sf::Vector2u(300, 16), callback0));
					contextButtons.push_back(new Button(sf::Vector2i(sys.Position.x, sys.Position.y + (1 * 16)), sf::Vector2u(300, 16), callback1));

					contextButtons[0]->SetButtonModeText("duplicate", sf::Color(70, 70, 70), 12);
					contextButtons[1]->SetButtonModeText("delete", sf::Color(70, 70, 70), 12);
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

		RenderUI(&window);
		
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
