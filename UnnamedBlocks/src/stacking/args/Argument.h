#pragma once

#include <SFML/Graphics.hpp>

class Argument
{
public:
	Argument(sf::Vector2u relitivePosition);

	void SetupInBlock(sf::Vector2i* blockRelitive, sf::Vector2i* blockAbsolute);

	virtual void Deallocate();
	virtual void Render(sf::RenderTexture* render, sf::RenderWindow* window);
	virtual void FrameUpdate(sf::RenderWindow* window);
	virtual unsigned int GetArgumentRawWidth();
	virtual bool MouseButton(bool down, sf::Vector2i position, sf::Mouse::Button button);
	virtual bool HasData();
	virtual void SetData(std::string data);
	virtual std::string GetData();
	void Update(sf::RenderWindow* window, bool global = false);
	void SetRelitivePosition(sf::Vector2u relitivePosition);

	sf::Vector2i GetAbsolutePosition();
	sf::Vector2i GetRealAbsolutePosition();
	sf::Vector2u GetRelitivePosition();
private:
	sf::Vector2u m_relitivePosition;
	sf::Vector2i m_absolutePosition;
	sf::Vector2i m_realAbsolutePosition;
	sf::Vector2i* m_blockRelitive;
	sf::Vector2i* m_blockAbsolute;
};