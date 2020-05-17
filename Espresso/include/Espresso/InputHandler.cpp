#include "InputHandler.h"
#include "Global.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Clipboard.hpp>
#include <chrono>
#include <iostream>
#include <GL/glew.h>
#include <Cappuccino/Logger.h>

void InputHandler::Initialization()
{
	m_textCallbacks = new std::vector<std::function<void(const sf::Event::TextEvent&)>*>();
	m_keyCallbacks = new std::vector<std::function<void(const sf::Event::KeyEvent&)>*>();
}

void InputHandler::FireTextEvent(sf::Event::TextEvent ev) {
	for(std::function<void(const sf::Event::TextEvent&)>* i : *InputHandler::m_textCallbacks) {
		(*i)(ev);
	}
}

void InputHandler::FireKeyEvent(sf::Event::KeyEvent ev) {
	for(std::function<void(const sf::Event::KeyEvent&)>* i : *InputHandler::m_keyCallbacks) {
		(*i)(ev);
	}
}

void InputHandler::RunTextProccess(std::string* text, uint64_t* locHigh, uint64_t* loc, std::function<void()>* enter, std::function<void()>* escape, const sf::Event::KeyEvent& ev)
{
	std::function<void(const char& ch)> insertChar = [&](const char& ch)
	{
		std::string nt;

		for (uint64_t i = 0; i < std::min(*locHigh, *loc); i++)
			nt += text->at(i);

		nt += ch;

		for (uint64_t i = std::max(*locHigh, *loc); i < text->size(); i++)
			nt += text->at(i);

		*text = nt;

		*loc = std::min(*locHigh, *loc) + 1;
		*locHigh = *loc;
	};

	std::function<void(const std::string& ch)> insertString = [&](const std::string& ch)
	{
		std::string nt;

		for (uint64_t i = 0; i < std::min(*locHigh, *loc); i++)
			nt += text->at(i);

		nt += ch;

		for (uint64_t i = std::max(*locHigh, *loc); i < text->size(); i++)
			nt += text->at(i);

		*text = nt;

		*loc = std::min(*locHigh, *loc) + ch.length();
		*locHigh = *loc;
	};

	if (ev.code == sf::Keyboard::Key::Left)
	{
		if (*loc > 0)
		{
			if (ev.control)
			{
				if (text->at(*loc - 1) == ' ')
				{
					for (int64_t i = *loc - 1; i >= 0; i--)
					{
						if (text->at(i) != ' ')
							break;

						(*loc)--;
					}
				}
				else if (text->at(*loc - 1) == '	')
				{
					for (int64_t i = *loc - 1; i >= 0; i--)
					{
						if (text->at(i) != '	')
							break;

						(*loc)--;
					}
				}
				else
				{
					uint64_t oLoc = *loc;
					for (int64_t i = *loc - 1; i >= 0; i--)
					{
						char re = text->at(i);
						if (i != oLoc - 1 && (
							text->at(i) == ' ' || text->at(i) == '`' || text->at(i) == '~' || text->at(i) == '!' || text->at(i) == '@' || text->at(i) == '#' || text->at(i) == '$' ||
							text->at(i) == '%' || text->at(i) == '^' || text->at(i) == '&' || text->at(i) == '*' || text->at(i) == '(' || text->at(i) == ')' || text->at(i) == '-' ||
							text->at(i) == '=' || text->at(i) == '+' || text->at(i) == '[' || text->at(i) == ']' || text->at(i) == '{' || text->at(i) == '}' || text->at(i) == '\\' ||
							text->at(i) == '|' || text->at(i) == ';' || text->at(i) == ':' || text->at(i) == '\'' || text->at(i) == '"' || text->at(i) == ',' || text->at(i) == '.' ||
							text->at(i) == '<' || text->at(i) == '>' || text->at(i) == '/' || text->at(i) == '?' || text->at(i) == '	'))
							break;

						(*loc)--;
					}
				}
			}
			else
				(*loc)--;
		}

		if (!ev.shift)
			*locHigh = *loc;
	}
	else if (ev.code == sf::Keyboard::Key::Right)
	{
		if (*loc < text->size())
		{
			if (ev.control)
			{
				if (text->at(*loc) == ' ')
				{
					for (uint64_t i = *loc; i < text->size(); i++)
					{
						if (text->at(i) != ' ')
							break;

						(*loc)++;
					}
				}
				else if (text->at(*loc) == '	')
				{
					for (uint64_t i = *loc; i < text->size(); i++)
					{
						if (text->at(i) != '	')
							break;

						(*loc)++;
					}
				}
				else
				{
					uint64_t oLoc = *loc;
					for (uint64_t i = *loc; i < text->size(); i++)
					{
						if (i != oLoc && (
							text->at(i) == ' ' || text->at(i) == '`' || text->at(i) == '~' || text->at(i) == '!' || text->at(i) == '@' || text->at(i) == '#' || text->at(i) == '$' ||
							text->at(i) == '%' || text->at(i) == '^' || text->at(i) == '&' || text->at(i) == '*' || text->at(i) == '(' || text->at(i) == ')' || text->at(i) == '-' ||
							text->at(i) == '=' || text->at(i) == '+' || text->at(i) == '[' || text->at(i) == ']' || text->at(i) == '{' || text->at(i) == '}' || text->at(i) == '\\' ||
							text->at(i) == '|' || text->at(i) == ';' || text->at(i) == ':' || text->at(i) == '\'' || text->at(i) == '"' || text->at(i) == ',' || text->at(i) == '.' ||
							text->at(i) == '<' || text->at(i) == '>' || text->at(i) == '/' || text->at(i) == '?' || text->at(i) == '	'))
							break;

						(*loc)++;
					}
				}
			}
			else
				(*loc)++;
		}

		if (!ev.shift)
			*locHigh = *loc;
	}
	else if (ev.code >= 0 && ev.code <= 25)
	{
		if (ev.control && ev.code == sf::Keyboard::Key::A)
		{
			*locHigh = 0;
			*loc = text->size();
		}
		else if (ev.control && ev.code == sf::Keyboard::Key::C)
			sf::Clipboard::setString(text->substr(std::min(*locHigh, *loc), std::max(*locHigh, *loc) - std::min(*locHigh, *loc)));
		else if (ev.control && ev.code == sf::Keyboard::Key::V)
			insertString(sf::Clipboard::getString());
		else
		{
			if (ev.shift)
				insertChar(ev.code + 65);
			else
				insertChar(ev.code + 97);
		}
	}
	else if (ev.code == sf::Keyboard::Key::Backspace)
	{
		if (*locHigh != *loc)
		{
			std::string nt;

			for (uint64_t i = 0; i < std::min(*locHigh, *loc); i++)
				nt += text->at(i);

			for (uint64_t i = std::max(*locHigh, *loc); i < text->size(); i++)
				nt += text->at(i);

			*text = nt;

			*loc = std::min(*locHigh, *loc);
			*locHigh = *loc;
		}
		else if (*loc > 0)
		{
			std::string nt;

			for (uint64_t i = 0; i < *loc - 1; i++)
				nt += text->at(i);

			for (uint64_t i = *loc; i < text->size(); i++)
				nt += text->at(i);

			*text = nt;

			(*loc)--;
			*locHigh = *loc;
		}
	}
	else if (ev.code == sf::Keyboard::Key::Delete)
	{
		if (*locHigh != *loc)
		{
			std::string nt;

			for (uint64_t i = 0; i < std::min(*locHigh, *loc); i++)
				nt += text->at(i);

			for (uint64_t i = std::max(*locHigh, *loc); i < text->size(); i++)
				nt += text->at(i);

			*text = nt;

			*loc = std::min(*locHigh, *loc);
			*locHigh = *loc;
		}
		else if (*loc < text->size())
		{
			std::string nt;

			for (uint64_t i = 0; i < *loc; i++)
				nt += text->at(i);

			for (uint64_t i = *loc + 1; i < text->size(); i++)
				nt += text->at(i);

			*text = nt;
		}
	}
	else if (ev.code == sf::Keyboard::Key::Escape)
	{
		if (escape != nullptr)
			(*escape)();
	}
	else if (ev.code == sf::Keyboard::Key::Enter)
	{
		if (enter != nullptr)
			(*enter)();
	}
	else if (ev.code >= 26 && ev.code <= 35)
	{
		if (ev.shift)
		{
			if (ev.code == sf::Keyboard::Key::Num0)
				insertChar(')');
			else if (ev.code == sf::Keyboard::Key::Num1)
				insertChar('!');
			else if (ev.code == sf::Keyboard::Key::Num2)
				insertChar('@');
			else if (ev.code == sf::Keyboard::Key::Num3)
				insertChar('#');
			else if (ev.code == sf::Keyboard::Key::Num4)
				insertChar('$');
			else if (ev.code == sf::Keyboard::Key::Num5)
				insertChar('%');
			else if (ev.code == sf::Keyboard::Key::Num6)
				insertChar('^');
			else if (ev.code == sf::Keyboard::Key::Num7)
				insertChar('&');
			else if (ev.code == sf::Keyboard::Key::Num8)
				insertChar('*');
			else if (ev.code == sf::Keyboard::Key::Num9)
				insertChar('(');
		}
		else
			insertChar(ev.code + 22);
	}
	else if (ev.code == sf::Keyboard::Key::Space)
		insertChar(' ');
	else if (ev.code == sf::Keyboard::Key::Tab)
		insertChar('	');
	else if (ev.code == sf::Keyboard::Key::Tilde)
	{
		if (ev.shift)
			insertChar('~');
		else
			insertChar('`');
	}
	else if (ev.code == sf::Keyboard::Key::Hyphen)
	{
		if (ev.shift)
			insertChar('_');
		else
			insertChar('-');
	}
	else if (ev.code == sf::Keyboard::Key::Equal)
	{
		if (ev.shift)
			insertChar('+');
		else
			insertChar('=');
	}
	else if (ev.code == sf::Keyboard::Key::LBracket)
	{
		if (ev.shift)
			insertChar('{');
		else
			insertChar('[');
	}
	else if (ev.code == sf::Keyboard::Key::RBracket)
	{
		if (ev.shift)
			insertChar(']');
		else
			insertChar('}');
	}
	else if (ev.code == sf::Keyboard::Key::BackSlash)
	{
		if (ev.shift)
			insertChar('|');
		else
			insertChar('\\');
	}
	else if (ev.code == sf::Keyboard::Key::SemiColon)
	{
		if (ev.shift)
			insertChar(':');
		else
			insertChar(';');
	}
	else if (ev.code == sf::Keyboard::Key::Quote)
	{
		if (ev.shift)
			insertChar('"');
		else
			insertChar('\'');
	}
	else if (ev.code == sf::Keyboard::Key::Comma)
	{
		if (ev.shift)
			insertChar('<');
		else
			insertChar(',');
	}
	else if (ev.code == sf::Keyboard::Key::Period)
	{
		if (ev.shift)
			insertChar('>');
		else
			insertChar('.');
	}
	else if (ev.code == sf::Keyboard::Key::Slash)
	{
		if (ev.shift)
			insertChar('?');
		else
			insertChar('/');
	}
	else if (ev.code >= 75 && ev.code <= 84)
		insertChar(ev.code - 27);
	else if (ev.code == 67)
		insertChar('+');
	else if (ev.code == 68)
		insertChar('-');
	else if (ev.code == 69)
		insertChar('*');
	else if (ev.code == 70)
		insertChar('/');
}

