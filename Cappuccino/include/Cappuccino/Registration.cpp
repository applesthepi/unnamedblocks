#include "Registration.h"
#include "PreProcessorData.h"

#include <vector>
#include <string>
#include <cassert>
#include <functional>

static void ThreadUtilRelease()
{
	while (!Registration::GetUtilFinished())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		Registration::RunUtilityTick();
	}

	// for sync
	Registration::RunUtilityTick();
	Registration::SetUtilReturnFinished(true);
}

static void ThreadUtilDebug()
{
	while (!Registration::GetUtilFinished())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		Registration::RunUtilityTick();
	}

	// for sync
	Registration::RunUtilityTick();
	Registration::SetUtilReturnFinished(true);
}

void Registration::Initialize()
{
	m_utilFinished = false;
	m_utilReturnFinished = false;
	m_allDone = false;
	m_stop = false;
	m_debugBuild = true;
	m_super = nullptr;

	m_breakFull = false;
	m_breakSingle = false;
	m_breakSingleData = 0;

	m_breakFullResume = false;
	m_breakSingleResume = false;

	m_passes.clear();
	m_passesFlagged.clear();
	m_execution.clear();
	m_executionFlagged.clear();
	m_executionJoin.clear();

	m_customRegister.clear();
	m_variableRegistry.clear();

	m_variableRealCount.clear();
	m_variableBoolCount.clear();
	m_variableStringCount.clear();

	m_variableRealTemplate.clear();
	m_variableBoolTemplate.clear();
	m_variableStringTemplate.clear();

	m_timeBegin = std::chrono::steady_clock::now();
}

void Registration::RegisterPass(ModBlockPass* pass)
{
	std::unique_lock<std::mutex> lock(m_passesMutex);

	m_passes.push_back(pass);
	m_passesFlagged.push_back(false);
}

void Registration::UnRegisterPass(ModBlockPass* pass)
{
	std::unique_lock<std::mutex> lock(m_passesMutex);

	for (uint64_t i = 0; i < m_passes.size(); i++)
	{
		if (m_passes[i] == pass)
		{
			m_passesFlagged[i] = true;
			return;
		}
	}
}

void Registration::RegisterExecutionThread(ExecutionThread* thr)
{
	std::unique_lock<std::mutex> lock(m_executionMutex);

	m_execution.push_back(thr);
	m_executionFlagged.push_back(false);
	m_executionJoin.push_back(false);
}

void Registration::UnRegisterExecutionThread(ExecutionThread* thr, bool join)
{
	std::unique_lock<std::mutex> lock(m_executionMutex);

	for (uint64_t i = 0; i < m_execution.size(); i++)
	{
		if (m_execution[i] == thr)
		{
			m_executionFlagged[i] = true;
			m_executionJoin[i] = join;
			return;
		}
	}
}

void Registration::SetFunctionMain(uint64_t main)
{
	m_functionMain = main;
}

void Registration::SetFunctionCallCount(uint64_t* functionCallCount)
{
	m_functionCallCount = functionCallCount;
}

void Registration::SetFunctionTotalCount(uint64_t functionTotalCount)
{
	m_functionTotalCount = functionTotalCount;
}

void Registration::SetCalls(executionFunctionStackList calls)
{
	m_calls = calls;
}

void Registration::SetData(ModBlockData** data)
{
	m_data = data;
}

void Registration::SetBlocks(ModBlock*** blocks)
{
	m_blocks = blocks;
}

void Registration::SetDebug(bool debugBuild)
{
	m_debugBuild = debugBuild;
}

void Registration::SetSuper(uint8_t* super, int64_t* superData, void* superMutex)
{
	m_super = super;
	m_superData = superData;
	m_superMutex = (std::mutex*)superMutex;
}

void Registration::EndAll(ModBlockPass* whitelist)
{
	// dont lock execution mutex because it is already locked

	if (whitelist == nullptr)
	{
		for (uint64_t i = 0; i < m_execution.size(); i++)
		{
			m_execution[i]->End();
			m_executionFlagged[i] = true;
		}

		for (uint64_t i = 0; i < m_passes.size(); i++)
			m_passesFlagged[i] = true;
	}
	else
	{
		for (uint64_t i = 0; i < m_execution.size(); i++)
		{
			if (m_passes[i] != whitelist)
			{
				m_execution[i]->End();
				m_executionFlagged[i] = true;
			}
		}

		for (uint64_t i = 0; i < m_passes.size(); i++)
			m_passesFlagged[i] = true;
	}
}

