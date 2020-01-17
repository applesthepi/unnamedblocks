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
	static void Alloc();
	static void Dealloc();

	static void FrameUpdate(sf::RenderWindow* window);
	static void Render(sf::RenderWindow* window);

	static RuntimeObject* GetObject(unsigned long long objectId);
	static unsigned long long CreateObject(RuntimeObject* object);
	static bool DestroyObject(unsigned long long objectId);

	static std::shared_mutex ObjectMutex;
private:
	static std::vector<RuntimeObject*>* m_objects;
	static unsigned long long* m_idCount;
};