void InputHandler::RunNumberProccess(std::string* text, uint64_t* locHigh, uint64_t* loc, std::function<void()>* enter, std::function<void()>* escape, const sf::Event::KeyEvent& ev)
{
	std::function<void(const char& ch)> insertChar = [&](const char& ch)
	{
		std::string nt;

		for (uint64_t i = 0; i < std::min(*locHigh, *loc); i++)
			nt += text->at(i);

		nt += ch;

		for (uint64_t i = std::max(*locHigh, *loc); i < text->size(); i++)
			nt += text->at(i);

		*text = nt;

		*loc = std::min(*locHigh, *loc) + 1;
		*locHigh = *loc;
	};

	std::function<void(const std::string& ch)> insertString = [&](const std::string& ch)
	{
		// whitelist chars

		std::string nCh;
		nCh.reserve(ch.length());
		bool hasDecimal = false;

		for (uint64_t i = 0; i < ch.length(); i++)
		{
			if (i == 0 && ch[i] == '-')
				nCh += ch[i];
			else if (ch[i] >= 48 && ch[i] <= 57)
				nCh += ch[i];
			else if (ch[i] == '.')
			{
				if (hasDecimal)
					continue;
				
				hasDecimal = true;
				nCh += ch[i];
			}
		}

		std::string nt;

		for (uint64_t i = 0; i < std::min(*locHigh, *loc); i++)
			nt += text->at(i);

		nt += ch;

		for (uint64_t i = std::max(*locHigh, *loc); i < text->size(); i++)
			nt += text->at(i);

		*text = nt;

		*loc = std::min(*locHigh, *loc) + ch.length();
		*locHigh = *loc;
	};

	if (ev.code == sf::Keyboard::Key::Left)
	{
		if (*loc > 0)
		{
			if (ev.control)
			{
				if (text->at(*loc - 1) == ' ')
				{
					for (int64_t i = *loc - 1; i >= 0; i--)
					{
						if (text->at(i) != ' ')
							break;

						(*loc)--;
					}
				}
				else if (text->at(*loc - 1) == '	')
				{
					for (int64_t i = *loc - 1; i >= 0; i--)
					{
						if (text->at(i) != '	')
							break;

						(*loc)--;
					}
				}
				else
				{
					uint64_t oLoc = *loc;
					for (int64_t i = *loc - 1; i >= 0; i--)
					{
						char re = text->at(i);
						if (i != oLoc - 1 && (
							text->at(i) == ' ' || text->at(i) == '`' || text->at(i) == '~' || text->at(i) == '!' || text->at(i) == '@' || text->at(i) == '#' || text->at(i) == '$' ||
							text->at(i) == '%' || text->at(i) == '^' || text->at(i) == '&' || text->at(i) == '*' || text->at(i) == '(' || text->at(i) == ')' || text->at(i) == '-' ||
							text->at(i) == '=' || text->at(i) == '+' || text->at(i) == '[' || text->at(i) == ']' || text->at(i) == '{' || text->at(i) == '}' || text->at(i) == '\\' ||
							text->at(i) == '|' || text->at(i) == ';' || text->at(i) == ':' || text->at(i) == '\'' || text->at(i) == '"' || text->at(i) == ',' || text->at(i) == '.' ||
							text->at(i) == '<' || text->at(i) == '>' || text->at(i) == '/' || text->at(i) == '?' || text->at(i) == '	'))
							break;

						(*loc)--;
					}
				}
			}
			else
				(*loc)--;
		}

		if (!ev.shift)
			*locHigh = *loc;
	}
	else if (ev.code == sf::Keyboard::Key::Right)
	{
		if (*loc < text->size())
		{
			if (ev.control)
			{
				if (text->at(*loc) == ' ')
				{
					for (uint64_t i = *loc; i < text->size(); i++)
					{
						if (text->at(i) != ' ')
							break;

						(*loc)++;
					}
				}
				else if (text->at(*loc) == '	')
				{
					for (uint64_t i = *loc; i < text->size(); i++)
					{
						if (text->at(i) != '	')
							break;

						(*loc)++;
					}
				}
				else
				{
					uint64_t oLoc = *loc;
					for (uint64_t i = *loc; i < text->size(); i++)
					{
						if (i != oLoc && (
							text->at(i) == ' ' || text->at(i) == '`' || text->at(i) == '~' || text->at(i) == '!' || text->at(i) == '@' || text->at(i) == '#' || text->at(i) == '$' ||
							text->at(i) == '%' || text->at(i) == '^' || text->at(i) == '&' || text->at(i) == '*' || text->at(i) == '(' || text->at(i) == ')' || text->at(i) == '-' ||
							text->at(i) == '=' || text->at(i) == '+' || text->at(i) == '[' || text->at(i) == ']' || text->at(i) == '{' || text->at(i) == '}' || text->at(i) == '\\' ||
							text->at(i) == '|' || text->at(i) == ';' || text->at(i) == ':' || text->at(i) == '\'' || text->at(i) == '"' || text->at(i) == ',' || text->at(i) == '.' ||
							text->at(i) == '<' || text->at(i) == '>' || text->at(i) == '/' || text->at(i) == '?' || text->at(i) == '	'))
							break;

						(*loc)++;
					}
				}
			}
			else
				(*loc)++;
		}

		if (!ev.shift)
			*locHigh = *loc;
	}
	else if (ev.control && ev.code == sf::Keyboard::Key::A)
	{
		*locHigh = 0;
		*loc = text->size();
	}
	else if (ev.code == sf::Keyboard::Key::Backspace)
	{
		if (*locHigh != *loc)
		{
			std::string nt;

			for (uint64_t i = 0; i < std::min(*locHigh, *loc); i++)
				nt += text->at(i);

			for (uint64_t i = std::max(*locHigh, *loc); i < text->size(); i++)
				nt += text->at(i);

			*text = nt;

			*loc = std::min(*locHigh, *loc);
			*locHigh = *loc;
		}
		else if (*loc > 0)
		{
			std::string nt;

			for (uint64_t i = 0; i < *loc - 1; i++)
				nt += text->at(i);

			for (uint64_t i = *loc; i < text->size(); i++)
				nt += text->at(i);

			*text = nt;

			(*loc)--;
			*locHigh = *loc;
		}
	}
	else if (ev.code == sf::Keyboard::Key::Delete)
	{
		if (*locHigh != *loc)
		{
			std::string nt;

			for (uint64_t i = 0; i < std::min(*locHigh, *loc); i++)
				nt += text->at(i);

			for (uint64_t i = std::max(*locHigh, *loc); i < text->size(); i++)
				nt += text->at(i);

			*text = nt;

			*loc = std::min(*locHigh, *loc);
			*locHigh = *loc;
		}
		else if (*loc < text->size())
		{
			std::string nt;

			for (uint64_t i = 0; i < *loc; i++)
				nt += text->at(i);

			for (uint64_t i = *loc + 1; i < text->size(); i++)
				nt += text->at(i);

			*text = nt;
		}
	}
	else if (ev.code == sf::Keyboard::Key::Escape)
	{
		if (escape != nullptr)
			(*escape)();
	}
	else if (ev.code == sf::Keyboard::Key::Enter)
	{
		if (enter != nullptr)
			(*enter)();
	}
	else if (ev.code >= 26 && ev.code <= 35 && !ev.shift)
		insertChar(ev.code + 22);
	else if (ev.code == sf::Keyboard::Key::Period && !ev.shift)
	{
		bool found = false;

		for (uint64_t i = 0; i < text->length(); i++)
		{
			if (text->at(i) == '.')
			{
				found = true;
				break;
			}
		}
		
		if (!found)
			insertChar('.');
	}
	else if (ev.code == sf::Keyboard::Key::Hyphen && !ev.shift && (*loc == 0 || *locHigh == 0))
	{
		if (text->at(0) != '-')
			insertChar('-');
	}
	else if (ev.code >= 75 && ev.code <= 84)
		insertChar(ev.code - 27);
	else if (ev.control && ev.code == sf::Keyboard::Key::C)
		sf::Clipboard::setString(text->substr(std::min(*locHigh, *loc), std::max(*locHigh, *loc) - std::min(*locHigh, *loc)));
	else if (ev.control && ev.code == sf::Keyboard::Key::V)
		insertString(sf::Clipboard::getString());
}

