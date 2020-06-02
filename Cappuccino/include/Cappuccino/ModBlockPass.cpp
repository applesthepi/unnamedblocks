#include "ModBlockPass.h"
#include "Registration.h"

#include <thread>
#include <cstddef>
#include <cstdio>
#include <locale>
#include <ctime>
#include <cstring>
#include <chrono>

static double gReal = 0.0;
static bool gBool = false;
static std::string gString;

ModBlockPass::ModBlockPass(const ModBlockPassInitializer& init)
{
	if (init.DebugMode)
	{
		m_getReal = &ModBlockPass::GetRealDebug;
		m_getBool = &ModBlockPass::GetBoolDebug;
		m_getString = &ModBlockPass::GetStringDebug;

		m_getPreData = &ModBlockPass::GetPreDataDebug;
	}
	else
	{
		m_getReal = &ModBlockPass::GetRealRelease;
		m_getBool = &ModBlockPass::GetBoolRelease;
		m_getString = &ModBlockPass::GetStringRelease;
		
		m_getPreData = &ModBlockPass::GetPreDataRelease;
	}

	m_variablesRealCount = init.VariablesRealCount;
	m_variablesBoolCount = init.VariablesBoolCount;
	m_variablesStringCount = init.VariablesStringCount;
	m_customRegistrerMutex = init.CustomRegisterMutex;
	m_customRegister = init.CustomRegister;
	m_stop = init.Stop;
	m_variableRegistry = init.VariableRegistry;
	m_beginTime = init.BeginTime;
	m_random.seed(std::time(0));
	m_messages.reserve(1024);
	m_activeIdx = nullptr;

	m_data = Registration::GetData();

	// ======================================================================================================
	// ================== setup functions
	// ======================================================================================================

	// pre

	for (uint64_t i = 0; i < Registration::GetFunctionTotalCount(); i++)
	{
		void*** stack = new void**[Registration::GetFunctionCallCount()[i]];
		std::vector<uint64_t> sizes;

		for (uint64_t a = 0; a < Registration::GetFunctionCallCount()[i]; a++)
		{
			stack[a] = m_data[i][a].GetPreData().data();
			sizes.push_back(m_data[i][a].GetPreData().size());
		}

		uint64_t* hSizes = new uint64_t[sizes.size()];

		for (uint64_t a = 0; a < sizes.size(); a++)
			hSizes[a] = sizes[a];

		m_dataStackPre.push_back(stack);
		m_preDataCount.push_back(hSizes);
	}

	// idx

	for (uint64_t i = 0; i < Registration::GetFunctionTotalCount(); i++)
	{
		uint64_t** stack = new uint64_t*[Registration::GetFunctionCallCount()[i]];

		for (uint64_t a = 0; a < Registration::GetFunctionCallCount()[i]; a++)
			stack[a] = (uint64_t*)m_data[i][a].GetRuntimeData().data();

		m_dataStackIdx.push_back((const uint64_t**)stack);
	}

	// ======================================================================================================
	// ================== setup types
	// ======================================================================================================

	const std::vector<double*>& realTemplate = Registration::GetRealTemplate();
	const std::vector<bool*>& boolTemplate = Registration::GetBoolTemplate();
	const std::vector<std::string*>& stringTemplate = Registration::GetStringTemplate();

	for (uint64_t i = 0; i < Registration::GetFunctionTotalCount(); i++)
	{
		m_dataStackReal.push_back(new double[m_variablesRealCount->at(i)]);

		for (uint64_t a = 0; a < m_variablesRealCount->at(i); a++)
			m_dataStackReal[i][a] = realTemplate[i][a];
	}

	for (uint64_t i = 0; i < Registration::GetFunctionTotalCount(); i++)
	{
		m_dataStackBool.push_back(new bool[m_variablesBoolCount->at(i)]);

		for (uint64_t a = 0; a < m_variablesBoolCount->at(i); a++)
			m_dataStackBool[i][a] = boolTemplate[i][a];
	}

	for (uint64_t i = 0; i < Registration::GetFunctionTotalCount(); i++)
	{
		m_dataStackString.push_back(new std::string[m_variablesStringCount->at(i)]);

		for (uint64_t a = 0; a < m_variablesStringCount->at(i); a++)
			m_dataStackString[i][a] = stringTemplate[i][a];
	}
}

