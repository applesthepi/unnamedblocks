#pragma once
#include "IUI.h"
#include "ITransformable.h"

#include <SFML/Graphics.hpp>
#include <functional>
#include <Espresso/Global.h>

#define HOVOR_SHADE_HARD 0.7
#define HOVOR_SHADE_LIGHT 0.85

class Button : public IUI, public ITransformable
{
public:
	Button(std::function<void()>* callback, const sf::Vector2f& size);

	void setEnabled(bool enabled);
	bool getEnabled();
protected:
	std::function<void()>* m_callback;
	bool m_broken;
private:
	sf::Vector2f m_size;
	bool m_enabled;
};