bool InputHandler::RunMouseProccess(sf::Text* text, uint64_t* locHigh, uint64_t* loc, bool* isDown, const bool& down, const sf::Vector2i& pos, const uint64_t& yOverride)
{
	uint64_t yHeight = yOverride;

	if (yHeight == 0)
		yHeight = text->getLocalBounds().height;

	if (pos.x >= text->getPosition().x && pos.x <= text->getPosition().x + text->getLocalBounds().width)
	{
		if (down && pos.y >= text->getPosition().y && pos.y <= text->getPosition().y + yHeight)
		{
			*isDown = true;

			for (uint64_t i = 0; i < text->getString().getSize() + 1; i++)
			{
				uint64_t ix = text->getPosition().x;

				if (i == text->getString().getSize())
				{
					*loc = i;
					*locHigh = i;
					break;
				}
				else
				{
					ix += sf::Text(text->getString().substring(0, i), *Global::Font, 16).getLocalBounds().width;
					ix += sf::Text(text->getString().substring(i, 1), *Global::Font, 16).getLocalBounds().width / 2.0;
				}

				if (pos.x <= ix)
				{
					*loc = i;
					*locHigh = i;
					break;
				}
			}
		}
		else
			*isDown = false;

		return pos.y >= text->getPosition().y && pos.y <= text->getPosition().y + yHeight;
	}

	return false;
}

