#pragma once
#include "args/Argument.h"
#include "RHR/ui/ITransformable.h"

#include <vector>
#include <SFML/Graphics.hpp>

#include <Cappuccino/block/ModBlock.h>
#include <Espresso/catagory/ModCatagory.h>

class Block : public ITransformable
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
private:
	void UpdateArguments();
	void UpdateWidth();

	uint32_t m_width;

	std::vector<Argument*> m_arguments;
	const ModBlock* m_modBlock;
	const ModCatagory* m_modCategory;
};