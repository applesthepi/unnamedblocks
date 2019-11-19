#include "BlockRegistry.h"

void BlockRegistry::Initialize()
{
	m_blockArgs = new std::vector<std::vector<BlockArgument>>();
	m_blockUnlocalizedNames = new std::vector<std::string>();
}

void BlockRegistry::CreateBlock(std::string unlocalizedName, std::vector<BlockArgument> args)
{
	m_blockUnlocalizedNames->push_back(unlocalizedName);
	m_blockArgs->push_back(args);
}

std::vector<BlockArgument>* BlockRegistry::GetHeapedBlockArguments(std::string unlocalizedName)
{
	for (unsigned int i = 0; i < m_blockUnlocalizedNames->size(); i++)
	{
		if ((*m_blockUnlocalizedNames)[i] == unlocalizedName);
		{
			std::vector<BlockArgument>* args = new std::vector<BlockArgument>();
			*args = (*m_blockArgs)[i];
			return args;
		}
	}

	return nullptr;
}

std::vector<std::vector<BlockArgument>>* BlockRegistry::m_blockArgs;

std::vector<std::string>* BlockRegistry::m_blockUnlocalizedNames;

void BlockArgument::SetupTEXT(const char* value)
{
	Type = BlockArgumentType::TEXT;
	T_STRING = value;
}

void BlockArgument::SetupREAL(double value)
{
	Type = BlockArgumentType::REAL;
	T_REAL = value;
}

void BlockArgument::SetupBOOL(bool value)
{
	Type = BlockArgumentType::BOOL;
	T_BOOL = value;
}

void BlockArgument::SetupSTRING(const char* value)
{
	Type = BlockArgumentType::STRING;
	T_STRING = value;
}

void BlockArgument::SetupObject(unsigned long long value)
{
	Type = BlockArgumentType::OBJECT;
	T_UINT64 = value;
}
