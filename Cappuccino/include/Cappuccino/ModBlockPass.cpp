#include "ModBlockPass.h"

#include <cstddef>
#include <cstdio>
#include <locale>
#include <time.h>
#include <cstring>
#include <chrono>

typedef std::chrono::system_clock Clock;

ModBlockPass::ModBlockPass(void* window, bool debugMode)
	:m_window(window), m_data(nullptr)
{
	if (debugMode)
	{
		m_getRenderWindow = &ModBlockPass::GetRenderWindowDebug;
		m_getData = &ModBlockPass::GetDataDebug;
	}
	else
	{
		m_getRenderWindow = &ModBlockPass::GetRenderWindowRelease;
		m_getData = &ModBlockPass::GetDataRelease;
	}
}

void* ModBlockPass::GetRenderWindow()
{
	return (this->*(m_getRenderWindow))(this);
}

void ModBlockPass::SetData(void** data)
{
	m_data = data;
}

void** ModBlockPass::GetData()
{
	return (this->*(m_getData))(this);
}

void ModBlockPass::LogInfo(const std::string& message)
{
	auto now = Clock::now();
	auto seconds = std::chrono::time_point_cast<std::chrono::seconds>(now);
	auto fraction = now - seconds;
	time_t cnow = Clock::to_time_t(now);

	auto hr = std::chrono::duration_cast<std::chrono::hours>(fraction);
	auto min = std::chrono::duration_cast<std::chrono::minutes>(fraction);
	auto sec = std::chrono::duration_cast<std::chrono::seconds>(fraction);
	auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(fraction);
	auto micro = std::chrono::duration_cast<std::chrono::microseconds>(fraction);
	auto nano = std::chrono::duration_cast<std::chrono::nanoseconds>(fraction);

	// Should be enough
	char prefix[100];
	snprintf(prefix, 100, "[%02u:%02u:%02u] [%03u:%03u:%03u] [INFO] ", hr.count(), min.count(), sec.count(), milliseconds.count(), micro.count(), nano.count());
	
	std::unique_lock<std::mutex> lock = std::unique_lock<std::mutex>(m_messagesMutex);

	// This is how to get only 1 heap allocation, the reserve call.
	m_messages.emplace_back();
	size_t index = m_messages.size() - 1;
	m_messages[index].reserve(strlen(prefix) + message.length());

	m_messages[index] += prefix;
	m_messages[index] += message;
}

void ModBlockPass::LogError(const std::string& message)
{
	auto now = Clock::now();
	auto seconds = std::chrono::time_point_cast<std::chrono::seconds>(now);
	auto fraction = now - seconds;
	time_t cnow = Clock::to_time_t(now);

	auto hr = std::chrono::duration_cast<std::chrono::hours>(fraction);
	auto min = std::chrono::duration_cast<std::chrono::minutes>(fraction);
	auto sec = std::chrono::duration_cast<std::chrono::seconds>(fraction);
	auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(fraction);
	auto micro = std::chrono::duration_cast<std::chrono::microseconds>(fraction);
	auto nano = std::chrono::duration_cast<std::chrono::nanoseconds>(fraction);

	// Should be enough
	char prefix[100];
	snprintf(prefix, 100, "[%02u:%02u:%02u] [%03u:%03u:%03u] [ERROR] ", hr.count(), min.count(), sec.count(), milliseconds.count(), micro.count(), nano.count());
	
	std::unique_lock<std::mutex> lock = std::unique_lock<std::mutex>(m_messagesMutex);

	// This is how to get only 1 heap allocation, the reserve call.
	m_messages.emplace_back();
	size_t index = m_messages.size()-1;
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
	m_messagesMutex.unlock();
}

void* ModBlockPass::GetRenderWindowDebug(ModBlockPass* /*pass*/)
{
	return m_window;
}

void* ModBlockPass::GetRenderWindowRelease(ModBlockPass*)
{
	return m_window;
}

void** ModBlockPass::GetDataDebug(ModBlockPass* /*pass*/)
{
	return m_data;
}

void** ModBlockPass::GetDataRelease(ModBlockPass*)
{
	return m_data;
}
