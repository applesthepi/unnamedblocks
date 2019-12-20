#pragma once

#include <RHR/RHR.h>

void ContentLoadVariables()
{
	{
		RegBlock* block = new RegBlock();

		std::vector<BlockArgument> argList = std::vector<BlockArgument>();

		BlockArgument arg0 = BlockArgument();
		BlockArgument arg1 = BlockArgument();

		arg0.SetupTEXT("stack real");
		arg1.SetupSTRING("0var");

		argList.push_back(arg0);
		argList.push_back(arg1);

		block->Args = argList;
		block->Catagory = "vin_variables";
		block->UnlocalizedName = "vin_stack_real";

		std::function<bool(std::vector<std::string>*)>* execute = new std::function<bool(std::vector<std::string>*)>();
		*execute = [](std::vector<std::string>* args)
		{
			std::string varText = (*args)[0].substr(1, (*args)[0].length() - 1);

			if ((*args)[0][0] == '1')
			{
				Logger::Error("expecting text only! got \"" + varText + "\"");
				return false;
			}

			bool result = VariableHandler::StackReal(varText.c_str());
			if (!result)
			{
				Logger::Error("failed to allocate");
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

		arg0.SetupTEXT("stack string");
		arg1.SetupSTRING("0var");

		argList.push_back(arg0);
		argList.push_back(arg1);

		block->Args = argList;
		block->Catagory = "vin_variables";
		block->UnlocalizedName = "vin_stack_string";

		std::function<bool(std::vector<std::string>*)>* execute = new std::function<bool(std::vector<std::string>*)>();
		*execute = [](std::vector<std::string>* args)
		{
			std::string varText = (*args)[0].substr(1, (*args)[0].length() - 1);

			if ((*args)[0][0] == '1')
			{
				Logger::Error("expecting text only! got \"" + varText + "\"");
				return false;
			}

			bool result = VariableHandler::StackString(varText.c_str());
			if (!result)
			{
				Logger::Error("failed to allocate");
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

		arg0.SetupTEXT("stack bool");
		arg1.SetupSTRING("0var");

		argList.push_back(arg0);
		argList.push_back(arg1);

		block->Args = argList;
		block->Catagory = "vin_variables";
		block->UnlocalizedName = "vin_stack_bool";

		std::function<bool(std::vector<std::string>*)>* execute = new std::function<bool(std::vector<std::string>*)>();
		*execute = [](std::vector<std::string>* args)
		{
			std::string varText = (*args)[0].substr(1, (*args)[0].length() - 1);

			if ((*args)[0][0] == '1')
			{
				Logger::Error("expecting text only! got \"" + varText + "\"");
				return false;
			}

			bool result = VariableHandler::StackBool(varText.c_str());
			if (!result)
			{
				Logger::Error("failed to allocate");
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

		arg0.SetupTEXT("heap real");
		arg1.SetupSTRING("0var");

		argList.push_back(arg0);
		argList.push_back(arg1);

		block->Args = argList;
		block->Catagory = "vin_variables";
		block->UnlocalizedName = "vin_heap_real";

		std::function<bool(std::vector<std::string>*)>* execute = new std::function<bool(std::vector<std::string>*)>();
		*execute = [](std::vector<std::string>* args)
		{
			std::string varText = (*args)[0].substr(1, (*args)[0].length() - 1);

			if ((*args)[0][0] == '1')
			{
				Logger::Error("expecting text only! got \"" + varText + "\"");
				return false;
			}

			VariableHandler::HeapReal(varText.c_str());

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

		arg0.SetupTEXT("heap string");
		arg1.SetupSTRING("0var");

		argList.push_back(arg0);
		argList.push_back(arg1);

		block->Args = argList;
		block->Catagory = "vin_variables";
		block->UnlocalizedName = "vin_heap_string";

		std::function<bool(std::vector<std::string>*)>* execute = new std::function<bool(std::vector<std::string>*)>();
		*execute = [](std::vector<std::string>* args)
		{
			std::string varText = (*args)[0].substr(1, (*args)[0].length() - 1);

			if ((*args)[0][0] == '1')
			{
				Logger::Error("expecting text only! got \"" + varText + "\"");
				return false;
			}

			VariableHandler::HeapString(varText.c_str());

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

		arg0.SetupTEXT("heap bool");
		arg1.SetupSTRING("0var");

		argList.push_back(arg0);
		argList.push_back(arg1);

		block->Args = argList;
		block->Catagory = "vin_variables";
		block->UnlocalizedName = "vin_heap_bool";

		std::function<bool(std::vector<std::string>*)>* execute = new std::function<bool(std::vector<std::string>*)>();
		*execute = [](std::vector<std::string>* args)
		{
			std::string varText = (*args)[0].substr(1, (*args)[0].length() - 1);

			if ((*args)[0][0] == '1')
			{
				Logger::Error("expecting text only! got \"" + varText + "\"");
				return false;
			}

			VariableHandler::HeapBool(varText.c_str());
			
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

		arg0.SetupTEXT("free real");
		arg1.SetupREAL("1var");

		argList.push_back(arg0);
		argList.push_back(arg1);

		block->Args = argList;
		block->Catagory = "vin_variables";
		block->UnlocalizedName = "vin_free_real";

		std::function<bool(std::vector<std::string>*)>* execute = new std::function<bool(std::vector<std::string>*)>();
		*execute = [](std::vector<std::string>* args)
		{
			std::string varText = (*args)[0].substr(1, (*args)[0].length() - 1);

			if ((*args)[0][0] == '0')
			{
				Logger::Error("expecting variable only! got \"" + varText + "\"");
				return false;
			}

			bool result = VariableHandler::FreeReal(varText.c_str());
			if (!result)
			{
				Logger::Error("failed to deallocate");
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

		arg0.SetupTEXT("free string");
		arg1.SetupSTRING("1var");

		argList.push_back(arg0);
		argList.push_back(arg1);

		block->Args = argList;
		block->Catagory = "vin_variables";
		block->UnlocalizedName = "vin_free_string";

		std::function<bool(std::vector<std::string>*)>* execute = new std::function<bool(std::vector<std::string>*)>();
		*execute = [](std::vector<std::string>* args)
		{
			std::string varText = (*args)[0].substr(1, (*args)[0].length() - 1);

			if ((*args)[0][0] == '0')
			{
				Logger::Error("expecting variable only! got \"" + varText + "\"");
				return false;
			}

			bool result = VariableHandler::FreeString(varText.c_str());
			if (!result)
			{
				Logger::Error("failed to deallocate");
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

		arg0.SetupTEXT("free bool");
		arg1.SetupBOOL("1var");

		argList.push_back(arg0);
		argList.push_back(arg1);

		block->Args = argList;
		block->Catagory = "vin_variables";
		block->UnlocalizedName = "vin_free_bool";

		std::function<bool(std::vector<std::string>*)>* execute = new std::function<bool(std::vector<std::string>*)>();
		*execute = [](std::vector<std::string>* args)
		{
			std::string varText = (*args)[0].substr(1, (*args)[0].length() - 1);

			if ((*args)[0][0] == '0')
			{
				Logger::Error("expecting variable only! got \"" + varText + "\"");
				return false;
			}

			bool result = VariableHandler::FreeBool(varText.c_str());
			if (!result)
			{
				Logger::Error("failed to deallocate");
				return false;
			}

			return true;
		};

		//block->Execute = execute;
		BlockRegistry::CreateBlock(*block);
	}

	{
		RegBlock* block = new RegBlock();

		std::vector<BlockArgument> argList = std::vector<BlockArgument>();

		BlockArgument arg0 = BlockArgument();
		BlockArgument arg1 = BlockArgument();

		arg0.SetupTEXT("log");
		arg1.SetupSTRING("1var");

		argList.push_back(arg0);
		argList.push_back(arg1);

		block->Args = argList;
		block->Catagory = "vin_variables";
		block->UnlocalizedName = "vin_log";

		std::function<bool(std::vector<std::string>*)>* execute = new std::function<bool(std::vector<std::string>*)>();
		*execute = [](std::vector<std::string>* args)
		{
			std::string varText = (*args)[0].substr(1, (*args)[0].length() - 1);

			if ((*args)[0][0] == '0')
			{
				Logger::Info("[RAW] " + varText);
			}
			else
			{
				double* resultReal = VariableHandler::GetReal(varText.c_str());
				std::string* resultString = VariableHandler::GetString(varText.c_str());
				bool* resultBool = VariableHandler::GetBool(varText.c_str());

				if (resultReal != nullptr)
					Logger::Info("[REAL] " + std::to_string(*resultReal));
				else if (resultString != nullptr)
					Logger::Info("[STRING] " + (*resultString));
				else if (resultBool != nullptr)
					Logger::Info("[BOOL] " + ((*resultBool) ? std::string("true") : std::string("false")));
				else
				{
					Logger::Error("variable \"" + varText + "\" does not exist");
					return false;
				}
			}

			return true;
		};

		block->Execute = execute;
		BlockRegistry::CreateBlock(*block);
	}
}
