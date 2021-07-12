#include "Block.hpp"

#include "stacking/args/ArgumentText.hpp"
#include "stacking/args/ArgumentReal.hpp"
#include "stacking/args/ArgumentBoolean.hpp"
#include "stacking/args/ArgumentString.hpp"
#include "stacking/args/ArgumentAny.hpp"
#include "registries/BlockRegistry.hpp"
#include "ui/Renderer.hpp"

Block::Block(const std::string& unlocalizedName)
	: m_modBlock(BlockRegistry::GetRegistry().GetBlock(unlocalizedName))
	, m_Background(std::make_shared<vui::RenderRectangle>())
{
	m_Size = { 100, Block::Height };
	m_modCategory = BlockRegistry::GetRegistry().GetCategory(m_modBlock->GetCategory());
	m_Background->SetWeak(m_Background);
	m_Background->SetColor(m_modCategory->GetColor());
	m_Background->SetDepth(Renderer::DepthBlock);

	UpdateArguments();
}

//Block::Block(const Block& block)
//	:m_modBlock(block.m_modBlock)
//{
//	m_modCategory = block.m_modCategory;
//
//	//for (uint64_t i = 0; i < m_arguments.size(); i++)
//	//	delete m_arguments[i];
//
//	m_arguments.clear();
//
//	for (uint64_t i = 0; i < block.m_arguments.size(); i++)
//		m_arguments.push_back(new Argument(*block.m_arguments[i]));
//}

Block::~Block()
{
	//for (uint64_t i = 0; i < m_arguments.size(); i++)
	//	delete m_arguments[i];
}

void Block::AddArgument(std::shared_ptr<Argument> argument)
{
	m_arguments.push_back(argument);

	UpdateWidth();
}

void Block::AddArguments(const std::vector<std::shared_ptr<Argument>>& arguments)
{
	if (m_arguments.size() + arguments.size() >= m_arguments.capacity())
		m_arguments.reserve((uint64_t)std::ceil((float)(m_arguments.size() + arguments.size()) * 1.5f + 10.0f));

	for (uint64_t i = 0; i < arguments.size(); i++)
		m_arguments.push_back(arguments[i]);

	UpdateWidth();
}

const std::vector<std::shared_ptr<Argument>>& Block::GetArguments()
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

int16_t Block::Padding = 4;
int16_t Block::Height = 30;
int16_t Block::HeightContent = Height - (Padding * 2);

void Block::FrameUpdate(double deltaTime)
{
	for (auto& arg : m_arguments)
		arg->FrameUpdate(deltaTime);
}

void Block::OnRender()
{
	for (auto& arg : m_arguments)
		arg->Render();

	m_Background->Render();
}

void Block::OnUpdateBuffers()
{
	for (auto& arg : m_arguments)
		arg->UpdateBuffers();

	m_Background->UpdateBuffers();
}

void Block::OnReloadSwapChain()
{
	for (auto& arg : m_arguments)
		arg->ReloadSwapChain();

	m_Background->ReloadSwapChain();
}

void Block::PostPositionUpdate()
{
	for (auto& arg : m_arguments)
		arg->SetSuperOffset(m_Position + m_SuperOffset);

	m_Background->SetSuperOffset(m_Position + m_SuperOffset);
}

void Block::UpdateArguments()
{
	//for (uint64_t i = 0; i < m_arguments.size(); i++)
	//	delete m_arguments[i];

	m_arguments.clear();
	m_arguments.reserve(m_modBlock->GetArguments().size());

	std::vector<BlockArgumentInitializer> argumentInit = m_modBlock->GetArguments();
	std::vector<std::shared_ptr<Argument>> args;

	uint32_t width = Block::Padding;
	Color argColor = Color().FromNormalized(m_modCategory->GetColor().GetNormalized() * 0.1f);

	for (uint64_t i = 0; i < argumentInit.size(); i++)
	{
		if (argumentInit[i].Type == BlockArgumentType::TEXT)
		{
			std::shared_ptr<ArgumentText> arg = std::make_shared<ArgumentText>(argColor);
			arg->SetWeak(arg);
			args.push_back(arg);

			arg->SetPosition({ width, Block::Padding });
			arg->SetSuperOffset(m_Position + m_SuperOffset);
			arg->SetData(argumentInit[i].DefaultValue);

			width += arg->GetWidth() + Block::Padding;
		}
		else if (argumentInit[i].Type == BlockArgumentType::REAL)
		{
			std::shared_ptr<ArgumentReal> arg = std::make_shared<ArgumentReal>(argColor);
			arg->SetWeak(arg);
			args.push_back(arg);

			arg->SetPosition({ width, Block::Padding });
			arg->SetSuperOffset(m_Position + m_SuperOffset);
			arg->SetData(argumentInit[i].DefaultValue);

			width += arg->GetWidth() + Block::Padding;
		}
		/*else if (argumentInit[i].Type == BlockArgumentType::STRING)
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

	m_Size.x = m_width;

	m_Background->SetSize(m_Size);
}