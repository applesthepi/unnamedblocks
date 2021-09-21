#pragma once
#include "mod_vin/categories.hpp"

#include <Cappuccino/Logger.hpp>

#define FUNCTION_FINDER_NAME "vin_function_finder"

class FunctionFinder
{
public:
	void AddFunction(const std::string& name, u64 stackIdx)
	{
		for (u64 i = 0; i < m_names.size(); i++)
		{
			if (m_names[i] == name)
				return;
		}

		m_names.push_back(name);
		m_stackIdx.push_back(stackIdx);
	}

	u64 GetFunctionStackIdx(const std::string& name)
	{
		for (u64 i = 0; i < m_names.size(); i++)
		{
			if (m_names[i] == name)
				return m_stackIdx[i];
		}

		Logger::Error("failed to locate function \"" + name + "\"");

		return 0;
	}
private:
	std::vector<std::string> m_names;
	std::vector<u64> m_stackIdx;
};

class BlockUtilityFunctionDefine : public ModBlock
{
public:
	const char* get_unlocalized_name() const override;
	const char* get_category() const override;

	bool IsTopical() const override;

	cap::mod::block::block::execution pull_execute_debug() const override;
	cap::mod::block::block::execution pull_execute_release() const override;

	blockInitialization GetRuntimeGlobalPreInit() const override;
	blockInitialization GetRuntimeGlobalPostInit() const override;

	std::vector<std::pair<blockDataInitialization, u16>> GetRuntimeStages() const override;

	std::vector<cap::mod::block::block::argument::initializer> get_arguments() const override;
};
