#include "VariableHandler.h"
#include "handlers/Logger.h"
#include <cstring>

#define MEM_COUNT 10000

void VariableHandler::Alloc()
{
	m_stackNames = (const char**)calloc(MEM_COUNT, sizeof(const char*));
	m_heapNames = new std::vector<const char*>();

	m_stackReal = (double**)calloc(MEM_COUNT, sizeof(double*));
	m_stackString = (std::string**)calloc(MEM_COUNT, sizeof(std::string*));
	m_stackBool = (bool**)calloc(MEM_COUNT, sizeof(bool*));

	m_heapReal = new std::vector<double*>();
	m_heapString = new std::vector<std::string*>();
	m_heapBool = new std::vector<bool*>();
}

void VariableHandler::Dealloc()
{
	for (unsigned int i = 0; i < MEM_COUNT; i++)
	{
		if (m_stackNames[i] != nullptr)
		{
			free((void*)m_stackNames[i]);

			if (m_stackReal[i] != nullptr)
				free((void*)m_stackReal[i]);
			if (m_stackString[i] != nullptr)
				free((void*)m_stackString[i]);
			if (m_stackBool[i] != nullptr)
				free((void*)m_stackBool[i]);
		}
	}

	free(m_stackNames);

	free(m_stackReal);
	free(m_stackString);
	free(m_stackBool);

	for (unsigned int i = 0; i < m_heapNames->size(); i++)
		free((void*)(*m_heapNames)[i]);

	delete m_heapNames;
	delete m_heapReal;
	delete m_heapString;
	delete m_heapBool;
}

bool VariableHandler::StackReal(const char* name)
{
	m_mutex.lock();
	unsigned int i = 0;
	while (m_stackNames[i] != nullptr)
	{
		i++;
		if (i == MEM_COUNT)
		{
			Logger::Error("reached maximum stack size");
			m_mutex.unlock();
			return false;
		}
	}

	char* nName = (char*)calloc(strlen(name) + 1, sizeof(char));
	strcpy(nName, name);

	m_stackNames[i] = nName;
	m_stackReal[i] = new double(0);

	m_mutex.unlock();
	return true;
}

bool VariableHandler::StackString(const char* name)
{
	m_mutex.lock();
	unsigned int i = 0;
	while (m_stackNames[i] != nullptr)
	{
		i++;
		if (i == MEM_COUNT)
		{
			Logger::Error("reached maximum stack size");
			m_mutex.unlock();
			return false;
		}
	}

	char* nName = (char*)calloc(strlen(name) + 1, sizeof(char));
	strcpy(nName, name);

	m_stackNames[i] = nName;
	m_stackString[i] = new std::string();

	m_mutex.unlock();
	return true;
}

bool VariableHandler::StackBool(const char* name)
{
	m_mutex.lock();
	unsigned int i = 0;
	while (m_stackNames[i] != nullptr)
	{
		i++;
		if (i == MEM_COUNT)
		{
			Logger::Error("reached maximum stack size");
			m_mutex.unlock();
			return false;
		}
	}

	char* nName = (char*)calloc(strlen(name) + 1, sizeof(char));
	strcpy(nName, name);

	m_stackNames[i] = nName;
	m_stackBool[i] = new bool(false);
	
	m_mutex.unlock();
	return true;
}

void VariableHandler::HeapReal(const char* name)
{
	m_mutex.lock();
	char* nName = (char*)calloc(strlen(name) + 1, sizeof(char));
	strcpy(nName, name);
	m_heapNames->push_back(nName);

	m_heapReal->push_back(new double(0.0));
	m_heapString->push_back(nullptr);
	m_heapBool->push_back(nullptr);

	m_mutex.unlock();
}

void VariableHandler::HeapString(const char* name)
{
	m_mutex.lock();
	char* nName = (char*)calloc(strlen(name) + 1, sizeof(char));
	strcpy(nName, name);
	m_heapNames->push_back(nName);
	
	m_heapReal->push_back(nullptr);
	m_heapString->push_back(new std::string());
	m_heapBool->push_back(nullptr);

	m_mutex.unlock();
}

