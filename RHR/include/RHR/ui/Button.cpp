#include "Button.h"

#include <Espresso/Global.h>

Button::Button(std::function<void()>* callback, const sf::Vector2f& size)
	:m_callback(callback), m_broken(false), m_size(size), m_enabled(true) {}

void Button::setEnabled(bool enabled)
{
	m_enabled = enabled;
}

bool Button::getEnabled()
{
	return m_enabled;
}
