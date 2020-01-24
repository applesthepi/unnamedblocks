#include "BlockRegistry.h"
#include "handlers/Logger.h"
#include "handlers/runtime/VariableHandler.h"

void BlockRegistry::Initialize()
{
	MainRegistry = new BlockRegistry();
}

BlockRegistry::BlockRegistry()
{
	m_blocks = new std::vector<RegBlock>();
	m_catagories = new std::vector<RegCatagory>();
}

void BlockRegistry::RegisterCatagory(RegCatagory* catagory)
{
	m_catagories->push_back(*catagory);
}

void BlockRegistry::RegisterBlock(RegBlock* block, VariableHandler* variables)
{
	FinalizeBlock(block, variables);
	m_blocks->push_back(*block);
}

RegBlock* BlockRegistry::CreateBlock(const std::string unlocalizedName, const std::string catagory, std::function<bool(const std::vector<std::string>&)>* execute, const std::vector<BlockArgumentInitializer> blockInit)
{
	RegBlock* block = new RegBlock();

	block->UnlocalizedName = unlocalizedName;
	block->Catagory = catagory;

	//carry
	block->BlockInit = new std::vector<BlockArgumentInitializer>(blockInit);
	block->BlockExecute = new std::function<bool(const std::vector<std::string>&)>(*execute);
	
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

	std::function<bool(const std::vector<BlockArgumentCaller>&)>* parentExecution = new std::function<bool(const std::vector<BlockArgumentCaller>&)>();
	*parentExecution = [block, variables, blockUseArgs](const std::vector<BlockArgumentCaller>& args)
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
			else if (blockUseArgs.at(i).Mode == BlockArgumentVariableMode::VAR)
			{
				if (blockUseArgs.at(i).Type == BlockArgumentType::STRING)
				{
					std::string* data = variables->GetString(args[i].Value.c_str());
					if (data == nullptr)
					{
						Logger::Error("variable \"" + args[i].Value + "\" does not exist");
						return false;
					}

					parsedArgs.push_back(*data);
				}
				else if (blockUseArgs.at(i).Type == BlockArgumentType::BOOL)
				{
					bool* data = variables->GetBool(args[i].Value.c_str());
					if (data == nullptr)
					{
						Logger::Error("variable \"" + args[i].Value + "\" does not exist");
						return false;
					}

					parsedArgs.push_back(*data ? "1" : "0");
				}
				else if (blockUseArgs.at(i).Type == BlockArgumentType::REAL)
				{
					double* data = variables->GetReal(args[i].Value.c_str());
					if (data == nullptr)
					{
						Logger::Error("variable \"" + args[i].Value + "\" does not exist");
						return false;
					}

					parsedArgs.push_back(std::to_string(*data));
				}
			}
		}

		return (*block->BlockExecute)(parsedArgs);
	};

	block->Execute = parentExecution;
}

BlockRegistry* BlockRegistry::MainRegistry;

const RegBlock* BlockRegistry::GetBlock(std::string unlocalizedName)
{
	for (unsigned int i = 0; i < m_blocks->size(); i++)
	{
		if ((*m_blocks)[i].UnlocalizedName == unlocalizedName)
			return (const RegBlock*)(&(*m_blocks)[i]);
	}

	return nullptr;
}

const RegCatagory* BlockRegistry::GetCatagory(std::string unlocalizedName)
{
	for (unsigned int i = 0; i < m_catagories->size(); i++)
	{
		if ((*m_catagories)[i].UnlocalizedName == unlocalizedName)
			return (const RegCatagory*)(&(*m_catagories)[i]);
	}

	return nullptr;
}

std::vector<RegBlock>* BlockRegistry::GetBlocks()
{
	return m_blocks;
}

std::vector<RegCatagory>* BlockRegistry::GetCatagories()
{
	return m_catagories;
}

void BlockArgument::SetupTEXT(std::string value)
{
	Type = BlockArgumentType::TEXT;
	Value = value;
}

void BlockArgument::SetupREAL(std::string value)
{
	Type = BlockArgumentType::REAL;
	Value = value;
}

void BlockArgument::SetupBOOL(std::string value)
{
	Type = BlockArgumentType::BOOL;
	Value = value;
}

void BlockArgument::SetupSTRING(std::string value)
{
	Type = BlockArgumentType::STRING;
	Value = value;
}
