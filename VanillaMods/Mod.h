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

	RegBlock* block = BlockRegistry::CreateBlock("objects_new", "vin_objects", execution, {
		{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "new object"},
		{BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "default obj"}
	});
	data->RegisterBlock(*block);
}
