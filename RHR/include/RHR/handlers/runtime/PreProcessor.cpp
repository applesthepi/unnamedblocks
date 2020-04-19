#include "PreProcessor.h"
#include "RHR/handlers/ProjectHandler.h"

#include <libtcc/libtcc.h>

void PullFile(std::vector<std::string>& lines, const std::string& file)
{
	std::ifstream stream(file);
	std::string line;

	while (std::getline(stream, line))
		lines.push_back(line);

	stream.close();
}

void PullFileSingle(std::string& line, const std::string& file)
{
	std::ifstream stream(file);
	std::string pulledLine;

	while (std::getline(stream, pulledLine))
		line += pulledLine + "\n";

	stream.close();
}

void ThreadPreProcessorExecution()
{
	PreProcessor::SetFinished(false);// just in case

	std::string line;
	PullFileSingle(line, "res/comp.c");

	TCCState* state = tcc_new();

	tcc_add_library_path(state, "mods/");

	for (uint32_t i = 0; i < ProjectHandler::Mods.size(); i++)
	{
		if (ProjectHandler::Mods[i] == "")
		{
			Logger::Warn("failed to register unknown mod into static build");
			continue;
		}

		tcc_add_library(state, ProjectHandler::Mods[i].c_str());
	}

	tcc_set_output_type(state, TCC_OUTPUT_MEMORY);
	tcc_compile_string(state, line.c_str());

	std::string functions;

	void (**calls)(ModBlockPass* pass);
	calls = (void (**)(ModBlockPass* pass))malloc(sizeof(void (*)(ModBlockPass* pass)) * 2);

	tcc_add_symbol(state, "calls", calls);

	void* mem = malloc(tcc_relocate(state, nullptr));
	tcc_relocate(state, mem);

	typedef void(*fun_main)();

	fun_main f_main = (fun_main)tcc_get_symbol(state, "main");

	tcc_delete(state);

	f_main();
	PreProcessor::SetFinished(true);
}

void PreProcessor::Cleanup()
{
	m_finished = false;
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

void PreProcessor::SetFinished(const bool& finished)
{
	m_finished = finished;
}

std::thread PreProcessor::m_thread;

std::atomic<bool> PreProcessor::m_finished;