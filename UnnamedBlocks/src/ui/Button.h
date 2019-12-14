#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

enum ButtonMode
{
	Unconfigured, Color, Text, Image
};

class Button
{
public:
	Button(sf::Vector2i position, sf::Vector2u size, std::function<void()>* functionCallback);

	void SetButtonModeColor(sf::Color color);
	void SetButtonModeText(std::string text, sf::Color backgroundColor, unsigned short charSize);
	void SetButtonModeImage(std::string path);

	void FrameUpdate(sf::RenderWindow* window);
	void Render(sf::RenderWindow* window);

	void SetSize(sf::Vector2u size);
	void SetPosition(sf::Vector2i position);
private:
	sf::Vector2i m_position;
	sf::Vector2u m_size;
	std::function<void()>* m_functionCallback;
	ButtonMode m_mode;
	bool m_wasDown;

	sf::RectangleShape* m_modeColor;
	sf::Text* m_modeText;
	sf::Texture* m_modeImageTexture;
	sf::Sprite* m_modeImageSprite;
};