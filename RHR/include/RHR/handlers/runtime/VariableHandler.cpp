#include "VariableHandler.h"
#include "handlers/Logger.h"
#include <cstring>

#define MEM_COUNT 10000

VariableHandler::VariableHandler()
{
	m_mutex = new std::shared_timed_mutex();
}

VariableHandler::~VariableHandler()
{
	delete m_mutex;
}

void VariableHandler::Reset()
{
	this->~VariableHandler();
	
	m_layers.clear();
	m_mutex = new std::shared_timed_mutex();
}

bool VariableHandler::StackReal(uint64_t layerIdx, uint64_t variableIdx)
{
	std::unique_lock<std::shared_timed_mutex> lock(*m_mutex);
	uint64_t lIdx = FindLayerIdx(layerIdx);
	
	unsigned int i = 0;
	while (m_layers[lIdx].StackIdx[i] != 0)
	{
		i++;
		if (i == MEM_COUNT)
		{
			Logger::Error("reached maximum stack size");
			return false;
		}
	}

	m_layers[lIdx].StackIdx[i] = variableIdx;
	m_layers[lIdx].StackReal[i] = 0.0;

	return true;
}

bool VariableHandler::StackString(uint64_t layerIdx, uint64_t variableIdx)
{
	std::unique_lock<std::shared_timed_mutex> lock(*m_mutex);
	uint64_t lIdx = FindLayerIdx(layerIdx);

	unsigned int i = 0;
	while (m_layers[lIdx].StackIdx[i] != 0)
	{
		i++;
		if (i == MEM_COUNT)
		{
			Logger::Error("reached maximum stack size");
			return false;
		}
	}

	m_layers[lIdx].StackIdx[i] = variableIdx;
	m_layers[lIdx].StackString[i] = std::string();

	return true;
}

bool VariableHandler::StackBool(uint64_t layerIdx, uint64_t variableIdx)
{
	std::unique_lock<std::shared_timed_mutex> lock(*m_mutex);
	uint64_t lIdx = FindLayerIdx(layerIdx);

	unsigned int i = 0;
	while (m_layers[lIdx].StackIdx[i] != 0)
	{
		i++;
		if (i == MEM_COUNT)
		{
			Logger::Error("reached maximum stack size");
			return false;
		}
	}

	m_layers[lIdx].StackIdx[i] = variableIdx;
	m_layers[lIdx].StackBool[i] = false;

	return true;
}

void VariableHandler::HeapVariable(uint64_t layerIdx, uint64_t variableIdx)
{
	std::unique_lock<std::shared_timed_mutex> lock(*m_mutex);
	uint64_t lIdx = FindLayerIdx(layerIdx);

	m_layers[lIdx].HeapIdx.push_back(variableIdx);
	m_layers[lIdx].HeapReal.push_back(0.0);
	m_layers[lIdx].HeapString.push_back("");
	m_layers[lIdx].HeapBool.push_back(false);
}

bool VariableHandler::FreeVariable(uint64_t layerIdx, uint64_t variableIdx)
{
	std::unique_lock<std::shared_timed_mutex> lock(*m_mutex);
	uint64_t lIdx = FindLayerIdx(layerIdx);

	for (uint32_t i = 0; i < MEM_COUNT; i++)
	{
		if (m_layers[lIdx].StackIdx[i] == variableIdx)
		{
			m_layers[lIdx].StackIdx[i] = 0;
			return true;
		}
	}

	for (uint64_t i = 0; i < m_layers[lIdx].HeapIdx.size(); i++)
	{
		if (m_layers[layerIdx].HeapIdx[i] == variableIdx)
		{
			m_layers[lIdx].HeapIdx.erase(m_layers[lIdx].HeapIdx.begin() + i);
			m_layers[lIdx].HeapReal.erase(m_layers[lIdx].HeapReal.begin() + i);
			m_layers[lIdx].HeapString.erase(m_layers[lIdx].HeapString.begin() + i);
			m_layers[lIdx].HeapBool.erase(m_layers[lIdx].HeapBool.begin() + i);

			return true;
		}
	}

	return false;
}

const double* VariableHandler::GetReal(uint64_t layerIdx, uint64_t variableIdx)
{
	std::shared_lock<std::shared_timed_mutex> lock(*m_mutex);
	uint64_t lIdx = FindLayerIdx(layerIdx);

	for (uint32_t i = 0; i < MEM_COUNT; i++)
	{
		if (m_layers[lIdx].StackIdx[i] == variableIdx)
			return &m_layers[lIdx].StackReal[i];
	}

	for (uint64_t i = 0; i < m_layers[lIdx].HeapIdx.size(); i++)
	{
		if (m_layers[lIdx].HeapIdx[i] == variableIdx)
			return &m_layers[lIdx].HeapReal[i];
	}

	return nullptr;
}

const std::string* VariableHandler::GetString(uint64_t layerIdx, uint64_t variableIdx)
{
	std::shared_lock<std::shared_timed_mutex> lock(*m_mutex);
	uint64_t lIdx = FindLayerIdx(layerIdx);

	for (uint32_t i = 0; i < MEM_COUNT; i++)
	{
		if (m_layers[lIdx].StackIdx[i] == variableIdx)
			return &m_layers[lIdx].StackString[i];
	}

	for (uint64_t i = 0; i < m_layers[lIdx].HeapIdx.size(); i++)
	{
		if (m_layers[lIdx].HeapIdx[i] == variableIdx)
			return &m_layers[lIdx].HeapString[i];
	}

	return nullptr;
}

