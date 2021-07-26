#include "preprocessor.hpp"

//#include "handlers/ProjectHandler.hpp"

//#include <Cappuccino/runtime/ModBlockData.hpp>
//#include <libtcc.h>

/*
/// Reads file at file_path and stores its contents in ptr
void PullFileSingle(char** ptr, const char* file_path)
{
	std::FILE *fp = std::fopen(file_path, "rb");
	if (fp)
	{
		std::fseek(fp, 0, SEEK_END);
		usize size = static_cast<usize>(std::ftell(fp));
		*ptr = static_cast<char*>( malloc(size+1) );//
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
	std::vector<u32> functionIds;
	std::vector<u64> functionCallCount;
	std::vector<std::vector<void(*)(ModBlockPass* pass)>> functionCalls;

	u64 functionMain = 0;
	bool functionMainFound = false;

	std::vector<std::shared_ptr<Stack>> stacks;

	for (u64 i = 0; i < PreProcessor::GetPlaneCopy()->get_collections().size(); i++)
	{
		const std::vector<std::shared_ptr<Stack>>& colStacks = PreProcessor::GetPlaneCopy()->get_collections()[i]->get_stacks();

		for (u64 a = 0; a < colStacks.size(); a++)
			stacks.push_back(colStacks[a]);
	}

	ModBlockData** functionData = new ModBlockData*[stacks.size()];
	ModBlock*** modBlocks = new ModBlock**[stacks.size()];
	
	for (u64 i = 0; i < stacks.size(); i++)
	{
		functionData[i] = new ModBlockData[stacks[i]->get_blocks().size()];
		modBlocks[i] = new ModBlock*[stacks[i]->get_blocks().size()];

		if (stacks[i]->get_blocks().size() >= 1 && std::string(stacks[i]->get_blocks()[0]->GetModBlock()->GetUnlocalizedName()) == "vin_main")
		{
			if (functionMainFound)
			{
				Logger::Error("program has more than 1 entry points; can not run program without exactly 1 entry point (vin_main)");
				PreProcessor::SetFinished(true);

				return;
			}

			functionMain = i;
			functionMainFound = true;
		}
		
		// TODO function references to indices (use lambda code in Cappuccino/Registration.cpp)

		//functionReferences.push_back(*stacks->at(i)->GetBlock(0)->GetArgument(1)->GetDataRaw());
		//functionIds.push_back(functionReferences.size());

		std::vector<void(*)(ModBlockPass* pass)> transCalls;

		for (u64 a = 0; a < stacks[i]->get_blocks().size(); a++)
		{
			if (debugBuild)
				transCalls.push_back(stacks[i]->get_blocks()[a]->GetModBlock()->PullExecuteDebug());
			else
				transCalls.push_back(stacks[i]->get_blocks()[a]->GetModBlock()->PullExecuteRelease());

			modBlocks[i][a] = (ModBlock*)stacks[i]->get_blocks()[a]->GetModBlock();

			std::vector<void*> argData;
			std::vector<ModBlockDataType> argTypes;
			std::vector<ModBlockDataInterpretation> argInterpretations;

			for (u64 b = 0; b < stacks[i]->get_blocks()[a]->GetArguments().size(); b++)
			{
				BlockArgumentType type = stacks[i]->get_blocks()[a]->GetArguments()[b]->get_type();

				if (stacks[i]->get_blocks()[a]->GetArguments()[b]->get_mode() == BlockArgumentVariableMode::VAR)
				{
					std::string* dt = new std::string();

					try
					{
						*dt = stacks[i]->get_blocks()[a]->GetArguments()[b]->get_data();
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
					else if (type == BlockArgumentType::ANY)
						argInterpretations.push_back(ModBlockDataInterpretation::ANY);
				}
				else if (type == BlockArgumentType::STRING)
				{
					std::string* dt = new std::string();
					
					try
					{
						*dt = stacks[i]->get_blocks()[a]->GetArguments()[b]->get_data();
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
					bool* dt = new bool;

					try
					{
						*dt = stacks[i]->get_blocks()[a]->GetArguments()[b]->get_data() == "1";
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
					f64* dt = new f64;
					
					try
					{
						*dt = std::stod(stacks[i]->get_blocks()[a]->GetArguments()[b]->get_data());
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
			ModBlockData blockdata = ModBlockData(argData, argTypes, argInterpretations);
			functionData[i][a] = blockdata;
		}

		functionCalls.push_back(transCalls);
		functionCallCount.push_back(transCalls.size());
	}

	if (!functionMainFound)
	{
		Logger::Error("program has no entry points; can not run program without exactly 1 entry point (vin_main)");
		PreProcessor::SetFinished(true);

		return;
	}

	void (***calls)(ModBlockPass*);
	calls = (void(***)(ModBlockPass*))malloc(sizeof(void(**)(ModBlockPass*)) * functionCalls.size());

	for (u64 i = 0; i < functionCalls.size(); i++)
	{
		void (**callsInside)(ModBlockPass*);
		callsInside = (void(**)(ModBlockPass*))malloc(sizeof(void(*)(ModBlockPass*)) * functionCalls[i].size());

		for (u64 a = 0; a < functionCalls[i].size(); a++)
			callsInside[a] = functionCalls[i][a];

		calls[i] = callsInside;
	}

	u64* functionCallCountC = new u64[functionCalls.size()];

	for (u64 i = 0; i < functionCallCount.size(); i++)
		functionCallCountC[i] = functionCallCount[i];

	bool buildType = debugBuild;

	u64 functionTotalCount = stacks.size();

	u8* super = PreProcessor::MakeSuper();
	i64* superData = PreProcessor::GetMadeData();
	std::mutex* superMutex = PreProcessor::GetMadeMutex();

#if LINUX
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
	[[maybe_unused]] int r9002 = tcc_add_symbol(state, "modBlocks", &modBlocks);
	[[maybe_unused]] int r9001 = tcc_add_symbol(state, "functionTotalCount", &functionTotalCount);
	[[maybe_unused]] int r9003 = tcc_add_symbol(state, "superInstruction", &super);
	[[maybe_unused]] int r9005 = tcc_add_symbol(state, "superData", &superData);
	[[maybe_unused]] int r9004 = tcc_add_symbol(state, "superMutex", (void**)&superMutex);

	// libs
	
// 	tcc_add_library_path(state, "mods/");
// 
// 	for (u32 i = 0; i < ProjectHandler::Mods.size(); i++)
// 	{
// 		if (ProjectHandler::Mods[i] == "")
// 		{
// 			Logger::Warn("failed to register unknown mod into static build");
// 			continue;
// 		}
// 
// 		if (tcc_add_library(state, (ProjectHandler::Mods[i]).c_str()) == -1)
// 			Logger::Error("failed to link \"" + ProjectHandler::Mods[i] + "\"");
// 	}

	tcc_add_library_path(state, "Cappuccino/lib");
	[[maybe_unused]] int re = tcc_add_library(state, "Cappuccino");
	tcc_add_library_path(state, "csfml/lib/gcc");

	tcc_add_library(state, "csfml-audio");
	tcc_add_library(state, "csfml-graphics");
	tcc_add_library(state, "csfml-network");
	tcc_add_library(state, "csfml-system");
	tcc_add_library(state, "csfml-window");

	tcc_run(state, 0, 0);
	
	for (u64 i = 0; i < functionTotalCount; i++)
		free(calls[i]);
	free(calls);

	delete[] functionCallCountC;

	for (u64 i = 0; i < functionTotalCount; i++)
		delete[] functionData[i];
	delete[] functionData;

	free(file);

	PreProcessor::SetFinished(true);
}

void PreProcessor::Initialize()
{
	m_planeCopy = nullptr;
	m_super = nullptr;
	m_finished = true;
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

void PreProcessor::Start(Plane* planeCopy, bool debugBuild)
{
	m_planeCopy = planeCopy;
	m_finished = false;
	m_thread = std::thread(ThreadPreProcessorExecution, debugBuild);
	m_thread.detach();
}

bool PreProcessor::IsFinished()
{
	return m_finished;
}

void PreProcessor::SetFinished(bool finished)
{
	m_finished = finished;
}

Plane* PreProcessor::GetPlaneCopy()
{
	return m_planeCopy;
}

void PreProcessor::SetSuper(u8 super, i16 superData)
{
	std::unique_lock<std::mutex> lock(*m_superMutex);

	*m_super = super;
	*m_superData = superData;
}

u8 PreProcessor::GetSuper()
{
	std::unique_lock<std::mutex> lock(*m_superMutex);

	return *m_super;
}

i64 PreProcessor::GetSuperData()
{
	std::unique_lock<std::mutex> lock(*m_superMutex);

	return *m_superData;
}

u8* PreProcessor::MakeSuper()
{
	if (m_super != nullptr)
	{
		delete m_super;
		delete m_superMutex;
	}

	m_super = new u8;
	*m_super = 0;

	m_superData = new i64;
	*m_superData = 0;

	m_superMutex = new std::mutex();

	return m_super;
}

i64* PreProcessor::GetMadeData()
{
	return m_superData;
}

std::mutex* PreProcessor::GetMadeMutex()
{
	return m_superMutex;
}

std::thread PreProcessor::m_thread;

std::atomic<bool> PreProcessor::m_finished;

Plane* PreProcessor::m_planeCopy;

u8* PreProcessor::m_super;

i64* PreProcessor::m_superData;

std::mutex* PreProcessor::m_superMutex;
*/