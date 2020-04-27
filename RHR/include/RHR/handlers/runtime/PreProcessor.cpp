#include "PreProcessor.h"
#include "RHR/handlers/ProjectHandler.h"

#include <cstring>
#include <fstream>
#include <libtcc.h>
#include <Cappuccino/runtime/ModBlockData.h>

/// Reads file at file_path and stores its contents in ptr
void PullFileSingle(char** ptr, const char* file_path)
{
	std::FILE *fp = std::fopen(file_path, "rb");
	if (fp)
	{
		std::fseek(fp, 0, SEEK_END);
		size_t size = static_cast<size_t>(std::ftell(fp));
		*ptr = static_cast<char*>( malloc(size+1) );
		(*ptr)[size] = 0;
		std::rewind(fp);
		std::fread(*ptr, 1, size, fp);
		std::fclose(fp);
	}
	else
	{
		throw(std::strerror(errno));
	}
}

void ThreadPreProcessorExecution(bool debugBuild)
{
	PreProcessor::SetFinished(false);// just in case
	char* file;
	PullFileSingle(&file, "res/comp.c");
	TCCState* state = tcc_new();

	tcc_add_include_path(state, "Cappuccino/include");
	tcc_add_include_path(state, "csfml/include");
	tcc_add_include_path(state, "res");

	std::vector<std::string> functionReferences;
	std::vector<uint32_t> functionIds;
	std::vector<uint64_t> functionCallCount;
	std::vector<std::vector<void(*)(ModBlockPass* pass)>> functionCalls;

	uint64_t* functionMain = static_cast<uint64_t*>(malloc(sizeof(uint64_t)));
	bool functionMainFound = false;
	*functionMain = 0;

	const std::vector<Stack*>* stacks = PreProcessor::GetPlaneCopy()->GetAllStacks();

	ModBlockData** functionData = (ModBlockData**)malloc(sizeof(ModBlockData*) * stacks->size());

	for (uint64_t i = 0; i < stacks->size(); i++)
	{
		functionData[i] = (ModBlockData*)malloc(sizeof(ModBlockData) * stacks->at(i)->GetBlockCount());

		if (stacks->at(i)->GetBlockCount() >= 1 && stacks->at(i)->GetBlock(0)->GetUnlocalizedName() == "vin_main")
		{
			if (functionMainFound)
			{
				Logger::Error("program has more than 1 entry points; can not run program without exactly 1 entry point (vin_main)");

				free(functionMain);

				PreProcessor::SetFinished(true);
				return;
			}

			*functionMain = i;
			functionMainFound = true;
		}

		//functionReferences.push_back(*stacks->at(i)->GetBlock(0)->GetArgument(1)->GetDataRaw());
		//functionIds.push_back(functionReferences.size());

		std::vector<void(*)(ModBlockPass* pass)> transCalls;

		for (uint64_t a = 0; a < stacks->at(i)->GetBlockCount(); a++)
		{
			if (debugBuild)
				transCalls.push_back(PreProcessor::GetRegistry()->GetExeDebug(stacks->at(i)->GetBlock(a)->GetUnlocalizedName()));
			else
				transCalls.push_back(PreProcessor::GetRegistry()->GetExeRelease(stacks->at(i)->GetBlock(a)->GetUnlocalizedName()));

			std::vector<void*> argData;
			std::vector<ModBlockDataType> argTypes;
			std::vector<ModBlockDataInterpretation> argInterpretations;

			for (uint64_t b = 0; b < stacks->at(i)->GetBlock(a)->GetArgumentCount(); b++)
			{
				BlockArgumentType type = stacks->at(i)->GetBlock(a)->GetArgument(b)->GetType();

				if (*stacks->at(i)->GetBlock(a)->GetArgument(b)->GetMode() == BlockArgumentVariableMode::VAR)
				{
					std::string* dt = (std::string*)malloc(sizeof(std::string));

					try
					{
						*dt = *stacks->at(i)->GetBlock(a)->GetArgument(b)->GetDataRaw();
					}
					catch (const std::invalid_argument&)
					{
						Logger::Error("invalid argument exception in preprocessor");
						return;
					}

					argData.push_back((void*)dt);
					argTypes.push_back(ModBlockDataType::VAR);
					
					if (type == BlockArgumentType::TEXT)
						argInterpretations.push_back(ModBlockDataInterpretation::TEXT);
					else if (type == BlockArgumentType::BOOL)
						argInterpretations.push_back(ModBlockDataInterpretation::BOOL);
					else if (type == BlockArgumentType::REAL)
						argInterpretations.push_back(ModBlockDataInterpretation::REAL);
					else if (type == BlockArgumentType::STRING)
						argInterpretations.push_back(ModBlockDataInterpretation::STRING);
				}
				else if (type == BlockArgumentType::STRING)
				{
					std::string* dt = (std::string*)malloc(sizeof(std::string));
					
					try
					{
						*dt = *stacks->at(i)->GetBlock(a)->GetArgument(b)->GetDataRaw();
					}
					catch (const std::invalid_argument&)
					{
						Logger::Error("invalid argument exception in preprocessor");
						return;
					}

					argData.push_back((void*)dt);
					argTypes.push_back(ModBlockDataType::RAW);
					argInterpretations.push_back(ModBlockDataInterpretation::STRING);
				}
				else if (type == BlockArgumentType::BOOL)
				{
					bool* dt = (bool*)malloc(sizeof(bool));

					try
					{
						*dt = *stacks->at(i)->GetBlock(a)->GetArgument(b)->GetDataRaw() == "1";
					}
					catch (const std::invalid_argument&)
					{
						Logger::Error("invalid argument exception in preprocessor");
						return;
					}

					argData.push_back((void*)dt);
					argTypes.push_back(ModBlockDataType::RAW);
					argInterpretations.push_back(ModBlockDataInterpretation::BOOL);
				}
				else if (type == BlockArgumentType::REAL)
				{
					double* dt = (double*)malloc(sizeof(double));
					
					try
					{
						*dt = std::stod(*stacks->at(i)->GetBlock(a)->GetArgument(b)->GetDataRaw());
					}
					catch (const std::invalid_argument&)
					{
						Logger::Error("invalid argument exception in preprocessor");
						return;
					}

					argData.push_back((void*)dt);
					argTypes.push_back(ModBlockDataType::RAW);
					argInterpretations.push_back(ModBlockDataInterpretation::REAL);
				}
			}

			functionData[i][a] = ModBlockData(argData, argTypes, argInterpretations);
		}

		functionCalls.push_back(transCalls);
		functionCallCount.push_back(transCalls.size());
	}

	if (!functionMainFound)
	{
		Logger::Error("program has no entry points; can not run program without exactly 1 entry point (vin_main)");

		free(functionMain);

		PreProcessor::SetFinished(true);
		return;
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

	bool buildType = debugBuild;

	uint64_t* functionTotalCount = (uint64_t*)malloc(sizeof(uint64_t));
	*functionTotalCount = stacks->size();

#ifdef LINUX
	tcc_define_symbol(state, "LINUX", "1");
#endif

	// compile

	tcc_set_output_type(state, TCC_OUTPUT_MEMORY);
	[[maybe_unused]] int r2 = tcc_compile_string(state, file);

	// NOTE: Referencing the pointers is needed. The dereferenced value is what is sent over. int* -> int, int** -> int*, bool* -> bool etc
	[[maybe_unused]] int r14 = tcc_add_symbol(state, "functionMain", &functionMain);
	[[maybe_unused]] int r7 = tcc_add_symbol(state, "calls", &calls);
	[[maybe_unused]] int r8 = tcc_add_symbol(state, "functionCallCount", &functionCallCountC);
	[[maybe_unused]] int r9 = tcc_add_symbol(state, "debugBuild", &buildType);
	[[maybe_unused]] int r900 = tcc_add_symbol(state, "functionData", &functionData);
	[[maybe_unused]] int r9001 = tcc_add_symbol(state, "functionTotalCount", &functionTotalCount);

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

	tcc_run(state, 0, 0);
	
	//
	//void* mem = malloc(tcc_relocate(state, nullptr));
	//tcc_relocate(state, mem);
	//
	//typedef int(*fun_main)();
	//fun_main f_main = (fun_main)tcc_get_symbol(state, "main");
	//
	//int rs = f_main();
	//tcc_delete(state);

	


	// TODO delete memory

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