bool InputHandler::RunMouseProccess(TextSystem& system, const sf::Vector2i& tPos, const sf::Vector2u& tSize, const bool& down, const sf::Vector2i& pos, const uint16_t& fontSize, const int64_t& vanityOffset)
{
	if (pos.x >= tPos.x && pos.x <= tPos.x + tSize.x)
	{
		if (!down)
			*system.IsDown = false;

		if (down && pos.y >= tPos.y && pos.y <= tPos.y + tSize.y)
		{
			*system.IsDown = true;

			for (uint64_t i = 0; i < system.Text->length() + 1; i++)
			{
				uint64_t ix = tPos.x + vanityOffset + Global::BlockBorder;

				if (i == system.Text->length())
				{
					*system.Loc = i;
					*system.LocHigh = i;
					break;
				}
				else
				{
					ix += sf::Text(system.Text->substr(0, i), *Global::Font, fontSize).getLocalBounds().width;
					ix += sf::Text(system.Text->substr(i, 1), *Global::Font, fontSize).getLocalBounds().width / 2.0;
				}

				if (pos.x <= ix)
				{
					*system.Loc = i;
					*system.LocHigh = i;
					break;
				}
			}
		}

		return pos.y >= tPos.y && pos.y <= tPos.y + tSize.y;
	}

	return false;
}

