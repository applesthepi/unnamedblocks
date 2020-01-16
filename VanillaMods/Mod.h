#pragma once
#include <RHR/RHR.h>

#ifdef LINUX
extern "C" int test() {
	return 5;
}
#else

#define UB_EXPORT extern "C" __declspec(dllexport)

UB_EXPORT void __cdecl Initialization(ModData* data)
{
	{
		RegCatagory* cat = new RegCatagory();
		cat->Color = sf::Color(237, 231, 109);
		cat->DisplayName = "objects";
		cat->UnlocalizedName = "vin_objects";

		data->RegisterCatagory(*cat);
		delete cat;
	}

	std::function<bool(const std::vector<std::string>&)>* execution = new std::function<bool(const std::vector<std::string>&)>();
	*execution = [](const std::vector<std::string>& args)
	{
		RuntimeObject* run = new RuntimeObject();
		run->ImageIndex = 0;
		run->Position = sf::Vector2f(0.0f, 0.0f);
		run->Scale = sf::Vector2f(1.0f, 1.0f);
		run->Angle = 0.0;

		bool result = VariableHandler::SetReal(args[0].c_str(), ObjectHandler::CreateObject(run));
		if (!result)
		{
			Logger::Error("variable \"" + args[0] + "\" does not exist");
			return false;
		}

		return true;
	};

	std::vector<BlockArgumentInitializer> args = {
		{BlockArgumentType::TEXT, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "new object"},
		{BlockArgumentType::STRING, BlockArgumentVariableModeRestriction::NONE, BlockArgumentVariableMode::RAW, "default obj"}
	};

	RegBlock* block = BlockRegistry::CreateBlock("objects_new", "vin_objects", execution, args);
	data->RegisterBlock(*block);
}
#endif