void VariableHandler::HeapBool(const char* name)
{
	m_mutex.lock();
	char* nName = (char*)calloc(strlen(name) + 1, sizeof(char));
	strcpy(nName, name);
	m_heapNames->push_back(nName);
	
	m_heapReal->push_back(nullptr);
	m_heapString->push_back(nullptr);
	m_heapBool->push_back(new bool(false));

	m_mutex.unlock();
}

bool VariableHandler::FreeReal(const char* name)
{
	m_mutex.lock();
	for (unsigned int i = 0; i < MEM_COUNT; i++)
	{
		if (m_stackNames[i] != nullptr && strcmp(m_stackNames[i], name) == 0)
		{
			free((void*)m_stackNames[i]);
			free((void*)m_stackReal[i]);
			m_stackNames[i] = nullptr;
			m_stackReal[i] = nullptr;

			m_mutex.unlock();
			return true;
		}
	}

	for (unsigned int i = 0; i < m_heapNames->size(); i++)
	{
		if ((*m_heapNames)[i] != nullptr && strcmp((*m_heapNames)[i], name) == 0)
		{
			delete (*m_heapNames)[i];
			delete (*m_heapReal)[i];

			m_heapNames->erase(m_heapNames->begin() + i);
			m_heapReal->erase(m_heapReal->begin() + i);
			m_heapString->erase(m_heapString->begin() + i);
			m_heapBool->erase(m_heapBool->begin() + i);

			m_mutex.unlock();
			return true;
		}
	}

	m_mutex.unlock();
	return false;
}

bool VariableHandler::FreeString(const char* name)
{
	m_mutex.lock();
	for (unsigned int i = 0; i < MEM_COUNT; i++)
	{
		if (m_stackNames[i] != nullptr && strcmp(m_stackNames[i], name) == 0)
		{
			free((void*)m_stackNames[i]);
			free((void*)m_stackString[i]);

			m_stackNames[i] = nullptr;
			m_stackString[i] = nullptr;

			m_mutex.unlock();
			return true;
		}
	}

	for (unsigned int i = 0; i < m_heapNames->size(); i++)
	{
		if ((*m_heapNames)[i] != nullptr && strcmp((*m_heapNames)[i], name) == 0)
		{
			delete (*m_heapNames)[i];
			delete (*m_heapString)[i];

			m_heapNames->erase(m_heapNames->begin() + i);
			m_heapReal->erase(m_heapReal->begin() + i);
			m_heapString->erase(m_heapString->begin() + i);
			m_heapBool->erase(m_heapBool->begin() + i);

			m_mutex.unlock();
			return true;
		}
	}

	m_mutex.unlock();
	return false;
}

bool VariableHandler::FreeBool(const char* name)
{
	m_mutex.lock();
	for (unsigned int i = 0; i < MEM_COUNT; i++)
	{
		if (m_stackNames[i] != nullptr && strcmp(m_stackNames[i], name) == 0)
		{
			free((void*)m_stackNames[i]);
			free((void*)m_stackBool[i]);

			m_stackNames[i] = nullptr;
			m_stackBool[i] = nullptr;

			m_mutex.unlock();
			return true;
		}
	}

	for (unsigned int i = 0; i < m_heapNames->size(); i++)
	{
		if ((*m_heapNames)[i] != nullptr && strcmp((*m_heapNames)[i], name) == 0)
		{
			delete (*m_heapNames)[i];
			delete (*m_heapBool)[i];

			m_heapNames->erase(m_heapNames->begin() + i);
			m_heapReal->erase(m_heapReal->begin() + i);
			m_heapString->erase(m_heapString->begin() + i);
			m_heapBool->erase(m_heapBool->begin() + i);

			m_mutex.unlock();
			return true;
		}
	}

	m_mutex.unlock();
	return false;
}

double* VariableHandler::GetReal(const char* name)
{
	m_mutex.lock();
	for (unsigned int i = 0; i < MEM_COUNT; i++)
	{
		if (m_stackNames[i] != nullptr && strcmp(m_stackNames[i], name) == 0)
		{
			m_mutex.unlock();
			return m_stackReal[i];
		}
	}

	for (unsigned int i = 0; i < m_heapNames->size(); i++)
	{
		if ((*m_heapNames)[i] != nullptr && strcmp((*m_heapNames)[i], name) == 0)
		{
			m_mutex.unlock();
			return (*m_heapReal)[i];
		}
	}

	m_mutex.unlock();
	return nullptr;
}

