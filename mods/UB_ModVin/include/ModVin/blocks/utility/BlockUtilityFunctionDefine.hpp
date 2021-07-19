#pragma once
#include "ModVin/ModCategories.hpp"

#include <Cappuccino/Logger.hpp>

#define FUNCTION_FINDER_NAME "vin_function_finder"

class FunctionFinder
{
public:
	void AddFunction(const std::string& name, uint64_t stackIdx)
	{
		for (uint64_t i = 0; i < m_names.size(); i++)
		{
			if (m_names[i] == name)
				return;
		}

		m_names.push_back(name);
		m_stackIdx.push_back(stackIdx);
	}

	uint64_t GetFunctionStackIdx(const std::string& name)
	{
		for (uint64_t i = 0; i < m_names.size(); i++)
		{
			if (m_names[i] == name)
				return m_stackIdx[i];
		}

		Logger::Error("failed to locate function \"" + name + "\"");

		return 0;
	}
private:
	std::vector<std::string> m_names;
	std::vector<uint64_t> m_stackIdx;
};

class BlockUtilityFunctionDefine : public ModBlock
{
public:
	const char* GetUnlocalizedName() const override;
	const char* GetCategory() const override;

	bool IsTopical() const override;

	blockExecution PullExecuteDebug() const override;
	blockExecution PullExecuteRelease() const override;

	blockInitialization GetRuntimeGlobalPreInit() const override;
	blockInitialization GetRuntimeGlobalPostInit() const override;

	std::vector<std::pair<blockDataInitialization, uint16_t>> GetRuntimeStages() const override;

	const std::vector<BlockArgumentInitializer> GetArguments() const override;
};
