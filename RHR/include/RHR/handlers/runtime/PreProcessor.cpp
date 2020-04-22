#include "PreProcessor.h"
#include "RHR/handlers/ProjectHandler.h"

#include <libtcc.h>

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

void ThreadPreProcessorExecution(bool debugBuild)
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

		tcc_add_library(state, (ProjectHandler::Mods[i] + "-s.lib").c_str());
	}

	int r0 = tcc_add_library_path(state, "Cappuccino/lib");
	int r1 = tcc_add_library(state, "Cappuccino");

	tcc_add_include_path(state, "Cappuccino/include");
	tcc_add_include_path(state, "csfml/include");

	tcc_set_output_type(state, TCC_OUTPUT_MEMORY);
	tcc_compile_string(state, line.c_str());

	std::vector<std::string> functionReferences;
	std::vector<uint32_t> functionIds;
	std::vector<uint64_t> functionCallCount;
	std::vector<std::vector<void(*)(ModBlockPass* pass)>> functionCalls;

	const std::vector<Stack*>* stacks = PreProcessor::GetPlaneCopy()->GetAllStacks();

	for (uint64_t i = 0; i < stacks->size(); i++)
	{
		if (stacks->at(i)->GetBlockCount() >= 1 && stacks->at(i)->GetBlock(0)->GetUnlocalizedName() == "vin_function_begin")
		{
			functionReferences.push_back(*stacks->at(i)->GetBlock(0)->GetArgument(1)->GetDataRaw());
			functionIds.push_back(functionReferences.size());

			std::vector<void(*)(ModBlockPass* pass)> transCalls;

			for (uint64_t a = 1; a < stacks->at(i)->GetBlockCount(); a++)
			{
				if (debugBuild)
					transCalls.push_back(PreProcessor::GetRegistry()->GetExeDebug(stacks->at(i)->GetBlock(a)->GetUnlocalizedName()));
				else
					transCalls.push_back(PreProcessor::GetRegistry()->GetExeRelease(stacks->at(i)->GetBlock(a)->GetUnlocalizedName()));
			}

			functionCalls.push_back(transCalls);
			functionCallCount.push_back(transCalls.size());
		}
	}

	void (***calls)(ModBlockPass*);
	calls = (void(***)(ModBlockPass*))malloc(sizeof(void(**)(ModBlockPass*)) * functionCalls.size());

	for (uint64_t i = 0; i < functionCalls.size(); i++)
	{
		void (**callsInside)(ModBlockPass*);
		callsInside = (void(**)(ModBlockPass*))malloc(sizeof(void(*)(ModBlockPass*)) * functionCalls[i].size());

		for (uint64_t a = 0; a < functionCalls[i].size(); a++)
			callsInside[a] = functionCalls[i][a];

		calls[i] = callsInside;
	}

	uint64_t* functionCallCountC = (uint64_t*)malloc(sizeof(uint64_t) * functionCalls.size());

	for (uint64_t i = 0; i < functionCallCount.size(); i++)
		functionCallCountC[i] = functionCallCount[i];

	bool* buildType = (bool*)malloc(sizeof(bool));
	*buildType = debugBuild;

	tcc_add_symbol(state, "calls", calls);
	tcc_add_symbol(state, "functionCallCount", functionCallCountC);
	tcc_add_symbol(state, "debugBuild", buildType);

	void* mem = malloc(tcc_relocate(state, nullptr));
	tcc_relocate(state, mem);

	typedef void(*fun_main)();
	fun_main f_main = (fun_main)tcc_get_symbol(state, "main");

	tcc_delete(state);
	f_main();

	//TODO delete memory

	PreProcessor::SetFinished(true);
}

void PreProcessor::Initialize()
{
	m_planeCopy = nullptr;
}

void PreProcessor::Cleanup()
{
	m_finished = false;
	if (m_planeCopy != nullptr)
	{
		delete m_planeCopy;
		m_planeCopy = nullptr;
	}
}

void PreProcessor::Start(Plane* planeCopy, BlockRegistry* registry, bool debugBuild)
{
	m_planeCopy = planeCopy;
	m_registry = registry;
	m_finished = false;
	m_thread = std::thread(ThreadPreProcessorExecution, debugBuild);
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

Plane* PreProcessor::GetPlaneCopy()
{
	return m_planeCopy;
}

BlockRegistry* PreProcessor::GetRegistry()
{
	return m_registry;
}

std::thread PreProcessor::m_thread;

std::atomic<bool> PreProcessor::m_finished;

Plane* PreProcessor::m_planeCopy;

BlockRegistry* PreProcessor::m_registry;
