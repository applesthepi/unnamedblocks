#pragma once

#include <RHR/RHR.h>

void ContentLoadTransform()
{
	{
		RegBlock* block = new RegBlock();

		std::vector<BlockArgument> argList = std::vector<BlockArgument>();

		BlockArgument arg0 = BlockArgument();
		BlockArgument arg1 = BlockArgument();
		BlockArgument arg2 = BlockArgument();
		BlockArgument arg3 = BlockArgument();
		BlockArgument arg4 = BlockArgument();

		arg0.SetupTEXT("translate");
		arg1.SetupREAL("1var");
		arg2.SetupTEXT("to");
		arg3.SetupREAL("00");
		arg4.SetupREAL("00");

		argList.push_back(arg0);
		argList.push_back(arg1);
		argList.push_back(arg2);
		argList.push_back(arg3);
		argList.push_back(arg4);

		block->Args = argList;
		block->Catagory = "vin_transform";
		block->UnlocalizedName = "vin_transform_translate_to";

		std::function<bool(std::vector<std::string>*)>* execute = new std::function<bool(std::vector<std::string>*)>();
		*execute = [](std::vector<std::string>* args)
		{
			std::string objText = (*args)[0].substr(1, (*args)[0].length() - 1);
			std::string xText = (*args)[1].substr(1, (*args)[1].length() - 1);
			std::string yText = (*args)[2].substr(1, (*args)[2].length() - 1);

			if ((*args)[0][0] == '0')
			{
				Logger::Error("expecting variable only! got \"" + objText + "\"");
				return false;
			}

			float x;
			float y;

			if ((*args)[1][0] == '0')
			{
				try
				{
					x = std::stod(xText);
				}
				catch (std::invalid_argument & e)
				{
					Logger::Error("invalid argument exception: expected real but got \"" + xText + "\"");
					return false;
				}
			}
			else
			{
				double* value = VariableHandler::GetReal(xText.c_str());

				if (value == nullptr)
				{
					Logger::Error("variable \"" + xText + "\" does not exist");
					return false;
				}

				x = *value;
			}

			if ((*args)[2][0] == '0')
			{
				try
				{
					y = std::stod(yText);
				}
				catch (std::invalid_argument & e)
				{
					Logger::Error("invalid argument exception: expected real but got \"" + yText + "\"");
					return false;
				}
			}
			else
			{
				double* value = VariableHandler::GetReal(yText.c_str());

				if (value == nullptr)
				{
					Logger::Error("variable \"" + yText + "\" does not exist");
					return false;
				}

				y = *value;
			}

			double* objectId = VariableHandler::GetReal(objText.c_str());
			if (objectId == nullptr)
			{
				Logger::Error("variable \"" + objText + "\" does not exist");
				return false;
			}

			RuntimeObject* run = ObjectHandler::GetObject(*objectId);
			if (run == nullptr)
			{
				Logger::Error("object \"" + std::to_string(*objectId) + "\" does not exist");
				return false;
			}
			
			run->Position.x = x;
			run->Position.y = y;

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
		BlockArgument arg4 = BlockArgument();

		arg0.SetupTEXT("translate");
		arg1.SetupREAL("1var");
		arg2.SetupTEXT("by");
		arg3.SetupREAL("00");
		arg4.SetupREAL("00");

		argList.push_back(arg0);
		argList.push_back(arg1);
		argList.push_back(arg2);
		argList.push_back(arg3);
		argList.push_back(arg4);

		block->Args = argList;
		block->Catagory = "vin_transform";
		block->UnlocalizedName = "vin_transform_translate_by";

		std::function<bool(std::vector<std::string>*)>* execute = new std::function<bool(std::vector<std::string>*)>();
		*execute = [](std::vector<std::string>* args)
		{
			std::string objText = (*args)[0].substr(1, (*args)[0].length() - 1);
			std::string xText = (*args)[1].substr(1, (*args)[1].length() - 1);
			std::string yText = (*args)[2].substr(1, (*args)[2].length() - 1);

			if ((*args)[0][0] == '0')
			{
				Logger::Error("expecting variable only! got \"" + objText + "\"");
				return false;
			}

			float x;
			float y;

			if ((*args)[1][0] == '0')
			{
				try
				{
					x = std::stod(xText);
				}
				catch (std::invalid_argument & e)
				{
					Logger::Error("invalid argument exception: expected real but got \"" + xText + "\"");
					return false;
				}
			}
			else
			{
				double* value = VariableHandler::GetReal(xText.c_str());

				if (value == nullptr)
				{
					Logger::Error("variable \"" + xText + "\" does not exist");
					return false;
				}

				x = *value;
			}

			if ((*args)[2][0] == '0')
			{
				try
				{
					y = std::stod(yText);
				}
				catch (std::invalid_argument & e)
				{
					Logger::Error("invalid argument exception: expected real but got \"" + yText + "\"");
					return false;
				}
			}
			else
			{
				double* value = VariableHandler::GetReal(yText.c_str());

				if (value == nullptr)
				{
					Logger::Error("variable \"" + yText + "\" does not exist");
					return false;
				}

				y = *value;
			}

			double* objectId = VariableHandler::GetReal(objText.c_str());
			if (objectId == nullptr)
			{
				Logger::Error("variable \"" + objText + "\" does not exist");
				return false;
			}

			RuntimeObject* run = ObjectHandler::GetObject(*objectId);
			if (run == nullptr)
			{
				Logger::Error("object \"" + std::to_string(*objectId) + "\" does not exist");
				return false;
			}

			run->Position.x += x;
			run->Position.y += y;

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
		BlockArgument arg4 = BlockArgument();

		arg0.SetupTEXT("scale");
		arg1.SetupREAL("1var");
		arg2.SetupTEXT("to");
		arg3.SetupREAL("00");
		arg4.SetupREAL("00");

		argList.push_back(arg0);
		argList.push_back(arg1);
		argList.push_back(arg2);
		argList.push_back(arg3);
		argList.push_back(arg4);

		block->Args = argList;
		block->Catagory = "vin_transform";
		block->UnlocalizedName = "vin_transform_scale_to";

		std::function<bool(std::vector<std::string>*)>* execute = new std::function<bool(std::vector<std::string>*)>();
		*execute = [](std::vector<std::string>* args)
		{
			std::string objText = (*args)[0].substr(1, (*args)[0].length() - 1);
			std::string xText = (*args)[1].substr(1, (*args)[1].length() - 1);
			std::string yText = (*args)[2].substr(1, (*args)[2].length() - 1);

			if ((*args)[0][0] == '0')
			{
				Logger::Error("expecting variable only! got \"" + objText + "\"");
				return false;
			}

			float x;
			float y;

			if ((*args)[1][0] == '0')
			{
				try
				{
					x = std::stod(xText);
				}
				catch (std::invalid_argument & e)
				{
					Logger::Error("invalid argument exception: expected real but got \"" + xText + "\"");
					return false;
				}
			}
			else
			{
				double* value = VariableHandler::GetReal(xText.c_str());

				if (value == nullptr)
				{
					Logger::Error("variable \"" + xText + "\" does not exist");
					return false;
				}

				x = *value;
			}

			if ((*args)[2][0] == '0')
			{
				try
				{
					y = std::stod(yText);
				}
				catch (std::invalid_argument & e)
				{
					Logger::Error("invalid argument exception: expected real but got \"" + yText + "\"");
					return false;
				}
			}
			else
			{
				double* value = VariableHandler::GetReal(yText.c_str());

				if (value == nullptr)
				{
					Logger::Error("variable \"" + yText + "\" does not exist");
					return false;
				}

				y = *value;
			}

			double* objectId = VariableHandler::GetReal(objText.c_str());
			if (objectId == nullptr)
			{
				Logger::Error("variable \"" + objText + "\" does not exist");
				return false;
			}

			RuntimeObject* run = ObjectHandler::GetObject(*objectId);
			if (run == nullptr)
			{
				Logger::Error("object \"" + std::to_string(*objectId) + "\" does not exist");
				return false;
			}

			run->Scale.x = x;
			run->Scale.y = y;

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
		BlockArgument arg4 = BlockArgument();

		arg0.SetupTEXT("scale");
		arg1.SetupREAL("1var");
		arg2.SetupTEXT("by");
		arg3.SetupREAL("00");
		arg4.SetupREAL("00");

		argList.push_back(arg0);
		argList.push_back(arg1);
		argList.push_back(arg2);
		argList.push_back(arg3);
		argList.push_back(arg4);

		block->Args = argList;
		block->Catagory = "vin_transform";
		block->UnlocalizedName = "vin_transform_scale_by";

		std::function<bool(std::vector<std::string>*)>* execute = new std::function<bool(std::vector<std::string>*)>();
		*execute = [](std::vector<std::string>* args)
		{
			std::string objText = (*args)[0].substr(1, (*args)[0].length() - 1);
			std::string xText = (*args)[1].substr(1, (*args)[1].length() - 1);
			std::string yText = (*args)[2].substr(1, (*args)[2].length() - 1);

			if ((*args)[0][0] == '0')
			{
				Logger::Error("expecting variable only! got \"" + objText + "\"");
				return false;
			}

			float x;
			float y;

			if ((*args)[1][0] == '0')
			{
				try
				{
					x = std::stod(xText);
				}
				catch (std::invalid_argument & e)
				{
					Logger::Error("invalid argument exception: expected real but got \"" + xText + "\"");
					return false;
				}
			}
			else
			{
				double* value = VariableHandler::GetReal(xText.c_str());

				if (value == nullptr)
				{
					Logger::Error("variable \"" + xText + "\" does not exist");
					return false;
				}

				x = *value;
			}

			if ((*args)[2][0] == '0')
			{
				try
				{
					y = std::stod(yText);
				}
				catch (std::invalid_argument & e)
				{
					Logger::Error("invalid argument exception: expected real but got \"" + yText + "\"");
					return false;
				}
			}
			else
			{
				double* value = VariableHandler::GetReal(yText.c_str());

				if (value == nullptr)
				{
					Logger::Error("variable \"" + yText + "\" does not exist");
					return false;
				}

				y = *value;
			}

			double* objectId = VariableHandler::GetReal(objText.c_str());
			if (objectId == nullptr)
			{
				Logger::Error("variable \"" + objText + "\" does not exist");
				return false;
			}

			RuntimeObject* run = ObjectHandler::GetObject(*objectId);
			if (run == nullptr)
			{
				Logger::Error("object \"" + std::to_string(*objectId) + "\" does not exist");
				return false;
			}

			run->Scale.x += x;
			run->Scale.y += y;

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

		arg0.SetupTEXT("rotate");
		arg1.SetupREAL("1var");
		arg2.SetupTEXT("to");
		arg3.SetupREAL("00");

		argList.push_back(arg0);
		argList.push_back(arg1);
		argList.push_back(arg2);
		argList.push_back(arg3);

		block->Args = argList;
		block->Catagory = "vin_transform";
		block->UnlocalizedName = "vin_transform_rotate_to";

		std::function<bool(std::vector<std::string>*)>* execute = new std::function<bool(std::vector<std::string>*)>();
		*execute = [](std::vector<std::string>* args)
		{
			std::string objText = (*args)[0].substr(1, (*args)[0].length() - 1);
			std::string rotText = (*args)[1].substr(1, (*args)[1].length() - 1);

			if ((*args)[0][0] == '0')
			{
				Logger::Error("expecting variable only! got \"" + objText + "\"");
				return false;
			}

			float angle;

			if ((*args)[1][0] == '0')
			{
				try
				{
					angle = std::stod(rotText);
				}
				catch (std::invalid_argument & e)
				{
					Logger::Error("invalid argument exception: expected real but got \"" + rotText + "\"");
					return false;
				}
			}
			else
			{
				double* value = VariableHandler::GetReal(rotText.c_str());

				if (value == nullptr)
				{
					Logger::Error("variable \"" + rotText + "\" does not exist");
					return false;
				}

				angle = *value;
			}

			double* objectId = VariableHandler::GetReal(objText.c_str());
			if (objectId == nullptr)
			{
				Logger::Error("variable \"" + objText + "\" does not exist");
				return false;
			}

			RuntimeObject* run = ObjectHandler::GetObject(*objectId);
			if (run == nullptr)
			{
				Logger::Error("object \"" + std::to_string(*objectId) + "\" does not exist");
				return false;
			}

			run->Angle = angle;

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

		arg0.SetupTEXT("rotate");
		arg1.SetupREAL("1var");
		arg2.SetupTEXT("by");
		arg3.SetupREAL("00");

		argList.push_back(arg0);
		argList.push_back(arg1);
		argList.push_back(arg2);
		argList.push_back(arg3);

		block->Args = argList;
		block->Catagory = "vin_transform";
		block->UnlocalizedName = "vin_transform_rotate_by";

		std::function<bool(std::vector<std::string>*)>* execute = new std::function<bool(std::vector<std::string>*)>();
		*execute = [](std::vector<std::string>* args)
		{
			std::string objText = (*args)[0].substr(1, (*args)[0].length() - 1);
			std::string rotText = (*args)[1].substr(1, (*args)[1].length() - 1);

			if ((*args)[0][0] == '0')
			{
				Logger::Error("expecting variable only! got \"" + objText + "\"");
				return false;
			}

			float angle;

			if ((*args)[1][0] == '0')
			{
				try
				{
					angle = std::stod(rotText);
				}
				catch (std::invalid_argument & e)
				{
					Logger::Error("invalid argument exception: expected real but got \"" + rotText + "\"");
					return false;
				}
			}
			else
			{
				double* value = VariableHandler::GetReal(rotText.c_str());

				if (value == nullptr)
				{
					Logger::Error("variable \"" + rotText + "\" does not exist");
					return false;
				}

				angle = *value;
			}

			double* objectId = VariableHandler::GetReal(objText.c_str());
			if (objectId == nullptr)
			{
				Logger::Error("variable \"" + objText + "\" does not exist");
				return false;
			}

			RuntimeObject* run = ObjectHandler::GetObject(*objectId);
			if (run == nullptr)
			{
				Logger::Error("object \"" + std::to_string(*objectId) + "\" does not exist");
				return false;
			}

			run->Angle += angle;

			return true;
		};

		block->Execute = execute;
		BlockRegistry::CreateBlock(*block);
	}
}