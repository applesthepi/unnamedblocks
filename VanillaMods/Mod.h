#pragma once
#include <RHR/RHR.h>

#ifdef LINUX
#define UB_EXPORT extern "C"
#else
#define UB_EXPORT extern "C" __declspec(dllexport)
#endif

UB_EXPORT void Initialization(ModData* data)
{
	data->RegisterCatagory(RegCatagory("vin_objects", "objects", sf::Color(237, 231, 109)));
	data->RegisterCatagory(RegCatagory("vin_textures", "textures", sf::Color(68, 212, 68)));
	data->RegisterCatagory(RegCatagory("vin_transform", "transform", sf::Color(70, 200, 200)));
	data->RegisterCatagory(RegCatagory("vin_bytes", "bytes", sf::Color(140, 200, 170)));
	data->RegisterCatagory(RegCatagory("vin_variables", "variables", sf::Color(255, 161, 74)));
	data->RegisterCatagory(RegCatagory("vin_threading", "threading", sf::Color(110, 125, 230)));
	data->RegisterCatagory(RegCatagory("vin_operations", "operations", sf::Color(73, 143, 76)));
	data->RegisterCatagory(RegCatagory("vin_execution", "execution", sf::Color(255, 113, 74)));
	data->RegisterCatagory(RegCatagory("vin_input", "input", sf::Color(123, 188, 199)));

	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			return VariableHandler::SetReal(args[0].c_str(), ObjectHandler::CreateObject(new RuntimeObject()));
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_object_new", "vin_objects", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "new object"},
			{BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "obj"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			return ObjectHandler::DestroyObject(std::stod(args[0]));
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_object_delete", "vin_objects", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "destroy object"},
			{BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "obj"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			return VariableHandler::SetReal(args[1].c_str(), ByteHandler::AllocateBytes(std::stod(args[0])));
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_byte_alloc", "vin_bytes", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "alloc"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "1"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "bytes to"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			return ByteHandler::DeallocateBytes(std::stod(args[1]), std::stod(args[0]));
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_byte_free", "vin_bytes", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "free"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "1"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "bytes at"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			return ByteHandler::SetByte(std::stod(args[0]), std::min((uint64_t)255, (uint64_t)std::stod(args[1])));
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_byte_set", "vin_bytes", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "set byte at"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "bytes to"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "0"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			unsigned char* bytes = ByteHandler::GetByte(std::stod(args[0]));

			if (bytes == nullptr)
				return false;

			return VariableHandler::SetReal(args[1].c_str(), *bytes);
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_byte_get", "vin_bytes", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "get byte at"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "for"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			Logger::Error("this block is not meant to be executed");
			return false;
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_execution_mark", "vin_execution", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "mark point"},
			{BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "point"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			Logger::Error("this block is not meant to be executed");
			return false;
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_execution_goto", "vin_execution", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "goto point"},
			{BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "point"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			Logger::Error("this block is not meant to be executed");
			return false;
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_execution_if", "vin_execution", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "goto point"},
			{BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "point"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "if"},
			{BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "1"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			Logger::Error("this block is not meant to be executed");
			return false;
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_execution_block_sync", "vin_execution", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "enable block sync"},
			{BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "1"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			Logger::Error("this block is not meant to be executed");
			return false;
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_execution_block_manual_render", "vin_execution", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "enable manual rendering"},
			{BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "1"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			Logger::Error("this block is not meant to be executed");
			return false;
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_execution_block_render_frame", "vin_execution", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "render frame"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			return VariableHandler::SetBool(args[0].c_str(), sf::Mouse::isButtonPressed(sf::Mouse::Left));
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_input_mouse_left", "vin_input", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "mouse left"},
			{BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			return VariableHandler::SetBool(args[0].c_str(), sf::Mouse::isButtonPressed(sf::Mouse::Right));
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_input_mouse_right", "vin_input", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "mouse right"},
			{BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			return VariableHandler::SetBool(args[0].c_str(), sf::Mouse::isButtonPressed(sf::Mouse::Middle));
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_input_mouse_middle", "vin_input", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "mouse middle"},
			{BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			return VariableHandler::SetReal(args[0].c_str(), sf::Mouse::getPosition(*RuntimeHandler::Window).x);
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_input_mouse_x", "vin_input", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "mouse x"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			return VariableHandler::SetReal(args[0].c_str(), sf::Mouse::getPosition(*RuntimeHandler::Window).y);
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_input_mouse_y", "vin_input", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "mouse y"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			int value = RuntimeHandler::GetScrolled();
			RuntimeHandler::ResetScrolled();

			return VariableHandler::SetReal(args[0].c_str(), value);
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_input_mouse_scroll", "vin_input", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "pull mouse scroll"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			RuntimeHandler::ResetScrolled();
			return true;
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_input_mouse_scroll_reset", "vin_input", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "reset mouse scroll"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			return VariableHandler::SetBool(args[1].c_str(), sf::Keyboard::isKeyPressed((sf::Keyboard::Key)(uint64_t)floor(std::stod(args[0]))));
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_input_mouse_key", "vin_input", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "key down"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "0"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "for"},
			{BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			double* gotValue = VariableHandler::GetReal(args[0].c_str());

			if (gotValue == nullptr)
				return false;

			return VariableHandler::SetReal(args[0].c_str(), *gotValue + std::stod(args[1]));
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_operations_+=", "vin_operations", execution, {
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "+="},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "1"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			double* gotValue = VariableHandler::GetReal(args[0].c_str());

			if (gotValue == nullptr)
				return false;

			return VariableHandler::SetReal(args[0].c_str(), *gotValue - std::stod(args[1]));
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_operations_-=", "vin_operations", execution, {
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "-="},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "1"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			double* gotValue = VariableHandler::GetReal(args[0].c_str());

			if (gotValue == nullptr)
				return false;

			return VariableHandler::SetReal(args[0].c_str(), *gotValue * std::stod(args[1]));
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_operations_*=", "vin_operations", execution, {
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "*="},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "1"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			double* gotValue = VariableHandler::GetReal(args[0].c_str());

			if (gotValue == nullptr)
				return false;

			return VariableHandler::SetReal(args[0].c_str(), *gotValue / std::stod(args[1]));
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_operations_/=", "vin_operations", execution, {
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "/="},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "1"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			double* gotValue = VariableHandler::GetReal(args[0].c_str());

			if (gotValue == nullptr)
				return false;

			return VariableHandler::SetReal(args[0].c_str(), (uint64_t)floor(*gotValue) % (uint64_t)floor(std::stod(args[1])));
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_operations_%=", "vin_operations", execution, {
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "%="},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "1"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			return VariableHandler::SetReal(args[0].c_str(), std::stod(args[1]));
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_operations_=_real", "vin_operations", execution, {
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "="},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "0"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			return VariableHandler::SetString(args[0].c_str(), args[1]);
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_operations_=_string", "vin_operations", execution, {
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "="},
			{BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, ""}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			return VariableHandler::SetBool(args[0].c_str(), args[1] == "1");
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_operations_=_bool", "vin_operations", execution, {
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "="},
			{BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "1"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			return VariableHandler::SetBool(args[0].c_str(), args[1] == "0");
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_operations_!=_bool", "vin_operations", execution, {
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "!="},
			{BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "1"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			double* gotValue = VariableHandler::GetReal(args[0].c_str());

			if (gotValue == nullptr)
				return false;

			return VariableHandler::SetReal(args[0].c_str(), floor(*gotValue));
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_operations_floor", "vin_operations", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "floor"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			double* gotValue = VariableHandler::GetReal(args[0].c_str());

			if (gotValue == nullptr)
				return false;

			return VariableHandler::SetReal(args[0].c_str(), round(*gotValue));
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_operations_round", "vin_operations", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "round"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			double* gotValue = VariableHandler::GetReal(args[0].c_str());

			if (gotValue == nullptr)
				return false;

			return VariableHandler::SetReal(args[0].c_str(), ceil(*gotValue));
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_operations_ceil", "vin_operations", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "ceil"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			double* gotValue0 = VariableHandler::GetReal(args[0].c_str());
			double* gotValue1 = VariableHandler::GetReal(args[1].c_str());

			if (gotValue0 == nullptr || gotValue1 == nullptr)
				return false;

			return VariableHandler::SetBool(args[2].c_str(), *gotValue0 == *gotValue1 ? "1" : "0");
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_operations_==_real", "vin_operations", execution, {
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "=="},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "for"},
			{BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			std::string* gotValue0 = VariableHandler::GetString(args[0].c_str());
			std::string* gotValue1 = VariableHandler::GetString(args[1].c_str());

			if (gotValue0 == nullptr || gotValue1 == nullptr)
				return false;

			return VariableHandler::SetBool(args[2].c_str(), *gotValue0 == *gotValue1 ? "1" : "0");
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_operations_==_string", "vin_operations", execution, {
			{BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "=="},
			{BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "for"},
			{BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			double* gotValue0 = VariableHandler::GetReal(args[0].c_str());
			double* gotValue1 = VariableHandler::GetReal(args[1].c_str());

			if (gotValue0 == nullptr || gotValue1 == nullptr)
				return false;

			return VariableHandler::SetBool(args[2].c_str(), *gotValue0 != *gotValue1 ? "1" : "0");
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_operations_!=_real", "vin_operations", execution, {
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "!="},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "for"},
			{BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			std::string* gotValue0 = VariableHandler::GetString(args[0].c_str());
			std::string* gotValue1 = VariableHandler::GetString(args[1].c_str());

			if (gotValue0 == nullptr || gotValue1 == nullptr)
				return false;

			return VariableHandler::SetBool(args[2].c_str(), *gotValue0 != *gotValue1 ? "1" : "0");
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_operations_!=_string", "vin_operations", execution, {
			{BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "!="},
			{BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "for"},
			{BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			double* gotValue0 = VariableHandler::GetReal(args[0].c_str());
			double* gotValue1 = VariableHandler::GetReal(args[1].c_str());

			if (gotValue0 == nullptr || gotValue1 == nullptr)
				return false;

			return VariableHandler::SetBool(args[2].c_str(), *gotValue0 > *gotValue1 ? "1" : "0");
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_operations_>_real", "vin_operations", execution, {
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, ">"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "for"},
			{BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			double* gotValue0 = VariableHandler::GetReal(args[0].c_str());
			double* gotValue1 = VariableHandler::GetReal(args[1].c_str());

			if (gotValue0 == nullptr || gotValue1 == nullptr)
				return false;

			return VariableHandler::SetBool(args[2].c_str(), *gotValue0 >= * gotValue1 ? "1" : "0");
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_operations_>=_real", "vin_operations", execution, {
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, ">="},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "for"},
			{BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			double* gotValue0 = VariableHandler::GetReal(args[0].c_str());
			double* gotValue1 = VariableHandler::GetReal(args[1].c_str());

			if (gotValue0 == nullptr || gotValue1 == nullptr)
				return false;

			return VariableHandler::SetBool(args[2].c_str(), *gotValue0 < * gotValue1 ? "1" : "0");
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_operations_<_real", "vin_operations", execution, {
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "<"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "for"},
			{BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			double* gotValue0 = VariableHandler::GetReal(args[0].c_str());
			double* gotValue1 = VariableHandler::GetReal(args[1].c_str());

			if (gotValue0 == nullptr || gotValue1 == nullptr)
				return false;

			return VariableHandler::SetBool(args[2].c_str(), *gotValue0 <= *gotValue1 ? "1" : "0");
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_operations_<=_real", "vin_operations", execution, {
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "<="},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "for"},
			{BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			RuntimeObject* run = ObjectHandler::GetObject(std::stod(args[1]));

			if (run == nullptr)
				return false;

			sf::Texture* txt = new sf::Texture();
			txt->loadFromFile(args[0]);

			sf::Sprite* sp = new sf::Sprite();
			sp->setTexture(*txt);

			std::unique_lock<std::shared_mutex> lock(ObjectHandler::ObjectMutex);

			run->Textures.push_back(txt);
			run->Sprites.push_back(sp);

			return true;
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_texture_add", "vin_textures", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "add texture"},
			{BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "path"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "for"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			RuntimeObject* run = ObjectHandler::GetObject(std::stod(args[1]));

			if (run == nullptr)
				return false;

			if ((uint64_t)std::stod(args[0]) >= run->Textures.size())
			{
				Logger::Error("can not remove texture \"" + args[0] + "\" of object \"" + args[1]);
				return false;
			}

			std::unique_lock<std::shared_mutex> lock(ObjectHandler::ObjectMutex);

			run->Textures.erase(run->Textures.begin() + (uint64_t)std::stod(args[0]));
			run->Sprites.erase(run->Sprites.begin() + (uint64_t)std::stod(args[0]));
			
			return true;
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_texture_remove", "vin_textures", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "remove texture at"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "0"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "for"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			RuntimeObject* run = ObjectHandler::GetObject(std::stod(args[1]));

			if (run == nullptr)
				return false;

			if ((uint64_t)std::stod(args[0]) > run->Textures.size())
			{
				Logger::Error("can not insert texture \"" + args[0] + "\" of object \"" + args[1]);
				return false;
			}

			sf::Texture* txt = new sf::Texture();
			txt->loadFromFile(args[2]);

			sf::Sprite* sp = new sf::Sprite();
			sp->setTexture(*txt);

			std::unique_lock<std::shared_mutex> lock(ObjectHandler::ObjectMutex);

			run->Textures.insert(run->Textures.begin() + (uint64_t)std::stod(args[0]), txt);
			run->Sprites.insert(run->Sprites.begin() + (uint64_t)std::stod(args[0]), sp);

			return true;
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_texture_insert", "vin_textures", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "insert texture at"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "0"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "for"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "with texture"},
			{BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "path"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			RuntimeObject* run = ObjectHandler::GetObject(std::stod(args[1]));

			if (run == nullptr)
				return false;

			if ((uint64_t)std::stod(args[0]) > run->Textures.size())
			{
				Logger::Error("can not replace texture \"" + args[0] + "\" of object \"" + args[1]);
				return false;
			}

			std::unique_lock<std::shared_mutex> lock(ObjectHandler::ObjectMutex);

			run->Textures[(uint64_t)std::stod(args[0])]->loadFromFile(args[2]);
			run->Sprites[(uint64_t)std::stod(args[0])]->setTexture(*run->Textures[(uint64_t)std::stod(args[0])]);

			return true;
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_texture_replace", "vin_textures", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "replace texture at"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "0"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "for"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "with texture"},
			{BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "path"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			RuntimeObject* run = ObjectHandler::GetObject(std::stod(args[1]));

			if (run == nullptr)
				return false;

			if ((uint64_t)std::stod(args[0]) > run->Textures.size())
			{
				Logger::Error("can not switch texture to \"" + args[0] + "\" of object \"" + args[1]);
				return false;
			}

			std::unique_lock<std::shared_mutex> lock(ObjectHandler::ObjectMutex);

			run->ImageIndex = (uint64_t)std::stod(args[0]);

			return true;
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_texture_switch", "vin_textures", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "switch texture to"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "0"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "for"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			return true;
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_thread_open", "vin_threading", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "open thread"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			std::this_thread::sleep_for(std::chrono::microseconds((uint64_t)floor(std::stod(args[0]) * 1000.0)));
			return true;
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_thread_sleep", "vin_threading", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "sleep thread"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "1000"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			Logger::Error("this block is not meant to be executed");
			return false;
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_thread_function_define", "vin_threading", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "function define"},
			{BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "function"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			Logger::Error("this block is not meant to be executed");
			return false;
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_thread_function_call", "vin_threading", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "function call"},
			{BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "function"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			int searchResult = RuntimeHandler::PerformFunctionSearch(args[0]);
			if (searchResult == -1)
			{
				Logger::Error("function \"" + args[0] + "\" does not exist");
				return false;
			}

			double threadId = ThreadHandler::SummonThread(searchResult);
			Logger::Debug("summoned thread " + std::to_string(threadId));

			if (!VariableHandler::SetReal(args[1].c_str(), threadId))
			{
				Logger::Warn("undocumented thread released! this may cause undefined behavior!");
				return false;
			}

			return true;
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_thread_function_thread", "vin_threading", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "function thread"},
			{BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "function"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "for"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "thread_id"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			Logger::Debug("killing thread " + args[0]);
			return ThreadHandler::KillThread((uint64_t)floor(std::stod(args[0])));
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_thread_kill_thread", "vin_threading", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "kill thread"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "thread_id"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			RuntimeObject* run = ObjectHandler::GetObject((uint64_t)floor(std::stod(args[0])));
			
			if (run == nullptr)
				return false;

			std::unique_lock<std::shared_mutex> lock(ObjectHandler::ObjectMutex);

			run->Position.x = std::stod(args[1]);
			run->Position.y = std::stod(args[2]);

			return true;
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_transform_translate_to", "vin_transform", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "translate"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "to"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "0"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "0"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			RuntimeObject* run = ObjectHandler::GetObject((uint64_t)floor(std::stod(args[0])));
			
			if (run == nullptr)
				return false;

			std::unique_lock<std::shared_mutex> lock(ObjectHandler::ObjectMutex);

			run->Position.x += std::stod(args[1]);
			run->Position.y += std::stod(args[2]);

			return true;
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_transform_translate_by", "vin_transform", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "translate"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "by"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "0"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "0"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			RuntimeObject* run = ObjectHandler::GetObject((uint64_t)floor(std::stod(args[0])));
			
			if (run == nullptr)
				return false;

			std::unique_lock<std::shared_mutex> lock(ObjectHandler::ObjectMutex);

			run->Scale.x = std::stod(args[1]);
			run->Scale.y = std::stod(args[2]);

			return true;
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_transform_scale_to", "vin_transform", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "scale"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "to"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "0"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "0"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			RuntimeObject* run = ObjectHandler::GetObject((uint64_t)floor(std::stod(args[0])));

			if (run == nullptr)
				return false;

			std::unique_lock<std::shared_mutex> lock(ObjectHandler::ObjectMutex);

			run->Scale.x += std::stod(args[1]);
			run->Scale.y += std::stod(args[2]);

			return true;
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_transform_scale_by", "vin_transform", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "scale"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "by"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "0"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "0"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			RuntimeObject* run = ObjectHandler::GetObject((uint64_t)floor(std::stod(args[0])));

			if (run == nullptr)
				return false;

			std::unique_lock<std::shared_mutex> lock(ObjectHandler::ObjectMutex);

			run->Angle = std::stod(args[1]);

			return true;
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_transform_rotate_to", "vin_transform", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "rotate"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "to"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "0"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			RuntimeObject* run = ObjectHandler::GetObject((uint64_t)floor(std::stod(args[0])));

			if (run == nullptr)
				return false;

			std::unique_lock<std::shared_mutex> lock(ObjectHandler::ObjectMutex);

			run->Angle += std::stod(args[1]);

			return true;
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_transform_rotate_by", "vin_transform", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "rotate"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "by"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "0"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			return VariableHandler::StackReal(args[0].c_str());
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_varaibles_stack_real", "vin_variables", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "stack real"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			return VariableHandler::StackString(args[0].c_str());
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_varaibles_stack_string", "vin_variables", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "stack string"},
			{BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			return VariableHandler::StackBool(args[0].c_str());
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_varaibles_stack_bool", "vin_variables", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "stack bool"},
			{BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			VariableHandler::HeapReal(args[0].c_str());
			return true;
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_varaibles_heap_real", "vin_variables", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "heap real"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			VariableHandler::HeapString(args[0].c_str());
			return true;
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_varaibles_heap_string", "vin_variables", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "heap string"},
			{BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			VariableHandler::HeapBool(args[0].c_str());
			return true;
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_varaibles_heap_bool", "vin_variables", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "heap bool"},
			{BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			return VariableHandler::FreeReal(args[0].c_str());
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_varaibles_free_real", "vin_variables", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "free real"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			return VariableHandler::FreeString(args[0].c_str());
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_varaibles_free_string", "vin_variables", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "free string"},
			{BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			return VariableHandler::FreeBool(args[0].c_str());
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_varaibles_free_bool", "vin_variables", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "free bool"},
			{BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			Logger::Info("[REAL] " + args[0]);
			return true;
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_varaibles_log_real", "vin_variables", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "log real"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			Logger::Info("[STRING] " + args[0]);
			return true;
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_varaibles_log_string", "vin_variables", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "log string"},
			{BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			Logger::Info(std::string("[BOOL] ") + (args[0] == "1" ? "true" : "false"));
			return true;
		};
		RegBlock* block = BlockRegistry::CreateBlock("vin_varaibles_log_bool", "vin_variables", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "log bool"},
			{BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
}
