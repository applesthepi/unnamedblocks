#pragma once

#include <vector>
#include <string>
#include <SFML/Graphics/Color.hpp>
#include <functional>

enum class BlockArgumentType
{
	TEXT, REAL, BOOL, STRING
};

enum class BlockArgumentVariableModeRestriction
{
	ONLY_RAW, ONLY_VAR, NONE
};

enum class BlockArgumentVariableMode
{
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
	std::function<bool(const std::vector<const BlockArgumentCaller>&)>* Execute;
	std::vector<BlockArgument> Args;
	std::string UnlocalizedName;
	std::string Catagory;
};

class RegCatagory
{
public:
	std::string UnlocalizedName;
	std::string DisplayName;
	sf::Color Color;
};

class BlockRegistry
{
public:
	static void Initialize();
	static void RegisterCatagory(const RegCatagory* catagory);
	static void RegisterBlock(const RegBlock* block);

	static const RegBlock* GetBlock(std::string unlocalizedName);
	static const RegCatagory* GetCatagory(std::string unlocalizedName);

	static std::vector<RegBlock>* GetBlocks();
	static std::vector<RegCatagory>* GetCatagories();

	static RegBlock* CreateBlock(const std::string unlocalizedName, const std::string catagory, std::function<bool(const std::vector<const BlockArgumentCaller>&)>* execute, const BlockArgumentInitializer blockInit[]);
private:
	static std::vector<RegBlock>* m_blocks;
	static std::vector<RegCatagory>* m_catagories;
};