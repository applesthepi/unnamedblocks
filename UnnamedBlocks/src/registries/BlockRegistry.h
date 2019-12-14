#pragma once

#include <vector>
#include <string>
#include <SFML/Graphics/Color.hpp>
#include <functional>

enum BlockArgumentType
{
	TEXT, REAL, BOOL, STRING
};

class BlockArgument
{
public:
	void SetupTEXT(std::string value);
	void SetupREAL(std::string value);
	void SetupBOOL(std::string value);
	void SetupSTRING(std::string value);

	BlockArgumentType Type;
	std::string Value;
};

class RegBlock
{
public:
	std::function<bool(std::vector<std::string>*)>* Execute;
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
	static void CreateCatagory(RegCatagory);
	static void CreateBlock(RegBlock);

	static const RegBlock* GetBlock(std::string unlocalizedName);
	static const RegCatagory* GetCatagory(std::string unlocalizedName);

	static std::vector<RegBlock>* GetBlocks();
	static std::vector<RegCatagory>* GetCatagories();
private:
	static std::vector<RegBlock>* m_blocks;
	static std::vector<RegCatagory>* m_catagories;
};