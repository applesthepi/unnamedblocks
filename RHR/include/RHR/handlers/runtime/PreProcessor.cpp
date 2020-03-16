#include "PreProcessor.h"

#include <boost/filesystem.hpp>

void ThreadPreProcessorBuild()
{
	PreProcessor::SetStatus(PreProcessorStatus::RUNNING);



	PreProcessor::SetStatus(PreProcessorStatus::DONE);
}

void ThreadPreProcessorExecution()
{
	PreProcessor::SetFinished(false);// just in case



	PreProcessor::SetFinished(true);
}

void PreProcessor::Cleanup()
{
	boost::filesystem::remove_all("runtime/build/");
	boost::filesystem::create_directory("runtime/build/");
	m_units.clear();
	m_status = PreProcessorStatus::NOT_READY;
	m_finished = false;
}

const uint64_t PreProcessor::InitializeTranslationUnit(const Stack* stack, BlockRegistry* blockRegistry)
{
	m_units.push_back(PreProcessorTranslationUnit(m_units.size(), stack, blockRegistry));
	return m_units.size() - 1;
}

PreProcessorTranslationUnitStatus PreProcessor::GetTranslationUnitStatus(const uint64_t& idx)
{
	return m_units[idx].GetTranslationUnitStatus();
}

void PreProcessor::BeginBuild(const bool& releaseBuild)
{
	m_thread = std::thread(ThreadPreProcessorBuild);
	m_thread.detach();
}

PreProcessorStatus PreProcessor::GetStatus()
{
	std::unique_lock<std::mutex> lock(m_statusMutex);
	return m_status;
}

void PreProcessor::Start()
{
	m_thread = std::thread(ThreadPreProcessorExecution);
	m_thread.detach();
}

const bool PreProcessor::IsFinished()
{
	return m_finished;
}

void PreProcessor::SetStatus(PreProcessorStatus status)
{
	std::unique_lock<std::mutex> lock(m_statusMutex);
	m_status = status;
}

void PreProcessor::SetFinished(const bool& finished)
{
	m_finished = finished;
}

std::vector<PreProcessorTranslationUnit> PreProcessor::m_units;

std::mutex PreProcessor::m_statusMutex;

PreProcessorStatus PreProcessor::m_status;

std::thread PreProcessor::m_thread;

std::atomic<bool> PreProcessor::m_finished;
