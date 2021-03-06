#pragma once
#include "ModVin/ModCategories.h"

#include <Espresso/Espresso.h>
#include <vector>
#include <string>

#define POINT_FINDER_NAME "vin_points"

class PointFinder
{
public:
	void AddPoint(const uint64_t& idx, std::string& data)
	{
		m_names.push_back(data);
		m_idx.push_back(idx);
	}

	const int64_t FindPoint(std::string& data)
	{
		for (uint64_t i = 0; i < m_names.size(); i++)
		{
			if (m_names[i] == data)
				return m_idx[i];
		}

		Logger::Error("must have matching points");
		return -1;
	}
private:
	std::vector<std::string> m_names;
	std::vector<uint64_t> m_idx;
};

class BlockUtilityMarkPoint : public ModBlock
{
public:
	const char* GetUnlocalizedName() const override;
	const char* GetCategory() const override;

	blockExecution PullExecuteDebug() const override;
	blockExecution PullExecuteRelease() const override;

	blockInitialization GetRuntimeLocalPreInit() const override;
	blockInitialization GetRuntimeLocalPostInit() const override;
	blockDataInitialization GetRuntimeInit() const override;

	const std::vector<BlockArgumentInitializer> GetArguments() const override;
};
