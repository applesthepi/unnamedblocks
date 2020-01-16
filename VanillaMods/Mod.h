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
			if (!VariableHandler::SetReal(args[0].c_str(), ObjectHandler::CreateObject(new RuntimeObject())))
			{
				Logger::Error("variable \"" + args[0] + "\" does not exist");
				return false;
			}

			return true;
		};

		RegBlock* block = BlockRegistry::CreateBlock("vin_object_new", "vin_objects", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "new object"},
			{BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "obj"}
			});
		data->RegisterBlock(*block);
	}
	{
		std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
		*execution = [](const std::vector<std::string>& args)
		{
			double* index = VariableHandler::GetReal(args[0].c_str());
			if (index == nullptr)
			{
				Logger::Error("variable \"" + args[0] + "\" does not exist");
				return false;
			}

			ObjectHandler::ObjectMutex->lock();
			bool result = ObjectHandler::DestroyObject(*index);
			ObjectHandler::ObjectMutex->unlock();

			if (!result)
			{
				Logger::Error("object \"" + std::to_string(*index) + "\" does not exist");
				return false;
			}

			return true;
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
			unsigned long long address = ByteHandler::AllocateBytes(std::stoull(args[0]));

			bool result = VariableHandler::SetReal(args[1].c_str(), address);
			if (!result)
			{
				Logger::Error("variable \"" + args[1] + "\" does not exist");
				return false;
			}

			return true;
		};

		RegBlock* block = BlockRegistry::CreateBlock("vin_byte_alloc", "vin_bytes", execution, {
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "alloc"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "10"},
			{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "bytes to"},
			{BlockArgumentType::REAL, BlockArgumentVariableModeRestriction::ONLY_VAR, BlockArgumentVariableMode::VAR, "var"}
			});
		data->RegisterBlock(*block);
	}
}
