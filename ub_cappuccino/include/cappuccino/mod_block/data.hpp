#pragma once
#include "config.h"

#include "cappuccino/utils.hpp"
#include "cappuccino/data_registry.hpp"

enum class ModBlockDataType
{
	VAR, RAW
};

enum class ModBlockDataInterpretation
{
	TEXT, STRING, REAL, BOOL, ANY
};

#if LINUX
#define MODBLOCK_EXPORT
#else
#define MODBLOCK_EXPORT __declspec(dllexport)
#endif

class ModBlockData
{
public:
	MODBLOCK_EXPORT ModBlockData();
	MODBLOCK_EXPORT ModBlockData(const std::vector<void*>& data, const std::vector<ModBlockDataType>& types, const std::vector<ModBlockDataInterpretation>& interpretations);

	MODBLOCK_EXPORT const std::vector<void*>& GetData();
	MODBLOCK_EXPORT const std::vector<u64>& GetRuntimeData();
	MODBLOCK_EXPORT std::vector<void*>& GetPreData();
	MODBLOCK_EXPORT const std::vector<ModBlockDataType>& GetTypes();
	MODBLOCK_EXPORT const std::vector<ModBlockDataInterpretation>& GetInterpretations();

	MODBLOCK_EXPORT void ClearData();
	MODBLOCK_EXPORT void SetRuntimeData(const std::vector<u64>& data);
	MODBLOCK_EXPORT void SetDataTemplates(u64 stackIdx);
	MODBLOCK_EXPORT void SetTypes(const std::vector<ModBlockDataType>& types);
	MODBLOCK_EXPORT void SetInterpretations(const std::vector<ModBlockDataInterpretation>& interpretations);
	MODBLOCK_EXPORT void SetInterpretation(const ModBlockDataInterpretation& interpretation, u64 idx);
	MODBLOCK_EXPORT ModBlockData& operator=(ModBlockData& data);
private:
	std::vector<void*> m_data;
	std::vector<u64> m_runtimeData;
	std::vector<void*> m_preData;
	std::vector<ModBlockDataType> m_types;
	std::vector<ModBlockDataInterpretation> m_interpretations;
};
