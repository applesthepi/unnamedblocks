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

	RegBlock* block = new RegBlock();

	std::vector<BlockArgument> argList = std::vector<BlockArgument>();

	BlockArgument arg0 = BlockArgument();
	BlockArgument arg1 = BlockArgument();

	arg0.SetupTEXT("new object");
	arg1.SetupREAL("1var");

	argList.push_back(arg0);
	argList.push_back(arg1);

	block->Args = argList;
	block->Catagory = "vin_objects";
	block->UnlocalizedName = "vin_object_new";

	std::function<bool(std::vector<std::string>*)>* execute = new std::function<bool(std::vector<std::string>*)>();
	*execute = [](std::vector<std::string>* args)
	{
		std::string realText = (*args)[0].substr(1, (*args)[0].length() - 1);

		if ((*args)[0][0] == '0')
		{
			Logger::Error("expecting variable only! got \"" + realText + "\"");
			return false;
		}

		RuntimeObject* run = new RuntimeObject();
		run->ImageIndex = 0;
		run->Position = sf::Vector2f(0.0f, 0.0f);
		run->Scale = sf::Vector2f(1.0f, 1.0f);
		run->Angle = 0.0;

		bool result = VariableHandler::SetReal(realText.c_str(), ObjectHandler::CreateObject(run));
		if (!result)
		{
			Logger::Error("variable \"" + realText + "\" does not exist");
			return false;
		}

		return true;
	};

	block->Execute = execute;

	data->RegisterBlock(*block);
	delete block;
}
#endif