CAP_DLL void ModBlockPass::SetData(ModBlockData** data)
{
	m_data = data;
}

CAP_DLL void ModBlockPass::SetCallstackStack(std::vector<uint64_t>* callstack)
{
	m_callstackStackIdx = callstack;
}

CAP_DLL void ModBlockPass::SetCallstackBlock(std::vector<uint64_t>* callstack)
{
	m_callstackBlockIdx = callstack;
}

CAP_DLL void ModBlockPass::SetSuccessful(bool* successful)
{
	m_successful = successful;
}

CAP_DLL void ModBlockPass::SetFinished(std::atomic<bool>* finished)
{
	m_finished = finished;
}

CAP_DLL void ModBlockPass::SetCallstackLocal(void(***localCallstack)(ModBlockPass*))
{
	m_localCallstack = localCallstack;
}

CAP_DLL void ModBlockPass::SetCalls(void(***calls)(ModBlockPass*))
{
	m_calls = calls;
}

CAP_DLL void ModBlockPass::PerformDeallocationCallbacks()
{
	for (uint64_t i = 0; i < m_deallocationCalls.size(); i++)
	{
		(*(m_deallocationCalls[i]))(this);
		delete m_deallocationCalls[i];
	}

	m_deallocationCalls.clear();
}

CAP_DLL std::chrono::steady_clock::time_point* ModBlockPass::GetBeginTime()
{
	return m_beginTime;
}

