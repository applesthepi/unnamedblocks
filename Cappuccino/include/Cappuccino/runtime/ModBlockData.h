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
	MODBLOCK_EXPORT ModBlockData(const std::vector<void*>& data, const std::vector<ModBlockDataType>& types, const std::vector<ModBlockDataInterpretation>& interpretations);

	const std::vector<void*>& GetData();
	void** GetCData();
	const std::vector<ModBlockDataType>& GetTypes();
	const std::vector<ModBlockDataInterpretation>& GetInterpretations();

	void HaulData(const std::vector<int64_t>& data);
	void SetTypes(const std::vector<ModBlockDataType>& types);
	void SetInterpretations(const std::vector<ModBlockDataInterpretation>& interpretations);
private:
	std::vector<void*> m_data;
	std::vector<ModBlockDataType> m_types;
	std::vector<ModBlockDataInterpretation> m_interpretations;
};