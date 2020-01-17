#pragma once

#include <vector>
#include <string>
#include <SFML/Graphics/Color.hpp>
#include <functional>

enum class BlockArgumentType
{
	/*
	This is the argument type, an argument can only allow this variable type to be used.
	*/
	TEXT, REAL, BOOL, STRING
};

enum class BlockArgumentVariableModeRestriction
{
	/*
	This controls how the input is given to the mod

	NONE          - excepts either raw or var, raw will be raw, var will be the value
	ONLY_RAW      - excepts only raw         , raw will be raw
	ONLY_VAR      - excepts only var         , var will be the value
	ONLY_VAR_KEEP - excepts only var         , var will be the variable name (inputed) - this is for if you want to "show"
	                                                                                     to the user that you expect a variable.
	*/
	NONE, ONLY_RAW, ONLY_VAR, ONLY_VAR_KEEP
};

enum class BlockArgumentVariableMode
{
	/*
	This is the default assignment of whether it starts as var or raw input. The user can change what they input.
	If you want to restrict how data is inputed see the "BlockArgumentVariableModeRestriction" class.
	*/
	VAR, RAW
};

class BlockArgumentInitializer
{
public:
	BlockArgumentInitializer(const BlockArgumentType type, const BlockArgumentVariableModeRestriction restriction, const BlockArgumentVariableMode mode, const std::string defaultValue)
		:Type(type), Restriction(restriction), Mode(mode), DefaultValue(defaultValue) {}

	BlockArgumentType Type;
	BlockArgumentVariableModeRestriction Restriction;
	BlockArgumentVariableMode Mode;
	std::string DefaultValue;
};

class BlockArgumentCaller
{
public:
	BlockArgumentCaller(const BlockArgumentVariableMode mode, const std::string value)
		:Mode(mode), Value(value) {}

	BlockArgumentVariableMode Mode;
	std::string Value;
};

class BlockArgument
{
public:
	void SetupTEXT(const std::string value);
	void SetupREAL(const std::string value);
	void SetupBOOL(const std::string value);
	void SetupSTRING(const std::string value);

	BlockArgumentType Type;
	std::string Value;
};

class RegBlock
{
public:
	std::function<bool(const std::vector<BlockArgumentCaller>&)>* Execute;
	std::vector<BlockArgument> Args;
	std::string UnlocalizedName;
	std::string Catagory;

	//carry
	std::vector<BlockArgumentInitializer>* BlockInit;
	std::function<bool(const std::vector<std::string>&)>* BlockExecute;
};

class RegCatagory
{
public:
	RegCatagory(const std::string unlocalizedName, const std::string displayName, const sf::Color color)
		:UnlocalizedName(unlocalizedName), DisplayName(displayName), Color(color) {}

	std::string UnlocalizedName;
	std::string DisplayName;
	sf::Color Color;
};

class BlockRegistry
{
public:
	static void Initialize();
	BlockRegistry();

	void RegisterCatagory(RegCatagory* catagory);
	void RegisterBlock(RegBlock* block);

	const RegBlock* GetBlock(std::string unlocalizedName);
	const RegCatagory* GetCatagory(std::string unlocalizedName);

	std::vector<RegBlock>* GetBlocks();
	std::vector<RegCatagory>* GetCatagories();

	RegBlock* CreateBlock(const std::string unlocalizedName, const std::string catagory, std::function<bool(const std::vector<std::string>&)>* execute, const std::vector<BlockArgumentInitializer> blockInit);
	void FinalizeBlock(RegBlock* block);

	static BlockRegistry* MainRegistry;
private:
	std::vector<RegBlock>* m_blocks;
	std::vector<RegCatagory>* m_catagories;
};