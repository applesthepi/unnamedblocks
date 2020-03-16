#include "PreProcessor.h"

#include <boost/filesystem.hpp>
#include <fstream>

#ifdef POSIX
#include <dlfcn.h>
#else
#include <windows.h>
#endif

void PullFile(std::vector<std::string>& lines, const std::string& file)
{
	std::ifstream stream(file);
	std::string line;

	while (std::getline(stream, line))
		lines.push_back(line);

	stream.close();
}

void ThreadPreProcessorBuild()
{
	PreProcessor::SetStatus(PreProcessorStatus::RUNNING);

	std::vector<std::string> finalLines;

	PullFile(finalLines, "res/templates/includes.c");
	PullFile(finalLines, "res/templates/main_begin.c");

	for (auto& entry : boost::filesystem::directory_iterator("runtime/build/"))//https://stackoverflow.com/questions/4430780/how-can-i-extract-the-file-name-and-extension-from-a-path-in-c
	{
		std::string na = entry.path().stem().string();
		std::string ex = entry.path().extension().string();

		PullFile(finalLines, "runtime/build/" + na + ex);
	}

	PullFile(finalLines, "res/templates/main_end.c");

	{
		std::ofstream stream("runtime/comp.c");
		for (uint64_t i = 0; i < finalLines.size(); i++)
			stream << finalLines[i] + "\n";

		stream.close();
	}
	

	PreProcessor::SetStatus(PreProcessorStatus::DONE);
}

void ThreadPreProcessorExecution()
{
	PreProcessor::SetFinished(false);// just in case

#ifdef POSIX

#else
	typedef void(*f_run)();

	boost::filesystem::remove("runtime/comp.dll");

	system("call \"runtime/tcc/tcc.exe\" runtime/comp.c -shared -o comp.dll");

	if (boost::filesystem::exists("runtime/comp.dll"))
	{
		HINSTANCE dll = LoadLibrary((LPCWSTR)"./comp.dll");
		f_run runCall = (f_run)GetProcAddress(dll, "re");

		Logger::Debug("...compiled successfully, running.");

		runCall();
	}
	else
		Logger::Debug("...compilation failed.");
#endif

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
	m_units.push_back(PreProcessorTranslationUnit());
	m_units.back().Start(m_units.size(), stack, blockRegistry);
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
