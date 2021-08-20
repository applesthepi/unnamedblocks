#include "preprocessor.hpp"

#include "rhr/handlers/project.hpp"

#include <cappuccino/mod_block/data.hpp>

#include <libtcc.h>

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
	rhr::handler::preprocessor::set_finished(false);// just in case
	char* file;
	PullFileSingle(&file, "res/comp.c");
	TCCState* state = tcc_new();

	tcc_add_include_path(state, "ub_cappuccino/include");
//	tcc_add_include_path(state, "csfml/include");
	tcc_add_include_path(state, "res");

	std::vector<std::string> functionReferences;
	std::vector<u32> functionIds;
	std::vector<u64> functionCallCount;
	std::vector<std::vector<void(*)(ModBlockPass* pass)>> functionCalls;

	u64 functionMain = 0;
	bool functionMainFound = false;

	std::vector<std::shared_ptr<rhr::stack::stack>> stacks;

	for (u64 i = 0; i < rhr::stack::plane::primary_plane->get_collections().size(); i++)
	{
		const std::vector<std::shared_ptr<rhr::stack::stack>>& colStacks = rhr::stack::plane::primary_plane->get_collections()[i]->get_stacks();

		for (u64 a = 0; a < colStacks.size(); a++)
			stacks.push_back(colStacks[a]);
	}

	ModBlockData** functionData = new ModBlockData*[stacks.size()];
	ModBlock*** modBlocks = new ModBlock**[stacks.size()];
	
	for (u64 i = 0; i < stacks.size(); i++)
	{
		functionData[i] = new ModBlockData[stacks[i]->get_blocks().size()];
		modBlocks[i] = new ModBlock*[stacks[i]->get_blocks().size()];

		if (stacks[i]->get_blocks().size() >= 1 && std::string(stacks[i]->get_blocks()[0]->get_mod_block()->GetUnlocalizedName()) == "vin_main")
		{
			if (functionMainFound)
			{
				Logger::Error("program has more than 1 entry points; can not run program without exactly 1 entry point (vin_main)");
				rhr::handler::preprocessor::set_finished(true);

				return;
			}

			functionMain = i;
			functionMainFound = true;
		}
		
		// TODO: function references to indices (use lambda code in Cappuccino/Registration.cpp)

		//functionReferences.push_back(*stacks->at(i)->GetBlock(0)->GetArgument(1)->GetDataRaw());
		//functionIds.push_back(functionReferences.size());

		std::vector<void(*)(ModBlockPass* pass)> transCalls;

		for (u64 a = 0; a < stacks[i]->get_blocks().size(); a++)
		{
			if (debugBuild)
				transCalls.push_back(stacks[i]->get_blocks()[a]->get_mod_block()->PullExecuteDebug());
			else
				transCalls.push_back(stacks[i]->get_blocks()[a]->get_mod_block()->PullExecuteRelease());

			modBlocks[i][a] = (ModBlock*)stacks[i]->get_blocks()[a]->get_mod_block();

			std::vector<void*> argData;
			std::vector<ModBlockDataType> argTypes;
			std::vector<ModBlockDataInterpretation> argInterpretations;

			for (u64 b = 0; b < stacks[i]->get_blocks()[a]->get_arguments().size(); b++)
			{
				BlockArgumentType type = stacks[i]->get_blocks()[a]->get_arguments()[b]->get_type();

				if (stacks[i]->get_blocks()[a]->get_arguments()[b]->get_mode() == BlockArgumentVariableMode::VAR)
				{
					std::string* dt = new std::string();

					try
					{
						*dt = stacks[i]->get_blocks()[a]->get_arguments()[b]->get_data();
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
						*dt = stacks[i]->get_blocks()[a]->get_arguments()[b]->get_data();
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
						*dt = stacks[i]->get_blocks()[a]->get_arguments()[b]->get_data() == "1";
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
						*dt = std::stod(stacks[i]->get_blocks()[a]->get_arguments()[b]->get_data());
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
		rhr::handler::preprocessor::set_finished(true);

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

	u8* super = rhr::handler::preprocessor::make_super();
	i64* superData = rhr::handler::preprocessor::get_made_data();
	std::mutex* superMutex = rhr::handler::preprocessor::get_made_mutex();

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

	tcc_add_library_path(state, "ub_cappuccino/lib");
	[[maybe_unused]] int re = tcc_add_library(state, "cappuccino");
//	tcc_add_library_path(state, "csfml/lib/gcc");

//	tcc_add_library(state, "csfml-audio");
//	tcc_add_library(state, "csfml-graphics");
//	tcc_add_library(state, "csfml-network");
//	tcc_add_library(state, "csfml-system");
//	tcc_add_library(state, "csfml-window");

	tcc_run(state, 0, 0);
	
	for (u64 i = 0; i < functionTotalCount; i++)
		free(calls[i]);
	free(calls);

	delete[] functionCallCountC;

	for (u64 i = 0; i < functionTotalCount; i++)
		delete[] functionData[i];
	delete[] functionData;

	free(file);

	rhr::handler::preprocessor::set_finished(true);
}

void rhr::handler::preprocessor::initialize()
{
//	m_planeCopy = nullptr;
	m_super = nullptr;
	m_finished = true;
}

void rhr::handler::preprocessor::clean_up()
{
	m_finished = false;
//	if (m_planeCopy != nullptr)
//	{
//		delete m_planeCopy;
//		m_planeCopy = nullptr;
//	}
}

void rhr::handler::preprocessor::build(bool debugBuild)
{
//	m_planeCopy = planeCopy;
	m_finished = false;
	m_thread = std::thread(ThreadPreProcessorExecution, debugBuild);
	m_thread.detach();
}

bool rhr::handler::preprocessor::is_finished()
{
	return m_finished;
}

void rhr::handler::preprocessor::set_finished(bool finished)
{
	m_finished = finished;
}

//Plane* rhr::handler::preprocessor::GetPlaneCopy()
//{
//	return m_planeCopy;
//}

void rhr::handler::preprocessor::set_super(u8 super, i16 super_data)
{
	std::unique_lock<std::mutex> lock(*m_super_mutex);

	*m_super = super;
	*m_super_data = super_data;
}

u8 rhr::handler::preprocessor::get_super()
{
	std::unique_lock<std::mutex> lock(*m_super_mutex);

	return *m_super;
}

i64 rhr::handler::preprocessor::get_super_data()
{
	std::unique_lock<std::mutex> lock(*m_super_mutex);

	return *m_super_data;
}

u8* rhr::handler::preprocessor::make_super()
{
	if (m_super != nullptr)
	{
		delete m_super;
		delete m_super_mutex;
	}

	m_super = new u8;
	*m_super = 0;

	m_super_data = new i64;
	*m_super_data = 0;

	m_super_mutex = new std::mutex();

	return m_super;
}

i64* rhr::handler::preprocessor::get_made_data()
{
	return m_super_data;
}

std::mutex* rhr::handler::preprocessor::get_made_mutex()
{
	return m_super_mutex;
}

std::thread rhr::handler::preprocessor::m_thread;
std::atomic<bool> rhr::handler::preprocessor::m_finished;
//Plane* rhr::handler::preprocessor::m_planeCopy;
u8* rhr::handler::preprocessor::m_super;
i64* rhr::handler::preprocessor::m_super_data;
std::mutex* rhr::handler::preprocessor::m_super_mutex;
