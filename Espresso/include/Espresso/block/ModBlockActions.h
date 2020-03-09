#pragma once

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
	virtual void Execute();
};

class ModBlockActions
{
public:
	ModBlockActions();
	
private:
	std::vector<
};