void InputHandler::RunMouseProccessFrame(sf::Text* text, uint64_t* loc, bool* isDown, const sf::Vector2i& pos, const uint64_t& yOverride)
{
	if (*isDown)
	{
		uint64_t yHeight = yOverride;

		if (yHeight == 0)
			yHeight = text->getLocalBounds().height;

		if (pos.x >= text->getPosition().x && pos.x <= text->getPosition().x + text->getLocalBounds().width)
		{
			for (uint64_t i = 0; i < text->getString().getSize() + 1; i++)
			{
				uint64_t ix = text->getPosition().x;

				if (i == text->getString().getSize())
				{
					*loc = i;
					break;
				}
				else
				{
					ix += sf::Text(text->getString().substring(0, i), *Global::Font, 16).getLocalBounds().width;
					ix += sf::Text(text->getString().substring(i, 1), *Global::Font, 16).getLocalBounds().width / 2.0;
				}

				if (pos.x <= ix)
				{
					*loc = i;
					break;
				}
			}
		}
	}
}

void InputHandler::RunMouseProccessFrame(std::string* text, const sf::Vector2i& tPos, const sf::Vector2u& tSize, uint64_t* loc, bool* isDown, const sf::Vector2i& pos, const uint16_t& fontSize, const int64_t& vanityOffset)
{
	if (*isDown)
	{
		if (pos.x >= tPos.x && pos.x <= tPos.x + tSize.x)
		{
			for (uint64_t i = 0; i < text->length() + 1; i++)
			{
				uint64_t ix = tPos.x + vanityOffset + Global::BlockBorder;

				if (i == text->length())
				{
					*loc = i;
					break;
				}
				else
				{
					ix += sf::Text(text->substr(0, i), *Global::Font, fontSize).getLocalBounds().width;
					ix += sf::Text(text->substr(i, 1), *Global::Font, fontSize).getLocalBounds().width / 2.0;
				}

				if (pos.x <= ix)
				{
					*loc = i;
					break;
				}
			}
		}
	}
}

