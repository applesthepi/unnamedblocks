#include "ModBlockActions.h"

ModBlockAction::ModBlockAction(ModBlockActionTypes type)
	:m_type(type)
{

}

ModBlockActionTypes ModBlockAction::GetType() const
{
	return m_type;
}

void Execute(PreProcessorData& data)
{

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
