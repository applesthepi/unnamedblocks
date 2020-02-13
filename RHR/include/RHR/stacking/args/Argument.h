#pragma once
#include "ArgumentEssentials.h"

#include <SFML/Graphics.hpp>
#include <functional>

class Argument
{
public:
	Argument(sf::Vector2u relitivePosition);
	virtual ~Argument() = default;
	void SetupInBlock(sf::Vector2i* blockRelitive, sf::Vector2i* blockAbsolute, std::function<void()>* functionUpdatePreTexture, std::function<void()>* functionSelect);

	virtual void Deallocate();
	virtual void Render(sf::RenderTexture* render);
	virtual void FrameUpdate();
	virtual unsigned int GetArgumentRawWidth();
	virtual bool MouseButton(bool down, sf::Vector2i position, sf::Mouse::Button button);
	virtual bool HasData();
	virtual void SetData(std::string data);
	virtual void SetMode(BlockArgumentVariableMode mode);
	virtual void Select();
	virtual void ReInspectData();
	virtual std::string GetData();
	void Update(bool global = false);
	void SetRelitivePosition(sf::Vector2u relitivePosition);
	bool GetNext();
	void UpdateTexture();
	void SelectGlobaly();

	sf::Vector2i GetAbsolutePosition();
	sf::Vector2i GetRealAbsolutePosition();
	sf::Vector2u GetRelitivePosition();
	bool Next;
private:
	sf::Vector2u m_relitivePosition;
	sf::Vector2i m_absolutePosition;
	sf::Vector2i m_realAbsolutePosition;
	sf::Vector2i* m_blockRelitive;
	sf::Vector2i* m_blockAbsolute;

	std::function<void()>* m_functionUpdatePreTexture;
	std::function<void()>* m_functionSelect;
};
