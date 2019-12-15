#pragma once

#include "registries/BlockRegistry.h"
#include "handlers/runtime/ObjectHandler.h"
#include "handlers/runtime/VariableHandler.h"
#include "handlers/runtime/ByteHandler.h"
#include "handlers/runtime/ThreadHandler.h"
#include "handlers/runtime/RuntimeHandler.h"
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

	{
		RegBlock* block = new RegBlock();

		std::vector<BlockArgument> argList = std::vector<BlockArgument>();

		BlockArgument arg0 = BlockArgument();
		BlockArgument arg1 = BlockArgument();

		arg0.SetupTEXT("function define");
		arg1.SetupSTRING("0function");

		argList.push_back(arg0);
		argList.push_back(arg1);

		block->Args = argList;
		block->Catagory = "vin_threading";
		block->UnlocalizedName = "vin_thread_function_define";

		BlockRegistry::CreateBlock(*block);
	}

	{
		RegBlock* block = new RegBlock();

		std::vector<BlockArgument> argList = std::vector<BlockArgument>();

		BlockArgument arg0 = BlockArgument();
		BlockArgument arg1 = BlockArgument();

		arg0.SetupTEXT("function call");
		arg1.SetupSTRING("0function");

		argList.push_back(arg0);
		argList.push_back(arg1);

		block->Args = argList;
		block->Catagory = "vin_threading";
		block->UnlocalizedName = "vin_thread_function_call";

		BlockRegistry::CreateBlock(*block);
	}

	{
		RegBlock* block = new RegBlock();

		std::vector<BlockArgument> argList = std::vector<BlockArgument>();

		BlockArgument arg0 = BlockArgument();
		BlockArgument arg1 = BlockArgument();
		BlockArgument arg2 = BlockArgument();
		BlockArgument arg3 = BlockArgument();

		arg0.SetupTEXT("function thread");
		arg1.SetupSTRING("0function");
		arg2.SetupTEXT("for");
		arg3.SetupREAL("1threadId");

		argList.push_back(arg0);
		argList.push_back(arg1);
		argList.push_back(arg2);
		argList.push_back(arg3);

		block->Args = argList;
		block->Catagory = "vin_threading";
		block->UnlocalizedName = "vin_thread_function_thread";

		std::function<bool(std::vector<std::string>*)>* execute = new std::function<bool(std::vector<std::string>*)>();
		*execute = [](std::vector<std::string>* args)
		{
			std::string functionText = (*args)[0].substr(1, (*args)[0].length() - 1);
			std::string varText = (*args)[1].substr(1, (*args)[1].length() - 1);

			if ((*args)[1][0] == '0')
			{
				Logger::Error("expecting variable only! got \"" + varText + "\"");
				return false;
			}

			std::string functionName = std::string();

			if ((*args)[0][0] == '0')
				functionName = functionText;
			else
			{
				std::string* value = VariableHandler::GetString(functionText.c_str());
				if (value == nullptr)
				{
					Logger::Error("variable \"" + functionText + "\" does not exist");
					return false;
				}

				functionName = *value;
			}
			
			int searchResult = RuntimeHandler::PerformFunctionSearch(functionName);
			if (searchResult == -1)
			{
				Logger::Error("function \"" + functionName + "\" does not exist");
				return false;
			}

			{//MUST EXIST
				double* var = VariableHandler::GetReal(varText.c_str());
				if (var == nullptr)
				{
					Logger::Error("variable \"" + varText + "\" does not exist");
					return false;
				}
			}

			double threadId = ThreadHandler::SummonThread(searchResult);
			Logger::Debug("summoned thread");

			bool worked = VariableHandler::SetReal(varText.c_str(), threadId);
			if (!worked)
			{
				Logger::Warn("undocumented thread released! this may cause undefined behavior!");
				Logger::Error("variable \"" + varText + "\" does not exist");
				return false;
			}

			return true;
		};

		block->Execute = execute;
		BlockRegistry::CreateBlock(*block);
	}
}