CAP_DLL void ModBlockPass::Stop()
{
	m_stop();

	while (!*m_finished)
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

CAP_DLL void ModBlockPass::AddCallstack(uint64_t stack, uint64_t block, bool special)
{
	m_callstackStackIdx->push_back(stack);
	m_callstackBlockIdx->push_back(block);
	
	if (special)
	{
		m_stackingSpecial.push_back(true);

		double* reals = new double[m_variablesRealCount->at(m_callstackStackIdx->back())];
		bool* bools = new bool[m_variablesBoolCount->at(m_callstackStackIdx->back())];
		std::string* strings = new std::string[m_variablesStringCount->at(m_callstackStackIdx->back())];

		for (uint64_t i = 0; i < m_variablesRealCount->at(m_callstackStackIdx->back()); i++)
			reals[i] = m_dataStackReal[m_callstackStackIdx->back()][i];

		for (uint64_t i = 0; i < m_variablesBoolCount->at(m_callstackStackIdx->back()); i++)
			bools[i] = m_dataStackBool[m_callstackStackIdx->back()][i];

		for (uint64_t i = 0; i < m_variablesStringCount->at(m_callstackStackIdx->back()); i++)
			strings[i] = m_dataStackString[m_callstackStackIdx->back()][i];

		m_stackingReal.push_back(reals);
		m_stackingBool.push_back(bools);
		m_stackingString.push_back(strings);
	}
	else
	{
		m_stackingSpecial.push_back(false);

		m_stackingReal.push_back(m_dataStackReal[m_callstackStackIdx->back()]);
		m_stackingBool.push_back(m_dataStackBool[m_callstackStackIdx->back()]);
		m_stackingString.push_back(m_dataStackString[m_callstackStackIdx->back()]);
	}

	UpdateLocations();
}

CAP_DLL void ModBlockPass::PopCallstack()
{
	m_callstackStackIdx->pop_back();
	m_callstackBlockIdx->pop_back();

	if (m_stackingSpecial.back())
	{
		delete[] m_stackingReal.back();
		delete[] m_stackingBool.back();
		delete[] m_stackingString.back();
	}

	m_stackingReal.pop_back();
	m_stackingBool.pop_back();
	m_stackingString.pop_back();

	if (m_callstackStackIdx->size() > 0)
		UpdateLocations();
	else
	{
		*m_finished = true;
		*m_successful = true;
	}
}

CAP_DLL void ModBlockPass::SetStackIdx(uint64_t idx)
{
	if (m_callstackStackIdx->size() > 0)
		m_callstackStackIdx->back() = idx;

	UpdateLocations();
}

CAP_DLL void ModBlockPass::SetBlockIdx(uint64_t idx)
{
	if (m_callstackBlockIdx->size() > 0)
		m_callstackBlockIdx->back() = idx;
}

CAP_DLL std::mt19937_64& ModBlockPass::GetRandomGenerator()
{
	return m_random;
}

CAP_DLL void ModBlockPass::AddDeallocation(std::function<void(ModBlockPass*)>* dealloc)
{
	m_deallocationCalls.push_back(dealloc);
}

CAP_DLL void ModBlockPass::RemoveDeallocation(std::function<void(ModBlockPass*)>* dealloc)
{
	for (uint64_t i = 0; i < m_deallocationCalls.size(); i++)
	{
		if (m_deallocationCalls[i] == dealloc)
		{
			m_deallocationCalls.erase(m_deallocationCalls.begin() + i);
			delete dealloc;
			return;
		}
	}
}

CAP_DLL uint64_t ModBlockPass::CustomPut(void* mem)
{
	// TODO change to shared
	std::unique_lock<std::mutex> lock(*m_customRegistrerMutex);

	uint64_t customIdx = m_customRegister->size();
	m_customRegister->push_back(mem);

	return customIdx;
}

CAP_DLL void* ModBlockPass::CustomGet(uint64_t idx)
{
	// TODO change to shared
	std::unique_lock<std::mutex> lock(*m_customRegistrerMutex);

	return m_customRegister->at(idx);
}

CAP_DLL void ModBlockPass::CustomFree(uint64_t idx, bool deallocate)
{
	std::unique_lock<std::mutex> lock(*m_customRegistrerMutex);

	if (deallocate)
		delete m_customRegister->at(idx);

	m_customRegister->at(idx) = nullptr;
}

CAP_DLL void ModBlockPass::LogDebug(const std::string& message)
{
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

	// Should be enough
	char prefix[100];

	std::unique_lock<std::mutex> lock = std::unique_lock<std::mutex>(m_messagesMutex);

	snprintf(prefix, 100, "[%02lu%02luu] %03luu:%03lu%03luu] [DEBUG] ",
		((uint64_t)std::chrono::duration_cast<std::chrono::minutes>(now - *m_beginTime).count()),
		((uint64_t)std::chrono::duration_cast<std::chrono::seconds>(now - *m_beginTime).count()),
		((uint64_t)std::chrono::duration_cast<std::chrono::milliseconds>(now - *m_beginTime).count()) % 1000,
		((uint64_t)std::chrono::duration_cast<std::chrono::microseconds>(now - *m_beginTime).count()) % 1000,
		((uint64_t)std::chrono::duration_cast<std::chrono::nanoseconds>(now - *m_beginTime).count()) % 1000);

	m_messages.emplace_back();
	size_t index = m_messages.size() - 1;
	m_messages[index].reserve(strlen(prefix) + message.length());

	m_messages[index] += prefix;
	m_messages[index] += message;
}

void ModBlockPass::LogInfo(const std::string& message)
{
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

	// Should be enough
	char prefix[100];

	std::unique_lock<std::mutex> lock = std::unique_lock<std::mutex>(m_messagesMutex);

	snprintf(prefix, 100, "[%02lu:%02lu] [%03lu%03luu%03luu] [INFO] ",
		((uint64_t)std::chrono::duration_cast<std::chrono::minutes>(now - *m_beginTime).count()),
		((uint64_t)std::chrono::duration_cast<std::chrono::seconds>(now - *m_beginTime).count()),
		((uint64_t)std::chrono::duration_cast<std::chrono::milliseconds>(now - *m_beginTime).count()) % 1000,
		((uint64_t)std::chrono::duration_cast<std::chrono::microseconds>(now - *m_beginTime).count()) % 1000,
		((uint64_t)std::chrono::duration_cast<std::chrono::nanoseconds>(now - *m_beginTime).count()) % 1000);

	m_messages.emplace_back();
	size_t index = m_messages.size() - 1;
	m_messages[index].reserve(strlen(prefix) + message.length());

	m_messages[index] += prefix;
	m_messages[index] += message;
}

CAP_DLL void ModBlockPass::LogWarning(const std::string& message)
{
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

	// Should be enough
	char prefix[100];

	std::unique_lock<std::mutex> lock = std::unique_lock<std::mutex>(m_messagesMutex);

	snprintf(prefix, 100, "[%02lu:%02lu] [%03lu%03luu%03luu] [WARN] ",
		((uint64_t)std::chrono::duration_cast<std::chrono::minutes>(now - *m_beginTime).count()),
		((uint64_t)std::chrono::duration_cast<std::chrono::seconds>(now - *m_beginTime).count()),
		((uint64_t)std::chrono::duration_cast<std::chrono::milliseconds>(now - *m_beginTime).count()) % 1000,
		((uint64_t)std::chrono::duration_cast<std::chrono::microseconds>(now - *m_beginTime).count()) % 1000,
		((uint64_t)std::chrono::duration_cast<std::chrono::nanoseconds>(now - *m_beginTime).count()) % 1000);

	m_messages.emplace_back();
	size_t index = m_messages.size() - 1;
	m_messages[index].reserve(strlen(prefix) + message.length());

	m_messages[index] += prefix;
	m_messages[index] += message;
}

void ModBlockPass::LogError(const std::string& message, const LoggerFatality& fatality)
{
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	
	// Should be enough
	char prefix[100];

	std::unique_lock<std::mutex> lock = std::unique_lock<std::mutex>(m_messagesMutex);

	if (fatality == LoggerFatality::OK)
		snprintf(prefix, 100, "[%02lu:%02lu] [%03lu:%03lu:%03lu] [ERROR-OK] ",
			((uint64_t)std::chrono::duration_cast<std::chrono::minutes>(now - *m_beginTime).count()),
			((uint64_t)std::chrono::duration_cast<std::chrono::seconds>(now - *m_beginTime).count()),
			((uint64_t)std::chrono::duration_cast<std::chrono::milliseconds>(now - *m_beginTime).count()) % 1000,
			((uint64_t)std::chrono::duration_cast<std::chrono::microseconds>(now - *m_beginTime).count()) % 1000,
			((uint64_t)std::chrono::duration_cast<std::chrono::nanoseconds>(now - *m_beginTime).count()) % 1000);
	else if (fatality == LoggerFatality::BREAK)
		snprintf(prefix, 100, "[%02lu:%02lu] [%03lu:%03lu:%03lu] [ERROR-BREAK] ",
			((uint64_t)std::chrono::duration_cast<std::chrono::minutes>(now - *m_beginTime).count()),
			((uint64_t)std::chrono::duration_cast<std::chrono::seconds>(now - *m_beginTime).count()),
			((uint64_t)std::chrono::duration_cast<std::chrono::milliseconds>(now - *m_beginTime).count()) % 1000,
			((uint64_t)std::chrono::duration_cast<std::chrono::microseconds>(now - *m_beginTime).count()) % 1000,
			((uint64_t)std::chrono::duration_cast<std::chrono::nanoseconds>(now - *m_beginTime).count()) % 1000);
	else if (fatality == LoggerFatality::ABORT)
		snprintf(prefix, 100, "[%02lu:%02lu] [%03lu:%03lu:%03lu] [ERROR-ABORT] ",
			((uint64_t)std::chrono::duration_cast<std::chrono::minutes>(now - *m_beginTime).count()),
			((uint64_t)std::chrono::duration_cast<std::chrono::seconds>(now - *m_beginTime).count()),
			((uint64_t)std::chrono::duration_cast<std::chrono::milliseconds>(now - *m_beginTime).count()) % 1000,
			((uint64_t)std::chrono::duration_cast<std::chrono::microseconds>(now - *m_beginTime).count()) % 1000,
			((uint64_t)std::chrono::duration_cast<std::chrono::nanoseconds>(now - *m_beginTime).count()) % 1000);
	
	// TODO abort

	m_messages.emplace_back();
	size_t index = m_messages.size() - 1;
	m_messages[index].reserve(strlen(prefix) + message.length());

	m_messages[index] += prefix;
	m_messages[index] += message;
}

CAP_DLL double& ModBlockPass::GetReal(uint64_t idx)
{
	return (this->*(m_getReal))(idx);
}

CAP_DLL bool& ModBlockPass::GetBool(uint64_t idx)
{
	return (this->*(m_getBool))(idx);
}

CAP_DLL std::string& ModBlockPass::GetString(uint64_t idx)
{
	return (this->*(m_getString))(idx);
}

CAP_DLL void* ModBlockPass::GetPreData(uint64_t idx)
{
	return (this->*(m_getPreData))(idx);
}

CAP_DLL const std::vector<std::string>& ModBlockPass::PullMessages()
{
	m_messagesMutex.lock();
	return m_messages;
}

CAP_DLL void ModBlockPass::ReturnMessages()
{
	m_messages.clear();
	m_messagesMutex.unlock();
}

void ModBlockPass::UpdateLocations()
{
	*m_localCallstack = m_calls[m_callstackStackIdx->back()];

	m_activeReal = m_stackingReal.back();
	m_activeBool = m_stackingBool.back();
	m_activeString = m_stackingString.back();
	m_activeIdx = m_dataStackIdx[m_callstackStackIdx->back()];
	m_activePre = m_dataStackPre[m_callstackStackIdx->back()];
}

double& ModBlockPass::GetRealDebug(uint64_t idx)
{
	if (idx >= m_variablesRealCount->at(m_callstackStackIdx->back()))
	{
		LogError("attempted to get real out of range \"" + std::to_string(idx) + "\". registry size is \"" + std::to_string(m_variablesRealCount->at(m_callstackStackIdx->back())) + "\"", LoggerFatality::ABORT);
		return gReal;
	}

	uint64_t vIdx = m_activeIdx[m_callstackBlockIdx->back()][idx];
	double& value = m_activeReal[vIdx];
	return value;
}

double& ModBlockPass::GetRealRelease(uint64_t idx)
{
	return m_activeReal[m_activeIdx[m_callstackBlockIdx->back()][idx]];
}

bool& ModBlockPass::GetBoolDebug(uint64_t idx)
{
	if (idx >= m_variablesBoolCount->at(m_callstackStackIdx->back()))
	{
		LogError("attempted to get bool out of range \"" + std::to_string(idx) + "\". registry size is \"" + std::to_string(m_variablesBoolCount->at(m_callstackStackIdx->back())) + "\"", LoggerFatality::ABORT);
		return gBool;
	}

	uint64_t vIdx = m_activeIdx[m_callstackBlockIdx->back()][idx];
	bool& value = m_activeBool[vIdx];
	return value;
}

bool& ModBlockPass::GetBoolRelease(uint64_t idx)
{
	return m_activeBool[m_activeIdx[m_callstackBlockIdx->back()][idx]];
}

std::string& ModBlockPass::GetStringDebug(uint64_t idx)
{
	if (idx >= m_variablesStringCount->at(m_callstackStackIdx->back()))
	{
		LogError("attempted to get string out of range \"" + std::to_string(idx) + "\". registry size is \"" + std::to_string(m_variablesStringCount->at(m_callstackStackIdx->back())) + "\"", LoggerFatality::ABORT);
		return gString;
	}

	uint64_t vIdx = m_activeIdx[m_callstackBlockIdx->back()][idx];
	std::string& value = m_activeString[vIdx];
	return value;
}

std::string& ModBlockPass::GetStringRelease(uint64_t idx)
{
	return m_activeString[m_activeIdx[m_callstackBlockIdx->back()][idx]];
}

void* ModBlockPass::GetPreDataDebug(uint64_t idx)
{
	if (idx >= m_preDataCount[m_callstackStackIdx->back()][m_callstackBlockIdx->back()])
	{
		LogError("attempted to get string out of range \"" + std::to_string(idx) + "\". registry size is \"" + std::to_string(m_variablesStringCount->at(m_callstackStackIdx->back())) + "\"", LoggerFatality::ABORT);
		return nullptr;
	}

	void* value = m_activePre[m_callstackBlockIdx->back()][idx];
	return value;
}

void* ModBlockPass::GetPreDataRelease(uint64_t idx)
{
	return m_activePre[m_callstackBlockIdx->back()][idx];
}

ModBlockPassInitializer::ModBlockPassInitializer()
{
	DebugMode = true;

	Data = nullptr;
	VariablesRealCount = nullptr;
	VariablesBoolCount = nullptr;
	VariablesStringCount = nullptr;

	CustomRegisterMutex = nullptr;
	CustomRegister = nullptr;
	Stop = nullptr;

	// debug only
	DataSize = 0;
	VariableRegistry = nullptr;
	BeginTime = nullptr;
}
