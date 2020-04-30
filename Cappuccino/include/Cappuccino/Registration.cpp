#include "Registration.h"

#include <chrono>
#include <vector>
#include <string>
#include <cassert>
#include <functional>

static void ThreadUtil()
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

	m_utilThread = std::thread(ThreadUtil);
	m_utilThread.detach();

	m_passes.clear();
	m_passesFlagged.clear();
	m_execution.clear();
	m_executionFlagged.clear();

	m_customRegister.clear();
	m_variableRegistry.clear();
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
}

void Registration::UnRegisterExecutionThread(ExecutionThread* thr)
{
	std::unique_lock<std::mutex> lock(m_executionMutex);

	for (uint64_t i = 0; i < m_execution.size(); i++)
	{
		if (m_execution[i] == thr)
		{
			m_executionFlagged[i] = true;
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

void Registration::SetDebug(bool debugBuild)
{
	m_debugBuild = debugBuild;
}

void Registration::EndAll(ModBlockPass* whitelist)
{
	if (whitelist == nullptr)
	{
		for (uint64_t i = 0; i < m_execution.size(); i++)
			m_execution[i]->End();
	}
	else
	{
		for (uint64_t i = 0; i < m_execution.size(); i++)
		{
			if (m_passes[i] != whitelist)
				m_execution[i]->End();
		}
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

		for (uint64_t a = 0; a < messages.size(); a++)
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

			m_execution.erase(m_execution.begin() + i);
			m_executionFlagged.erase(m_executionFlagged.begin() + i);

			exe->End();
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
	printf("compiling data...\n");

	if (m_debugBuild)
		CompileDataDebug();
	else
		CompileDataRelease();

	printf("...compilation succeeded\n");
	
	m_customRegister.reserve(10000);

	ModBlockPassInitializer init;

	init.DataSize = 0;
	init.Data = nullptr;
	init.VariablesReal = m_variablesReal;
	init.VariablesBool = m_variablesBool;
	init.VariablesString = m_variablesString;
	init.CustomRegisterMutex = &m_customRegisterMutex;
	init.CustomRegister = &m_customRegister;
	init.Stop = &Registration::Stop;
	init.VariableRegistry = &m_variableRegistry;

	ModBlockPass* pass = new ModBlockPass(init);
	RegisterPass(pass);

	ExecutionThread* thr = new ExecutionThread(m_functionMain, m_functionCallCount, m_calls, pass);
	RegisterExecutionThread(thr);

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

	printf("...finished deallocation\n");
	printf("#########[ Stopped Cappuccino\n");
}

bool Registration::IsAllDone()
{
	for (uint64_t i = 0; i < m_execution.size(); i++)
	{
		if (!m_execution[i]->GetFinished())
			return false;
	}

	return true;
}

ModBlockData** Registration::GetData()
{
	return m_data;
}

void Registration::CompileDataDebug()
{
	uint64_t variableIdx = 0;

	auto addToRegistry = [&](std::string name)
	{
		for (uint64_t i = 0; i < m_variableRegistry.size(); i++)
		{
			if (m_variableRegistry[i] == name)
			{
				variableIdx = i;
				return false;
			}
		}

		variableIdx = m_variableRegistry.size();
		m_variableRegistry.push_back(name);

		return true;
	};

	for (uint64_t i = 0; i < m_functionTotalCount; i++)
	{
		for (uint64_t a = 0; a < m_functionCallCount[i]; a++)
		{
			const std::vector<void*>& data = m_data[i][a].GetData();
			const std::vector<ModBlockDataType>& types = m_data[i][a].GetTypes();
			const std::vector<ModBlockDataInterpretation>& interpretations = m_data[i][a].GetInterpretations();

			std::vector<int64_t> hauledVariables;

			for (uint64_t b = 0; b < data.size(); b++)
			{
				if (types[b] == ModBlockDataType::VAR)
				{
					addToRegistry(*(std::string*)data[b]);
					hauledVariables.push_back(variableIdx);
				}
				else
					hauledVariables.push_back(-1);
			}

			m_data[i][a].HaulData(hauledVariables);
		}
	}

	m_variablesReal = new double[m_variableRegistry.size()];
	m_variablesBool = new bool[m_variableRegistry.size()];
	m_variablesString = new std::string[m_variableRegistry.size()];

	memset(m_variablesReal, 0, m_variableRegistry.size());
	memset(m_variablesBool, 0, m_variableRegistry.size());
}

void Registration::CompileDataRelease()
{
	std::vector<std::string> variableRegistryReal;
	std::vector<std::string> variableRegistryBool;
	std::vector<std::string> variableRegistryString;
	uint64_t variableIdx = 0;

	auto addToRegistryReal = [&](std::string name)
	{
		for (uint64_t i = 0; i < variableRegistryReal.size(); i++)
		{
			if (variableRegistryReal[i] == name)
			{
				variableIdx = i;
				return false;
			}
		}

		variableIdx = variableRegistryReal.size();
		variableRegistryReal.push_back(name);

		return true;
	};

	auto addToRegistryBool = [&](std::string name)
	{
		for (uint64_t i = 0; i < variableRegistryBool.size(); i++)
		{
			if (variableRegistryBool[i] == name)
			{
				variableIdx = i;
				return false;
			}
		}

		variableIdx = variableRegistryBool.size();
		variableRegistryBool.push_back(name);

		return true;
	};

	auto addToRegistryString = [&](std::string name)
	{
		for (uint64_t i = 0; i < variableRegistryString.size(); i++)
		{
			if (variableRegistryString[i] == name)
			{
				variableIdx = i;
				return false;
			}
		}

		variableIdx = variableRegistryString.size();
		variableRegistryString.push_back(name);

		return true;
	};

	for (uint64_t i = 0; i < m_functionTotalCount; i++)
	{
		for (uint64_t a = 0; a < m_functionCallCount[i]; a++)
		{
			const std::vector<void*>& data = m_data[i][a].GetData();
			const std::vector<ModBlockDataType>& types = m_data[i][a].GetTypes();
			const std::vector<ModBlockDataInterpretation>& interpretations = m_data[i][a].GetInterpretations();

			std::vector<int64_t> hauledVariables;

			for (uint64_t b = 0; b < data.size(); b++)
			{
				if (types[b] == ModBlockDataType::VAR)
				{
					if (interpretations[b] == ModBlockDataInterpretation::REAL)
						addToRegistryReal(*(std::string*)data[b]);
					else if (interpretations[b] == ModBlockDataInterpretation::BOOL)
						addToRegistryBool(*(std::string*)data[b]);
					else if (interpretations[b] == ModBlockDataInterpretation::STRING)
						addToRegistryString(*(std::string*)data[b]);

					hauledVariables.push_back(variableIdx);
				}
				else
					hauledVariables.push_back(-1);
			}

			m_data[i][a].HaulData(hauledVariables);
		}
	}

	m_variablesReal = new double[variableRegistryReal.size()];
	m_variablesBool = new bool[variableRegistryBool.size()];
	m_variablesString = new std::string[variableRegistryString.size()];
}

void Registration::RunContext()
{
	while (!m_allDone)
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

std::mutex Registration::m_passesMutex;

std::vector<ModBlockPass*> Registration::m_passes;

std::vector<bool> Registration::m_passesFlagged;

std::mutex Registration::m_executionMutex;

std::vector<ExecutionThread*> Registration::m_execution;

std::vector<bool> Registration::m_executionFlagged;

uint64_t Registration::m_functionMain;

uint64_t* Registration::m_functionCallCount;

uint64_t Registration::m_functionTotalCount;

executionFunctionStackList Registration::m_calls;

ModBlockData** Registration::m_data;

double* Registration::m_variablesReal;

bool* Registration::m_variablesBool;

std::string* Registration::m_variablesString;

std::mutex Registration::m_customRegisterMutex;

std::vector<void*> Registration::m_customRegister;

std::atomic<bool> Registration::m_utilFinished;

std::atomic<bool> Registration::m_utilReturnFinished;

std::atomic<bool> Registration::m_allDone;

std::atomic<bool> Registration::m_stop;

std::thread Registration::m_utilThread;

bool Registration::m_debugBuild;

std::vector<std::string> Registration::m_variableRegistry;
