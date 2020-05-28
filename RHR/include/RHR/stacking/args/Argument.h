#pragma once
#include "RHR/ui/MouseUpdatable.h"
#include "RHR/ui/UBRenderable.h"

#include <SFML/Graphics.hpp>
#include <functional>
#include <Cappuccino/block/ModBlock.h>

class Argument : public UBRenderable, public MouseUpdatable
{
public:
	Argument(const sf::Vector2u& relitivePosition);
	void SetupInBlock(sf::Vector2i* blockRelitive, sf::Vector2i* blockAbsolute, std::function<void()>* functionUpdatePreTexture, std::function<void()>* functionSelect);

	void preFrameUpdate(const bool& global);
	void frameUpdate(const double& deltaTime) override;

	virtual const bool HasData();
	virtual void SetData(const std::string& data);
	virtual const std::string& GetData();
	virtual const std::string& GetDataRaw();
	virtual void ReInspectData();

	virtual unsigned int GetArgumentRawWidth();
	virtual void SetMode(const BlockArgumentVariableMode& mode);
	virtual void Select();
	virtual const BlockArgumentVariableMode GetMode();
	virtual const BlockArgumentType GetType();
	void SetRelitivePosition(const sf::Vector2u& relitivePosition);
	const bool PullNext();
	void UpdateTexture();
	void SelectGlobaly();

	const sf::Vector2i& GetAbsolutePosition();
	const sf::Vector2i& GetRealAbsolutePosition();
	const sf::Vector2u& GetRelitivePosition();
protected:
	virtual void frameUpdateArgument(const double& deltaTime);

	sf::Vector2u m_relitivePosition;
	sf::Vector2i m_absolutePosition;
	sf::Vector2i m_realAbsolutePosition;
	sf::Vector2i* m_blockRelitive;
	sf::Vector2i* m_blockAbsolute;

	std::function<void()>* m_functionUpdatePreTexture;
	std::function<void()>* m_functionSelect;

	bool m_next;
};
