#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include <mutex>
#include <shared_mutex>

class RuntimeObject
{
public:

	~RuntimeObject();

	unsigned long long Id;
	unsigned int ImageIndex;
	sf::Vector2f Position;
	sf::Vector2f Scale;
	float Angle;

	std::vector<sf::Texture*> Textures;
	std::vector<sf::Sprite*> Sprites;
};

class ObjectHandler
{
public:
	ObjectHandler();
	~ObjectHandler();

	void FrameUpdate(sf::RenderWindow* window);
	void Render(sf::RenderWindow* window);
	void Reset();

	RuntimeObject* GetObject(unsigned long long objectId);
	unsigned long long CreateObject(RuntimeObject* object);
	bool DestroyObject(unsigned long long objectId);

	std::shared_timed_mutex ObjectMutex;
	ObjectHandler& operator=(const ObjectHandler& other);
private:
	std::vector<RuntimeObject*>* m_objects;
	unsigned long long* m_idCount;
};