void Registration::Stop()
{
	m_stop = true;
}

std::atomic<bool>& Registration::GetUtilFinished()
{
	return m_utilFinished;
}

std::atomic<bool>& Registration::GetStop()
{
	return m_stop;
}

uint64_t* Registration::GetFunctionCallCount()
{
	return m_functionCallCount;
}

CAP_DLL uint64_t Registration::GetFunctionTotalCount()
{
	return m_functionTotalCount;
}

executionFunctionStackList Registration::GetCalls()
{
	return m_calls;
}

void Registration::SetUtilReturnFinished(bool finished)
{
	m_utilReturnFinished = finished;
}

void Registration::RunUtilityTick()
{
	std::unique_lock<std::mutex> lock1(m_passesMutex);
	std::unique_lock<std::mutex> lock2(m_executionMutex);

	for (uint64_t i = 0; i < m_passes.size(); i++)
	{
		const std::vector<std::string>& messages = m_passes[i]->PullMessages();

		uint64_t amount = messages.size();
		if (amount > 256)
		{
			amount = 256;
			printf("restricting to 256 messages per tick...\n");
		}

		for (uint64_t a = 0; a < amount; a++)
			printf("%s\n", (messages[a]).c_str());

		m_passes[i]->ReturnMessages();
	}

	// cleanup passes

	for (uint64_t i = 0; i < m_passes.size(); i++)
	{
		if (m_passesFlagged[i])
		{
			ModBlockPass* pass = m_passes[i];

			m_passes.erase(m_passes.begin() + i);
			m_passesFlagged.erase(m_passesFlagged.begin() + i);
			
			delete pass;
		}
	}

	// cleanup threads

	for (uint64_t i = 0; i < m_execution.size(); i++)
	{
		if (m_executionFlagged[i])
		{
			ExecutionThread* exe = m_execution[i];

			if (m_executionJoin[i])
				exe->End();

			m_execution.erase(m_execution.begin() + i);
			m_executionFlagged.erase(m_executionFlagged.begin() + i);
			m_executionJoin.erase(m_executionJoin.begin() + i);
			
			delete exe;
		}
	}

	if (Registration::GetStop())
		Registration::EndAll();

	m_allDone = IsAllDone();

	if (m_allDone)
		m_utilFinished = true;
}

