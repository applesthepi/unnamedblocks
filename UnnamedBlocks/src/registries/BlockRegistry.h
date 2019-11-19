#pragma once

#include <vector>
#include <string>

enum BlockArgumentType
{
	TEXT, REAL, BOOL, STRING, OBJECT
};

class BlockArgument
{
public:
	void SetupTEXT(const char* value);
	void SetupREAL(double value);
	void SetupBOOL(bool value);
	void SetupSTRING(const char* vlaue);
	void SetupObject(unsigned long long value);

	BlockArgumentType Type;
	double T_REAL;
	const char* T_STRING;
	bool T_BOOL;
	unsigned long long T_UINT64;
};

class BlockRegistry
{
public:
	static void Initialize();
	static void CreateBlock(std::string unlocalizedName, std::vector<BlockArgument> args);

	static std::vector<BlockArgument>* GetHeapedBlockArguments(std::string unlocalizedName);
private:
	static std::vector<std::vector<BlockArgument>>* m_blockArgs;
	static std::vector<std::string>* m_blockUnlocalizedNames;
};