const uint8_t* VariableHandler::GetBool(uint64_t layerIdx, uint64_t variableIdx)
{
	std::shared_lock<std::shared_timed_mutex> lock(*m_mutex);
	uint64_t lIdx = FindLayerIdx(layerIdx);

	for (uint32_t i = 0; i < MEM_COUNT; i++)
	{
		if (m_layers[lIdx].StackIdx[i] == variableIdx)
			return &m_layers[lIdx].StackBool[i];
	}

	for (uint64_t i = 0; i < m_layers[lIdx].HeapIdx.size(); i++)
	{
		if (m_layers[lIdx].HeapIdx[i] == variableIdx)
			return &m_layers[lIdx].HeapBool[i];
	}

	return nullptr;
}

bool VariableHandler::SetReal(const uint64_t& layerIdx, const uint64_t& variableIdx, const double& value)
{
	std::unique_lock<std::shared_timed_mutex> lock(*m_mutex);
	uint64_t lIdx = FindLayerIdx(layerIdx);

	for (unsigned int i = 0; i < MEM_COUNT; i++)
	{
		if (m_layers[lIdx].StackIdx[i] == variableIdx)
		{
			m_layers[lIdx].StackReal[i] = value;
			return true;
		}
	}

	for (unsigned int i = 0; i < m_layers[lIdx].HeapIdx.size(); i++)
	{
		if (m_layers[lIdx].HeapIdx[i] == variableIdx)
		{
			m_layers[lIdx].HeapReal[i] = value;
			return true;
		}
	}

	Logger::Error("preprocessed variable idx \"" + std::to_string(variableIdx) + "\" does not exist");

	return false;
}

bool VariableHandler::SetString(const uint64_t& layerIdx, const uint64_t& variableIdx, const std::string& value)
{
	std::unique_lock<std::shared_timed_mutex> lock(*m_mutex);
	uint64_t lIdx = FindLayerIdx(layerIdx);

	for (unsigned int i = 0; i < MEM_COUNT; i++)
	{
		if (m_layers[lIdx].StackIdx[i] == variableIdx)
		{
			m_layers[lIdx].StackString[i] = value;
			return true;
		}
	}

	for (unsigned int i = 0; i < m_layers[lIdx].HeapIdx.size(); i++)
	{
		if (m_layers[lIdx].HeapIdx[i] == variableIdx)
		{
			m_layers[lIdx].HeapString[i] = value;
			return true;
		}
	}

	Logger::Error("preprocessed variable idx \"" + std::to_string(variableIdx) + "\" does not exist");

	return false;
}

bool VariableHandler::SetBool(const uint64_t& layerIdx, const uint64_t& variableIdx, const uint8_t& value)
{
	std::unique_lock<std::shared_timed_mutex> lock(*m_mutex);
	uint64_t lIdx = FindLayerIdx(layerIdx);

	for (unsigned int i = 0; i < MEM_COUNT; i++)
	{
		if (m_layers[lIdx].StackIdx[i] == variableIdx)
		{
			m_layers[lIdx].StackBool[i] = value;
			return true;
		}
	}

	for (unsigned int i = 0; i < m_layers[lIdx].HeapIdx.size(); i++)
	{
		if (m_layers[lIdx].HeapIdx[i] == variableIdx)
		{
			m_layers[lIdx].HeapBool[i] = value;
			return true;
		}
	}

	Logger::Error("preprocessed variable idx \"" + std::to_string(variableIdx) + "\" does not exist");

	return false;
}

uint64_t VariableHandler::PushStack()
{
	std::unique_lock<std::shared_timed_mutex> lock(*m_mutex);

	uint64_t sId = 0;
	bool foundSId = false;
	
	for (uint64_t i = 0; i < m_layers.size(); i++)
	{
		bool found = false;
		for (uint64_t a = 0; a < m_layers.size(); a++)
		{
			if (m_layers[a].Idx == sId)
			{
				sId++;
				found = true;
				break;
			}
		}

		if (!found)
		{
			foundSId = true;
			break;
		}
	}

	if (!foundSId)
		sId++;

	m_layers.push_back(StackLayer(sId));
	return sId;
}

void VariableHandler::PopStack(const uint64_t& idx)
{
	std::unique_lock<std::shared_timed_mutex> lock(*m_mutex);

	for (uint64_t i = 0; i < m_layers.size(); i++)
	{
		if (m_layers[i].Idx == idx)
		{
			m_layers.erase(m_layers.begin() + i);
			return;
		}
	}
}

uint64_t VariableHandler::FindLayerIdx(const uint64_t& idx)
{
	for (uint64_t i = 0; i < m_layers.size(); i++)
	{
		if (m_layers[i].Idx == idx)
			return i;
	}

	Logger::Error("layer not found, defaulting to 0!");
	return 0;
}

VariableHandler& VariableHandler::operator=(const VariableHandler& other)
{
	return *this;
}

StackLayer::StackLayer(uint64_t idx)
{
	Idx = idx;
	
	StackIdx = (uint64_t*)calloc(MEM_COUNT, sizeof(uint64_t));
	StackReal = (double*)calloc(MEM_COUNT, sizeof(double));
	StackString = (std::string*)calloc(MEM_COUNT, sizeof(std::string));
	StackBool = (uint8_t*)calloc(MEM_COUNT, sizeof(uint8_t));
}

StackLayer::~StackLayer()
{
	free(StackIdx);
	free(StackReal);
	free(StackString);
	free(StackBool);
}
