#pragma once

#include "registries/BlockRegistry.h"
#include "args/Argument.h"

#include <SFML/Graphics.hpp>
#include <functional>

class BlockRuntimeReturn
{
public:
	BlockRuntimeReturn(const std::vector<BlockArgumentCaller>* args, unsigned char length)
		:Args(args), Length(length)
	{}

	~BlockRuntimeReturn()
	{
		delete Args;
	}

	const std::vector<BlockArgumentCaller>* Args;
	unsigned char Length;
};

class Block
{
public:
	Block(std::string type);
	~Block();

	void Render(sf::RenderTexture* render, sf::RenderWindow* window);
	void FrameUpdate(sf::RenderWindow* window, bool global = false);
	void SetArgData(const std::vector<BlockArgumentCaller>& data);
	void SetArgData(const std::vector<std::string> data);
	void SetupInStack(unsigned int index, sf::Vector2i* stackAbsolute, sf::Vector2i* stackRelitive, std::function<void(unsigned int index, sf::Vector2i mousePosition)>* functionSplit, std::function<void(unsigned int index, sf::Vector2i mousePosition)>* functionContext);
	unsigned int GetWidth();
	Argument* GetArgument(unsigned int index);
	BlockRuntimeReturn GetUsedArgumentsRuntime();
	std::vector<std::string>* GetUsedArgumentSetup();
	unsigned int GetArgumentCount();
	std::string GetUnlocalizedName();
	bool MouseButton(bool down, sf::Vector2i position, sf::Mouse::Button button);
private:
	std::string m_unlocalizedName;
	std::vector<Argument*> m_args;
	sf::Vector2i* m_absolutePosition;
	sf::Vector2i* m_relitivePosition;
	sf::Vector2i* m_stackAbsolute;
	sf::Vector2i* m_stackRelitive;
	std::function<void(unsigned int index, sf::Vector2i mousePosition)>* m_functionSplit;
	std::function<void(unsigned int index, sf::Vector2i mousePosition)>* m_functionContext;
	sf::RectangleShape m_background;
	
	unsigned int m_index;
	unsigned int m_width;
	bool m_selected;
	bool m_wasDown;
};