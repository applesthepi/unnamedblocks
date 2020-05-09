#include "InputHandler.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <chrono>
#include <iostream>
#include <GL/glew.h>

void InputHandler::Initialization()
{
	text_callbacks = new std::vector<std::function<void(const sf::Event::TextEvent&)>*>();
	key_callbacks = new std::vector<std::function<void(const sf::Event::KeyEvent&)>*>();
}

void InputHandler::FireTextEvent(sf::Event::TextEvent ev) {
	for(std::function<void(const sf::Event::TextEvent&)>* i : *InputHandler::text_callbacks) {
		(*i)(ev);
	}
}

void InputHandler::FireKeyEvent(sf::Event::KeyEvent ev) {
	for(std::function<void(const sf::Event::KeyEvent&)>* i : *InputHandler::key_callbacks) {
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

	if (ev.code == sf::Keyboard::Key::Left)
	{
		if (*loc > 0)
			(*loc)--;

		if (!ev.shift)
			*locHigh = *loc;
	}
	else if (ev.code == sf::Keyboard::Key::Right)
	{
		if (*loc < text->size())
			(*loc)++;

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
		(*escape)();
	else if (ev.code == sf::Keyboard::Key::Enter)
		(*enter)();
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
		if (ev.shift)
			insertChar('~');
		else
			insertChar('`');
	else if (ev.code == sf::Keyboard::Key::Hyphen)
		if (ev.shift)
			insertChar('_');
		else
			insertChar('-');
	else if (ev.code == sf::Keyboard::Key::Equal)
		if (ev.shift)
			insertChar('+');
		else
			insertChar('=');
	else if (ev.code == sf::Keyboard::Key::LBracket)
		if (ev.shift)
			insertChar('{');
		else
			insertChar('[');
	else if (ev.code == sf::Keyboard::Key::RBracket)
		if (ev.shift)
			insertChar(']');
		else
			insertChar('}');
	else if (ev.code == sf::Keyboard::Key::BackSlash)
		if (ev.shift)
			insertChar('|');
		else
			insertChar('\\');
	else if (ev.code == sf::Keyboard::Key::SemiColon)
		if (ev.shift)
			insertChar(':');
		else
			insertChar(';');
	else if (ev.code == sf::Keyboard::Key::Quote)
		if (ev.shift)
			insertChar('"');
		else
			insertChar('\'');
	else if (ev.code == sf::Keyboard::Key::Comma)
		if (ev.shift)
			insertChar('<');
		else
			insertChar(',');
	else if (ev.code == sf::Keyboard::Key::Period)
		if (ev.shift)
			insertChar('>');
		else
			insertChar('.');
	else if (ev.code == sf::Keyboard::Key::Slash)
		if (ev.shift)
			insertChar('?');
		else
			insertChar('/');
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

	if (ev.code == sf::Keyboard::Key::Left)
	{
		if (*loc > 0)
			(*loc)--;

		if (!ev.shift)
			*locHigh = *loc;
	}
	else if (ev.code == sf::Keyboard::Key::Right)
	{
		if (*loc < text->size())
			(*loc)++;

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
		(*escape)();
	else if (ev.code == sf::Keyboard::Key::Enter)
		(*enter)();
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
	else if (ev.code >= 75 && ev.code <= 84)
		insertChar(ev.code - 27);
}

void InputHandler::RegisterTextCallback(std::function<void(const sf::Event::TextEvent&)>* fun)
{
	text_callbacks->push_back(fun);
}

void InputHandler::RegisterKeyCallback(std::function<void(const sf::Event::KeyEvent&)>* fun)
{
	key_callbacks->push_back(fun);
}

void InputHandler::UnregisterTextCallback(std::function<void(const sf::Event::TextEvent&)>* fun)
{
	for (uint64_t i = 0; i < text_callbacks->size(); i++)
	{
		if ((*text_callbacks)[i] == fun)
		{
			text_callbacks->erase(text_callbacks->begin() + static_cast<int32_t>(i));
			return;
		}
	}
}

void InputHandler::UnregisterKeyCallback(std::function<void(const sf::Event::KeyEvent&)>* fun)
{
	for (uint64_t i = 0; i < key_callbacks->size(); i++)
	{
		if ((*key_callbacks)[i] == fun)
		{
			key_callbacks->erase(key_callbacks->begin() + static_cast<int32_t>(i));
			return;
		}
	}
}

std::vector<std::function<void(const sf::Event::TextEvent&)>*>* InputHandler::text_callbacks;

std::vector<std::function<void(const sf::Event::KeyEvent&)>*>* InputHandler::key_callbacks;
