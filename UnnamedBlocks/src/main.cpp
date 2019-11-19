#include "stacking/Plane.h"
#include "registries/BlockRegistry.h"
#include "Global.h"

#include <iostream>

int main()
{
	// Initialization

	BlockRegistry::Initialize();
	Global::LoadDefaults();

	// Setup
	
	{
		std::vector<BlockArgument> argList = std::vector<BlockArgument>();

		BlockArgument arg0 = BlockArgument();
		BlockArgument arg1 = BlockArgument();

		arg0.SetupTEXT("new");
		arg1.SetupSTRING("test idk reee");

		argList.push_back(arg0);
		argList.push_back(arg1);

		BlockRegistry::CreateBlock("block_new", argList);
	}

	{
		std::vector<BlockArgument> argList = std::vector<BlockArgument>();

		BlockArgument arg0 = BlockArgument();
		BlockArgument arg1 = BlockArgument();

		arg0.SetupTEXT("destroy");
		arg1.SetupSTRING("test idk reee");

		argList.push_back(arg0);
		argList.push_back(arg1);

		BlockRegistry::CreateBlock("block_destroy", argList);
	}

	Plane* primaryPlane = new Plane();
	
	{
		Stack startingStack = Stack();

		Block startingBlock0 = Block("block_new");
		Block startingBlock1 = Block("block_new");
		Block startingBlock2 = Block("block_destroy");
		Block startingBlock3 = Block("block_destroy");

		startingStack.AddBlock(startingBlock0);
		startingStack.AddBlock(startingBlock1);
		startingStack.AddBlock(startingBlock2);
		startingStack.AddBlock(startingBlock3);

		primaryPlane->AddStack(startingStack);
	}

	// Main

	sf::RenderWindow window;
	window.create(sf::VideoMode(1280, 720, 32), "Unnamed Blocks", sf::Style::Default);

	// Default Settings

	window.setVerticalSyncEnabled(true);

	while (window.isOpen())
	{
		sf::Event ev;
		while (window.pollEvent(ev))
		{
			if (ev.type == sf::Event::Closed)
				window.close();
			if (ev.type == sf::Event::Resized)
			{
				sf::FloatRect visibleArea(0, 0, ev.size.width, ev.size.height);
				window.setView(sf::View(visibleArea));
			}
		}

		window.clear();
		primaryPlane->Render(&window);
		window.display();
	}

	return 0;
}