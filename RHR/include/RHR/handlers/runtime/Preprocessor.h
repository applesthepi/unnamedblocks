#pragma once
#include "PreProcessorTranslationUnit.h"

#include <vector>
#include <map>
#include <string>
#include <thread>
#include <atomic>

enum class PreProcessorStatus
{
	NOT_READY,
	RUNNING,
	DONE,
	DONE_ERROR
};

class PreProcessor
{
public:
	// preprocessor translation unit building
	static void Cleanup();
	static const uint64_t InitializeTranslationUnit(const Stack* stack, BlockRegistry* blockRegistry);
	static PreProcessorTranslationUnitStatus GetTranslationUnitStatus(const uint64_t& idx);
	
	// building
	static void BeginBuild(const bool& releaseBuild);
	static PreProcessorStatus GetStatus();

	// executing
	static void Start();
	static const bool IsFinished();
	
	// util
	static void SetStatus(PreProcessorStatus status);
	static void SetFinished(const bool& finished);
private:
	static std::vector<PreProcessorTranslationUnit> m_units;

	static std::mutex m_statusMutex;
	static PreProcessorStatus m_status;

	static std::thread m_thread;
	static std::atomic<bool> m_finished;
};
