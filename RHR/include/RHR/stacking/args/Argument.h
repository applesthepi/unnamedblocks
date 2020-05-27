#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <Cappuccino/block/ModBlock.h>

class Argument
{
public:
	Argument(const sf::Vector2u& relitivePosition);
	virtual ~Argument() = default;
	void SetupInBlock(sf::Vector2i* blockRelitive, sf::Vector2i* blockAbsolute, std::function<void()>* functionUpdatePreTexture, std::function<void()>* functionSelect);

	virtual void Deallocate();
	virtual void Render(sf::RenderTexture* render);
	virtual void FrameUpdate();
	virtual unsigned int GetArgumentRawWidth();
	virtual const bool MouseButton(const bool& down, const sf::Vector2i& position, const sf::Mouse::Button& button);
	virtual const bool HasData();
	virtual void SetData(const std::string& data);
	virtual void SetMode(const BlockArgumentVariableMode& mode);
	virtual void Select();
	virtual void ReInspectData();
	virtual const std::string& GetData();
	virtual const std::string& GetDataRaw();
	virtual const BlockArgumentVariableMode GetMode();
	virtual const BlockArgumentType GetType();
	void Update(bool global = false);
	void SetRelitivePosition(const sf::Vector2u& relitivePosition);
	const bool GetNext();
	void UpdateTexture();
	void SelectGlobaly();

	const sf::Vector2i& GetAbsolutePosition();
	const sf::Vector2i& GetRealAbsolutePosition();
	const sf::Vector2u& GetRelitivePosition();
	bool Next;
protected:
	sf::Vector2u m_relitivePosition;
	sf::Vector2i m_absolutePosition;
	sf::Vector2i m_realAbsolutePosition;
	sf::Vector2i* m_blockRelitive;
	sf::Vector2i* m_blockAbsolute;

	std::function<void()>* m_functionUpdatePreTexture;
	std::function<void()>* m_functionSelect;
};
