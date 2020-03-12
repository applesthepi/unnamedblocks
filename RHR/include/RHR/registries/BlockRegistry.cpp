#include "BlockRegistry.h"
#include "handlers/Logger.h"

BlockRegistry::BlockRegistry()
{

}

void BlockRegistry::RegisterCatagory(const ModCatagory& catagory)
{
	m_catagories->push_back(catagory);
}

void BlockRegistry::RegisterBlock(const ModBlock& block)
{
	m_blocks->push_back(block);
}

RegBlock* BlockRegistry::CreateBlock(const std::string unlocalizedName, const std::string catagory, std::function<bool(const std::vector<std::string>&)>* execute, const std::vector<BlockArgumentInitializer> blockInit)
{
	RegBlock* block = new RegBlock();

	block->UnlocalizedName = unlocalizedName;
	block->Catagory = catagory;

	//carry
	block->BlockInit = new std::vector<BlockArgumentInitializer>(blockInit);
	block->BlockExecute = new std::function<bool(const std::vector<std::string>&)>(*execute);
	block->BlockExecuteIdx = nullptr;
	
	for (uint16_t i = 0; i < blockInit.size(); i++)
	{
		BlockArgument arg;
		
		if (blockInit[i].Type == BlockArgumentType::TEXT)
			arg.SetupTEXT(blockInit[i].DefaultValue);
		else if (blockInit[i].Type == BlockArgumentType::BOOL)
		{
			if (blockInit[i].Mode == BlockArgumentVariableMode::RAW)
				arg.SetupBOOL("0" + blockInit[i].DefaultValue);
			else if (blockInit[i].Mode == BlockArgumentVariableMode::VAR)
				arg.SetupBOOL("1" + blockInit[i].DefaultValue);
		}
		else if (blockInit[i].Type == BlockArgumentType::REAL)
		{
			if (blockInit[i].Mode == BlockArgumentVariableMode::RAW)
				arg.SetupREAL("0" + blockInit[i].DefaultValue);
			else if (blockInit[i].Mode == BlockArgumentVariableMode::VAR)
				arg.SetupREAL("1" + blockInit[i].DefaultValue);
		}
		else if (blockInit[i].Type == BlockArgumentType::STRING)
		{
			if (blockInit[i].Mode == BlockArgumentVariableMode::RAW)
				arg.SetupSTRING("0" + blockInit[i].DefaultValue);
			else if (blockInit[i].Mode == BlockArgumentVariableMode::VAR)
				arg.SetupSTRING("1" + blockInit[i].DefaultValue);
		}

		block->Args.push_back(arg);
	}

	return block;
}

RegBlock* BlockRegistry::CreateBlock(const std::string unlocalizedName, const std::string catagory, std::function<bool(const std::vector<std::string>&, const uint64_t&)>* execute, const std::vector<BlockArgumentInitializer> blockInit)
{
	RegBlock* block = new RegBlock();

	block->UnlocalizedName = unlocalizedName;
	block->Catagory = catagory;

	//carry
	block->BlockInit = new std::vector<BlockArgumentInitializer>(blockInit);
	block->BlockExecute = nullptr;
	block->BlockExecuteIdx = new std::function<bool(const std::vector<std::string>&, const uint64_t&)>(*execute);

	for (uint16_t i = 0; i < blockInit.size(); i++)
	{
		BlockArgument arg;

		if (blockInit[i].Type == BlockArgumentType::TEXT)
			arg.SetupTEXT(blockInit[i].DefaultValue);
		else if (blockInit[i].Type == BlockArgumentType::BOOL)
		{
			if (blockInit[i].Mode == BlockArgumentVariableMode::RAW)
				arg.SetupBOOL("0" + blockInit[i].DefaultValue);
			else if (blockInit[i].Mode == BlockArgumentVariableMode::VAR)
				arg.SetupBOOL("1" + blockInit[i].DefaultValue);
		}
		else if (blockInit[i].Type == BlockArgumentType::REAL)
		{
			if (blockInit[i].Mode == BlockArgumentVariableMode::RAW)
				arg.SetupREAL("0" + blockInit[i].DefaultValue);
			else if (blockInit[i].Mode == BlockArgumentVariableMode::VAR)
				arg.SetupREAL("1" + blockInit[i].DefaultValue);
		}
		else if (blockInit[i].Type == BlockArgumentType::STRING)
		{
			if (blockInit[i].Mode == BlockArgumentVariableMode::RAW)
				arg.SetupSTRING("0" + blockInit[i].DefaultValue);
			else if (blockInit[i].Mode == BlockArgumentVariableMode::VAR)
				arg.SetupSTRING("1" + blockInit[i].DefaultValue);
		}

		block->Args.push_back(arg);
	}

	return block;
}

