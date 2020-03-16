#pragma once
#include "RHR/stacking/Stack.h"
#include "RHR/registries/BlockRegistry.h"

#include <thread>
#include <mutex>
#include <shared_mutex>
#include <vector>

enum class PreProcessorTranslationUnitStatus
{
	NOT_READY,
	RUNNING,
	DONE,
	DONE_ERROR
};

class PreProcessorTranslationUnit
{
public:
	PreProcessorTranslationUnit(PreProcessorTranslationUnit&& unit);
	PreProcessorTranslationUnit(const uint64_t& idx, const Stack* stack, BlockRegistry* blockRegistry);

	void SetTranslationUnitStatus(PreProcessorTranslationUnitStatus status);

	PreProcessorTranslationUnitStatus GetTranslationUnitStatus();
	const std::string& GetFinishedPath() const;
	const Stack* GetStack() const;
	BlockRegistry* GetBlockRegistry() const;
private:
	std::mutex m_statusMutex;
	PreProcessorTranslationUnitStatus m_status;

	std::thread m_thread;
	std::string m_path;
	const Stack* m_stack;
	BlockRegistry* m_blockRegistry;
};