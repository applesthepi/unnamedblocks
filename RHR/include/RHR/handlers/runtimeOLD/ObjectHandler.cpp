#include "ObjectHandler.h"
#include "handlers/Logger.h"

ObjectHandler::ObjectHandler()
{
	m_objects = new std::vector<RuntimeObject*>();
	m_idCount = new unsigned long long(0);
}

ObjectHandler::~ObjectHandler()
{
	for (unsigned int i = 0; i < m_objects->size(); i++)
		delete (*m_objects)[i];

	delete m_objects;
	delete m_idCount;
}

void ObjectHandler::FrameUpdate(sf::RenderWindow* window)
{
	std::shared_lock<std::shared_timed_mutex> lock(ObjectMutex);

	for (unsigned int i = 0; i < m_objects->size(); i++)
	{
		RuntimeObject* obj = (*m_objects)[i];
		if (obj->ImageIndex < obj->Sprites.size())
		{
			obj->Sprites[obj->ImageIndex]->setPosition(obj->Position);
			obj->Sprites[obj->ImageIndex]->setScale(obj->Scale);
			obj->Sprites[obj->ImageIndex]->setRotation(obj->Angle);
		}
	}
}

void ObjectHandler::Render(sf::RenderWindow* window)
{
	std::shared_lock<std::shared_timed_mutex> lock(ObjectMutex);

	for (unsigned int i = 0; i < m_objects->size(); i++)
	{
		RuntimeObject* obj = (*m_objects)[i];
		if (obj->ImageIndex < obj->Sprites.size())
			window->draw(*obj->Sprites[obj->ImageIndex]);
	}
}

void ObjectHandler::Reset()
{
	*m_idCount = 0;

	for (unsigned int i = 0; i < m_objects->size(); i++)
		delete (*m_objects)[i];

	m_objects->clear();
}

RuntimeObject* ObjectHandler::GetObject(unsigned long long objectId)
{
	std::shared_lock<std::shared_timed_mutex> lock(ObjectMutex);

	for (unsigned long long i = 0; i < m_objects->size(); i++)
	{
		if ((*m_objects)[i]->Id == objectId)
		{
			return (*m_objects)[i];
		}
	}

	Logger::Error("object \"" + std::to_string(objectId) + "\" does not exist");
	return nullptr;
}

unsigned long long ObjectHandler::CreateObject(RuntimeObject* object)
{
	std::unique_lock<std::shared_timed_mutex> lock(ObjectMutex);

	object->Id = ++(*m_idCount);
	m_objects->push_back(object);
	
	return *m_idCount;
}

bool ObjectHandler::DestroyObject(unsigned long long objectId)
{
	std::unique_lock<std::shared_timed_mutex> lock(ObjectMutex);

	for (unsigned long long i = 0; i < m_objects->size(); i++)
	{
		if ((*m_objects)[i]->Id == objectId)
		{
			delete (*m_objects)[i];
			m_objects->erase(m_objects->begin() + i);
			return true;
		}
	}

	Logger::Error("object \"" + std::to_string(objectId) + "\" does not exist");
	return false;
}

ObjectHandler& ObjectHandler::operator=(const ObjectHandler& other)
{
	return *this;
}

RuntimeObject::RuntimeObject()
	:ImageIndex(0), Position(sf::Vector2f(0, 0)), Scale(sf::Vector2f(1, 1)), Angle(0.0f)
{

}

RuntimeObject::~RuntimeObject()
{
	for (unsigned int i = 0; i < Sprites.size(); i++)
	{
		delete Sprites[i];
		delete Textures[i];
	}
}