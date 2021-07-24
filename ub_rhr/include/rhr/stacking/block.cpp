#include "Block.hpp"

#include "rhr/stacking/arguments/text.hpp"
#include "rhr/stacking/arguments/real.hpp"
#include "rhr/stacking/arguments/boolean.hpp"
#include "rhr/stacking/arguments/string.hpp"
#include "rhr/stacking/arguments/any.hpp"
#include "rhr/registries/block.hpp"
#include "rhr/rendering/renderer.hpp"

rhr::stack::block::block(const std::string& unlocalized_name)
	: m_modBlock(BlockRegistry::GetRegistry().GetBlock(unlocalized_name)->BlockModBlock)
	, m_Background(std::make_shared<vui::RenderRectangle>())
{
	m_Size = { 100, rhr::stack::block::Height };
	m_modCategory = BlockRegistry::GetRegistry().GetCategory(m_modBlock->GetCategory())->CatagoryModCatagory;
	m_Background->SetWeak(m_Background);
	m_Background->SetColor(m_modCategory->GetColor());
	m_Background->SetDepth(Renderer::depth_block);

	UpdateArguments();
}

//rhr::stack::block::Block(const Block& block)
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
//		m_arguments.push_back(new rhr::stack::argument::argument(*block.m_arguments[i]));
//}

rhr::stack::block::~Block()
{
	//for (uint64_t i = 0; i < m_arguments.size(); i++)
	//	delete m_arguments[i];
}

void rhr::stack::block::AddArgument(std::shared_ptr<rhr::stack::argument::argument> argument)
{
	m_arguments.push_back(argument);

	UpdateWidth();
}

void rhr::stack::block::AddArguments(const std::vector<std::shared_ptr<rhr::stack::argument::argument>>& arguments)
{
	if (m_arguments.size() + arguments.size() >= m_arguments.capacity())
		m_arguments.reserve((uint64_t)std::ceil((float)(m_arguments.size() + arguments.size()) * 1.5f + 10.0f));

	for (uint64_t i = 0; i < arguments.size(); i++)
		m_arguments.push_back(arguments[i]);

	UpdateWidth();
}

const std::vector<std::shared_ptr<rhr::stack::argument::argument>>& rhr::stack::block::GetArguments()
{
	return m_arguments;
}

uint32_t rhr::stack::block::get_width()
{
	return m_width;
}

const ModBlock* rhr::stack::block::GetModBlock()
{
	return m_modBlock;
}

const ModCatagory* rhr::stack::block::GetModCategory()
{
	return m_modCategory;
}

i16 rhr::stack::block::Padding = 2;
i16 rhr::stack::block::Height = 20;
i16 rhr::stack::block::HeightContent = Height - (Padding * 2);

void rhr::stack::block::FrameUpdate(double deltaTime)
{
	for (auto& arg : m_arguments)
		arg->FrameUpdate(deltaTime);
}

void rhr::stack::block::OnRender()
{
	m_Background->Render();

	for (auto& arg : m_arguments)
		arg->Render();
}

void rhr::stack::block::OnUpdateBuffers()
{
	m_Background->UpdateBuffers();

	for (auto& arg : m_arguments)
		arg->UpdateBuffers();
}

void rhr::stack::block::OnReloadSwapChain()
{
	m_Background->ReloadSwapChain();

	for (auto& arg : m_arguments)
		arg->ReloadSwapChain();
}

void rhr::stack::block::PostPositionUpdate()
{
	m_Background->SetSuperOffset(m_Position + m_SuperOffset);

	for (auto& arg : m_arguments)
		arg->SetSuperOffset(m_Position + m_SuperOffset);
}

void rhr::stack::block::UpdateArguments()
{
	//for (uint64_t i = 0; i < m_arguments.size(); i++)
	//	delete m_arguments[i];

	m_arguments.clear();
	m_arguments.reserve(m_modBlock->GetArguments().size());

	std::vector<BlockArgumentInitializer> argumentInit = m_modBlock->GetArguments();
	std::vector<std::shared_ptr<rhr::stack::argument::argument>> args;

	uint32_t width = rhr::stack::block::Padding;
	Color argColor = Color().FromNormalized(m_modCategory->GetColor().GetNormalized() * 0.25f);

	for (uint64_t i = 0; i < argumentInit.size(); i++)
	{
		if (argumentInit[i].Type == BlockArgumentType::TEXT)
		{
			std::shared_ptr<ArgumentText> arg = std::make_shared<ArgumentText>(argColor);
			arg->SetWeak(arg);
			args.push_back(arg);

			arg->SetPosition({ width, rhr::stack::block::Padding });
			arg->SetSuperOffset(m_Position + m_SuperOffset);
			arg->set_data(argumentInit[i].DefaultValue);

			width += arg->get_width() + rhr::stack::block::Padding;
		}
		else if (argumentInit[i].Type == BlockArgumentType::REAL)
		{
			std::shared_ptr<ArgumentReal> arg = std::make_shared<ArgumentReal>(argColor);
			arg->SetWeak(arg);
			args.push_back(arg);

			arg->SetPosition({ width, rhr::stack::block::Padding });
			arg->SetSuperOffset(m_Position + m_SuperOffset);
			arg->set_data(argumentInit[i].DefaultValue);

			width += arg->get_width() + rhr::stack::block::Padding;
		}
		else if (argumentInit[i].Type == BlockArgumentType::STRING)
		{
			std::shared_ptr<ArgumentString> arg = std::make_shared<ArgumentString>(argColor);
			arg->SetWeak(arg);
			args.push_back(arg);

			arg->SetPosition({ width, rhr::stack::block::Padding });
			arg->SetSuperOffset(m_Position + m_SuperOffset);
			arg->set_data(argumentInit[i].DefaultValue);

			width += arg->get_width() + rhr::stack::block::Padding;
		}
		else if (argumentInit[i].Type == BlockArgumentType::BOOL)
		{
			std::shared_ptr<ArgumentBoolean> arg = std::make_shared<ArgumentBoolean>(argColor);
			arg->SetWeak(arg);
			args.push_back(arg);

			arg->SetPosition({ width, rhr::stack::block::Padding });
			arg->SetSuperOffset(m_Position + m_SuperOffset);
			arg->set_data(argumentInit[i].DefaultValue);

			width += arg->get_width() + rhr::stack::block::Padding;
		}
		/*else if (argumentInit[i].Type == BlockArgumentType::ANY)
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

void rhr::stack::block::UpdateWidth()
{
	m_width = rhr::stack::block::Padding;

	for (uint64_t i = 0; i < m_arguments.size(); i++)
	{
		m_width += m_arguments[i]->get_width();
		m_width += rhr::stack::block::Padding;
	}

	m_Size.x = m_width;

	m_Background->SetSize(m_Size);
}