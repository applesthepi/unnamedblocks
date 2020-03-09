#pragma once

#include <string>
#include <SFML/Graphics.hpp>

class ModCatagory
{
public:
	ModCatagory();

	void SetColor(const sf::Color& color);
	void SetUnlocalizedName(const std::string& unlocalizedName);
	void SetDisplayName(const std::string& displayName);

	const sf::Color& GetColor() const;
	const std::string& GetUnlocalizedName() const;
	const std::string& GetDisplayName() const;
private:
	sf::Color m_color;
	std::string m_unlocalizedName;
	std::string m_displayName;
};

