#pragma once
#include "mod_vin/categories.hpp"

#include <espresso/mod/data.hpp>
#include <vector>
#include <string>

#define POINT_FINDER_NAME "vin_points"

class PointFinder
{
public:
	void AddPoint(u64 idx, std::string& data)
	{
		m_names.push_back(data);
		m_idx.push_back(idx);
	}

	i64 FindPoint(std::string& data)
	{
		for (u64 i = 0; i < m_names.size(); i++)
		{
			if (m_names[i] == data)
				return m_idx[i];
		}

		Logger::Error("must have matching points");
		return -1;
	}
private:
	std::vector<std::string> m_names;
	std::vector<u64> m_idx;
};

class BlockUtilityMarkPoint : public ModBlock
{
public:
	const char* get_unlocalized_name() const override;
	const char* GetCategory() const override;

	blockExecution PullExecuteDebug() const override;
	blockExecution PullExecuteRelease() const override;

	blockInitialization GetRuntimeLocalPreInit() const override;
	blockInitialization GetRuntimeLocalPostInit() const override;
	std::vector<std::pair<blockDataInitialization, u16>> GetRuntimeStages() const override;

	const std::vector<BlockArgumentInitializer> GetArguments() const override;
};
