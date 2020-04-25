#include "PreProcessor.h"
#include "RHR/handlers/ProjectHandler.h"

#include <fstream>
#include <libtcc.h>

void PullFileSingle(std::string& file, const char* file_path)
{
	std::FILE *fp = std::fopen(file_path, "rb");
	if (fp)
	{
		std::fseek(fp, 0, SEEK_END);
		file.resize(std::ftell(fp));
		std::rewind(fp);
		std::fread(&file[0], 1, file.size(), fp);
		std::fclose(fp);
	}
	else
	{
		throw(errno);
	}
}
void nop(){}

void ThreadPreProcessorExecution(bool debugBuild)
{
	//system("COPY %CD%\\Cappuccino\\lib\\Cappuccino.dll Cappuccino.dll");
	//system("tcc.exe res/comp.c -I Cappuccino/include -I csfml/include -I res -LCappuccino/lib -Lcsfml/lib/gcc -lCappuccino -lcsfml-graphics -lcsfml-window -lcsfml-system");
	//system("comp.exe");

	//return;

	PreProcessor::SetFinished(false);// just in case
	std::string file;
	TCCState* state = tcc_new();

	tcc_add_include_path(state, "Cappuccino/include");
	tcc_add_include_path(state, "csfml/include");
	tcc_add_include_path(state, "res");

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

#ifdef LINUX
	tcc_define_symbol(state, "LINUX", "1");
#endif

	// compile

	tcc_set_output_type(state, TCC_OUTPUT_MEMORY);
	[[maybe_unused]] int r2 = tcc_compile_string(state, file.c_str());

	[[maybe_unused]] int r14 = tcc_add_symbol(state, "functionMain", (void*)nop);
	[[maybe_unused]] int r7 = tcc_add_symbol(state, "calls", calls);
	[[maybe_unused]] int r8 = tcc_add_symbol(state, "functionCallCount", functionCallCountC);
	[[maybe_unused]] int r9 = tcc_add_symbol(state, "debugBuild", buildType);
	// libs

	tcc_add_library_path(state, "mods/");

	for (uint32_t i = 0; i < ProjectHandler::Mods.size(); i++)
	{
		if (ProjectHandler::Mods[i] == "")
		{
			Logger::Warn("failed to register unknown mod into static build");
			continue;
		}

		if (tcc_add_library(state, (ProjectHandler::Mods[i]).c_str()) == -1)
			Logger::Error("failed to link \"" + ProjectHandler::Mods[i] + "\"");
	}

	tcc_add_library_path(state, "Cappuccino/lib");
	[[maybe_unused]] int re = tcc_add_library(state, "Cappuccino");
	tcc_add_library_path(state, "csfml/lib/gcc");

	tcc_add_library(state, "csfml-audio");
	tcc_add_library(state, "csfml-graphics");
	tcc_add_library(state, "csfml-network");
	tcc_add_library(state, "csfml-system");
	tcc_add_library(state, "csfml-window");

	// run

	//tcc_run(state, 0, 0);

	//void* mem = malloc(tcc_relocate(state, nullptr));
	//tcc_relocate(state, mem);
	//
	//typedef int(*fun_main)();
	//fun_main f_main = (fun_main)tcc_get_symbol(state, "main");
	//
	//int rs = f_main();
	//tcc_delete(state);

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

bool PreProcessor::IsFinished()
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