void InputHandler::RegisterKeyCallback(std::function<void(const sf::Event::KeyEvent&)>* fun)
{
	m_keyCallbacks->push_back(fun);
}

void InputHandler::UnregisterKeyCallback(std::function<void(const sf::Event::KeyEvent&)>* fun)
{
	for (uint64_t i = 0; i < m_keyCallbacks->size(); i++)
	{
		if ((*m_keyCallbacks)[i] == fun)
		{
			m_keyCallbacks->erase(m_keyCallbacks->begin() + static_cast<int32_t>(i));
			return;
		}
	}

	Logger::Error("failed to unregister key callback");
}

void InputHandler::RegisterTextCallback(std::function<void(const sf::Event::TextEvent&)>* fun)
{
	m_textCallbacks->push_back(fun);
}

void InputHandler::UnregisterTextCallback(std::function<void(const sf::Event::TextEvent&)>* fun)
{
	for (uint64_t i = 0; i < m_textCallbacks->size(); i++)
	{
		if ((*m_textCallbacks)[i] == fun)
		{
			m_textCallbacks->erase(m_textCallbacks->begin() + static_cast<int32_t>(i));
			return;
		}
	}

	Logger::Error("failed to unregister text callback");
}

std::vector<std::function<void(const sf::Event::TextEvent&)>*>* InputHandler::m_textCallbacks;

std::vector<std::function<void(const sf::Event::KeyEvent&)>*>* InputHandler::m_keyCallbacks;
