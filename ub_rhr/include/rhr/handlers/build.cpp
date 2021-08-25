#include "build.hpp"

#include "rhr/handlers/project.hpp"

#include <cappuccino/mod_block/data.hpp>

void thread_build(cap::build_system::method build_method, cap::build_system::type build_type)
{
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
				rhr::handler::build::confirm_terminated();
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
			if (build_type == cap::build_system::type::DEBUG)
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
		rhr::handler::build::confirm_terminated();
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

	bool buildType = build_type == cap::build_system::type::DEBUG;

	u64 functionTotalCount = stacks.size();

	cap::build_system::set_main(functionMain);
	cap::build_system::set_function_call_count(functionCallCount.data());
	cap::build_system::set_function_total_count(functionTotalCount);
	cap::build_system::set_calls(calls);
	cap::build_system::set_function_data(functionData);
	cap::build_system::set_mod_blocks(modBlocks);
	cap::build_system::execute(build_method, build_type);

	for (u64 i = 0; i < functionTotalCount; i++)
		free(calls[i]);
	free(calls);

	delete[] functionCallCountC;

	for (u64 i = 0; i < functionTotalCount; i++)
		delete[] functionData[i];
	delete[] functionData;

	rhr::handler::build::confirm_terminated();
}

void rhr::handler::build::initialize()
{
	m_terminate = false;
	m_status = cap::build_system::status::READY;
}

void rhr::handler::build::execute(cap::build_system::method build_method, cap::build_system::type build_type)
{
	if (m_status == cap::build_system::status::RUNNING)
	{
		Logger::Error("can not execute a build while already running");
		return;
	}

	m_status = cap::build_system::status::RUNNING;
	m_thread = std::thread(thread_build, build_method, build_type);
}

cap::build_system::status rhr::handler::build::get_status()
{
	return m_status;
}

void rhr::handler::build::terminate()
{
	if (m_status == cap::build_system::status::RUNNING)
		m_terminate = true;
}

void rhr::handler::build::confirm_terminated()
{
	m_terminate = false;
	m_status = cap::build_system::status::READY;
}

std::thread rhr::handler::build::m_thread;
std::atomic<bool> rhr::handler::build::m_terminate;
cap::build_system::status rhr::handler::build::m_status;