void Registration::Run()
{
	printf("#########[ Started Cappuccino\n");

	if (m_debugBuild)
		CompileDataDebug();
	else
		CompileDataRelease();

	printf("...compilation succeeded\n");
	
	m_customRegister.reserve(10000);

	PreProcessorData data(m_customRegister);

	if (!GlobalPre(data))
		return;

	if (!LocalPre(data))
		return;

	printf("...preinitialization succeeded\n");

	if (!Init(data, m_data))
		return;

	printf("...initialization succeeded\n");

	if (!LocalPost(data))
		return;

	if (!GlobalPost(data))
		return;

	printf("...postinitialization succeeded\n");

	ModBlockPassInitializer init;

	init.DataSize = 0;
	init.Data = nullptr;
	init.VariablesRealCount = &m_variableRealCount;
	init.VariablesBoolCount = &m_variableBoolCount;
	init.VariablesStringCount = &m_variableStringCount;
	init.CustomRegisterMutex = &m_customRegisterMutex;
	init.CustomRegister = &m_customRegister;
	init.Stop = &Registration::Stop;
	init.VariableRegistry = nullptr;// isnt working right now due to changes &m_variableRegistry;
	init.DebugMode = m_debugBuild;
	init.BeginTime = &m_timeBegin;

	ModBlockPass* pass = new ModBlockPass(init);
	RegisterPass(pass);

	ExecutionThread* thr = new ExecutionThread(m_functionMain, m_functionCallCount, m_calls, pass);
	RegisterExecutionThread(thr);

	if (m_debugBuild)
		m_utilThread = std::thread(ThreadUtilDebug);
	else
		m_utilThread = std::thread(ThreadUtilRelease);

	m_utilThread.detach();

	RunContext();

	{
		m_utilFinished = true;

		while (!m_utilReturnFinished)
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	printf("finished execution; deallocating...\n");

	for (uint64_t i = 0; i < m_customRegister.size(); i++)
	{
		if (m_customRegister[i] != nullptr)
			delete m_customRegister[i];
	}

	for (uint64_t i = 0; i < m_variableRealTemplate.size(); i++)
		delete[] m_variableRealTemplate[i];

	for (uint64_t i = 0; i < m_variableBoolTemplate.size(); i++)
		delete[] m_variableBoolTemplate[i];

	for (uint64_t i = 0; i < m_variableStringTemplate.size(); i++)
		delete[] m_variableStringTemplate[i];

	m_variableRealTemplate.clear();
	m_variableBoolTemplate.clear();
	m_variableStringTemplate.clear();

	m_variableRealCount.clear();
	m_variableBoolCount.clear();
	m_variableStringCount.clear();

	printf("#########[ Stopped Cappuccino\n");
}

bool Registration::IsAllDone()
{
	// dont lock execution mutex because it is already locked
	return m_execution.size() == 0;
	/*
	for (uint64_t i = 0; i < m_execution.size(); i++)
	{
		if (!m_execution[i]->GetFinished())
			return false;
	}

	return true;
	*/
}

const std::vector<double*>& Registration::GetRealTemplate()
{
	return m_variableRealTemplate;
}

const std::vector<bool*>& Registration::GetBoolTemplate()
{
	return m_variableBoolTemplate;
}

const std::vector<std::string*>& Registration::GetStringTemplate()
{
	return m_variableStringTemplate;
}

CAP_DLL const std::vector<uint64_t>* Registration::GetRealCount()
{
	return &m_variableRealCount;
}

CAP_DLL const std::vector<uint64_t>* Registration::GetBoolCount()
{
	return &m_variableBoolCount;
}

CAP_DLL const std::vector<uint64_t>* Registration::GetStringCount()
{
	return &m_variableStringCount;
}

CAP_DLL std::mutex* Registration::GetCustomMutex()
{
	return &m_customRegisterMutex;
}

CAP_DLL std::vector<void*>* Registration::GetCustomRegistry()
{
	return &m_customRegister;
}

ModBlockData** Registration::GetData()
{
	return m_data;
}

bool Registration::GlobalPre(PreProcessorData& data)
{
	std::vector<ModBlock*> singleBlocks;

	for (uint64_t i = 0; i < m_functionTotalCount; i++)
	{
		for (uint64_t a = 0; a < m_functionCallCount[i]; a++)
		{
			bool found = false;

			for (uint64_t b = 0; b < singleBlocks.size(); b++)
			{
				if (singleBlocks[b]->GetUnlocalizedName() == m_blocks[i][a]->GetUnlocalizedName())
				{
					found = true;
					break;
				}
			}

			if (!found)
				singleBlocks.push_back(m_blocks[i][a]);
		}
	}

	for (uint64_t i = 0; i < singleBlocks.size(); i++)
	{
		if (!singleBlocks[i]->GetRuntimeGlobalPreInit()(data))
		{
			Logger::Error("failed on globalPreInit");
			return false;
		}
	}

	return true;
}

bool Registration::GlobalPost(PreProcessorData& data)
{
	std::vector<ModBlock*> singleBlocks;

	for (uint64_t i = 0; i < m_functionTotalCount; i++)
	{
		for (uint64_t a = 0; a < m_functionCallCount[i]; a++)
		{
			bool found = false;

			for (uint64_t b = 0; b < singleBlocks.size(); b++)
			{
				if (singleBlocks[b]->GetUnlocalizedName() == m_blocks[i][a]->GetUnlocalizedName())
				{
					found = true;
					break;
				}
			}

			if (!found)
				singleBlocks.push_back(m_blocks[i][a]);
		}
	}

	for (uint64_t i = 0; i < singleBlocks.size(); i++)
	{
		if (!singleBlocks[i]->GetRuntimeGlobalPostInit()(data))
		{
			Logger::Error("failed on globalPostInit");
			return false;
		}
	}

	return true;
}

bool Registration::LocalPre(PreProcessorData& data)
{
	for (uint64_t i = 0; i < m_functionTotalCount; i++)
	{
		std::vector<ModBlock*> singleBlocks;

		for (uint64_t a = 0; a < m_functionCallCount[i]; a++)
		{
			bool found = false;

			for (uint64_t b = 0; b < singleBlocks.size(); b++)
			{
				if (singleBlocks[b]->GetUnlocalizedName() == m_blocks[i][a]->GetUnlocalizedName())
				{
					found = true;
					break;
				}
			}

			if (!found)
				singleBlocks.push_back(m_blocks[i][a]);
		}

		data.StackIdx = i;

		for (uint64_t a = 0; a < singleBlocks.size(); a++)
		{
			if (!singleBlocks[a]->GetRuntimeLocalPreInit()(data))
			{
				Logger::Error("failed on localPreInit");
				return false;
			}
		}
	}

	return true;
}

bool Registration::LocalPost(PreProcessorData& data)
{
	for (uint64_t i = 0; i < m_functionTotalCount; i++)
	{
		std::vector<ModBlock*> singleBlocks;

		for (uint64_t a = 0; a < m_functionCallCount[i]; a++)
		{
			bool found = false;

			for (uint64_t b = 0; b < singleBlocks.size(); b++)
			{
				if (singleBlocks[b]->GetUnlocalizedName() == m_blocks[i][a]->GetUnlocalizedName())
				{
					found = true;
					break;
				}
			}

			if (!found)
				singleBlocks.push_back(m_blocks[i][a]);
		}

		data.StackIdx = i;

		for (uint64_t a = 0; a < singleBlocks.size(); a++)
		{
			if (!singleBlocks[a]->GetRuntimeLocalPostInit()(data))
			{
				Logger::Error("failed on localPostInit");
				return false;
			}
		}
	}

	return true;
}

bool Registration::Init(PreProcessorData& preData, ModBlockData** blockData)
{
	std::vector<std::vector<blockDataInitialization>> stages;
	std::vector<std::vector<uint64_t>> stageStackIdx;
	std::vector<std::vector<uint64_t>> stageBlockIdx;

	for (uint64_t i = 0; i < m_functionTotalCount; i++)
	{
		for (uint64_t a = 0; a < m_functionCallCount[i]; a++)
		{
			std::vector<std::pair<blockDataInitialization, uint16_t>> initStages = m_blocks[i][a]->GetRuntimeStages();

			for (uint64_t b = 0; b < initStages.size(); b++)
			{
				if (initStages[b].second >= stages.size())
				{
					uint64_t stagesSize = stages.size();

					for (uint64_t c = 0; c < (initStages[b].second - stagesSize) + 1; c++)
					{
						stages.push_back(std::vector<blockDataInitialization>());
						stageStackIdx.push_back(std::vector<uint64_t>());
						stageBlockIdx.push_back(std::vector<uint64_t>());
					}
				}

				stages[initStages[b].second].push_back(initStages[b].first);
				stageStackIdx[initStages[b].second].push_back(i);
				stageBlockIdx[initStages[b].second].push_back(a);
			}
		}
	}

	for (uint64_t i = 0; i < stages.size(); i++)
	{
		for (uint64_t a = 0; a < stages[i].size(); a++)
		{
			preData.StackIdx = stageStackIdx[i][a];
			preData.BlockIdx = stageBlockIdx[i][a];

			if (!stages[i][a](preData, blockData[stageStackIdx[i][a]][stageBlockIdx[i][a]]))
			{
				Logger::Error("failed on initialization of stage \"" + std::to_string(i) + "\"");
				return false;
			}
		}
	}

	return true;
}

bool Registration::TestSuperBase()
{
	std::unique_lock<std::mutex> lock(*m_superMutex);

	if (*m_super == 1)
	{
		*m_super = 0;
		printf("stopping execution...\n");

		std::unique_lock<std::mutex> lock1(m_passesMutex);
		std::unique_lock<std::mutex> lock2(m_executionMutex);

		EndAll();
		return true;
	}
	else if (*m_super == 2)
	{
		*m_super = 0;
		
		if (m_breakSingle)
		{
			printf("can not break full while in break single mode\n");
			return true;
		}
		
		printf("breaking all execution...\n");
		
		m_breakFull = true;
		m_breakFullResume = false;

		std::unique_lock<std::mutex> lock(m_executionMutex);

		for (uint64_t i = 0; i < m_execution.size(); i++)
			m_execution[i]->Break(&m_breakFullResume);

		return true;
	}
	else if (*m_super == 3)
	{
		*m_super = 0;

		if (!m_breakFull)
		{
			printf("can not resume all if not in not in break all mode\n");
			return true;
		}

		printf("resuming all execution...\n");

		m_breakFull = false;
		m_breakFullResume = true;

		return true;
	}

	return false;
}

bool Registration::TestSuperDebug()
{
	if (*m_super == 4)
	{
		*m_super = 0;

		if (m_breakFull)
		{
			printf("can not break single while in break all mode\n");
			return true;
		}

		printf("breaking single execution...\n");

		m_breakSingle = true;
		m_breakSingleResume = false;

		std::unique_lock<std::mutex> lock(m_executionMutex);
		m_execution[reinterpret_cast<uint64_t>(m_superData)]->Break(&m_breakSingleResume);
		
		return true;
	}
	else if (*m_super == 6)
	{
		*m_super = 0;

		if (!m_breakSingle)
		{
			printf("can not resume single if not in not in break single mode\n");
			return true;
		}

		printf("resuming single execution...\n");

		m_breakSingle = false;
		m_breakSingleResume = true;

		return true;
	}
	else if (*m_super == 5)
	{
		*m_super = 0;

		if (!m_breakSingle)
		{
			printf("can not step single if not in not in break single mode\n");
			return true;
		}

		printf("stepping single execution...\n");

		std::unique_lock<std::mutex> lock(m_executionMutex);
		m_execution[reinterpret_cast<uint64_t>(m_superData)]->Step();

		return true;
	}

	return false;
}

void Registration::CompileDataDebug()
{
	uint64_t tempTotal = 0;
	uint64_t variableIdx = 0;
	std::vector<std::vector<ModBlockDataInterpretation>> tempRegsitryTypes;
	std::vector<std::vector<void*>> tempRegsitryValues;

	auto addToRegistry = [&](const std::string& name, uint64_t idx, const ModBlockDataInterpretation& interp, void* use = nullptr)
	{
		for (uint64_t i = 0; i < m_variableRegistry[idx].size(); i++)
		{
			if (m_variableRegistry[idx][i] == name)
			{
				variableIdx = i;
				return false;
			}
		}

		variableIdx = m_variableRegistry[idx].size();
		tempTotal++;
		m_variableRegistry[idx].push_back(name);
		tempRegsitryTypes[idx].push_back(interp);
		tempRegsitryValues[idx].push_back(use);

		return true;
	};

	std::vector<std::vector<std::vector<uint64_t>*>*> hauledVariablesPlane;

	for (uint64_t i = 0; i < m_functionTotalCount; i++)
	{
		m_variableRegistry.push_back(std::vector<std::string>());
		tempRegsitryTypes.push_back(std::vector<ModBlockDataInterpretation>());
		tempRegsitryValues.push_back(std::vector<void*>());
		m_variableRegistryOffsets.push_back(tempTotal);

		std::vector<std::vector<uint64_t>*>* hauledVariablesStack = new std::vector<std::vector<uint64_t>*>();

		for (uint64_t a = 0; a < m_functionCallCount[i]; a++)
		{
			const std::vector<void*>& data = m_data[i][a].GetData();
			const std::vector<ModBlockDataType>& types = m_data[i][a].GetTypes();
			const std::vector<ModBlockDataInterpretation>& interpretations = m_data[i][a].GetInterpretations();

			std::vector<uint64_t>* hauledVariablesBlock = new std::vector<uint64_t>();

			for (uint64_t b = 0; b < data.size(); b++)
			{
				const uint64_t countTotal = tempTotal - m_variableRegistry[i].size();

				if (types[b] == ModBlockDataType::VAR)
				{
					if (interpretations[b] == ModBlockDataInterpretation::ANY)
					{
						bool found = false;

						for (uint64_t c = 0; c < m_variableRegistry[i].size(); c++)
						{
							if (m_variableRegistry[i][c] == "_L_" + *(std::string*)data[b])
							{
								hauledVariablesBlock->push_back(countTotal);
								m_data[i][a].SetInterpretation(tempRegsitryTypes[i][c], b);

								found = true;
								break;
							}
						}

						if (!found)
							Logger::Warn("*any* variable \"" + *(std::string*)data[b] + "\" was not found");
					}
					else
					{
						addToRegistry("_L_" + *(std::string*)data[b], i, interpretations[b]);
						hauledVariablesBlock->push_back(variableIdx);
					}
				}
				else
				{
					char buffer[20];
					sprintf(buffer, "_R_%lu_%lu_%lu", i, a, b);
					
					if (interpretations[b] == ModBlockDataInterpretation::REAL)
						addToRegistry(std::string(buffer), i, ModBlockDataInterpretation::REAL, new double(*(double*)data[b]));
					else if (interpretations[b] == ModBlockDataInterpretation::BOOL)
						addToRegistry(std::string(buffer), i, ModBlockDataInterpretation::BOOL, new bool(*(bool*)data[b]));
					else if (interpretations[b] == ModBlockDataInterpretation::STRING)
						addToRegistry(std::string(buffer), i, ModBlockDataInterpretation::STRING, new std::string(*(std::string*)data[b]));

					hauledVariablesBlock->push_back(variableIdx);
				}
			}

			hauledVariablesStack->push_back(hauledVariablesBlock);
		}

		hauledVariablesPlane.push_back(hauledVariablesStack);
	}

	for (uint64_t i = 0; i < m_functionTotalCount; i++)
	{
		m_variableRealCount.push_back(m_variableRegistry[i].size());
		m_variableBoolCount.push_back(m_variableRegistry[i].size());
		m_variableStringCount.push_back(m_variableRegistry[i].size());
	}

	for (uint64_t i = 0; i < m_functionTotalCount; i++)
	{
		m_variableRealTemplate.push_back(new double[m_variableRealCount[i]]);
		m_variableBoolTemplate.push_back(new bool[m_variableBoolCount[i]]);
		m_variableStringTemplate.push_back(new std::string[m_variableStringCount[i]]);

		for (uint64_t a = 0; a < m_variableRealCount[i]; a++)
		{
			if (tempRegsitryValues[i][a] != nullptr && tempRegsitryTypes[i][a] == ModBlockDataInterpretation::REAL)
				m_variableRealTemplate.back()[a] = *(double*)tempRegsitryValues[i][a];
		}

		for (uint64_t a = 0; a < m_variableBoolCount[i]; a++)
		{
			if (tempRegsitryValues[i][a] != nullptr && tempRegsitryTypes[i][a] == ModBlockDataInterpretation::BOOL)
				m_variableBoolTemplate.back()[a] = *(bool*)tempRegsitryValues[i][a];
		}

		for (uint64_t a = 0; a < m_variableStringCount[i]; a++)
		{
			if (tempRegsitryValues[i][a] != nullptr && tempRegsitryTypes[i][a] == ModBlockDataInterpretation::STRING)
				m_variableStringTemplate.back()[a] = *(std::string*)tempRegsitryValues[i][a];
		}

		for (uint64_t a = 0; a < m_functionCallCount[i]; a++)
		{
			m_data[i][a].ClearData();
			m_data[i][a].SetRuntimeData(*hauledVariablesPlane[i]->at(a));
			m_data[i][a].SetDataTemplates(i);
		}
	}
}

void Registration::CompileDataRelease()
{
	uint64_t variableIdx = 0;

	std::vector<std::vector<std::string>> tempRegistryReal;
	std::vector<std::vector<double*>> tempRegsitryValueReal;
	std::vector<uint64_t> tempOffsetReal;
	uint64_t tempTotalReal = 0;

	std::vector<std::vector<std::string>> tempRegistryBool;
	std::vector<std::vector<bool*>> tempRegsitryValueBool;
	std::vector<uint64_t> tempOffsetBool;
	uint64_t tempTotalBool = 0;

	std::vector<std::vector<std::string>> tempRegistryString;
	std::vector<std::vector<std::string*>> tempRegsitryValueString;
	std::vector<uint64_t> tempOffsetString;
	uint64_t tempTotalString = 0;

	auto addToRegistryReal = [&](const std::string& name, uint64_t idx, double* use = nullptr)
	{
		for (uint64_t i = 0; i < tempRegistryReal[idx].size(); i++)
		{
			if (tempRegistryReal[idx][i] == name)
			{
				variableIdx = i;
				return false;
			}
		}

		variableIdx = tempRegistryReal[idx].size();
		tempTotalReal++;
		tempRegistryReal[idx].push_back(name);
		tempRegsitryValueReal[idx].push_back(use);

		return true;
	};

	auto addToRegistryBool = [&](const std::string& name, uint64_t idx, bool* use = nullptr)
	{
		for (uint64_t i = 0; i < tempRegistryBool[idx].size(); i++)
		{
			if (tempRegistryBool[idx][i] == name)
			{
				variableIdx = i;
				return false;
			}
		}

		variableIdx = tempRegistryBool[idx].size();
		tempTotalBool++;
		tempRegistryBool[idx].push_back(name);
		tempRegsitryValueBool[idx].push_back(use);

		return true;
	};

	auto addToRegistryString = [&](const std::string& name, uint64_t idx, std::string* use = nullptr)
	{
		for (uint64_t i = 0; i < tempRegistryString[idx].size(); i++)
		{
			if (tempRegistryString[idx][i] == name)
			{
				variableIdx = i;
				return false;
			}
		}

		variableIdx = tempRegistryString[idx].size();
		tempTotalString++;
		tempRegistryString[idx].push_back(name);
		tempRegsitryValueString[idx].push_back(use);

		return true;
	};

	std::vector<std::vector<std::vector<uint64_t>*>*> hauledVariablesPlane;

	for (uint64_t i = 0; i < m_functionTotalCount; i++)
	{
		tempRegistryReal.push_back(std::vector<std::string>());
		tempRegistryBool.push_back(std::vector<std::string>());
		tempRegistryString.push_back(std::vector<std::string>());

		tempRegsitryValueReal.push_back(std::vector<double*>());
		tempRegsitryValueBool.push_back(std::vector<bool*>());
		tempRegsitryValueString.push_back(std::vector<std::string*>());

		tempOffsetReal.push_back(tempTotalReal);
		tempOffsetBool.push_back(tempTotalBool);
		tempOffsetString.push_back(tempTotalString);

		std::vector<std::vector<uint64_t>*>* hauledVariablesStack = new std::vector<std::vector<uint64_t>*>();

		for (uint64_t a = 0; a < m_functionCallCount[i]; a++)
		{
			const std::vector<void*>& data = m_data[i][a].GetData();
			const std::vector<ModBlockDataType>& types = m_data[i][a].GetTypes();
			const std::vector<ModBlockDataInterpretation>& interpretations = m_data[i][a].GetInterpretations();

			std::vector<uint64_t>* hauledVariablesBlock = new std::vector<uint64_t>();

			for (uint64_t b = 0; b < data.size(); b++)
			{
				if (types[b] == ModBlockDataType::VAR)
				{
					if (interpretations[b] == ModBlockDataInterpretation::ANY)
					{
						bool found = false;

						if (!found)
						{
							for (uint64_t c = 0; c < tempRegistryReal.size(); c++)
							{
								for (uint64_t d = 0; d < tempRegistryReal[c].size(); d++)
								{
									if (tempRegistryReal[c][d] == "_L_" + *(std::string*)data[b])
									{
										hauledVariablesBlock->push_back(c);
										m_data[i][a].SetInterpretation(ModBlockDataInterpretation::REAL, b);

										found = true;
										break;
									}
								}

								if (found)
									break;
							}
						}

						if (!found)
						{
							for (uint64_t c = 0; c < tempRegistryBool.size(); c++)
							{
								for (uint64_t d = 0; d < tempRegistryBool[c].size(); d++)
								{
									if (tempRegistryBool[c][d] == "_L_" + *(std::string*)data[b])
									{
										hauledVariablesBlock->push_back(c);
										m_data[i][a].SetInterpretation(ModBlockDataInterpretation::BOOL, b);

										found = true;
										break;
									}
								}

								if (found)
									break;
							}
						}

						if (!found)
						{
							for (uint64_t c = 0; c < tempRegistryString.size(); c++)
							{
								for (uint64_t d = 0; d < tempRegistryString[c].size(); d++)
								{
									if (tempRegistryString[c][d] == "_L_" + *(std::string*)data[b])
									{
										hauledVariablesBlock->push_back(c);
										m_data[i][a].SetInterpretation(ModBlockDataInterpretation::STRING, b);

										found = true;
										break;
									}
								}

								if (found)
									break;
							}
						}

						if (!found)
							Logger::Warn("*any* variable \"" + *(std::string*)data[b] + "\" was not found");
					}
					else
					{
						if (interpretations[b] == ModBlockDataInterpretation::REAL)
						{
							addToRegistryReal("_L_" + *(std::string*)data[b], i);
							hauledVariablesBlock->push_back(variableIdx);
						}
						else if (interpretations[b] == ModBlockDataInterpretation::BOOL)
						{
							addToRegistryBool("_L_" + *(std::string*)data[b], i);
							hauledVariablesBlock->push_back(variableIdx);
						}
						else if (interpretations[b] == ModBlockDataInterpretation::STRING)
						{
							addToRegistryString("_L_" + *(std::string*)data[b], i);
							hauledVariablesBlock->push_back(variableIdx);
						}
					}
				}
				else
				{
					char buffer[20];
					sprintf(buffer, "_R_%lu_%lu_%lu", i, a, b);

					if (interpretations[b] == ModBlockDataInterpretation::REAL)
					{
						addToRegistryReal(std::string(buffer), i, new double(*(double*)data[b]));
						hauledVariablesBlock->push_back(variableIdx);
					}
					else if (interpretations[b] == ModBlockDataInterpretation::BOOL)
					{
						addToRegistryBool(std::string(buffer), i, new bool(*(bool*)data[b]));
						hauledVariablesBlock->push_back(variableIdx);
					}
					else if (interpretations[b] == ModBlockDataInterpretation::STRING)
					{
						addToRegistryString(std::string(buffer), i, new std::string(*(std::string*)data[b]));
						hauledVariablesBlock->push_back(variableIdx);
					}
				}
			}

			hauledVariablesStack->push_back(hauledVariablesBlock);
		}

		hauledVariablesPlane.push_back(hauledVariablesStack);
	}

	for (uint64_t i = 0; i < m_functionTotalCount; i++)
	{
		m_variableRealCount.push_back(tempRegistryReal[i].size());
		m_variableBoolCount.push_back(tempRegistryBool[i].size());
		m_variableStringCount.push_back(tempRegistryString[i].size());
	}

	for (uint64_t i = 0; i < m_functionTotalCount; i++)
	{
		m_variableRealTemplate.push_back(new double[m_variableRealCount[i]]);
		m_variableBoolTemplate.push_back(new bool[m_variableBoolCount[i]]);
		m_variableStringTemplate.push_back(new std::string[m_variableStringCount[i]]);

		for (uint64_t a = 0; a < m_variableRealCount[i]; a++)
		{
			if (tempRegsitryValueReal[i][a] != nullptr)
				m_variableRealTemplate.back()[a] = *(double*)tempRegsitryValueReal[i][a];
		}

		for (uint64_t a = 0; a < m_variableBoolCount[i]; a++)
		{
			if (tempRegsitryValueBool[i][a] != nullptr)
				m_variableBoolTemplate.back()[a] = *(bool*)tempRegsitryValueBool[i][a];
		}

		for (uint64_t a = 0; a < m_variableStringCount[i]; a++)
		{
			if (tempRegsitryValueString[i][a] != nullptr)
				m_variableStringTemplate.back()[a] = *(std::string*)tempRegsitryValueString[i][a];
		}

		for (uint64_t a = 0; a < m_functionCallCount[i]; a++)
		{
			m_data[i][a].ClearData();
			m_data[i][a].SetRuntimeData(*hauledVariablesPlane[i]->at(a));
			m_data[i][a].SetDataTemplates(i);
		}
	}
}

void Registration::RunContext()
{
	if (m_debugBuild)
	{
		bool flaggedStop = false;

		while (!m_allDone)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));

			if (TestSuperBase() || TestSuperDebug())
				flaggedStop = true;
		}

		if (flaggedStop)
			printf("...stopped execution\n");
	}
	else
	{
		bool flaggedStop = false;

		while (!m_allDone)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));

			if (TestSuperBase())
				flaggedStop = true;
		}

		if (flaggedStop)
			printf("...stopped execution\n");
	}
}