std::string* VariableHandler::GetString(const char* name)
{
	m_mutex.lock();
	for (unsigned int i = 0; i < MEM_COUNT; i++)
	{
		if (m_stackNames[i] != nullptr && strcmp(m_stackNames[i], name) == 0)
		{
			m_mutex.unlock();
			return m_stackString[i];
		}
	}

	for (unsigned int i = 0; i < m_heapNames->size(); i++)
	{
		if ((*m_heapNames)[i] != nullptr && strcmp((*m_heapNames)[i], name) == 0)
		{
			m_mutex.unlock();
			return (*m_heapString)[i];
		}
	}

	m_mutex.unlock();
	return nullptr;
}

bool* VariableHandler::GetBool(const char* name)
{
	m_mutex.lock();
	for (unsigned int i = 0; i < MEM_COUNT; i++)
	{
		if (m_stackNames[i] != nullptr && strcmp(m_stackNames[i], name) == 0)
		{
			m_mutex.unlock();
			return m_stackBool[i];
		}
	}

	for (unsigned int i = 0; i < m_heapNames->size(); i++)
	{
		if ((*m_heapNames)[i] != nullptr && strcmp((*m_heapNames)[i], name) == 0)
		{
			m_mutex.unlock();
			return (*m_heapBool)[i];
		}
	}

	m_mutex.unlock();
	return nullptr;
}

bool VariableHandler::SetReal(const char* name, double value)
{
	m_mutex.lock();
	for (unsigned int i = 0; i < MEM_COUNT; i++)
	{
		if (m_stackNames[i] != nullptr && strcmp(m_stackNames[i], name) == 0)
		{
			*(m_stackReal[i]) = value;
			m_mutex.unlock();
			return true;
		}
	}

	for (unsigned int i = 0; i < m_heapNames->size(); i++)
	{
		if ((*m_heapNames)[i] != nullptr && strcmp((*m_heapNames)[i], name) == 0)
		{
			*((*m_heapReal)[i]) = value;
			m_mutex.unlock();
			return true;
		}
	}

	m_mutex.unlock();
	Logger::Error("variable \"" + std::string(name) + "\" does not exist");

	return false;
}

bool VariableHandler::SetString(const char* name, std::string value)
{
	m_mutex.lock();
	for (unsigned int i = 0; i < MEM_COUNT; i++)
	{
		if (m_stackNames[i] != nullptr && strcmp(m_stackNames[i], name) == 0)
		{
			*(m_stackString[i]) = value;
			m_mutex.unlock();
			return true;
		}
	}

	for (unsigned int i = 0; i < m_heapNames->size(); i++)
	{
		if ((*m_heapNames)[i] != nullptr && strcmp((*m_heapNames)[i], name) == 0)
		{
			*((*m_heapString)[i]) = value;
			m_mutex.unlock();
			return true;
		}
	}

	m_mutex.unlock();
	Logger::Error("variable \"" + std::string(name) + "\" does not exist");

	return false;
}

bool VariableHandler::SetBool(const char* name, bool value)
{
	m_mutex.lock();
	for (unsigned int i = 0; i < MEM_COUNT; i++)
	{
		if (m_stackNames[i] != nullptr && strcmp(m_stackNames[i], name) == 0)
		{
			*(m_stackBool[i]) = value;
			m_mutex.unlock();
			return true;
		}
	}

	for (unsigned int i = 0; i < m_heapNames->size(); i++)
	{
		if ((*m_heapNames)[i] != nullptr && strcmp((*m_heapNames)[i], name) == 0)
		{
			*((*m_heapBool)[i]) = value;
			m_mutex.unlock();
			return true;
		}
	}

	m_mutex.unlock();
	Logger::Error("variable \"" + std::string(name) + "\" does not exist");

	return false;
}

std::mutex VariableHandler::m_mutex;

const char** VariableHandler::m_stackNames;

std::vector<const char*>* VariableHandler::m_heapNames;

double** VariableHandler::m_stackReal;

std::string** VariableHandler::m_stackString;

bool** VariableHandler::m_stackBool;

std::vector<double*>* VariableHandler::m_heapReal;

std::vector<std::string*>* VariableHandler::m_heapString;

std::vector<bool*>* VariableHandler::m_heapBool;

