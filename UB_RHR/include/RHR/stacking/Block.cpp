#include "Block.hpp"

#include "stacking/args/ArgumentText.hpp"
#include "stacking/args/ArgumentReal.hpp"
#include "stacking/args/ArgumentBoolean.hpp"
#include "stacking/args/ArgumentString.hpp"
#include "stacking/args/ArgumentAny.hpp"
#include "registries/BlockRegistry.hpp"

Block::Block(const std::string& unlocalizedName)
	:m_modBlock(BlockRegistry::GetRegistry().GetBlock(unlocalizedName))
{
	m_modCategory = BlockRegistry::GetRegistry().GetCategory(m_modBlock->GetCategory());
	UpdateArguments();
}

Block::Block(const Block& block)
	:m_modBlock(block.m_modBlock)
{
	m_modCategory = block.m_modCategory;

	for (uint64_t i = 0; i < m_arguments.size(); i++)
		delete m_arguments[i];

	m_arguments.clear();

	for (uint64_t i = 0; i < block.m_arguments.size(); i++)
		m_arguments.push_back(new Argument(*block.m_arguments[i]));
}

Block::~Block()
{
	for (uint64_t i = 0; i < m_arguments.size(); i++)
		delete m_arguments[i];
}

void Block::AddArgument(Argument* argument)
{
	m_arguments.push_back(argument);

	UpdateWidth();
}

void Block::AddArguments(const std::vector<Argument*>& arguments)
{
	if (m_arguments.size() + arguments.size() >= m_arguments.capacity())
		m_arguments.reserve((uint64_t)std::ceil((float)(m_arguments.size() + arguments.size()) * 1.5f + 10.0f));

	for (uint64_t i = 0; i < arguments.size(); i++)
		m_arguments.push_back(arguments[i]);

	UpdateWidth();
}

const std::vector<Argument*>& Block::GetArguments()
{
	return m_arguments;
}

uint32_t Block::GetWidth()
{
	return m_width;
}

const ModBlock* Block::GetModBlock()
{
	return m_modBlock;
}

const ModCatagory* Block::GetModCategory()
{
	return m_modCategory;
}

int16_t Block::Padding = 2;
int16_t Block::Height = 20;

void Block::UpdateArguments()
{
	for (uint64_t i = 0; i < m_arguments.size(); i++)
		delete m_arguments[i];

	m_arguments.clear();
	m_arguments.reserve(m_modBlock->GetArguments().size());

	std::vector<BlockArgumentInitializer> argumentInit = m_modBlock->GetArguments();
	std::vector<Argument*> args;

	uint32_t width = Block::Padding;

	for (uint64_t i = 0; i < argumentInit.size(); i++)
	{
		if (argumentInit[i].Type == BlockArgumentType::TEXT)
		{
			args.push_back(new ArgumentText());

			args.back()->SetPosition({ width, Block::Padding });
			args.back()->SetData(argumentInit[i].DefaultValue);

			width += args.back()->GetWidth();
		}
		/*else if (argumentInit[i].Type == BlockArgumentType::REAL)
		{
			args.push_back(new ArgumentReal(argumentInit[i].Restriction == BlockArgumentVariableModeRestriction::NONE));

			args.back()->setPosition(width, Global::BlockBorder);
			args.back()->SetMode(argumentInit[i].Mode);
			args.back()->SetData(argumentInit[i].DefaultValue);
			args.back()->UpdateData();

			width += args.back()->GetWidth();
		}
		else if (argumentInit[i].Type == BlockArgumentType::STRING)
		{
			args.push_back(new ArgumentString(argumentInit[i].Restriction == BlockArgumentVariableModeRestriction::NONE));

			args.back()->setPosition(width, Global::BlockBorder);
			args.back()->SetMode(argumentInit[i].Mode);
			args.back()->SetData(argumentInit[i].DefaultValue);
			args.back()->UpdateData();

			width += args.back()->GetWidth();
		}
		else if (argumentInit[i].Type == BlockArgumentType::BOOL)
		{
			args.push_back(new ArgumentBool(argumentInit[i].Restriction == BlockArgumentVariableModeRestriction::NONE));

			args.back()->setPosition(width, Global::BlockBorder);
			args.back()->SetMode(argumentInit[i].Mode);
			args.back()->SetData(argumentInit[i].DefaultValue);
			args.back()->UpdateData();

			width += args.back()->GetWidth();
		}
		else if (argumentInit[i].Type == BlockArgumentType::ANY)
		{
			args.push_back(new ArgumentAny());

			args.back()->setPosition(width, Global::BlockBorder);
			args.back()->SetData(argumentInit[i].DefaultValue);
			args.back()->UpdateData();

			width += args.back()->GetWidth();
		}*/
	}

	AddArguments(args);
}

void Block::UpdateWidth()
{
	m_width = Block::Padding;

	for (uint64_t i = 0; i < m_arguments.size(); i++)
	{
		m_width += m_arguments[i]->GetWidth();
		m_width += Block::Padding;
	}
}