std::mutex Registration::m_passesMutex;

std::vector<ModBlockPass*> Registration::m_passes;

std::vector<bool> Registration::m_passesFlagged;

std::mutex Registration::m_executionMutex;

std::vector<ExecutionThread*> Registration::m_execution;

std::vector<bool> Registration::m_executionFlagged;

std::vector<bool> Registration::m_executionJoin;

uint64_t Registration::m_functionMain;

uint64_t* Registration::m_functionCallCount;

uint64_t Registration::m_functionTotalCount;

executionFunctionStackList Registration::m_calls;

ModBlockData** Registration::m_data;

ModBlock*** Registration::m_blocks;

std::vector<uint64_t> Registration::m_variableRealCount;

std::vector<uint64_t> Registration::m_variableBoolCount;

std::vector<uint64_t> Registration::m_variableStringCount;

std::vector<double*> Registration::m_variableRealTemplate;

std::vector<bool*> Registration::m_variableBoolTemplate;

std::vector<std::string*> Registration::m_variableStringTemplate;

std::mutex Registration::m_customRegisterMutex;

std::vector<void*> Registration::m_customRegister;

std::atomic<bool> Registration::m_utilFinished;

std::atomic<bool> Registration::m_utilReturnFinished;

std::atomic<bool> Registration::m_allDone;

std::atomic<bool> Registration::m_stop;

std::thread Registration::m_utilThread;

bool Registration::m_debugBuild;

uint8_t* Registration::m_super;

int64_t* Registration::m_superData;

std::mutex* Registration::m_superMutex;

std::chrono::steady_clock::time_point Registration::m_timeBegin;

bool Registration::m_breakFull;

bool Registration::m_breakSingle;

int64_t Registration::m_breakSingleData;

std::atomic<bool> Registration::m_breakFullResume;

std::atomic<bool> Registration::m_breakSingleResume;

std::vector<std::vector<std::string>> Registration::m_variableRegistry;

std::vector<uint64_t> Registration::m_variableRegistryOffsets;
