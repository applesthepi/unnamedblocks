#pragma once

#include "registries/BlockRegistry.h"
#include "handlers/runtime/ObjectHandler.h"
#include "handlers/runtime/VariableHandler.h"
#include "handlers/runtime/RuntimeHandler.h"
#include "handlers/runtime/ByteHandler.h"
#include "handlers/Logger.h"

#include <iostream>

void ContentLoadInput()
{
	{
		RegBlock* block = new RegBlock();

		std::vector<BlockArgument> argList = std::vector<BlockArgument>();

		BlockArgument arg0 = BlockArgument();
		BlockArgument arg1 = BlockArgument();

		arg0.SetupTEXT("mouse left");
		arg1.SetupBOOL("1down");

		argList.push_back(arg0);
		argList.push_back(arg1);

		block->Args = argList;
		block->Catagory = "vin_input";
		block->UnlocalizedName = "vin_input_mouse_left";

		std::function<bool(std::vector<std::string>*)>* execute = new std::function<bool(std::vector<std::string>*)>();
		*execute = [](std::vector<std::string>* args)
		{
			std::string realText = (*args)[0].substr(1, (*args)[0].length() - 1);

			if ((*args)[0][0] == '0')
			{
				Logger::Error("expecting variable only! got \"" + realText + "\"");
				return false;
			}

			bool value = sf::Mouse::isButtonPressed(sf::Mouse::Left);
			
			bool result = VariableHandler::SetBool(realText.c_str(), value);
			if (!result)
			{
				Logger::Error("variable \"" + realText + "\" does not exist");
				return false;
			}

			return true;
		};

		block->Execute = execute;
		BlockRegistry::CreateBlock(*block);
	}

	{
		RegBlock* block = new RegBlock();

		std::vector<BlockArgument> argList = std::vector<BlockArgument>();

		BlockArgument arg0 = BlockArgument();
		BlockArgument arg1 = BlockArgument();

		arg0.SetupTEXT("mouse right");
		arg1.SetupBOOL("1down");

		argList.push_back(arg0);
		argList.push_back(arg1);

		block->Args = argList;
		block->Catagory = "vin_input";
		block->UnlocalizedName = "vin_input_mouse_right";

		std::function<bool(std::vector<std::string>*)>* execute = new std::function<bool(std::vector<std::string>*)>();
		*execute = [](std::vector<std::string>* args)
		{
			std::string realText = (*args)[0].substr(1, (*args)[0].length() - 1);

			if ((*args)[0][0] == '0')
			{
				Logger::Error("expecting variable only! got \"" + realText + "\"");
				return false;
			}

			bool value = sf::Mouse::isButtonPressed(sf::Mouse::Right);

			bool result = VariableHandler::SetBool(realText.c_str(), value);
			if (!result)
			{
				Logger::Error("variable \"" + realText + "\" does not exist");
				return false;
			}

			return true;
		};

		block->Execute = execute;
		BlockRegistry::CreateBlock(*block);
	}

	{
		RegBlock* block = new RegBlock();

		std::vector<BlockArgument> argList = std::vector<BlockArgument>();

		BlockArgument arg0 = BlockArgument();
		BlockArgument arg1 = BlockArgument();

		arg0.SetupTEXT("mouse middle");
		arg1.SetupBOOL("1down");

		argList.push_back(arg0);
		argList.push_back(arg1);

		block->Args = argList;
		block->Catagory = "vin_input";
		block->UnlocalizedName = "vin_input_mouse_middle";

		std::function<bool(std::vector<std::string>*)>* execute = new std::function<bool(std::vector<std::string>*)>();
		*execute = [](std::vector<std::string>* args)
		{
			std::string realText = (*args)[0].substr(1, (*args)[0].length() - 1);

			if ((*args)[0][0] == '0')
			{
				Logger::Error("expecting variable only! got \"" + realText + "\"");
				return false;
			}

			bool value = sf::Mouse::isButtonPressed(sf::Mouse::Middle);

			bool result = VariableHandler::SetBool(realText.c_str(), value);
			if (!result)
			{
				Logger::Error("variable \"" + realText + "\" does not exist");
				return false;
			}

			return true;
		};

		block->Execute = execute;
		BlockRegistry::CreateBlock(*block);
	}

	{
		RegBlock* block = new RegBlock();

		std::vector<BlockArgument> argList = std::vector<BlockArgument>();

		BlockArgument arg0 = BlockArgument();
		BlockArgument arg1 = BlockArgument();

		arg0.SetupTEXT("mouse x");
		arg1.SetupREAL("1mouseX");

		argList.push_back(arg0);
		argList.push_back(arg1);

		block->Args = argList;
		block->Catagory = "vin_input";
		block->UnlocalizedName = "vin_input_mouse_x";

		std::function<bool(std::vector<std::string>*)>* execute = new std::function<bool(std::vector<std::string>*)>();
		*execute = [](std::vector<std::string>* args)
		{
			std::string realText = (*args)[0].substr(1, (*args)[0].length() - 1);

			if ((*args)[0][0] == '0')
			{
				Logger::Error("expecting variable only! got \"" + realText + "\"");
				return false;
			}

			int value = sf::Mouse::getPosition(RuntimeHandler::Window).x;

			bool result = VariableHandler::SetReal(realText.c_str(), value);
			if (!result)
			{
				Logger::Error("variable \"" + realText + "\" does not exist");
				return false;
			}

			return true;
		};

		block->Execute = execute;
		BlockRegistry::CreateBlock(*block);
	}

	{
		RegBlock* block = new RegBlock();

		std::vector<BlockArgument> argList = std::vector<BlockArgument>();

		BlockArgument arg0 = BlockArgument();
		BlockArgument arg1 = BlockArgument();

		arg0.SetupTEXT("mouse y");
		arg1.SetupREAL("1mouseY");

		argList.push_back(arg0);
		argList.push_back(arg1);

		block->Args = argList;
		block->Catagory = "vin_input";
		block->UnlocalizedName = "vin_input_mouse_y";

		std::function<bool(std::vector<std::string>*)>* execute = new std::function<bool(std::vector<std::string>*)>();
		*execute = [](std::vector<std::string>* args)
		{
			std::string realText = (*args)[0].substr(1, (*args)[0].length() - 1);

			if ((*args)[0][0] == '0')
			{
				Logger::Error("expecting variable only! got \"" + realText + "\"");
				return false;
			}

			int value = sf::Mouse::getPosition(RuntimeHandler::Window).y;

			bool result = VariableHandler::SetReal(realText.c_str(), value);
			if (!result)
			{
				Logger::Error("variable \"" + realText + "\" does not exist");
				return false;
			}

			return true;
		};

		block->Execute = execute;
		BlockRegistry::CreateBlock(*block);
	}

	{
		RegBlock* block = new RegBlock();

		std::vector<BlockArgument> argList = std::vector<BlockArgument>();

		BlockArgument arg0 = BlockArgument();
		BlockArgument arg1 = BlockArgument();

		arg0.SetupTEXT("pull mouse scroll");
		arg1.SetupREAL("1mouseScroll");

		argList.push_back(arg0);
		argList.push_back(arg1);

		block->Args = argList;
		block->Catagory = "vin_input";
		block->UnlocalizedName = "vin_input_mouse_scroll";

		std::function<bool(std::vector<std::string>*)>* execute = new std::function<bool(std::vector<std::string>*)>();
		*execute = [](std::vector<std::string>* args)
		{
			std::string realText = (*args)[0].substr(1, (*args)[0].length() - 1);

			if ((*args)[0][0] == '0')
			{
				Logger::Error("expecting variable only! got \"" + realText + "\"");
				return false;
			}

			int value = RuntimeHandler::GetScrolled();
			RuntimeHandler::ResetScrolled();

			bool result = VariableHandler::SetReal(realText.c_str(), value);
			if (!result)
			{
				Logger::Error("variable \"" + realText + "\" does not exist");
				return false;
			}

			return true;
		};

		block->Execute = execute;
		BlockRegistry::CreateBlock(*block);
	}

	{
		RegBlock* block = new RegBlock();

		std::vector<BlockArgument> argList = std::vector<BlockArgument>();

		BlockArgument arg0 = BlockArgument();

		arg0.SetupTEXT("reset scroll");

		argList.push_back(arg0);

		block->Args = argList;
		block->Catagory = "vin_input";
		block->UnlocalizedName = "vin_input_mouse_scroll_reset";

		std::function<bool(std::vector<std::string>*)>* execute = new std::function<bool(std::vector<std::string>*)>();
		*execute = [](std::vector<std::string>* args)
		{
			RuntimeHandler::ResetScrolled();
			return true;
		};

		block->Execute = execute;
		BlockRegistry::CreateBlock(*block);
	}

	{
		RegBlock* block = new RegBlock();

		std::vector<BlockArgument> argList = std::vector<BlockArgument>();

		BlockArgument arg0 = BlockArgument();
		BlockArgument arg1 = BlockArgument();
		BlockArgument arg2 = BlockArgument();
		BlockArgument arg3 = BlockArgument();

		arg0.SetupTEXT("key down");
		arg1.SetupREAL("00");
		arg2.SetupTEXT("for");
		arg3.SetupBOOL("1down");

		argList.push_back(arg0);
		argList.push_back(arg1);
		argList.push_back(arg2);
		argList.push_back(arg3);

		block->Args = argList;
		block->Catagory = "vin_input";
		block->UnlocalizedName = "vin_input_mouse_key";

		std::function<bool(std::vector<std::string>*)>* execute = new std::function<bool(std::vector<std::string>*)>();
		*execute = [](std::vector<std::string>* args)
		{
			std::string keyText = (*args)[0].substr(1, (*args)[0].length() - 1);
			std::string varText = (*args)[1].substr(1, (*args)[1].length() - 1);

			if ((*args)[1][0] == '0')
			{
				Logger::Error("expecting variable only! got \"" + varText + "\"");
				return false;
			}

			unsigned int key = 0;

			if ((*args)[0][0] == '0')
			{
				try
				{
					key = std::stoul(keyText);
				}
				catch (std::invalid_argument & e)
				{
					Logger::Error("invalid argument exception: expected real but got \"" + keyText + "\"");
					return false;
				}
			}
			else
			{
				double* value = VariableHandler::GetReal(keyText.c_str());

				if (value == nullptr)
				{
					Logger::Error("variable \"" + keyText + "\" does not exist");
					return false;
				}

				key = round(*value);
			}

			bool result = VariableHandler::SetBool(varText.c_str(), sf::Keyboard::isKeyPressed((sf::Keyboard::Key)key));
			if (!result)
			{
				Logger::Error("variable \"" + varText + "\" does not exist");
				return false;
			}

			return true;
		};

		block->Execute = execute;
		BlockRegistry::CreateBlock(*block);
	}
}