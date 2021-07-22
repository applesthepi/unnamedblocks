#include "ModBlockActions.hpp"

void Execute(PreProcessorData&)
{
	Logger::Warn("empty action executed");
}

ModBlockActions::ModBlockActions()
{

}

void ModBlockActions::AddAction(const ModBlockAction& action)
{
	m_actions.push_back(action);
}

const std::vector<ModBlockAction>& ModBlockActions::PullActions() const
{
	return m_actions;
}
