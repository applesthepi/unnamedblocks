#pragma once

#include <RHR/RHR.h>

void ContentLoadExecution()
{
	{
		RegBlock* block = new RegBlock();

		std::vector<BlockArgument> argList = std::vector<BlockArgument>();

		BlockArgument arg0 = BlockArgument();
		BlockArgument arg1 = BlockArgument();

		arg0.SetupTEXT("mark point");
		arg1.SetupSTRING("0point");

		argList.push_back(arg0);
		argList.push_back(arg1);

		block->Args = argList;
		block->Catagory = "vin_execution";
		block->UnlocalizedName = "vin_execution_mark";

		BlockRegistry::CreateBlock(*block);
	}

	{
		RegBlock* block = new RegBlock();

		std::vector<BlockArgument> argList = std::vector<BlockArgument>();

		BlockArgument arg0 = BlockArgument();
		BlockArgument arg1 = BlockArgument();

		arg0.SetupTEXT("goto point");
		arg1.SetupSTRING("0point");

		argList.push_back(arg0);
		argList.push_back(arg1);

		block->Args = argList;
		block->Catagory = "vin_execution";
		block->UnlocalizedName = "vin_execution_goto";

		BlockRegistry::CreateBlock(*block);
	}

	{
		RegBlock* block = new RegBlock();

		std::vector<BlockArgument> argList = std::vector<BlockArgument>();

		BlockArgument arg0 = BlockArgument();
		BlockArgument arg1 = BlockArgument();
		BlockArgument arg2 = BlockArgument();
		BlockArgument arg3 = BlockArgument();

		arg0.SetupTEXT("goto point");
		arg1.SetupSTRING("0point");
		arg2.SetupTEXT("if");
		arg3.SetupBOOL("00");

		argList.push_back(arg0);
		argList.push_back(arg1);
		argList.push_back(arg2);
		argList.push_back(arg3);

		block->Args = argList;
		block->Catagory = "vin_execution";
		block->UnlocalizedName = "vin_execution_goto_if";

		BlockRegistry::CreateBlock(*block);
	}

	{
		RegBlock* block = new RegBlock();

		std::vector<BlockArgument> argList = std::vector<BlockArgument>();

		BlockArgument arg0 = BlockArgument();
		BlockArgument arg1 = BlockArgument();

		arg0.SetupTEXT("enable block sync");
		arg1.SetupBOOL("01");

		argList.push_back(arg0);
		argList.push_back(arg1);

		block->Args = argList;
		block->Catagory = "vin_execution";
		block->UnlocalizedName = "vin_execution_block_sync";

		BlockRegistry::CreateBlock(*block);
	}

	{
		RegBlock* block = new RegBlock();

		std::vector<BlockArgument> argList = std::vector<BlockArgument>();

		BlockArgument arg0 = BlockArgument();
		BlockArgument arg1 = BlockArgument();

		arg0.SetupTEXT("enable manual rendering");
		arg1.SetupBOOL("01");

		argList.push_back(arg0);
		argList.push_back(arg1);

		block->Args = argList;
		block->Catagory = "vin_execution";
		block->UnlocalizedName = "vin_execution_manual_render";

		BlockRegistry::CreateBlock(*block);
	}

	{
		RegBlock* block = new RegBlock();

		std::vector<BlockArgument> argList = std::vector<BlockArgument>();

		BlockArgument arg0 = BlockArgument();

		arg0.SetupTEXT("render frame");

		argList.push_back(arg0);

		block->Args = argList;
		block->Catagory = "vin_execution";
		block->UnlocalizedName = "vin_execution_render_frame";

		BlockRegistry::CreateBlock(*block);
	}
}