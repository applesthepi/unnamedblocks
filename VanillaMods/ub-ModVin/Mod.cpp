#include "Mod.h"//i dont care to figure out why im getting linker errors if this is above the RHR include
//#include <RHR/RHR.h>


UB_EXPORT void Initialization(ModData* data)
{
	data->RegisterCatagory(RegCatagory("~", "", sf::Color(222, 102, 222)));
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
			Logger::Error("executed a null block!");
			return false;
		};
		RegBlock* block = data->Registry->CreateBlock("vin_null", "~", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "NULL"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&, const std::string&)>* execution = new std::function<bool(const std::vector<std::string>&, const std::string&)>();
		*execution = [data](const std::vector<std::string>& args, const std::string& idx)
		{
			return data->Variable->SetReal((idx + args[0]).c_str(), data->Object->CreateObject(new RuntimeObject()));
		};
		RegBlock* block = data->Registry->CreateBlock("vin_object_new", "vin_objects", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "new object"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "obj"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [data](const std::vector<std::string>& args)
		{
			return data->Object->DestroyObject(std::stod(args[0]));
		};
		RegBlock* block = data->Registry->CreateBlock("vin_object_delete", "vin_objects", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "destroy object"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "obj"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&, const std::string&)>* execution = new std::function<bool(const std::vector<std::string>&, const std::string&)>();
		*execution = [data](const std::vector<std::string>& args, const std::string& idx)
		{
			return data->Variable->SetReal((idx + args[1]).c_str(), data->Byte->AllocateBytes(std::stod(args[0])));
		};
		RegBlock* block = data->Registry->CreateBlock("vin_byte_alloc", "vin_bytes", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "alloc"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "1"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "bytes to"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [data](const std::vector<std::string>& args)
		{
			return data->Byte->DeallocateBytes(std::stod(args[1]), std::stod(args[0]));
		};
		RegBlock* block = data->Registry->CreateBlock("vin_byte_free", "vin_bytes", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "free"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "1"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "bytes at"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [data](const std::vector<std::string>& args)
		{
			return data->Byte->SetByte(std::stod(args[0]), std::min((uint64_t)255, (uint64_t)std::stod(args[1])));
		};
		RegBlock* block = data->Registry->CreateBlock("vin_byte_set", "vin_bytes", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "set byte at"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "bytes to"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "0"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&, const std::string&)>* execution = new std::function<bool(const std::vector<std::string>&, const std::string&)>();
		*execution = [data](const std::vector<std::string>& args, const std::string& idx)
		{
			unsigned char* bytes = data->Byte->GetByte(std::stod(args[0]));

			if (bytes == nullptr)
				return false;

			return data->Variable->SetReal((idx + args[1]).c_str(), *bytes);
		};
		RegBlock* block = data->Registry->CreateBlock("vin_byte_get", "vin_bytes", execution, {
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
		RegBlock* block = data->Registry->CreateBlock("vin_execution_if", "vin_execution", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "if"},
			{BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "condition"}
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
		RegBlock* block = data->Registry->CreateBlock("vin_execution_if_end", "vin_execution", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "endif"}
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
		RegBlock* block = data->Registry->CreateBlock("vin_execution_else", "vin_execution", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "else"}
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
		RegBlock* block = data->Registry->CreateBlock("vin_execution_else_if", "vin_execution", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "else if"},
			{BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "condition"}
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
		RegBlock* block = data->Registry->CreateBlock("vin_execution_mark", "vin_execution", execution, {
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
		RegBlock* block = data->Registry->CreateBlock("vin_execution_mutex_create", "vin_execution", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "create mutex"},
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
		RegBlock* block = data->Registry->CreateBlock("vin_execution_mutex_free", "vin_execution", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "free mutex"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "var"}
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
		RegBlock* block = data->Registry->CreateBlock("vin_execution_mutex_shared_lock", "vin_execution", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "mutex shared lock"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "var"}
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
		RegBlock* block = data->Registry->CreateBlock("vin_execution_mutex_unique_lock", "vin_execution", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "mutex unique lock"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "var"}
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
		RegBlock* block = data->Registry->CreateBlock("vin_execution_mutex_shared_release", "vin_execution", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "mutex shared release"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "var"}
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
		RegBlock* block = data->Registry->CreateBlock("vin_execution_mutex_unique_release", "vin_execution", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "mutex unique release"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "var"}
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
		RegBlock* block = data->Registry->CreateBlock("vin_execution_goto", "vin_execution", execution, {
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
		RegBlock* block = data->Registry->CreateBlock("vin_execution_goto_if", "vin_execution", execution, {
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
		RegBlock* block = data->Registry->CreateBlock("vin_execution_block_sync", "vin_execution", execution, {
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
		RegBlock* block = data->Registry->CreateBlock("vin_execution_block_manual_render", "vin_execution", execution, {
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
		RegBlock* block = data->Registry->CreateBlock("vin_execution_block_render_frame", "vin_execution", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "render frame"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&, const std::string&)>* execution = new std::function<bool(const std::vector<std::string>&, const std::string&)>();
		*execution = [data](const std::vector<std::string>& args, const std::string& idx)
		{
			return data->Variable->SetBool((idx + args[0]).c_str(), sf::Mouse::isButtonPressed(sf::Mouse::Left));
		};
		RegBlock* block = data->Registry->CreateBlock("vin_input_mouse_left", "vin_input", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "mouse left"},
			{BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&, const std::string&)>* execution = new std::function<bool(const std::vector<std::string>&, const std::string&)>();
		*execution = [data](const std::vector<std::string>& args, const std::string& idx)
		{
			return data->Variable->SetBool((idx + args[0]).c_str(), sf::Mouse::isButtonPressed(sf::Mouse::Right));
		};
		RegBlock* block = data->Registry->CreateBlock("vin_input_mouse_right", "vin_input", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "mouse right"},
			{BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&, const std::string&)>* execution = new std::function<bool(const std::vector<std::string>&, const std::string&)>();
		*execution = [data](const std::vector<std::string>& args, const std::string& idx)
		{
			return data->Variable->SetBool((idx + args[0]).c_str(), sf::Mouse::isButtonPressed(sf::Mouse::Middle));
		};
		RegBlock* block = data->Registry->CreateBlock("vin_input_mouse_middle", "vin_input", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "mouse middle"},
			{BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&, const std::string&)>* execution = new std::function<bool(const std::vector<std::string>&, const std::string&)>();
		*execution = [data](const std::vector<std::string>& args, const std::string& idx)
		{
			return data->Variable->SetReal((idx + args[0]).c_str(), sf::Mouse::getPosition(*data->Runtime->Window).x);
		};
		RegBlock* block = data->Registry->CreateBlock("vin_input_mouse_x", "vin_input", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "mouse x"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&, const std::string&)>* execution = new std::function<bool(const std::vector<std::string>&, const std::string&)>();
		*execution = [data](const std::vector<std::string>& args, const std::string& idx)
		{
			return data->Variable->SetReal((idx + args[0]).c_str(), sf::Mouse::getPosition(*data->Runtime->Window).y);
		};
		RegBlock* block = data->Registry->CreateBlock("vin_input_mouse_y", "vin_input", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "mouse y"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&, const std::string&)>* execution = new std::function<bool(const std::vector<std::string>&, const std::string&)>();
		*execution = [data](const std::vector<std::string>& args, const std::string& idx)
		{
			int value = data->Runtime->GetScrolled();
			data->Runtime->ResetScrolled();

			return data->Variable->SetReal((idx + args[0]).c_str(), value);
		};
		RegBlock* block = data->Registry->CreateBlock("vin_input_mouse_scroll", "vin_input", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "pull mouse scroll"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [data](const std::vector<std::string>& args)
		{
			data->Runtime->ResetScrolled();
			return true;
		};
		RegBlock* block = data->Registry->CreateBlock("vin_input_mouse_scroll_reset", "vin_input", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "reset mouse scroll"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&, const std::string&)>* execution = new std::function<bool(const std::vector<std::string>&, const std::string&)>();
		*execution = [data](const std::vector<std::string>& args, const std::string& idx)
		{
			return data->Variable->SetBool((idx + args[1]).c_str(), sf::Keyboard::isKeyPressed((sf::Keyboard::Key)(uint64_t)floor(std::stod(args[0]))));
		};
		RegBlock* block = data->Registry->CreateBlock("vin_input_mouse_key", "vin_input", execution, {
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
			Logger::Error("this block is not meant to be executed");
			return false;
		};
		RegBlock* block = data->Registry->CreateBlock("vin_input_timer_get", "vin_input", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "get timer"},
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
		RegBlock* block = data->Registry->CreateBlock("vin_input_timer_reset", "vin_input", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "reset timer"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&, const std::string&)>* execution = new std::function<bool(const std::vector<std::string>&, const std::string&)>();
		*execution = [data](const std::vector<std::string>& args, const std::string& idx)
		{
			double* gotValue = data->Variable->GetReal((idx + args[0]).c_str());

			if (gotValue == nullptr)
				return false;

			return data->Variable->SetReal((idx + args[0]).c_str(), *gotValue + std::stod(args[1]));
		};
		RegBlock* block = data->Registry->CreateBlock("vin_operations_+=", "vin_operations", execution, {
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "+="},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "1"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&, const std::string&)>* execution = new std::function<bool(const std::vector<std::string>&, const std::string&)>();
		*execution = [data](const std::vector<std::string>& args, const std::string& idx)
		{
			double* gotValue = data->Variable->GetReal((idx + args[0]).c_str());

			if (gotValue == nullptr)
				return false;

			return data->Variable->SetReal((idx + args[0]).c_str(), *gotValue - std::stod(args[1]));
		};
		RegBlock* block = data->Registry->CreateBlock("vin_operations_-=", "vin_operations", execution, {
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "-="},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "1"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&, const std::string&)>* execution = new std::function<bool(const std::vector<std::string>&, const std::string&)>();
		*execution = [data](const std::vector<std::string>& args, const std::string& idx)
		{
			double* gotValue = data->Variable->GetReal((idx + args[0]).c_str());

			if (gotValue == nullptr)
				return false;

			return data->Variable->SetReal((idx + args[0]).c_str(), *gotValue * std::stod(args[1]));
		};
		RegBlock* block = data->Registry->CreateBlock("vin_operations_*=", "vin_operations", execution, {
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "*="},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "1"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&, const std::string&)>* execution = new std::function<bool(const std::vector<std::string>&, const std::string&)>();
		*execution = [data](const std::vector<std::string>& args, const std::string& idx)
		{
			double* gotValue = data->Variable->GetReal((idx + args[0]).c_str());

			if (gotValue == nullptr)
				return false;

			return data->Variable->SetReal((idx + args[0]).c_str(), *gotValue / std::stod(args[1]));
		};
		RegBlock* block = data->Registry->CreateBlock("vin_operations_/=", "vin_operations", execution, {
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "/="},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "1"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&, const std::string&)>* execution = new std::function<bool(const std::vector<std::string>&, const std::string&)>();
		*execution = [data](const std::vector<std::string>& args, const std::string& idx)
		{
			double* gotValue = data->Variable->GetReal((idx + args[0]).c_str());

			if (gotValue == nullptr)
				return false;

			return data->Variable->SetReal((idx + args[0]).c_str(), (uint64_t)floor(*gotValue) % (uint64_t)floor(std::stod(args[1])));
		};
		RegBlock* block = data->Registry->CreateBlock("vin_operations_%=", "vin_operations", execution, {
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "%="},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "1"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&, const std::string&)>* execution = new std::function<bool(const std::vector<std::string>&, const std::string&)>();
		*execution = [data](const std::vector<std::string>& args, const std::string& idx)
		{
			return data->Variable->SetReal((idx + args[0]).c_str(), std::stod(args[1]));
		};
		RegBlock* block = data->Registry->CreateBlock("vin_operations_=_real", "vin_operations", execution, {
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "="},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "0"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&, const std::string&)>* execution = new std::function<bool(const std::vector<std::string>&, const std::string&)>();
		*execution = [data](const std::vector<std::string>& args, const std::string& idx)
		{
			return data->Variable->SetString((idx + args[0]).c_str(), args[1]);
		};
		RegBlock* block = data->Registry->CreateBlock("vin_operations_=_string", "vin_operations", execution, {
			{BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "="},
			{BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, ""}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&, const std::string&)>* execution = new std::function<bool(const std::vector<std::string>&, const std::string&)>();
		*execution = [data](const std::vector<std::string>& args, const std::string& idx)
		{
			return data->Variable->SetBool((idx + args[0]).c_str(), args[1] == "1");
		};
		RegBlock* block = data->Registry->CreateBlock("vin_operations_=_bool", "vin_operations", execution, {
			{BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "="},
			{BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "1"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&, const std::string&)>* execution = new std::function<bool(const std::vector<std::string>&, const std::string&)>();
		*execution = [data](const std::vector<std::string>& args, const std::string& idx)
		{
			return data->Variable->SetBool((idx + args[0]).c_str(), args[1] == "0");
		};
		RegBlock* block = data->Registry->CreateBlock("vin_operations_!=_bool", "vin_operations", execution, {
			{BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "!="},
			{BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "1"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&, const std::string&)>* execution = new std::function<bool(const std::vector<std::string>&, const std::string&)>();
		*execution = [data](const std::vector<std::string>& args, const std::string& idx)
		{
			double* gotValue = data->Variable->GetReal((idx + args[0]).c_str());

			if (gotValue == nullptr)
				return false;

			return data->Variable->SetReal((idx + args[0]).c_str(), floor(*gotValue));
		};
		RegBlock* block = data->Registry->CreateBlock("vin_operations_floor", "vin_operations", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "floor"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&, const std::string&)>* execution = new std::function<bool(const std::vector<std::string>&, const std::string&)>();
		*execution = [data](const std::vector<std::string>& args, const std::string& idx)
		{
			double* gotValue = data->Variable->GetReal((idx + args[0]).c_str());

			if (gotValue == nullptr)
				return false;

			return data->Variable->SetReal((idx + args[0]).c_str(), round(*gotValue));
		};
		RegBlock* block = data->Registry->CreateBlock("vin_operations_round", "vin_operations", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "round"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&, const std::string&)>* execution = new std::function<bool(const std::vector<std::string>&, const std::string&)>();
		*execution = [data](const std::vector<std::string>& args, const std::string& idx)
		{
			double* gotValue = data->Variable->GetReal((idx + args[0]).c_str());

			if (gotValue == nullptr)
				return false;

			return data->Variable->SetReal((idx + args[0]).c_str(), ceil(*gotValue));
		};
		RegBlock* block = data->Registry->CreateBlock("vin_operations_ceil", "vin_operations", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "ceil"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&, const std::string&)>* execution = new std::function<bool(const std::vector<std::string>&, const std::string&)>();
		*execution = [data](const std::vector<std::string>& args, const std::string& idx)
		{
			return data->Variable->SetReal((idx + args[0]).c_str(), ((double)(rand() % 1000)) / 1000.0);
		};
		RegBlock* block = data->Registry->CreateBlock("vin_operations_rand", "vin_operations", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "random for"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&, const std::string&)>* execution = new std::function<bool(const std::vector<std::string>&, const std::string&)>();
		*execution = [data](const std::vector<std::string>& args, const std::string& idx)
		{
			double* gotValue = data->Variable->GetReal((idx + args[0]).c_str());

			if (gotValue == nullptr)
				return false;

			return data->Variable->SetReal((idx + args[0]).c_str(), abs(*gotValue));
		};
		RegBlock* block = data->Registry->CreateBlock("vin_operations_abs", "vin_operations", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "abs"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&, const std::string&)>* execution = new std::function<bool(const std::vector<std::string>&, const std::string&)>();
		*execution = [data](const std::vector<std::string>& args, const std::string& idx)
		{
			double* gotValue = data->Variable->GetReal((idx + args[0]).c_str());

			if (gotValue == nullptr)
				return false;

			return data->Variable->SetReal((idx + args[0]).c_str(), pow(*gotValue, std::stod(args[1])));
		};
		RegBlock* block = data->Registry->CreateBlock("vin_operations_pow", "vin_operations", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "pow"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "2"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&, const std::string&)>* execution = new std::function<bool(const std::vector<std::string>&, const std::string&)>();
		*execution = [data](const std::vector<std::string>& args, const std::string& idx)
		{
			return data->Variable->SetBool((idx + args[2]).c_str(), std::stod(args[0]) == std::stod(args[1]));
		};
		RegBlock* block = data->Registry->CreateBlock("vin_operations_==_real", "vin_operations", execution, {
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "=="},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "for"},
			{BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&, const std::string&)>* execution = new std::function<bool(const std::vector<std::string>&, const std::string&)>();
		*execution = [data](const std::vector<std::string>& args, const std::string& idx)
		{
			return data->Variable->SetBool((idx + args[2]).c_str(), args[0] == args[1]);
		};
		RegBlock* block = data->Registry->CreateBlock("vin_operations_==_string", "vin_operations", execution, {
			{BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "=="},
			{BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "for"},
			{BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&, const std::string&)>* execution = new std::function<bool(const std::vector<std::string>&, const std::string&)>();
		*execution = [data](const std::vector<std::string>& args, const std::string& idx)
		{
			return data->Variable->SetBool((idx + args[2]).c_str(), std::stod(args[0]) != std::stod(args[1]));
		};
		RegBlock* block = data->Registry->CreateBlock("vin_operations_!=_real", "vin_operations", execution, {
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "!="},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "for"},
			{BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&, const std::string&)>* execution = new std::function<bool(const std::vector<std::string>&, const std::string&)>();
		*execution = [data](const std::vector<std::string>& args, const std::string& idx)
		{
			return data->Variable->SetBool((idx + args[2]).c_str(), args[0] != args[1]);
		};
		RegBlock* block = data->Registry->CreateBlock("vin_operations_!=_string", "vin_operations", execution, {
			{BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "!="},
			{BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "for"},
			{BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&, const std::string&)>* execution = new std::function<bool(const std::vector<std::string>&, const std::string&)>();
		*execution = [data](const std::vector<std::string>& args, const std::string& idx)
		{
			return data->Variable->SetBool((idx + args[2]).c_str(), std::stod(args[0]) > std::stod(args[1]));
		};
		RegBlock* block = data->Registry->CreateBlock("vin_operations_>_real", "vin_operations", execution, {
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, ">"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "for"},
			{BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&, const std::string&)>* execution = new std::function<bool(const std::vector<std::string>&, const std::string&)>();
		*execution = [data](const std::vector<std::string>& args, const std::string& idx)
		{
			return data->Variable->SetBool((idx + args[2]).c_str(), std::stod(args[0]) >= std::stod(args[1]));
		};
		RegBlock* block = data->Registry->CreateBlock("vin_operations_>=_real", "vin_operations", execution, {
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, ">="},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "for"},
			{BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&, const std::string&)>* execution = new std::function<bool(const std::vector<std::string>&, const std::string&)>();
		*execution = [data](const std::vector<std::string>& args, const std::string& idx)
		{
			return data->Variable->SetBool((idx + args[2]).c_str(), std::stod(args[0]) < std::stod(args[1]));
		};
		RegBlock* block = data->Registry->CreateBlock("vin_operations_<_real", "vin_operations", execution, {
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "<"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "for"},
			{BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&, const std::string&)>* execution = new std::function<bool(const std::vector<std::string>&, const std::string&)>();
		*execution = [data](const std::vector<std::string>& args, const std::string& idx)
		{
			return data->Variable->SetBool((idx + args[2]).c_str(), std::stod(args[0]) <= std::stod(args[1]));
		};
		RegBlock* block = data->Registry->CreateBlock("vin_operations_<=_real", "vin_operations", execution, {
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "<="},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "for"},
			{BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&, const std::string&)>* execution = new std::function<bool(const std::vector<std::string>&, const std::string&)>();
		*execution = [data](const std::vector<std::string>& args, const std::string& idx)
		{
			return data->Variable->SetString((idx + args[1]).c_str(), args[0].c_str());
		};
		RegBlock* block = data->Registry->CreateBlock("vin_operations_rtos", "vin_operations", execution, {
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "r to s"},
			{BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&, const std::string&)>* execution = new std::function<bool(const std::vector<std::string>&, const std::string&)>();
		*execution = [data](const std::vector<std::string>& args, const std::string& idx)
		{
			return data->Variable->SetString((idx + args[1]).c_str(), args[0].c_str());
		};
		RegBlock* block = data->Registry->CreateBlock("vin_operations_btos", "vin_operations", execution, {
			{BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "b to s"},
			{BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&, const std::string&)>* execution = new std::function<bool(const std::vector<std::string>&, const std::string&)>();
		*execution = [data](const std::vector<std::string>& args, const std::string& idx)
		{
			return data->Variable->SetReal((idx + args[1]).c_str(), std::stod(args[0]));
		};
		RegBlock* block = data->Registry->CreateBlock("vin_operations_stor", "vin_operations", execution, {
			{BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "s to r"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&, const std::string&)>* execution = new std::function<bool(const std::vector<std::string>&, const std::string&)>();
		*execution = [data](const std::vector<std::string>& args, const std::string& idx)
		{
			return data->Variable->SetBool((idx + args[1]).c_str(), args[0] == "1");
		};
		RegBlock* block = data->Registry->CreateBlock("vin_operations_stob", "vin_operations", execution, {
			{BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "s to b"},
			{BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&, const std::string&)>* execution = new std::function<bool(const std::vector<std::string>&, const std::string&)>();
		*execution = [data](const std::vector<std::string>& args, const std::string& idx)
		{
			return data->Variable->SetReal((idx + args[1]).c_str(), args[0].length());
		};
		RegBlock* block = data->Registry->CreateBlock("vin_operations_strlen", "vin_operations", execution, {
			{BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "length for"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&, const std::string&)>* execution = new std::function<bool(const std::vector<std::string>&, const std::string&)>();
		*execution = [data](const std::vector<std::string>& args, const std::string& idx)
		{
			return data->Variable->SetString((idx + args[3]).c_str(), args[0].substr(std::stoull(args[1]), std::stoull(args[2])));
		};
		RegBlock* block = data->Registry->CreateBlock("vin_operations_substr", "vin_operations", execution, {
			{BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "sub str from"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "0"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "0"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "for"},
			{BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [data](const std::vector<std::string>& args)
		{
			RuntimeObject* run = data->Object->GetObject(std::stod(args[1]));

			if (run == nullptr)
				return false;

			sf::Texture* txt = new sf::Texture();
			txt->loadFromFile(args[0]);
			
			sf::Sprite* sp = new sf::Sprite();
			sp->setTexture(*txt);
			
			std::unique_lock<std::shared_timed_mutex> lock(data->Object->ObjectMutex);

			run->Textures.push_back(txt);
			run->Sprites.push_back(sp);

			return true;
		};
		RegBlock* block = data->Registry->CreateBlock("vin_texture_add", "vin_textures", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "add texture"},
			{BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "path"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "for"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [data](const std::vector<std::string>& args)
		{
			RuntimeObject* run = data->Object->GetObject(std::stod(args[1]));

			if (run == nullptr)
				return false;

			if ((uint64_t)std::stod(args[0]) >= run->Textures.size())
			{
				Logger::Error("can not remove texture \"" + args[0] + "\" of object \"" + args[1]);
				return false;
			}

			std::unique_lock<std::shared_timed_mutex> lock(data->Object->ObjectMutex);

			run->Textures.erase(run->Textures.begin() + (uint64_t)std::stod(args[0]));
			run->Sprites.erase(run->Sprites.begin() + (uint64_t)std::stod(args[0]));
			
			return true;
		};
		RegBlock* block = data->Registry->CreateBlock("vin_texture_remove", "vin_textures", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "remove texture at"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "0"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "for"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [data](const std::vector<std::string>& args)
		{
			RuntimeObject* run = data->Object->GetObject(std::stod(args[1]));

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

			std::unique_lock<std::shared_timed_mutex> lock(data->Object->ObjectMutex);

			run->Textures.insert(run->Textures.begin() + (uint64_t)std::stod(args[0]), txt);
			run->Sprites.insert(run->Sprites.begin() + (uint64_t)std::stod(args[0]), sp);

			return true;
		};
		RegBlock* block = data->Registry->CreateBlock("vin_texture_insert", "vin_textures", execution, {
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
		*execution = [data](const std::vector<std::string>& args)
		{
			RuntimeObject* run = data->Object->GetObject(std::stod(args[1]));

			if (run == nullptr)
				return false;

			if ((uint64_t)std::stod(args[0]) > run->Textures.size())
			{
				Logger::Error("can not replace texture \"" + args[0] + "\" of object \"" + args[1]);
				return false;
			}

			std::unique_lock<std::shared_timed_mutex> lock(data->Object->ObjectMutex);

			run->Textures[(uint64_t)std::stod(args[0])]->loadFromFile(args[2]);
			run->Sprites[(uint64_t)std::stod(args[0])]->setTexture(*run->Textures[(uint64_t)std::stod(args[0])], true);

			return true;
		};
		RegBlock* block = data->Registry->CreateBlock("vin_texture_replace", "vin_textures", execution, {
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
		*execution = [data](const std::vector<std::string>& args)
		{
			RuntimeObject* run = data->Object->GetObject(std::stod(args[1]));

			if (run == nullptr)
				return false;

			if ((uint64_t)std::stod(args[0]) > run->Textures.size())
			{
				Logger::Error("can not switch texture to \"" + args[0] + "\" of object \"" + args[1]);
				return false;
			}

			std::unique_lock<std::shared_timed_mutex> lock(data->Object->ObjectMutex);

			run->ImageIndex = (uint64_t)std::stod(args[0]);

			return true;
		};
		RegBlock* block = data->Registry->CreateBlock("vin_texture_switch", "vin_textures", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "switch texture to"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "0"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "for"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [data](const std::vector<std::string>& args)
		{
			sf::Text txt;
			txt.setFont(*data->Font);
			txt.setCharacterSize(std::stoull(args[1]));
			txt.setString(args[0]);
			txt.setFillColor(sf::Color::White);
			
			sf::RenderTexture texture;
			texture.create(txt.getGlobalBounds().width, std::stoull(args[1]));
			texture.clear(sf::Color(0, 0, 0, 0));
			texture.draw(txt);

			sf::Image img = texture.getTexture().copyToImage();
			img.flipVertically();
			img.saveToFile(args[2]);

			return true;
		};
		RegBlock* block = data->Registry->CreateBlock("vin_texture_text", "vin_textures", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "pre render text"},
			{BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "text"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "size"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "18"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "at"},
			{BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "path"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			return true;
		};
		RegBlock* block = data->Registry->CreateBlock("vin_thread_open", "vin_threading", execution, {
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
		RegBlock* block = data->Registry->CreateBlock("vin_thread_sleep", "vin_threading", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "sleep thread"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "1000"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			Logger::Info("thread \"" + args[0] + "\" started execution");
			return true;
		};
		RegBlock* block = data->Registry->CreateBlock("vin_thread_function_define", "vin_threading", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "function define"},
			{BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::ONLY_RAW, BlockArgumentVariableMode::RAW, "function"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "pass"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "pass"}
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
		RegBlock* block = data->Registry->CreateBlock("vin_thread_function_call", "vin_threading", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "function call"},
			{BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::ONLY_RAW, BlockArgumentVariableMode::RAW, "function"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "pass"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "0"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&, const std::string&)>* execution = new std::function<bool(const std::vector<std::string>&, const std::string&)>();
		*execution = [data](const std::vector<std::string>& args, const std::string& idx)
		{
			int searchResult = data->Runtime->PerformFunctionSearch(args[0]);
			if (searchResult == -1)
			{
				Logger::Error("function \"" + args[0] + "\" does not exist");
				return false;
			}
			
			double threadId = data->Thread->SummonThread(searchResult, data->Runtime, data->Variable, data->Registry, true, std::stod(args[1]));
			Logger::Debug("summoned thread " + std::to_string(threadId));

			if (!data->Variable->SetReal(std::string(idx + args[2]).c_str(), threadId))
			{
				Logger::Warn("undocumented thread released! this may cause undefined behavior!");
				return false;
			}

			return true;
		};
		RegBlock* block = data->Registry->CreateBlock("vin_thread_function_thread", "vin_threading", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "function thread"},
			{BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::ONLY_RAW, BlockArgumentVariableMode::RAW, "function"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "pass"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "0"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "for"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "thread_id"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [data](const std::vector<std::string>& args)
		{
			Logger::Debug("killing thread " + args[0]);
			return data->Thread->KillThread((uint64_t)floor(std::stod(args[0])));
		};
		RegBlock* block = data->Registry->CreateBlock("vin_thread_kill_thread", "vin_threading", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "kill thread"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "thread_id"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [data](const std::vector<std::string>& args)
		{
			RuntimeObject* run = data->Object->GetObject((uint64_t)floor(std::stod(args[0])));
			
			if (run == nullptr)
				return false;

			std::unique_lock<std::shared_timed_mutex> lock(data->Object->ObjectMutex);

			run->Position.x = std::stod(args[1]);
			run->Position.y = std::stod(args[2]);

			return true;
		};
		RegBlock* block = data->Registry->CreateBlock("vin_transform_translate_to", "vin_transform", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "translate"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "to"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "0"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "0"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [data](const std::vector<std::string>& args)
		{
			RuntimeObject* run = data->Object->GetObject((uint64_t)floor(std::stod(args[0])));
			
			if (run == nullptr)
				return false;

			std::unique_lock<std::shared_timed_mutex> lock(data->Object->ObjectMutex);

			run->Position.x += std::stod(args[1]);
			run->Position.y += std::stod(args[2]);

			return true;
		};
		RegBlock* block = data->Registry->CreateBlock("vin_transform_translate_by", "vin_transform", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "translate"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "by"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "0"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "0"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [data](const std::vector<std::string>& args)
		{
			RuntimeObject* run = data->Object->GetObject((uint64_t)floor(std::stod(args[0])));
			
			if (run == nullptr)
				return false;

			std::unique_lock<std::shared_timed_mutex> lock(data->Object->ObjectMutex);

			run->Scale.x = std::stod(args[1]);
			run->Scale.y = std::stod(args[2]);

			return true;
		};
		RegBlock* block = data->Registry->CreateBlock("vin_transform_scale_to", "vin_transform", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "scale"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "to"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "0"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "0"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [data](const std::vector<std::string>& args)
		{
			RuntimeObject* run = data->Object->GetObject((uint64_t)floor(std::stod(args[0])));

			if (run == nullptr)
				return false;

			std::unique_lock<std::shared_timed_mutex> lock(data->Object->ObjectMutex);

			run->Scale.x += std::stod(args[1]);
			run->Scale.y += std::stod(args[2]);

			return true;
		};
		RegBlock* block = data->Registry->CreateBlock("vin_transform_scale_by", "vin_transform", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "scale"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "by"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "0"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "0"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [data](const std::vector<std::string>& args)
		{
			RuntimeObject* run = data->Object->GetObject((uint64_t)floor(std::stod(args[0])));

			if (run == nullptr)
				return false;

			std::unique_lock<std::shared_timed_mutex> lock(data->Object->ObjectMutex);

			run->Angle = std::stod(args[1]);

			return true;
		};
		RegBlock* block = data->Registry->CreateBlock("vin_transform_rotate_to", "vin_transform", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "rotate"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "to"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "0"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [data](const std::vector<std::string>& args)
		{
			RuntimeObject* run = data->Object->GetObject((uint64_t)floor(std::stod(args[0])));

			if (run == nullptr)
				return false;

			std::unique_lock<std::shared_timed_mutex> lock(data->Object->ObjectMutex);

			run->Angle += std::stod(args[1]);

			return true;
		};
		RegBlock* block = data->Registry->CreateBlock("vin_transform_rotate_by", "vin_transform", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "rotate"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "var"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "by"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "0"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&, const std::string&)>* execution = new std::function<bool(const std::vector<std::string>&, const std::string&)>();
		*execution = [data](const std::vector<std::string>& args, const std::string& idx)
		{
			return data->Variable->StackReal((idx + args[0]).c_str());
		};
		RegBlock* block = data->Registry->CreateBlock("vin_varaibles_stack_real", "vin_variables", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "stack real"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&, const std::string&)>* execution = new std::function<bool(const std::vector<std::string>&, const std::string&)>();
		*execution = [data](const std::vector<std::string>& args, const std::string& idx)
		{
			return data->Variable->StackString((idx + args[0]).c_str());
		};
		RegBlock* block = data->Registry->CreateBlock("vin_varaibles_stack_string", "vin_variables", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "stack string"},
			{BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&, const std::string&)>* execution = new std::function<bool(const std::vector<std::string>&, const std::string&)>();
		*execution = [data](const std::vector<std::string>& args, const std::string& idx)
		{
			return data->Variable->StackBool((idx + args[0]).c_str());
		};
		RegBlock* block = data->Registry->CreateBlock("vin_varaibles_stack_bool", "vin_variables", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "stack bool"},
			{BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&, const std::string&)>* execution = new std::function<bool(const std::vector<std::string>&, const std::string&)>();
		*execution = [data](const std::vector<std::string>& args, const std::string& idx)
		{
			data->Variable->HeapReal((idx + args[0]).c_str());
			return true;
		};
		RegBlock* block = data->Registry->CreateBlock("vin_varaibles_heap_real", "vin_variables", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "heap real"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&, const std::string&)>* execution = new std::function<bool(const std::vector<std::string>&, const std::string&)>();
		*execution = [data](const std::vector<std::string>& args, const std::string& idx)
		{
			data->Variable->HeapString((idx + args[0]).c_str());
			return true;
		};
		RegBlock* block = data->Registry->CreateBlock("vin_varaibles_heap_string", "vin_variables", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "heap string"},
			{BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&, const std::string&)>* execution = new std::function<bool(const std::vector<std::string>&, const std::string&)>();
		*execution = [data](const std::vector<std::string>& args, const std::string& idx)
		{
			data->Variable->HeapBool((idx + args[0]).c_str());
			return true;
		};
		RegBlock* block = data->Registry->CreateBlock("vin_varaibles_heap_bool", "vin_variables", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "heap bool"},
			{BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&, const std::string&)>* execution = new std::function<bool(const std::vector<std::string>&, const std::string&)>();
		*execution = [data](const std::vector<std::string>& args, const std::string& idx)
		{
			return data->Variable->FreeReal((idx + args[0]).c_str());
		};
		RegBlock* block = data->Registry->CreateBlock("vin_varaibles_free_real", "vin_variables", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "free real"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&, const std::string&)>* execution = new std::function<bool(const std::vector<std::string>&, const std::string&)>();
		*execution = [data](const std::vector<std::string>& args, const std::string& idx)
		{
			return data->Variable->FreeString((idx + args[0]).c_str());
		};
		RegBlock* block = data->Registry->CreateBlock("vin_varaibles_free_string", "vin_variables", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "free string"},
			{BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&, const std::string&)>* execution = new std::function<bool(const std::vector<std::string>&, const std::string&)>();
		*execution = [data](const std::vector<std::string>& args, const std::string& idx)
		{
			return data->Variable->FreeBool((idx + args[0]).c_str());
		};
		RegBlock* block = data->Registry->CreateBlock("vin_varaibles_free_bool", "vin_variables", execution, {
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
		RegBlock* block = data->Registry->CreateBlock("vin_varaibles_log_real", "vin_variables", execution, {
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
		RegBlock* block = data->Registry->CreateBlock("vin_varaibles_log_string", "vin_variables", execution, {
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
		RegBlock* block = data->Registry->CreateBlock("vin_varaibles_log_bool", "vin_variables", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "log bool"},
			{BlockArgumentType::BOOL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
}
