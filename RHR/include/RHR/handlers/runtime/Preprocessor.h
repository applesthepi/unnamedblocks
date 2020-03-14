#pragma once
#include "PreProcessorTranslationUnit.h"
#include "RHR/stacking/Stack.h"

#include <vector>
#include <map>
#include <string>

class PreProcessor
{
public:
	static const uint64_t InitializeTranslationUnit(const Stack& stack);
	static PreProcessorTranslationUnitStatus SubDivideTranslationUnit(const uint64_t& idx);
	static PreProcessorTranslationUnitStatus BuildTranslationUnit(const uint64_t& idx);
};
