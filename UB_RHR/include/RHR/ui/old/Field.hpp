#pragma once
#include "IUI.h"
#include "ITransformable.h"

#include <SFML/Graphics.hpp>
#include <string>
#include <functional>

enum FieldType
{
	TEXT, NUMBER
};

class Field : public IUI, public ITransformable
{
public:
	Field(const std::string& text, bool canSwitch, const FieldType& fieldType);

	void SetText(const std::string& text);
	void SetEnabled(bool enabled);
	void SetVariableMode(bool variableMode);
	void SetCalculationOffset(const sf::Vector2f& calculationOffset);
	void SetTab(std::function<void()>* tab);
	void SelectAll();

	bool GetSelected();
	uint32_t GetWidth();
	const std::string& GetText();

	void frameUpdate(double deltaTime) override;
	bool mouseButton(bool down, const sf::Vector2i& position, const sf::Mouse::Button& button) override;
protected:
	void draw(sf::RenderTarget& target, sf::RenderStates state) const override;

	std::string m_text;
	sf::Text m_input;
	sf::RectangleShape m_inputBackground;
	sf::RectangleShape m_inputLocHigh;
	sf::RectangleShape m_inputLoc;

	sf::Vector2f m_calculationOffset;
	FieldType m_fieldType;

	uint64_t m_textLoc;
	uint64_t m_textLocHigh;

	bool m_enabled;
	bool m_wasDown;
	bool m_selected;
	bool m_variableMode;
	bool m_canSwitch;

	std::function<void(const sf::Event::KeyEvent&)> m_textCallback;
	std::function<void()>* m_tab;
};
