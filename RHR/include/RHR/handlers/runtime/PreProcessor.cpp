#include "PreProcessor.h"

#include <libtcc/libtcc.h>

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
#ifdef POSIX
	std::string ty = "/linux/";
#else
	std::string ty = "/windows/";
#endif

	PullFile(finalLines, "res/templates" + ty + "includes.c");
	PullFile(finalLines, "res/templates" + ty + "main_begin.c");

	for (auto& entry : boost::filesystem::directory_iterator("runtime/build/"))//https://stackoverflow.com/questions/4430780/how-can-i-extract-the-file-name-and-extension-from-a-path-in-c
	{
		std::string na = entry.path().stem().string();
		std::string ex = entry.path().extension().string();

		PullFile(finalLines, "runtime/build/" + na + ex);
	}

	PullFile(finalLines, "res/templates" + ty + "main_end.c");

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

#ifdef LINUX
	typedef void(*f_run)();

	boost::filesystem::remove("\\runtime\\comp.so");

	system("./runtime/tcc/tcc runtime/comp.c -shared -o runtime/comp.so");

	if (boost::filesystem::exists("runtime/comp.so"))
	{
		PreProcessor::SetSo(".\\runtime\\comp.so");
		f_run runCall = (f_run)dlsym(PreProcessor::GetSo(), "start");

		Logger::Debug("...compiled successfully, running.");

		runCall();

		PreProcessor::SetFinished(true);
		dlclose(PreProcessor::GetSo());
		Logger::Debug("finished running.");
	}
	else
		Logger::Debug("...compilation failed.");
#else
	typedef void(*f_run)();

	std::string compC;
	
	{
		std::ifstream stream;
		std::string line;
		stream.open("runtime/comp.c");

		while (std::getline(stream, line))
			compC += line + "\n";

		stream.close();
	}

	TCCState* state = tcc_new();
	tcc_set_output_type(state, TCC_OUTPUT_MEMORY);
	tcc_compile_string(state, compC.c_str());
	
	void* mem = malloc(tcc_relocate(state, nullptr));
	tcc_relocate(state, mem);

	typedef void(*fun_start)();

	fun_start f_start = (fun_start)tcc_get_symbol(state, "start");

	tcc_delete(state);

	f_start();
	PreProcessor::SetFinished(true);
#endif
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
	m_finished = false;
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
#ifndef POSIX
void PreProcessor::SetDll(LPCWSTR path)
{
	m_dll = new HINSTANCE(LoadLibrary(path));
}

HINSTANCE* PreProcessor::GetDll()
{
	return m_dll;
}
#endif

#ifdef POSIX
void PreProcessor::SetSo(const char* path)
{
	m_so = dlopen(path, RTLD_NOW);
}

void* PreProcessor::GetSo()
{
	return m_so;
}
#endif

std::vector<PreProcessorTranslationUnit> PreProcessor::m_units;

std::mutex PreProcessor::m_statusMutex;

PreProcessorStatus PreProcessor::m_status;

std::thread PreProcessor::m_thread;

std::atomic<bool> PreProcessor::m_finished;

#ifdef POSIX
void* PreProcessor::m_so;
#else
HINSTANCE* PreProcessor::m_dll;
#endif
