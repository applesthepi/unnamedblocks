#pragma once
#include "RHR/ui/ITransformable.h"




#include "RHR/registries/BlockRegistry.h"
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
	Block(std::string type, BlockRegistry* registry, std::function<void()>* functionUpdatePreTexture, std::function<void()>* functionSelectStack);
	~Block();

	void UpdateShorts(std::function<void()>* functionUpdatePreTexture, std::function<void()>* functionSelectStack);
	void Render(sf::RenderTexture* render, sf::RenderWindow* window);
	void FrameUpdate(bool updateArgs, sf::Vector2f visualOffset, bool global = false);
	void SetArgData(const std::vector<BlockArgumentCaller>& data);
	void SetArgData(const std::vector<std::string> data);
	void RenderToImage(sf::RenderTexture* img, uint64_t idx, const sf::Vector2f& offset);
	void SetupInStack(unsigned int index, sf::Vector2i* stackAbsolute, sf::Vector2i* stackRelitive, std::function<void(unsigned int index, sf::Vector2i mousePosition)>* functionSplit, std::function<void(unsigned int index, sf::Vector2i mousePosition)>* functionContext);
	unsigned int GetWidth();
	Argument* GetArgument(unsigned int index);
	BlockRuntimeReturn GetUsedArgumentsRuntime();
	std::vector<std::string>* GetUsedArgumentSetup();
	unsigned int GetArgumentCount();
	const std::string& GetUnlocalizedName();
	bool MouseButton(bool down, sf::Vector2i position, sf::Mouse::Button button);
	bool GetNext();
	void SelectFirstArgument();
	void PreRender();
	bool IsBounding(const sf::Vector2f& mousePos);
private:
	void UpdateInner();

	std::string m_unlocalizedName;
	std::vector<Argument*> m_args;
	sf::Vector2f m_visualOffset;
	sf::Vector2i* m_absolutePosition;
	sf::Vector2i* m_relitivePosition;
	sf::Vector2i* m_stackAbsolute;
	sf::Vector2i* m_stackRelitive;
	sf::RenderTexture m_preTexture;
	sf::RectangleShape m_preShape;
	std::function<void(unsigned int index, sf::Vector2i mousePosition)>* m_functionSplit;
	std::function<void(unsigned int index, sf::Vector2i mousePosition)>* m_functionContext;
	std::function<void()>* m_functionUpdatePreTexture;
	std::function<void()>* m_functionUpdatePreTextureArgs;
	std::function<void()>* m_functionSelect;
	std::function<void()>* m_functionSelectStack;
	sf::RectangleShape m_background;
	
	unsigned int m_index;
	unsigned int m_width;
	bool m_selected;
	bool m_wasDown;
	bool m_next;
};
