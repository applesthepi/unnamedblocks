#pragma once

#include "registries/BlockRegistry.h"
#include "handlers/runtime/ObjectHandler.h"
#include "handlers/runtime/VariableHandler.h"
#include "handlers/runtime/ByteHandler.h"
#include "handlers/Logger.h"

void ContentLoadThreading()
{
	{
		RegBlock* block = new RegBlock();

		std::vector<BlockArgument> argList = std::vector<BlockArgument>();

		BlockArgument arg0 = BlockArgument();

		arg0.SetupTEXT("open thread");

		argList.push_back(arg0);

		block->Args = argList;
		block->Catagory = "vin_threading";
		block->UnlocalizedName = "vin_thread_open";

		BlockRegistry::CreateBlock(*block);
	}
	/*
	{
		RegBlock* block = new RegBlock();

		std::vector<BlockArgument> argList = std::vector<BlockArgument>();

		BlockArgument arg0 = BlockArgument();
		BlockArgument arg1 = BlockArgument();

		arg0.SetupTEXT("create thread");
		arg1.SetupSTRING("var");

		argList.push_back(arg0);
		argList.push_back(arg1);

		block->Args = argList;
		block->Catagory = "vin_threading";
		block->UnlocalizedName = "vin_thread_create";

		BlockRegistry::CreateBlock(*block);
	}

	{
		RegBlock* block = new RegBlock();

		std::vector<BlockArgument> argList = std::vector<BlockArgument>();

		BlockArgument arg0 = BlockArgument();
		BlockArgument arg1 = BlockArgument();

		arg0.SetupTEXT("kill thread");
		arg1.SetupSTRING("var");

		argList.push_back(arg0);
		argList.push_back(arg1);

		block->Args = argList;
		block->Catagory = "vin_threading";
		block->UnlocalizedName = "vin_thread_kill";

		BlockRegistry::CreateBlock(*block);
	}

	{
		RegBlock* block = new RegBlock();

		std::vector<BlockArgument> argList = std::vector<BlockArgument>();

		BlockArgument arg0 = BlockArgument();

		arg0.SetupTEXT("kill self");

		argList.push_back(arg0);

		block->Args = argList;
		block->Catagory = "vin_threading";
		block->UnlocalizedName = "vin_thread_kill_self";

		BlockRegistry::CreateBlock(*block);
	}
	*/
	{
		RegBlock* block = new RegBlock();

		std::vector<BlockArgument> argList = std::vector<BlockArgument>();

		BlockArgument arg0 = BlockArgument();
		BlockArgument arg1 = BlockArgument();

		arg0.SetupTEXT("sleep thread");
		arg1.SetupREAL("01000");

		argList.push_back(arg0);
		argList.push_back(arg1);

		block->Args = argList;
		block->Catagory = "vin_threading";
		block->UnlocalizedName = "vin_thread_sleep";

		std::function<bool(std::vector<std::string>*)>* execute = new std::function<bool(std::vector<std::string>*)>();
		*execute = [](std::vector<std::string>* args)
		{
			std::string realText = (*args)[0].substr(1, (*args)[0].length() - 1);

			unsigned long long micro = 0;
			if ((*args)[0][0] == '0')
			{
				try
				{
					micro = floor(std::stod(realText) * 1000.0);
				}
				catch (std::invalid_argument & e)
				{
					Logger::Error("invalid argument exception: expected real but got \"" + realText + "\"");
					return false;
				}
			}
			else
			{
				double* value = VariableHandler::GetReal(realText.c_str());
				if (value == nullptr)
				{
					Logger::Error("variable \"" + realText + "\" does not exist");
					return false;
				}

				micro = floor((*value) * 1000.0);
			}

			std::this_thread::sleep_for(std::chrono::microseconds(micro));

			return true;
		};

		block->Execute = execute;
		BlockRegistry::CreateBlock(*block);
	}
}