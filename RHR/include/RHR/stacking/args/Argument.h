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

	// data of the argument must be stored as string for saving
	void SetData(const std::string& data);
	const std::string& GetData();

	// raw or var
	void SetMode(const BlockArgumentVariableMode& mode);
	const BlockArgumentVariableMode& GetMode();

	// how to interpret the data
	virtual const BlockArgumentType GetType();

	// get width for surrounding argument spacing
	virtual const uint32_t GetWidth();

	// whether or not the argument contains data for storing
	virtual const bool HasData();
	
	// select the argument and enable it
	virtual void Select();

	// pull whether or not the argument is requesting to goto the next argument
	const bool PullNext();

	void UpdateTexture();
	void SelectGlobaly();
	void SetRelitivePosition(const sf::Vector2u& relitivePosition);

	const sf::Vector2i& GetAbsolutePosition();
	const sf::Vector2i& GetRealAbsolutePosition();
	const sf::Vector2u& GetRelitivePosition();
protected:
	virtual void frameUpdateArgument(const double& deltaTime);
	virtual void reloadData();

	sf::Vector2u m_relitivePosition;
	sf::Vector2i m_absolutePosition;
	sf::Vector2i m_realAbsolutePosition;
	sf::Vector2i* m_blockRelitive;
	sf::Vector2i* m_blockAbsolute;

	std::function<void()>* m_functionUpdatePreTexture;
	std::function<void()>* m_functionSelect;

	bool m_next;
	std::string m_data;
	BlockArgumentVariableMode m_mode;
};
