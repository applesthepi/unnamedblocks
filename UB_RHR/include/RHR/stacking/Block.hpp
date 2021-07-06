#pragma once
#include "config.h"

#include "stacking/args/Argument.hpp"

#include <Cappuccino/Utils.hpp>
#include <Cappuccino/block/ModBlock.hpp>
#include <Espresso/catagory/ModCatagory.hpp>

class Block
{
public:
	Block(const std::string& unlocalizedName);
	Block(const Block& block);

	~Block();

	// would not recommend calling these

	void AddArgument(Argument* argument);
	void AddArguments(const std::vector<Argument*>& arguments);

	const std::vector<Argument*>& GetArguments();
	uint32_t GetWidth();

	const ModBlock* GetModBlock();
	const ModCatagory* GetModCategory();

	static int16_t Padding;
	static int16_t Height;
private:
	void UpdateArguments();
	void UpdateWidth();

	uint32_t m_width;

	std::vector<Argument*> m_arguments;
	const ModBlock* m_modBlock;
	const ModCatagory* m_modCategory;
};