void BlockRegistry::FinalizeBlock(RegBlock* block, VariableHandler* variables)
{
	std::vector<BlockArgumentInitializer> blockUseArgs;

	for (uint32_t i = 0; i < block->BlockInit->size(); i++)
	{
		if (block->BlockInit->at(i).Type != BlockArgumentType::TEXT)
			blockUseArgs.push_back(block->BlockInit->at(i));
	}

	std::function<bool(const std::vector<BlockArgumentCaller>&, const uint64_t&)>* parentExecution = new std::function<bool(const std::vector<BlockArgumentCaller>&, const uint64_t&)>();
	*parentExecution = [block, variables, blockUseArgs](const std::vector<BlockArgumentCaller>& args, const uint64_t& idx)
	{
		std::vector<std::string> parsedArgs;

		for (uint16_t i = 0; i < args.size(); i++)
		{
			if (blockUseArgs.at(i).Restriction == BlockArgumentVariableModeRestriction::ONLY_RAW)
			{
				if (args[i].Mode == BlockArgumentVariableMode::VAR)
				{
					Logger::Error("argument #" + std::to_string(i) + " of block \"" + block->UnlocalizedName + "\" requires a raw value");
					return false;
				}
			}
			else if (blockUseArgs.at(i).Restriction == BlockArgumentVariableModeRestriction::ONLY_VAR)
			{
				if (args[i].Mode == BlockArgumentVariableMode::RAW)
				{
					Logger::Error("argument #" + std::to_string(i) + " of block \"" + block->UnlocalizedName + "\" requires a variable");
					return false;
				}
			}
			
			if (blockUseArgs.at(i).Restriction == BlockArgumentVariableModeRestriction::ONLY_VAR_KEEP || args[i].Mode == BlockArgumentVariableMode::RAW)
				parsedArgs.push_back(args[i].Value);
			else if (args[i].Mode == BlockArgumentVariableMode::VAR)
			{
				if (blockUseArgs.at(i).Type == BlockArgumentType::STRING)
				{
					const std::string* data = variables->GetString(idx, std::stoull(args[i].Value));
					if (data == nullptr)
					{
						Logger::Error("variable \"" + args[i].Value + "\" does not exist");
						return false;
					}

					parsedArgs.push_back(*data);
				}
				else if (blockUseArgs.at(i).Type == BlockArgumentType::BOOL)
				{
					const uint8_t* data = variables->GetBool(idx, std::stoull(args[i].Value));
					if (data == nullptr)
					{
						Logger::Error("variable \"" + args[i].Value + "\" does not exist");
						return false;
					}

					parsedArgs.push_back(*data ? "1" : "0");
				}
				else if (blockUseArgs.at(i).Type == BlockArgumentType::REAL)
				{
					const double* data = variables->GetReal(idx, std::stoull(args[i].Value));
					if (data == nullptr)
					{
						Logger::Error("variable \"" + args[i].Value + "\" does not exist");
						return false;
					}

					parsedArgs.push_back(std::to_string(*data));
				}
			}
		}

		if (block->BlockExecute == nullptr)
			return (*block->BlockExecuteIdx)(parsedArgs, idx);
		else
			return (*block->BlockExecute)(parsedArgs);
	};

	block->Execute = parentExecution;
}

const RegBlock& BlockRegistry::GetBlock(const std::string& unlocalizedName)
{
	for (unsigned int i = 0; i < m_blocks->size(); i++)
	{
		if ((*m_blocks)[i].UnlocalizedName == unlocalizedName)
			return (const RegBlock*)(&(*m_blocks)[i]);
	}

	for (unsigned int i = 0; i < m_blocks->size(); i++)
	{
		if ((*m_blocks)[i].UnlocalizedName == "vin_null")
			return (const RegBlock*)(&(*m_blocks)[i]);
	}

	Logger::Error("unexpected failure to recive vin_null block. Mod loading error?");
	return RegBlock();
}

const RegCatagory& BlockRegistry::GetCatagory(const std::string& unlocalizedName)
{
	for (unsigned int i = 0; i < m_catagories->size(); i++)
	{
		if ((*m_catagories)[i].UnlocalizedName == unlocalizedName)
			return (const RegCatagory*)(&(*m_catagories)[i]);
	}

	Logger::Error("unexpected failure to get a catagory. Mod loading error?");
	return RegCatagory();
}
