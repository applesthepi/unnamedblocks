#include "VariableHandler.h"
#include "handlers/Logger.h"
#include <cstring>

#define MEM_COUNT 10000

VariableHandler::VariableHandler()
{
	m_stackNames = (const char**)calloc(MEM_COUNT, sizeof(const char*));
	m_heapNames = new std::vector<const char*>();

	m_stackReal = (double**)calloc(MEM_COUNT, sizeof(double*));
	m_stackString = (std::string**)calloc(MEM_COUNT, sizeof(std::string*));
	m_stackBool = (bool**)calloc(MEM_COUNT, sizeof(bool*));

	m_heapReal = new std::vector<double*>();
	m_heapString = new std::vector<std::string*>();
	m_heapBool = new std::vector<bool*>();

	m_mutex = new std::shared_timed_mutex();
}

VariableHandler::~VariableHandler()
{
	for (unsigned int i = 0; i < MEM_COUNT; i++)
	{
		if (m_stackNames[i] != nullptr)
		{
			free((char*)m_stackNames[i]);

			if (m_stackReal[i] != nullptr)
				free((char*)m_stackReal[i]);
			if (m_stackString[i] != nullptr)
				free((char*)m_stackString[i]);
			if (m_stackBool[i] != nullptr)
				free((char*)m_stackBool[i]);
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

	delete m_mutex;
}

void VariableHandler::Reset()
{
	this->~VariableHandler();
	
	m_stackNames = (const char**)calloc(MEM_COUNT, sizeof(const char*));
	m_heapNames = new std::vector<const char*>();

	m_stackReal = (double**)calloc(MEM_COUNT, sizeof(double*));
	m_stackString = (std::string**)calloc(MEM_COUNT, sizeof(std::string*));
	m_stackBool = (bool**)calloc(MEM_COUNT, sizeof(bool*));

	m_heapReal = new std::vector<double*>();
	m_heapString = new std::vector<std::string*>();
	m_heapBool = new std::vector<bool*>();

	m_mutex = new std::shared_timed_mutex();
}

bool VariableHandler::StackReal(const char* name)
{
	std::unique_lock<std::shared_timed_mutex> lock(*m_mutex);
	
	unsigned int i = 0;
	while (m_stackNames[i] != nullptr)
	{
		i++;
		if (i == MEM_COUNT)
		{
			Logger::Error("reached maximum stack size");
			return false;
		}
	}

	char* nName = (char*)calloc(strlen(name) + 1, sizeof(char));
	strcpy(nName, name);

	m_stackNames[i] = nName;
	m_stackReal[i] = new double(0);

	return true;
}

bool VariableHandler::StackString(const char* name)
{
	std::unique_lock<std::shared_timed_mutex> lock(*m_mutex);

	unsigned int i = 0;
	while (m_stackNames[i] != nullptr)
	{
		i++;
		if (i == MEM_COUNT)
		{
			Logger::Error("reached maximum stack size");
			return false;
		}
	}

	char* nName = (char*)calloc(strlen(name) + 1, sizeof(char));
	strcpy(nName, name);

	m_stackNames[i] = nName;
	m_stackString[i] = new std::string();

	return true;
}

bool VariableHandler::StackBool(const char* name)
{
	std::unique_lock<std::shared_timed_mutex> lock(*m_mutex);
	unsigned int i = 0;
	while (m_stackNames[i] != nullptr)
	{
		i++;
		if (i == MEM_COUNT)
		{
			Logger::Error("reached maximum stack size");
			return false;
		}
	}

	char* nName = (char*)calloc(strlen(name) + 1, sizeof(char));
	strcpy(nName, name);

	m_stackNames[i] = nName;
	m_stackBool[i] = new bool(false);
	
	return true;
}

void VariableHandler::HeapReal(const char* name)
{
	std::unique_lock<std::shared_timed_mutex> lock(*m_mutex);

	char* nName = (char*)calloc(strlen(name) + 1, sizeof(char));
	strcpy(nName, name);
	m_heapNames->push_back(nName);

	m_heapReal->push_back(new double(0.0));
	m_heapString->push_back(nullptr);
	m_heapBool->push_back(nullptr);
}

void VariableHandler::HeapString(const char* name)
{
	std::unique_lock<std::shared_timed_mutex> lock(*m_mutex);

	char* nName = (char*)calloc(strlen(name) + 1, sizeof(char));
	strcpy(nName, name);
	m_heapNames->push_back(nName);
	
	m_heapReal->push_back(nullptr);
	m_heapString->push_back(new std::string());
	m_heapBool->push_back(nullptr);
}

void VariableHandler::HeapBool(const char* name)
{
	std::unique_lock<std::shared_timed_mutex> lock(*m_mutex);

	char* nName = (char*)calloc(strlen(name) + 1, sizeof(char));
	strcpy(nName, name);
	m_heapNames->push_back(nName);
	
	m_heapReal->push_back(nullptr);
	m_heapString->push_back(nullptr);
	m_heapBool->push_back(new bool(false));
}

bool VariableHandler::FreeReal(const char* name)
{
	std::unique_lock<std::shared_timed_mutex> lock(*m_mutex);

	for (unsigned int i = 0; i < MEM_COUNT; i++)
	{
		if (m_stackNames[i] != nullptr && strcmp(m_stackNames[i], name) == 0)
		{
			free((void*)m_stackNames[i]);
			delete m_stackReal[i];

			m_stackNames[i] = nullptr;
			m_stackReal[i] = nullptr;

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

			return true;
		}
	}

	return false;
}

bool VariableHandler::FreeString(const char* name)
{
	std::unique_lock<std::shared_timed_mutex> lock(*m_mutex);

	for (unsigned int i = 0; i < MEM_COUNT; i++)
	{
		if (m_stackNames[i] != nullptr && strcmp(m_stackNames[i], name) == 0)
		{
			free((void*)m_stackNames[i]);
			delete m_stackString[i];

			m_stackNames[i] = nullptr;
			m_stackString[i] = nullptr;

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

			return true;
		}
	}

	return false;
}

bool VariableHandler::FreeBool(const char* name)
{
	std::unique_lock<std::shared_timed_mutex> lock(*m_mutex);

	for (unsigned int i = 0; i < MEM_COUNT; i++)
	{
		if (m_stackNames[i] != nullptr && strcmp(m_stackNames[i], name) == 0)
		{
			free((void*)m_stackNames[i]);
			delete m_stackBool[i];

			m_stackNames[i] = nullptr;
			m_stackBool[i] = nullptr;

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

			return true;
		}
	}

	return false;
}

double* VariableHandler::GetReal(const char* name)
{
	std::shared_lock<std::shared_timed_mutex> lock(*m_mutex);

	for (unsigned int i = 0; i < MEM_COUNT; i++)
	{
		if (m_stackNames[i] != nullptr && strcmp(m_stackNames[i], name) == 0)
		{
			return m_stackReal[i];
		}
	}

	for (unsigned int i = 0; i < m_heapNames->size(); i++)
	{
		if ((*m_heapNames)[i] != nullptr && strcmp((*m_heapNames)[i], name) == 0)
		{
			return (*m_heapReal)[i];
		}
	}

	return nullptr;
}

std::string* VariableHandler::GetString(const char* name)
{
	std::shared_lock<std::shared_timed_mutex> lock(*m_mutex);

	for (unsigned int i = 0; i < MEM_COUNT; i++)
	{
		if (m_stackNames[i] != nullptr && strcmp(m_stackNames[i], name) == 0)
		{
			return m_stackString[i];
		}
	}

	for (unsigned int i = 0; i < m_heapNames->size(); i++)
	{
		if ((*m_heapNames)[i] != nullptr && strcmp((*m_heapNames)[i], name) == 0)
		{
			return (*m_heapString)[i];
		}
	}

	return nullptr;
}

bool* VariableHandler::GetBool(const char* name)
{
	std::shared_lock<std::shared_timed_mutex> lock(*m_mutex);

	for (unsigned int i = 0; i < MEM_COUNT; i++)
	{
		if (m_stackNames[i] != nullptr && strcmp(m_stackNames[i], name) == 0)
		{
			return m_stackBool[i];
		}
	}

	for (unsigned int i = 0; i < m_heapNames->size(); i++)
	{
		if ((*m_heapNames)[i] != nullptr && strcmp((*m_heapNames)[i], name) == 0)
		{
			return (*m_heapBool)[i];
		}
	}

	return nullptr;
}

bool VariableHandler::SetReal(const char* name, double value)
{
	std::unique_lock<std::shared_timed_mutex> lock(*m_mutex);

	for (unsigned int i = 0; i < MEM_COUNT; i++)
	{
		if (m_stackNames[i] != nullptr && strcmp(m_stackNames[i], name) == 0)
		{
			*(m_stackReal[i]) = value;
			return true;
		}
	}

	for (unsigned int i = 0; i < m_heapNames->size(); i++)
	{
		if ((*m_heapNames)[i] != nullptr && strcmp((*m_heapNames)[i], name) == 0)
		{
			*((*m_heapReal)[i]) = value;
			return true;
		}
	}

	Logger::Error("variable \"" + std::string(name) + "\" does not exist");

	return false;
}

bool VariableHandler::SetString(const char* name, std::string value)
{
	std::unique_lock<std::shared_timed_mutex> lock(*m_mutex);

	for (unsigned int i = 0; i < MEM_COUNT; i++)
	{
		if (m_stackNames[i] != nullptr && strcmp(m_stackNames[i], name) == 0)
		{
			*(m_stackString[i]) = value;
			return true;
		}
	}

	for (unsigned int i = 0; i < m_heapNames->size(); i++)
	{
		if ((*m_heapNames)[i] != nullptr && strcmp((*m_heapNames)[i], name) == 0)
		{
			*((*m_heapString)[i]) = value;
			return true;
		}
	}

	Logger::Error("variable \"" + std::string(name) + "\" does not exist");

	return false;
}

bool VariableHandler::SetBool(const char* name, bool value)
{
	std::unique_lock<std::shared_timed_mutex> lock(*m_mutex);

	for (unsigned int i = 0; i < MEM_COUNT; i++)
	{
		if (m_stackNames[i] != nullptr && strcmp(m_stackNames[i], name) == 0)
		{
			*(m_stackBool[i]) = value;
			return true;
		}
	}

	for (unsigned int i = 0; i < m_heapNames->size(); i++)
	{
		if ((*m_heapNames)[i] != nullptr && strcmp((*m_heapNames)[i], name) == 0)
		{
			*((*m_heapBool)[i]) = value;
			return true;
		}
	}

	Logger::Error("variable \"" + std::string(name) + "\" does not exist");

	return false;
}

VariableHandler& VariableHandler::operator=(const VariableHandler& other)
{
	return *this;
}
