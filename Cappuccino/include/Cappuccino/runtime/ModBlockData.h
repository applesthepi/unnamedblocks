#pragma once
#include "DataRegistry.h"
#include "config.h"

#include <vector>
#include <string>

enum class ModBlockDataType
{
	VAR, RAW
};

enum class ModBlockDataInterpretation
{
	TEXT, STRING, REAL, BOOL
};

#ifdef LINUX
#define MODBLOCK_EXPORT
#else
#define MODBLOCK_EXPORT __declspec(dllexport)
#endif

class ModBlockData
{
public:
	MODBLOCK_EXPORT ModBlockData();
	MODBLOCK_EXPORT ~ModBlockData();
	MODBLOCK_EXPORT ModBlockData(const std::vector<void*>& data, const std::vector<ModBlockDataType>& types, const std::vector<ModBlockDataInterpretation>& interpretations);

	MODBLOCK_EXPORT const std::vector<void*>& GetData();
	MODBLOCK_EXPORT std::vector<void*>& GetPreData();
	MODBLOCK_EXPORT void** GetCData();
	MODBLOCK_EXPORT void** GetCPreData();
	MODBLOCK_EXPORT const uint64_t GetDataSize();
	MODBLOCK_EXPORT const uint64_t GetPreDataSize();
	MODBLOCK_EXPORT const std::vector<ModBlockDataType>& GetTypes();
	MODBLOCK_EXPORT const std::vector<ModBlockDataInterpretation>& GetInterpretations();

	MODBLOCK_EXPORT void HaulData(const std::vector<int64_t>& data, double* vReal, bool* vBool, std::string* vString);
	MODBLOCK_EXPORT void SetTypes(const std::vector<ModBlockDataType>& types);
	MODBLOCK_EXPORT void SetInterpretations(const std::vector<ModBlockDataInterpretation>& interpretations);
	MODBLOCK_EXPORT ModBlockData& operator=(ModBlockData& data);
private:
	std::vector<void*> m_data;
	std::vector<void*> m_preData;
	std::vector<ModBlockDataType> m_types;
	std::vector<ModBlockDataInterpretation> m_interpretations;
	bool m_setup;
};