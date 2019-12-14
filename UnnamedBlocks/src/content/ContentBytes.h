#pragma once

#include "registries/BlockRegistry.h"
#include "handlers/runtime/ObjectHandler.h"
#include "handlers/runtime/VariableHandler.h"
#include "handlers/runtime/ByteHandler.h"
#include "handlers/Logger.h"

void ContentLoadBytes()
{
	{
		RegBlock* block = new RegBlock();

		std::vector<BlockArgument> argList = std::vector<BlockArgument>();

		BlockArgument arg0 = BlockArgument();
		BlockArgument arg1 = BlockArgument();
		BlockArgument arg2 = BlockArgument();
		BlockArgument arg3 = BlockArgument();

		arg0.SetupTEXT("alloc");
		arg1.SetupREAL("010");
		arg2.SetupTEXT("bytes to");
		arg3.SetupREAL("1var");

		argList.push_back(arg0);
		argList.push_back(arg1);
		argList.push_back(arg2);
		argList.push_back(arg3);

		block->Args = argList;
		block->Catagory = "vin_bytes";
		block->UnlocalizedName = "vin_byte_alloc";

		std::function<bool(std::vector<std::string>*)>* execute = new std::function<bool(std::vector<std::string>*)>();
		*execute = [](std::vector<std::string>* args)
		{
			std::string bytesText = (*args)[0].substr(1, (*args)[0].length() - 1);
			std::string varText = (*args)[1].substr(1, (*args)[1].length() - 1);

			if ((*args)[1][0] == '0')
			{
				Logger::Error("expecting variable only! got \"" + varText + "\"");
				return false;
			}

			unsigned long long bytes;

			if ((*args)[0][0] == '0')
			{
				try
				{
					bytes = std::stoull(bytesText);
				}
				catch (std::invalid_argument & e)
				{
					Logger::Error("invalid argument exception: expected real but got \"" + bytesText + "\"");
					return false;
				}
			}
			else
			{
				double* value = VariableHandler::GetReal(bytesText.c_str());

				if (value == nullptr)
				{
					Logger::Error("variable \"" + bytesText + "\" does not exist");
					return false;
				}

				bytes = *value;
			}

			unsigned long long address = ByteHandler::AllocateBytes(bytes);

			bool result = VariableHandler::SetReal(varText.c_str(), address);
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
		BlockArgument arg3 = BlockArgument();

		arg0.SetupTEXT("free");
		arg1.SetupREAL("010");
		arg2.SetupTEXT("bytes at");
		arg3.SetupREAL("1var");

		argList.push_back(arg0);
		argList.push_back(arg1);
		argList.push_back(arg2);
		argList.push_back(arg3);

		block->Args = argList;
		block->Catagory = "vin_bytes";
		block->UnlocalizedName = "vin_byte_free";

		std::function<bool(std::vector<std::string>*)>* execute = new std::function<bool(std::vector<std::string>*)>();
		*execute = [](std::vector<std::string>* args)
		{
			std::string bytesText = (*args)[0].substr(1, (*args)[0].length() - 1);
			std::string varText = (*args)[1].substr(1, (*args)[1].length() - 1);

			if ((*args)[1][0] == '0')
			{
				Logger::Error("expecting variable only! got \"" + varText + "\"");
				return false;
			}

			unsigned long long bytes;
			unsigned long long address;

			if ((*args)[0][0] == '0')
			{
				try
				{
					bytes = std::stoull(bytesText);
				}
				catch (std::invalid_argument & e)
				{
					Logger::Error("invalid argument exception: expected real but got \"" + bytesText + "\"");
					return false;
				}
			}
			else
			{
				double* value = VariableHandler::GetReal(bytesText.c_str());

				if (value == nullptr)
				{
					Logger::Error("variable \"" + bytesText + "\" does not exist");
					return false;
				}

				bytes = *value;
			}

			double* value = VariableHandler::GetReal(varText.c_str());
			if (value == nullptr)
			{
				Logger::Error("variable \"" + varText + "\" does not exist");
				return false;
			}

			address = unsigned long long(*value);

			bool result = ByteHandler::DeallocateBytes(address, bytes);
			if (!result)
			{
				Logger::Error("failed to deallocate " + std::to_string(bytes) + " bytes at " + std::to_string(address));
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
		BlockArgument arg3 = BlockArgument();

		arg0.SetupTEXT("set byte at");
		arg1.SetupREAL("1var");
		arg2.SetupTEXT("to");
		arg3.SetupREAL("00");

		argList.push_back(arg0);
		argList.push_back(arg1);
		argList.push_back(arg2);
		argList.push_back(arg3);

		block->Args = argList;
		block->Catagory = "vin_bytes";
		block->UnlocalizedName = "vin_byte_set";

		std::function<bool(std::vector<std::string>*)>* execute = new std::function<bool(std::vector<std::string>*)>();
		*execute = [](std::vector<std::string>* args)
		{
			std::string addressText = (*args)[0].substr(1, (*args)[0].length() - 1);
			std::string valueText = (*args)[1].substr(1, (*args)[1].length() - 1);

			if ((*args)[0][0] == '0')
			{
				Logger::Error("expecting variable only! got \"" + addressText + "\"");
				return false;
			}

			unsigned long long address;
			unsigned char charValue;

			if ((*args)[1][0] == '0')
			{
				try
				{
					charValue = std::stoull(valueText);
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

				charValue = *value;
			}

			double* addressVar = VariableHandler::GetReal(addressText.c_str());

			if (addressVar == nullptr)
			{
				Logger::Error("variable \"" + addressText + "\" does not exist");
				return false;
			}

			address = unsigned long long(*addressVar);

			bool result = ByteHandler::SetByte(address, charValue);
			if (!result)
			{
				Logger::Error("failed to set byte " + std::to_string(charValue) + " at address " + std::to_string(address));
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
		BlockArgument arg3 = BlockArgument();

		arg0.SetupTEXT("get byte at");
		arg1.SetupREAL("1var");
		arg2.SetupTEXT("for");
		arg3.SetupREAL("1var");

		argList.push_back(arg0);
		argList.push_back(arg1);
		argList.push_back(arg2);
		argList.push_back(arg3);

		block->Args = argList;
		block->Catagory = "vin_bytes";
		block->UnlocalizedName = "vin_byte_get";

		std::function<bool(std::vector<std::string>*)>* execute = new std::function<bool(std::vector<std::string>*)>();
		*execute = [](std::vector<std::string>* args)
		{
			std::string addressText = (*args)[0].substr(1, (*args)[0].length() - 1);
			std::string varText = (*args)[1].substr(1, (*args)[1].length() - 1);

			if ((*args)[0][0] == '0' || (*args)[1][0] == '0')
			{
				Logger::Error("expecting variable only! got \"" + addressText + "\"");
				return false;
			}

			unsigned long long address;

			double* addressVar = VariableHandler::GetReal(addressText.c_str());

			if (addressVar == nullptr)
			{
				Logger::Error("variable \"" + addressText + "\" does not exist");
				return false;
			}

			address = unsigned long long(*addressVar);

			unsigned char* gotByte = ByteHandler::GetByte(address);
			if (gotByte == nullptr)
			{
				Logger::Error("failed to get byte at " + std::to_string(address) + " for variable \"" + varText + "\"");
				return false;
			}

			bool result = VariableHandler::SetReal(varText.c_str(), *gotByte);
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