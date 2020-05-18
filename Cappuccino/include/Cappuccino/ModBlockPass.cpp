#include "ModBlockPass.h"

#include <thread>
#include <cstddef>
#include <cstdio>
#include <locale>
#include <ctime>
#include <cstring>
#include <chrono>

ModBlockPass::ModBlockPass(const ModBlockPassInitializer& init)
{
	if (init.DebugMode)
	{
		m_getReal = &ModBlockPass::GetRealDebug;
		m_getBool = &ModBlockPass::GetBoolDebug;
		m_getString = &ModBlockPass::GetStringDebug;
		m_getVaraibleReal = &ModBlockPass::GetVariableRealDebug;
		m_getVariableBool = &ModBlockPass::GetVariableBoolDebug;
		m_getVariableString = &ModBlockPass::GetVariableStringDebug;
		m_getPreData = &ModBlockPass::GetPreDataDebug;
	}
	else
	{
		m_getReal = &ModBlockPass::GetRealRelease;
		m_getBool = &ModBlockPass::GetBoolRelease;
		m_getString = &ModBlockPass::GetStringRelease;
		m_getVaraibleReal = &ModBlockPass::GetVariableRealRelease;
		m_getVariableBool = &ModBlockPass::GetVariableBoolRelease;
		m_getVariableString = &ModBlockPass::GetVariableStringRelease;
		m_getPreData = &ModBlockPass::GetPreDataRelease;
	}

	m_variablesReal = init.VariablesReal;
	m_variablesBool = init.VariablesBool;
	m_variablesString = init.VariablesString;
	m_customRegistrerMutex = init.CustomRegisterMutex;
	m_customRegister = init.CustomRegister;
	m_stop = init.Stop;
	m_variableRegistry = init.VariableRegistry;
	m_beginTime = init.BeginTime;
	m_random.seed(std::time(0));
	m_messages.reserve(1024);
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

CAP_DLL void ModBlockPass::PerformDeallocationCallbacks()
{
	for (uint64_t i = 0; i < m_deallocationCalls.size(); i++)
	{
		(*(m_deallocationCalls[i]))(this);
		delete m_deallocationCalls[i];
	}

	m_deallocationCalls.clear();
}

CAP_DLL double& ModBlockPass::GetVariableReal(const uint64_t& idx)
{
	return (this->*(m_getVaraibleReal))(idx);
}

CAP_DLL bool& ModBlockPass::GetVariableBool(const uint64_t& idx)
{
	return (this->*(m_getVariableBool))(idx);
}

CAP_DLL std::string& ModBlockPass::GetVariableString(const uint64_t& idx)
{
	return (this->*(m_getVariableString))(idx);
}

CAP_DLL double* ModBlockPass::GetVariableRegistryReal()
{
	return m_variablesReal;
}

CAP_DLL bool* ModBlockPass::GetVariableRegistryBool()
{
	return m_variablesBool;
}

CAP_DLL std::string* ModBlockPass::GetVariableRegistryString()
{
	return m_variablesString;
}

CAP_DLL std::mutex* ModBlockPass::GetCustomRegisterMutex()
{
	return m_customRegistrerMutex;
}

CAP_DLL std::vector<void*>* ModBlockPass::GetCustomRegister()
{
	return m_customRegister;
}

CAP_DLL std::vector<std::string>* ModBlockPass::GetVariableRegistry()
{
	return m_variableRegistry;
}

CAP_DLL std::chrono::steady_clock::time_point* ModBlockPass::GetBeginTime()
{
	return m_beginTime;
}

CAP_DLL void ModBlockPass::Stop()
{
	//*m_successful = true;
	m_stop();

	while (!*m_finished)
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

CAP_DLL std::vector<uint64_t>& ModBlockPass::GetCallstackStack()
{
	return *m_callstackStackIdx;
}

CAP_DLL std::vector<uint64_t>& ModBlockPass::GetCallstackBlock()
{
	return *m_callstackBlockIdx;
}

CAP_DLL std::mt19937_64& ModBlockPass::GetRandomGenerator()
{
	return m_random;
}

CAP_DLL void ModBlockPass::AddDeallocation(std::function<void(ModBlockPass*)>* dealloc)
{

}

CAP_DLL void ModBlockPass::RemoveDeallocation(std::function<void(ModBlockPass*)>* dealloc)
{

}

CAP_DLL const uint64_t ModBlockPass::CustomPut(void* mem)
{
	// TODO change to shared
	std::unique_lock<std::mutex> lock(*m_customRegistrerMutex);

	uint64_t customIdx = m_customRegister->size();
	m_customRegister->push_back(mem);

	return customIdx;
}

CAP_DLL void* ModBlockPass::CustomGet(const uint64_t& idx)
{
	// TODO change to shared
	std::unique_lock<std::mutex> lock(*m_customRegistrerMutex);

	return m_customRegister->at(idx);
}

CAP_DLL void ModBlockPass::CustomFree(const uint64_t& idx, bool deallocate)
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

	snprintf(prefix, 100, "[%02u:%02u] [%03u:%03u:%03u] [DEBUG] ",
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

	snprintf(prefix, 100, "[%02u:%02u] [%03u:%03u:%03u] [INFO] ",
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

	snprintf(prefix, 100, "[%02u:%02u] [%03u:%03u:%03u] [WARN] ",
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
		snprintf(prefix, 100, "[%02u:%02u] [%03u:%03u:%03u] [ERROR-OK] ",
			((uint64_t)std::chrono::duration_cast<std::chrono::minutes>(now - *m_beginTime).count()),
			((uint64_t)std::chrono::duration_cast<std::chrono::seconds>(now - *m_beginTime).count()),
			((uint64_t)std::chrono::duration_cast<std::chrono::milliseconds>(now - *m_beginTime).count()) % 1000,
			((uint64_t)std::chrono::duration_cast<std::chrono::microseconds>(now - *m_beginTime).count()) % 1000,
			((uint64_t)std::chrono::duration_cast<std::chrono::nanoseconds>(now - *m_beginTime).count()) % 1000);
	else if (fatality == LoggerFatality::BREAK)
		snprintf(prefix, 100, "[%02u:%02u] [%03u:%03u:%03u] [ERROR-BREAK] ",
			((uint64_t)std::chrono::duration_cast<std::chrono::minutes>(now - *m_beginTime).count()),
			((uint64_t)std::chrono::duration_cast<std::chrono::seconds>(now - *m_beginTime).count()),
			((uint64_t)std::chrono::duration_cast<std::chrono::milliseconds>(now - *m_beginTime).count()) % 1000,
			((uint64_t)std::chrono::duration_cast<std::chrono::microseconds>(now - *m_beginTime).count()) % 1000,
			((uint64_t)std::chrono::duration_cast<std::chrono::nanoseconds>(now - *m_beginTime).count()) % 1000);
	else if (fatality == LoggerFatality::ABORT)
		snprintf(prefix, 100, "[%02u:%02u] [%03u:%03u:%03u] [ERROR-ABORT] ",
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

CAP_DLL double* ModBlockPass::GetReal(const uint64_t& idx)
{
	return (this->*(m_getReal))(idx);
}

CAP_DLL bool* ModBlockPass::GetBool(const uint64_t& idx)
{
	return (this->*(m_getBool))(idx);
}

CAP_DLL std::string* ModBlockPass::GetString(const uint64_t& idx)
{
	return (this->*(m_getString))(idx);
}

CAP_DLL void* ModBlockPass::GetPreData(const uint64_t& idx)
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

double* ModBlockPass::GetRealDebug(const uint64_t& idx)
{
	if (m_data == nullptr)
		LogWarning("pulling invalid data! attempted to get real from index \"" + std::to_string(idx) + "\"");

	const std::vector<void*>& data = m_data[m_callstackStackIdx->back()][m_callstackBlockIdx->back()].GetData();

	if (idx >= data.size())
	{
		LogError("attempted to get real out of range \"" + std::to_string(idx) + "\". data size is \"" + std::to_string(data.size()) + "\"", LoggerFatality::ABORT);
		return nullptr;
	}

	return (double*)data[idx];
}

double* ModBlockPass::GetRealRelease(const uint64_t& idx)
{
	return (double*)m_data[m_callstackStackIdx->back()][m_callstackBlockIdx->back()].GetCData()[idx];
}

bool* ModBlockPass::GetBoolDebug(const uint64_t& idx)
{
	if (m_data == nullptr)
		LogWarning("pulling invalid data! attempted to get bool from index \"" + std::to_string(idx) + "\"");

	const std::vector<void*>& data = m_data[m_callstackStackIdx->back()][m_callstackBlockIdx->back()].GetData();

	if (idx >= data.size())
	{
		LogError("attempted to get bool out of range \"" + std::to_string(idx) + "\". data size is \"" + std::to_string(data.size()) + "\"", LoggerFatality::ABORT);
		return nullptr;
	}

	return (bool*)data[idx];
}

bool* ModBlockPass::GetBoolRelease(const uint64_t& idx)
{
	return (bool*)m_data[m_callstackStackIdx->back()][m_callstackBlockIdx->back()].GetCData()[idx];
}

std::string* ModBlockPass::GetStringDebug(const uint64_t& idx)
{
	if (m_data == nullptr)
		LogWarning("pulling invalid data! attempted to get string from index \"" + std::to_string(idx) + "\"");

	const std::vector<void*>& data = m_data[m_callstackStackIdx->back()][m_callstackBlockIdx->back()].GetData();
	
	if (idx >= data.size())
	{
		LogError("attempted to get string out of range \"" + std::to_string(idx) + "\". data size is \"" + std::to_string(data.size()) + "\"", LoggerFatality::ABORT);
		return nullptr;
	}

	return (std::string*)data[idx];
}

std::string* ModBlockPass::GetStringRelease(const uint64_t& idx)
{
	return (std::string*)m_data[m_callstackStackIdx->back()][m_callstackBlockIdx->back()].GetCData()[idx];
}

double& ModBlockPass::GetVariableRealDebug(const uint64_t& idx)
{
	if (idx >= m_variableRegistry->size())
	{
		LogError("attempted to get variable out of range", LoggerFatality::ABORT);
		double dud = 0.0;
		return dud;
	}
	else
	{
		LogDebug("getting variable \"" + m_variableRegistry->at(idx) + "\" with *real* value \"" + std::to_string(m_variablesReal[idx]) + "\"");
		return m_variablesReal[idx];
	}
}

double& ModBlockPass::GetVariableRealRelease(const uint64_t& idx)
{
	return m_variablesReal[idx];
}

bool& ModBlockPass::GetVariableBoolDebug(const uint64_t& idx)
{
	if (idx >= m_variableRegistry->size())
	{
		LogError("attempted to get variable out of range", LoggerFatality::ABORT);
		bool dud = false;
		return dud;
	}
	else
	{
		LogDebug("getting variable \"" + m_variableRegistry->at(idx) + "\" with *bool* value \"" + std::string(m_variablesBool[idx] ? "true" : "false") + "\"");
		return m_variablesBool[idx];
	}
}

bool& ModBlockPass::GetVariableBoolRelease(const uint64_t& idx)
{
	return m_variablesBool[idx];
}

std::string& ModBlockPass::GetVariableStringDebug(const uint64_t& idx)
{
	if (idx >= m_variableRegistry->size())
	{
		LogError("attempted to get variable out of range", LoggerFatality::ABORT);
		std::string dud;
		return dud;
	}
	else
	{
		LogDebug("getting variable \"" + m_variableRegistry->at(idx) + "\" with *string* value \"" + m_variablesString[idx] + "\"");
		return m_variablesString[idx];
	}
}

std::string& ModBlockPass::GetVariableStringRelease(const uint64_t& idx)
{
	return m_variablesString[idx];
}

void* ModBlockPass::GetPreDataDebug(const uint64_t& idx)
{
	const std::vector<void*>& data = m_data[m_callstackStackIdx->back()][m_callstackBlockIdx->back()].GetPreData();

	if (idx >= data.size())
	{
		LogError("attempted to get PreData out of range \"" + std::to_string(idx) + "\". PreData size is \"" + std::to_string(data.size()) + "\"", LoggerFatality::ABORT);
		return nullptr;
	}

	return data[idx];
}

void* ModBlockPass::GetPreDataRelease(const uint64_t& idx)
{
	return m_data[m_callstackStackIdx->back()][m_callstackBlockIdx->back()].GetCPreData()[idx];
}

ModBlockPassInitializer::ModBlockPassInitializer()
{
	DebugMode = true;

	Data = nullptr;
	VariablesReal = nullptr;
	VariablesBool = nullptr;
	VariablesString = nullptr;

	CustomRegisterMutex = nullptr;
	CustomRegister = nullptr;
	Stop = nullptr;

	// debug only
	DataSize = 0;
	VariableRegistry = nullptr;
	BeginTime = nullptr;
}
