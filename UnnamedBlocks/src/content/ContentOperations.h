#pragma once
#include <math.h>
#include <RHR/RHR.h>

void ContentLoadOperations()
{
	{
		RegBlock* block = new RegBlock();

		std::vector<BlockArgument> argList = std::vector<BlockArgument>();

		BlockArgument arg0 = BlockArgument();
		BlockArgument arg1 = BlockArgument();
		BlockArgument arg2 = BlockArgument();

		arg0.SetupREAL("1var");
		arg1.SetupTEXT("+=");
		arg2.SetupREAL("00");

		argList.push_back(arg0);
		argList.push_back(arg1);
		argList.push_back(arg2);

		block->Args = argList;
		block->Catagory = "vin_operations";
		block->UnlocalizedName = "vin_operation_+=";

		std::function<bool(std::vector<std::string>*)>* execute = new std::function<bool(std::vector<std::string>*)>();
		*execute = [](std::vector<std::string>* args)
		{
			std::string varText = (*args)[0].substr(1, (*args)[0].length() - 1);
			std::string valueText = (*args)[1].substr(1, (*args)[1].length() - 1);

			if ((*args)[0][0] == '0')
			{
				Logger::Error("expecting variable only! got \"" + varText + "\"");
				return false;
			}

			double opValue;

			if ((*args)[1][0] == '0')
			{
				try
				{
					opValue = std::stod(valueText);
				}
				catch (std::invalid_argument & e)
				{
					Logger::Error("invalid argument exception: expected real but got \"" + valueText + "\"");
					return false;
				}
			}
			else
			{
				double* value = VariableHandler::GetReal(valueText.c_str());

				if (value == nullptr)
				{
					Logger::Error("variable \"" + valueText + "\" does not exist");
					return false;
				}

				opValue = *value;
			}

			double* gotVar = VariableHandler::GetReal(varText.c_str());
			if (gotVar == nullptr)
			{
				Logger::Error("variable \"" + varText + "\" does not exist");
				return false;
			}

			return VariableHandler::SetReal(varText.c_str(), (*gotVar) + opValue);
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

		arg0.SetupREAL("1var");
		arg1.SetupTEXT("-=");
		arg2.SetupREAL("00");

		argList.push_back(arg0);
		argList.push_back(arg1);
		argList.push_back(arg2);

		block->Args = argList;
		block->Catagory = "vin_operations";
		block->UnlocalizedName = "vin_operation_-=";

		std::function<bool(std::vector<std::string>*)>* execute = new std::function<bool(std::vector<std::string>*)>();
		*execute = [](std::vector<std::string>* args)
		{
			std::string varText = (*args)[0].substr(1, (*args)[0].length() - 1);
			std::string valueText = (*args)[1].substr(1, (*args)[1].length() - 1);

			if ((*args)[0][0] == '0')
			{
				Logger::Error("expecting variable only! got \"" + varText + "\"");
				return false;
			}

			double opValue;

			if ((*args)[1][0] == '0')
			{
				try
				{
					opValue = std::stod(valueText);
				}
				catch (std::invalid_argument & e)
				{
					Logger::Error("invalid argument exception: expected real but got \"" + valueText + "\"");
					return false;
				}
			}
			else
			{
				double* value = VariableHandler::GetReal(valueText.c_str());

				if (value == nullptr)
				{
					Logger::Error("variable \"" + valueText + "\" does not exist");
					return false;
				}

				opValue = *value;
			}

			double* gotVar = VariableHandler::GetReal(varText.c_str());
			if (gotVar == nullptr)
			{
				Logger::Error("variable \"" + varText + "\" does not exist");
				return false;
			}

			return VariableHandler::SetReal(varText.c_str(), (*gotVar) - opValue);
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

		arg0.SetupREAL("1var");
		arg1.SetupTEXT("*=");
		arg2.SetupREAL("00");

		argList.push_back(arg0);
		argList.push_back(arg1);
		argList.push_back(arg2);

		block->Args = argList;
		block->Catagory = "vin_operations";
		block->UnlocalizedName = "vin_operation_*=";

		std::function<bool(std::vector<std::string>*)>* execute = new std::function<bool(std::vector<std::string>*)>();
		*execute = [](std::vector<std::string>* args)
		{
			std::string varText = (*args)[0].substr(1, (*args)[0].length() - 1);
			std::string valueText = (*args)[1].substr(1, (*args)[1].length() - 1);

			if ((*args)[0][0] == '0')
			{
				Logger::Error("expecting variable only! got \"" + varText + "\"");
				return false;
			}

			double opValue;

			if ((*args)[1][0] == '0')
			{
				try
				{
					opValue = std::stod(valueText);
				}
				catch (std::invalid_argument & e)
				{
					Logger::Error("invalid argument exception: expected real but got \"" + valueText + "\"");
					return false;
				}
			}
			else
			{
				double* value = VariableHandler::GetReal(valueText.c_str());

				if (value == nullptr)
				{
					Logger::Error("variable \"" + valueText + "\" does not exist");
					return false;
				}

				opValue = *value;
			}

			double* gotVar = VariableHandler::GetReal(varText.c_str());
			if (gotVar == nullptr)
			{
				Logger::Error("variable \"" + varText + "\" does not exist");
				return false;
			}

			return VariableHandler::SetReal(varText.c_str(), (*gotVar) * opValue);
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

		arg0.SetupREAL("1var");
		arg1.SetupTEXT("/=");
		arg2.SetupREAL("00");

		argList.push_back(arg0);
		argList.push_back(arg1);
		argList.push_back(arg2);

		block->Args = argList;
		block->Catagory = "vin_operations";
		block->UnlocalizedName = "vin_operation_/=";

		std::function<bool(std::vector<std::string>*)>* execute = new std::function<bool(std::vector<std::string>*)>();
		*execute = [](std::vector<std::string>* args)
		{
			std::string varText = (*args)[0].substr(1, (*args)[0].length() - 1);
			std::string valueText = (*args)[1].substr(1, (*args)[1].length() - 1);

			if ((*args)[0][0] == '0')
			{
				Logger::Error("expecting variable only! got \"" + varText + "\"");
				return false;
			}

			double opValue;

			if ((*args)[1][0] == '0')
			{
				try
				{
					opValue = std::stod(valueText);
				}
				catch (std::invalid_argument & e)
				{
					Logger::Error("invalid argument exception: expected real but got \"" + valueText + "\"");
					return false;
				}
			}
			else
			{
				double* value = VariableHandler::GetReal(valueText.c_str());

				if (value == nullptr)
				{
					Logger::Error("variable \"" + valueText + "\" does not exist");
					return false;
				}

				opValue = *value;
			}

			double* gotVar = VariableHandler::GetReal(varText.c_str());
			if (gotVar == nullptr)
			{
				Logger::Error("variable \"" + varText + "\" does not exist");
				return false;
			}

			return VariableHandler::SetReal(varText.c_str(), (*gotVar) / opValue);
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

		arg0.SetupSTRING("1var");
		arg1.SetupTEXT("%=");
		arg2.SetupREAL("00");

		argList.push_back(arg0);
		argList.push_back(arg1);
		argList.push_back(arg2);

		block->Args = argList;
		block->Catagory = "vin_operations";
		block->UnlocalizedName = "vin_operation_%";

		std::function<bool(std::vector<std::string>*)>* execute = new std::function<bool(std::vector<std::string>*)>();
		*execute = [](std::vector<std::string>* args)
		{
			std::string varText = (*args)[0].substr(1, (*args)[0].length() - 1);
			std::string valueText = (*args)[1].substr(1, (*args)[1].length() - 1);

			if ((*args)[0][0] == '0')
			{
				Logger::Error("expecting variable only! got \"" + varText + "\"");
				return false;
			}

			double opValue;

			if ((*args)[1][0] == '0')
			{
				try
				{
					opValue = std::stod(valueText);
				}
				catch (std::invalid_argument & e)
				{
					Logger::Error("invalid argument exception: expected real but got \"" + valueText + "\"");
					return false;
				}
			}
			else
			{
				double* value = VariableHandler::GetReal(valueText.c_str());

				if (value == nullptr)
				{
					Logger::Error("variable \"" + valueText + "\" does not exist");
					return false;
				}

				opValue = *value;
			}

			double* gotVar = VariableHandler::GetReal(varText.c_str());
			if (gotVar == nullptr)
			{
				Logger::Error("variable \"" + varText + "\" does not exist");
				return false;
			}

			return VariableHandler::SetReal(varText.c_str(), (unsigned long long)(floor(*gotVar)) % (unsigned long long)(floor(opValue)));
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

		arg0.SetupSTRING("1var");
		arg1.SetupTEXT("=");
		arg2.SetupREAL("00");

		argList.push_back(arg0);
		argList.push_back(arg1);
		argList.push_back(arg2);

		block->Args = argList;
		block->Catagory = "vin_operations";
		block->UnlocalizedName = "vin_operation_=_num";

		std::function<bool(std::vector<std::string>*)>* execute = new std::function<bool(std::vector<std::string>*)>();
		*execute = [](std::vector<std::string>* args)
		{
			std::string varText = (*args)[0].substr(1, (*args)[0].length() - 1);
			std::string valueText = (*args)[1].substr(1, (*args)[1].length() - 1);

			if ((*args)[0][0] == '0')
			{
				Logger::Error("expecting variable only! got \"" + varText + "\"");
				return false;
			}

			double opValue;

			if ((*args)[1][0] == '0')
			{
				try
				{
					opValue = std::stod(valueText);
				}
				catch (std::invalid_argument & e)
				{
					Logger::Error("invalid argument exception: expected real but got \"" + valueText + "\"");
					return false;
				}
			}
			else
			{
				double* value = VariableHandler::GetReal(valueText.c_str());

				if (value == nullptr)
				{
					Logger::Error("variable \"" + valueText + "\" does not exist");
					return false;
				}

				opValue = *value;
			}

			bool result = VariableHandler::SetReal(varText.c_str(), opValue);
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

	{
		RegBlock* block = new RegBlock();

		std::vector<BlockArgument> argList = std::vector<BlockArgument>();

		BlockArgument arg0 = BlockArgument();
		BlockArgument arg1 = BlockArgument();
		BlockArgument arg2 = BlockArgument();

		arg0.SetupSTRING("1var");
		arg1.SetupTEXT("=");
		arg2.SetupSTRING("0Kill, Me!");

		argList.push_back(arg0);
		argList.push_back(arg1);
		argList.push_back(arg2);

		block->Args = argList;
		block->Catagory = "vin_operations";
		block->UnlocalizedName = "vin_operation_=_string";

		std::function<bool(std::vector<std::string>*)>* execute = new std::function<bool(std::vector<std::string>*)>();
		*execute = [](std::vector<std::string>* args)
		{
			std::string varText = (*args)[0].substr(1, (*args)[0].length() - 1);
			std::string valueText = (*args)[1].substr(1, (*args)[1].length() - 1);

			if ((*args)[0][0] == '0')
			{
				Logger::Error("expecting variable only! got \"" + varText + "\"");
				return false;
			}

			std::string opValue;

			if ((*args)[1][0] == '0')
			{
				try
				{
					opValue = valueText;
				}
				catch (std::invalid_argument & e)
				{
					Logger::Error("invalid argument exception: expected real but got \"" + valueText + "\"");
					return false;
				}
			}
			else
			{
				std::string* value = VariableHandler::GetString(valueText.c_str());

				if (value == nullptr)
				{
					Logger::Error("variable \"" + valueText + "\" does not exist");
					return false;
				}

				opValue = *value;
			}

			bool result = VariableHandler::SetString(varText.c_str(), opValue);
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
	
	{
		RegBlock* block = new RegBlock();

		std::vector<BlockArgument> argList = std::vector<BlockArgument>();

		BlockArgument arg0 = BlockArgument();
		BlockArgument arg1 = BlockArgument();
		BlockArgument arg2 = BlockArgument();

		arg0.SetupSTRING("1var");
		arg1.SetupTEXT("=");
		arg2.SetupBOOL("00");

		argList.push_back(arg0);
		argList.push_back(arg1);
		argList.push_back(arg2);

		block->Args = argList;
		block->Catagory = "vin_operations";
		block->UnlocalizedName = "vin_operation_=_bool";

		std::function<bool(std::vector<std::string>*)>* execute = new std::function<bool(std::vector<std::string>*)>();
		*execute = [](std::vector<std::string>* args)
		{
			std::string varText = (*args)[0].substr(1, (*args)[0].length() - 1);
			std::string valueText = (*args)[1].substr(1, (*args)[1].length() - 1);

			if ((*args)[0][0] == '0')
			{
				Logger::Error("expecting variable only! got \"" + varText + "\"");
				return false;
			}

			bool opValue;

			if ((*args)[1][0] == '0')
			{
				try
				{
					opValue = valueText == "1";
				}
				catch (std::invalid_argument & e)
				{
					Logger::Error("invalid argument exception: expected real but got \"" + valueText + "\"");
					return false;
				}
			}
			else
			{
				bool* value = VariableHandler::GetBool(valueText.c_str());

				if (value == nullptr)
				{
					Logger::Error("variable \"" + valueText + "\" does not exist");
					return false;
				}

				opValue = *value;
			}

			bool result = VariableHandler::SetBool(varText.c_str(), opValue);
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

	{
		RegBlock* block = new RegBlock();

		std::vector<BlockArgument> argList = std::vector<BlockArgument>();

		BlockArgument arg0 = BlockArgument();
		BlockArgument arg1 = BlockArgument();

		arg0.SetupTEXT("round");
		arg1.SetupREAL("1var");

		argList.push_back(arg0);
		argList.push_back(arg1);

		block->Args = argList;
		block->Catagory = "vin_operations";
		block->UnlocalizedName = "vin_operation_round";

		std::function<bool(std::vector<std::string>*)>* execute = new std::function<bool(std::vector<std::string>*)>();
		*execute = [](std::vector<std::string>* args)
		{
			std::string varText = (*args)[0].substr(1, (*args)[0].length() - 1);

			if ((*args)[0][0] == '0')
			{
				Logger::Error("expecting variable only! got \"" + varText + "\"");
				return false;
			}

			double* gotVar = VariableHandler::GetReal(varText.c_str());
			if (gotVar == nullptr)
			{
				Logger::Error("variable \"" + varText + "\" does not exist");
				return false;
			}

			return VariableHandler::SetReal(varText.c_str(), round(*gotVar));
		};

		block->Execute = execute;
		BlockRegistry::CreateBlock(*block);
	}

	{
		RegBlock* block = new RegBlock();

		std::vector<BlockArgument> argList = std::vector<BlockArgument>();

		BlockArgument arg0 = BlockArgument();
		BlockArgument arg1 = BlockArgument();

		arg0.SetupTEXT("floor");
		arg1.SetupREAL("1var");

		argList.push_back(arg0);
		argList.push_back(arg1);

		block->Args = argList;
		block->Catagory = "vin_operations";
		block->UnlocalizedName = "vin_operation_floor";

		std::function<bool(std::vector<std::string>*)>* execute = new std::function<bool(std::vector<std::string>*)>();
		*execute = [](std::vector<std::string>* args)
		{
			std::string varText = (*args)[0].substr(1, (*args)[0].length() - 1);

			if ((*args)[0][0] == '0')
			{
				Logger::Error("expecting variable only! got \"" + varText + "\"");
				return false;
			}

			double* gotVar = VariableHandler::GetReal(varText.c_str());
			if (gotVar == nullptr)
			{
				Logger::Error("variable \"" + varText + "\" does not exist");
				return false;
			}

			return VariableHandler::SetReal(varText.c_str(), floor(*gotVar));
		};

		block->Execute = execute;
		BlockRegistry::CreateBlock(*block);
	}

	{
		RegBlock* block = new RegBlock();

		std::vector<BlockArgument> argList = std::vector<BlockArgument>();

		BlockArgument arg0 = BlockArgument();
		BlockArgument arg1 = BlockArgument();

		arg0.SetupTEXT("ceil");
		arg1.SetupREAL("1var");

		argList.push_back(arg0);
		argList.push_back(arg1);

		block->Args = argList;
		block->Catagory = "vin_operations";
		block->UnlocalizedName = "vin_operation_ceil";

		std::function<bool(std::vector<std::string>*)>* execute = new std::function<bool(std::vector<std::string>*)>();
		*execute = [](std::vector<std::string>* args)
		{
			std::string varText = (*args)[0].substr(1, (*args)[0].length() - 1);

			if ((*args)[0][0] == '0')
			{
				Logger::Error("expecting variable only! got \"" + varText + "\"");
				return false;
			}

			double* gotVar = VariableHandler::GetReal(varText.c_str());
			if (gotVar == nullptr)
			{
				Logger::Error("variable \"" + varText + "\" does not exist");
				return false;
			}

			return VariableHandler::SetReal(varText.c_str(), ceil(*gotVar));
		};

		block->Execute = execute;
		BlockRegistry::CreateBlock(*block);
	}
}
