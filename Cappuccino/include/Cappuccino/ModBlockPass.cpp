﻿#include "ModBlockPass.h"

#include <cstddef>
#include <cstdio>
#include <locale>
#include <time.h>
#include <cstring>
#include <chrono>

ModBlockPass::ModBlockPass(const ModBlockPassInitializer& init)
{
	if (init.DebugMode)
	{
		m_getData = &ModBlockPass::GetDataDebug;
		m_getVaraibleReal = &ModBlockPass::GetVariableRealDebug;
		m_getVariableBool = &ModBlockPass::GetVariableBoolDebug;
		m_getVariableString = &ModBlockPass::GetVariableStringDebug;
	}
	else
	{
		m_getData = &ModBlockPass::GetDataRelease;
		m_getVaraibleReal = &ModBlockPass::GetVariableRealRelease;
		m_getVariableBool = &ModBlockPass::GetVariableBoolRelease;
		m_getVariableString = &ModBlockPass::GetVariableStringRelease;
	}

	m_dataSize = init.DataSize;
	m_data = init.Data;
	m_variablesReal = init.VariablesReal;
	m_variablesBool = init.VariablesBool;
	m_variablesString = init.VariablesString;
	m_customRegistrerMutex = init.CustomRegisterMutex;
	m_customRegister = init.CustomRegister;
	m_stop = init.Stop;
	m_variableRegistry = init.VariableRegistry;
	m_beginTime = init.BeginTime;
}

void ModBlockPass::SetData(void** data)
{
	m_data = data;
}

void* ModBlockPass::GetData(const uint64_t& idx)
{
	return (this->*(m_getData))(idx);
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

CAP_DLL void ModBlockPass::Stop()
{
	m_stop();
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

CAP_DLL void ModBlockPass::CustomFree(const uint64_t& idx)
{
	std::unique_lock<std::mutex> lock(*m_customRegistrerMutex);

	delete m_customRegister->at(idx);
	m_customRegister->at(idx) = nullptr;
}

CAP_DLL void ModBlockPass::LogDebug(const std::string& message)
{
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

	// Should be enough
	char prefix[100];

	snprintf(prefix, 100, "[%02u:%02u] [%03u:%03u:%03u] [DEBUG] ",
		((uint64_t)std::chrono::duration_cast<std::chrono::minutes>(now - *m_beginTime).count()),
		((uint64_t)std::chrono::duration_cast<std::chrono::seconds>(now - *m_beginTime).count()),
		((uint64_t)std::chrono::duration_cast<std::chrono::milliseconds>(now - *m_beginTime).count()) % 1000,
		((uint64_t)std::chrono::duration_cast<std::chrono::microseconds>(now - *m_beginTime).count()) % 1000,
		((uint64_t)std::chrono::duration_cast<std::chrono::nanoseconds>(now - *m_beginTime).count()) % 1000);

	std::unique_lock<std::mutex> lock = std::unique_lock<std::mutex>(m_messagesMutex);

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

	snprintf(prefix, 100, "[%02u:%02u] [%03u:%03u:%03u] [INFO] ",
		((uint64_t)std::chrono::duration_cast<std::chrono::minutes>(now - *m_beginTime).count()),
		((uint64_t)std::chrono::duration_cast<std::chrono::seconds>(now - *m_beginTime).count()),
		((uint64_t)std::chrono::duration_cast<std::chrono::milliseconds>(now - *m_beginTime).count()) % 1000,
		((uint64_t)std::chrono::duration_cast<std::chrono::microseconds>(now - *m_beginTime).count()) % 1000,
		((uint64_t)std::chrono::duration_cast<std::chrono::nanoseconds>(now - *m_beginTime).count()) % 1000);

	std::unique_lock<std::mutex> lock = std::unique_lock<std::mutex>(m_messagesMutex);

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

	snprintf(prefix, 100, "[%02u:%02u] [%03u:%03u:%03u] [WARN] ",
		((uint64_t)std::chrono::duration_cast<std::chrono::minutes>(now - *m_beginTime).count()),
		((uint64_t)std::chrono::duration_cast<std::chrono::seconds>(now - *m_beginTime).count()),
		((uint64_t)std::chrono::duration_cast<std::chrono::milliseconds>(now - *m_beginTime).count()) % 1000,
		((uint64_t)std::chrono::duration_cast<std::chrono::microseconds>(now - *m_beginTime).count()) % 1000,
		((uint64_t)std::chrono::duration_cast<std::chrono::nanoseconds>(now - *m_beginTime).count()) % 1000);

	std::unique_lock<std::mutex> lock = std::unique_lock<std::mutex>(m_messagesMutex);

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

	std::unique_lock<std::mutex> lock = std::unique_lock<std::mutex>(m_messagesMutex);

	m_messages.emplace_back();
	size_t index = m_messages.size() - 1;
	m_messages[index].reserve(strlen(prefix) + message.length());

	m_messages[index] += prefix;
	m_messages[index] += message;
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

CAP_DLL void ModBlockPass::SetDataSize(const uint64_t& size)
{
	m_dataSize = size;
}

void* ModBlockPass::GetDataDebug(const uint64_t& idx)
{
	if (m_data == nullptr)
		LogWarning("pulling invalid data!");
	else if (m_dataSize == 0)
		LogWarning("pulling empty data!");
	else if (idx >= m_dataSize)
	{
		LogError("attempted to get data from index out of range", LoggerFatality::ABORT);
		return nullptr;
	}

	return m_data[idx];
}

void* ModBlockPass::GetDataRelease(const uint64_t& idx)
{
	return m_data[idx];
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
