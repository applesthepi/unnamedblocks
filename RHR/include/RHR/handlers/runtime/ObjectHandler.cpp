#include "ObjectHandler.h"

void ObjectHandler::Alloc()
{
	m_objects = new std::vector<RuntimeObject*>();
	m_idCount = new unsigned long long(0);
	ObjectMutex = new std::mutex();
}

void ObjectHandler::Dealloc()
{
	for (unsigned int i = 0; i < m_objects->size(); i++)
	{
		delete (*m_objects)[i];
	}

	delete m_objects;
	delete m_idCount;

	delete ObjectMutex;
}

void ObjectHandler::FrameUpdate(sf::RenderWindow* window)
{
	ObjectHandler::ObjectMutex->lock();
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
	ObjectHandler::ObjectMutex->unlock();
}

void ObjectHandler::Render(sf::RenderWindow* window)
{
	ObjectHandler::ObjectMutex->lock();
	for (unsigned int i = 0; i < m_objects->size(); i++)
	{
		RuntimeObject* obj = (*m_objects)[i];
		if (obj->ImageIndex < obj->Sprites.size())
			window->draw(*obj->Sprites[obj->ImageIndex]);
	}
	ObjectHandler::ObjectMutex->unlock();
}

RuntimeObject* ObjectHandler::GetObject(unsigned long long objectId)
{
	for (unsigned long long i = 0; i < m_objects->size(); i++)
	{
		if ((*m_objects)[i]->Id == objectId)
		{
			return (*m_objects)[i];
		}
	}

	return nullptr;
}

unsigned long long ObjectHandler::CreateObject(RuntimeObject* object)
{
	object->Id = ++(*m_idCount);
	m_objects->push_back(object);
	return *m_idCount;
}

bool ObjectHandler::DestroyObject(unsigned long long objectId)
{
	for (unsigned long long i = 0; i < m_objects->size(); i++)
	{
		if ((*m_objects)[i]->Id == objectId)
		{
			delete (*m_objects)[i];
			m_objects->erase(m_objects->begin() + i);
			return true;
		}
	}

	return false;
}

std::mutex* ObjectHandler::ObjectMutex;

std::vector<RuntimeObject*>* ObjectHandler::m_objects;

unsigned long long* ObjectHandler::m_idCount;

RuntimeObject::~RuntimeObject()
{
	for (unsigned int i = 0; i < Sprites.size(); i++)
	{
		delete Sprites[i];
		delete Textures[i];
	}
}
