#pragma once
#include "../preprocessor/PreProcessorData.h"

#include <vector>
enum class ModBlockActionTypes
{
	ActionLogInfo,
	ActionLogWarn,
	ActionLogError,
	ActionCreateLocal,
	ActionCreateGlobal,
	ActionAddLocal,
	ActionAddGlobal,
	ActionPullLocal,
	ActionPullGlobal,
	ActionRemoveLocal,
	ActionRemoveGlobal
};

class ModBlockAction
{
public:
	ModBlockAction(ModBlockActionTypes type);
	
	ModBlockActionTypes GetType() const;
	virtual void Execute(PreProcessorData& data) const;
private:
	ModBlockActionTypes m_type;
};

class ModBlockActions
{
public:
	ModBlockActions();

	void AddAction(const ModBlockAction& action);
	const std::vector<ModBlockAction>& PullActions() const;
private:
	std::vector<ModBlockAction> m_actions;
};
