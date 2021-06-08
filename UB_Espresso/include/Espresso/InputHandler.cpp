#include "InputHandler.hpp"

void InputHandler::Initialization()
{
	m_MouseWasDown = false;
	m_ShiftDown = false;

	m_MousePosition = { 0, 0 };
}

// void InputHandler::RunTextProccess(std::string* text, uint64_t* locHigh, uint64_t* loc, std::function<void()>* enter, std::function<void()>* escape, const sf::Event::KeyEvent& ev)
// {
// 	std::function<void(const char& ch)> insertChar = [&](const char& ch)
// 	{
// 		std::string nt;

// 		for (uint64_t i = 0; i < std::min(*locHigh, *loc); i++)
// 			nt += text->at(i);

// 		nt += ch;

// 		for (uint64_t i = std::max(*locHigh, *loc); i < text->size(); i++)
// 			nt += text->at(i);

// 		*text = nt;

// 		*loc = std::min(*locHigh, *loc) + 1;
// 		*locHigh = *loc;
// 	};

// 	std::function<void(const std::string& ch)> insertString = [&](const std::string& ch)
// 	{
// 		std::string nt;

// 		for (uint64_t i = 0; i < std::min(*locHigh, *loc); i++)
// 			nt += text->at(i);

// 		nt += ch;

// 		for (uint64_t i = std::max(*locHigh, *loc); i < text->size(); i++)
// 			nt += text->at(i);

// 		*text = nt;

// 		*loc = std::min(*locHigh, *loc) + ch.length();
// 		*locHigh = *loc;
// 	};

// 	if (ev.code == sf::Keyboard::Key::Left)
// 	{
// 		if (*loc > 0)
// 		{
// 			if (ev.control)
// 			{
// 				if (text->at(*loc - 1) == ' ')
// 				{
// 					for (int64_t i = *loc - 1; i >= 0; i--)
// 					{
// 						if (text->at(i) != ' ')
// 							break;

// 						(*loc)--;
// 					}
// 				}
// 				else if (text->at(*loc - 1) == '	')
// 				{
// 					for (int64_t i = *loc - 1; i >= 0; i--)
// 					{
// 						if (text->at(i) != '	')
// 							break;

// 						(*loc)--;
// 					}
// 				}
// 				else
// 				{
// 					uint64_t oLoc = *loc;
// 					for (int64_t i = *loc - 1; i >= 0; i--)
// 					{
// 						char re = text->at(i);
// 						if (i != static_cast<int64_t>(oLoc) - 1 && (
// 							text->at(i) == ' ' || text->at(i) == '`' || text->at(i) == '~' || text->at(i) == '!' || text->at(i) == '@' || text->at(i) == '#' || text->at(i) == '$' ||
// 							text->at(i) == '%' || text->at(i) == '^' || text->at(i) == '&' || text->at(i) == '*' || text->at(i) == '(' || text->at(i) == ')' || text->at(i) == '-' ||
// 							text->at(i) == '=' || text->at(i) == '+' || text->at(i) == '[' || text->at(i) == ']' || text->at(i) == '{' || text->at(i) == '}' || text->at(i) == '\\' ||
// 							text->at(i) == '|' || text->at(i) == ';' || text->at(i) == ':' || text->at(i) == '\'' || text->at(i) == '"' || text->at(i) == ',' || text->at(i) == '.' ||
// 							text->at(i) == '<' || text->at(i) == '>' || text->at(i) == '/' || text->at(i) == '?' || text->at(i) == '	'))
// 							break;

// 						(*loc)--;
// 					}
// 				}
// 			}
// 			else
// 				(*loc)--;
// 		}

// 		if (!ev.shift)
// 			*locHigh = *loc;
// 	}
// 	else if (ev.code == sf::Keyboard::Key::Right)
// 	{
// 		if (*loc < text->size())
// 		{
// 			if (ev.control)
// 			{
// 				if (text->at(*loc) == ' ')
// 				{
// 					for (uint64_t i = *loc; i < text->size(); i++)
// 					{
// 						if (text->at(i) != ' ')
// 							break;

// 						(*loc)++;
// 					}
// 				}
// 				else if (text->at(*loc) == '	')
// 				{
// 					for (uint64_t i = *loc; i < text->size(); i++)
// 					{
// 						if (text->at(i) != '	')
// 							break;

// 						(*loc)++;
// 					}
// 				}
// 				else
// 				{
// 					uint64_t oLoc = *loc;
// 					for (uint64_t i = *loc; i < text->size(); i++)
// 					{
// 						if (i != oLoc && (
// 							text->at(i) == ' ' || text->at(i) == '`' || text->at(i) == '~' || text->at(i) == '!' || text->at(i) == '@' || text->at(i) == '#' || text->at(i) == '$' ||
// 							text->at(i) == '%' || text->at(i) == '^' || text->at(i) == '&' || text->at(i) == '*' || text->at(i) == '(' || text->at(i) == ')' || text->at(i) == '-' ||
// 							text->at(i) == '=' || text->at(i) == '+' || text->at(i) == '[' || text->at(i) == ']' || text->at(i) == '{' || text->at(i) == '}' || text->at(i) == '\\' ||
// 							text->at(i) == '|' || text->at(i) == ';' || text->at(i) == ':' || text->at(i) == '\'' || text->at(i) == '"' || text->at(i) == ',' || text->at(i) == '.' ||
// 							text->at(i) == '<' || text->at(i) == '>' || text->at(i) == '/' || text->at(i) == '?' || text->at(i) == '	'))
// 							break;

// 						(*loc)++;
// 					}
// 				}
// 			}
// 			else
// 				(*loc)++;
// 		}

// 		if (!ev.shift)
// 			*locHigh = *loc;
// 	}
// 	else if (ev.code >= 0 && ev.code <= 25)
// 	{
// 		if (ev.control && ev.code == sf::Keyboard::Key::A)
// 		{
// 			*locHigh = 0;
// 			*loc = text->size();
// 		}
// 		else if (ev.control && ev.code == sf::Keyboard::Key::C)
// 			sf::Clipboard::setString(text->substr(std::min(*locHigh, *loc), std::max(*locHigh, *loc) - std::min(*locHigh, *loc)));
// 		else if (ev.control && ev.code == sf::Keyboard::Key::V)
// 			insertString(sf::Clipboard::getString());
// 		else
// 		{
// 			if (ev.shift)
// 				insertChar(ev.code + 65);
// 			else
// 				insertChar(ev.code + 97);
// 		}
// 	}
// 	else if (ev.code == sf::Keyboard::Key::Backspace)
// 	{
// 		if (*locHigh != *loc)
// 		{
// 			std::string nt;

// 			for (uint64_t i = 0; i < std::min(*locHigh, *loc); i++)
// 				nt += text->at(i);

// 			for (uint64_t i = std::max(*locHigh, *loc); i < text->size(); i++)
// 				nt += text->at(i);

// 			*text = nt;

// 			*loc = std::min(*locHigh, *loc);
// 			*locHigh = *loc;
// 		}
// 		else if (*loc > 0)
// 		{
// 			std::string nt;

// 			for (uint64_t i = 0; i < *loc - 1; i++)
// 				nt += text->at(i);

// 			for (uint64_t i = *loc; i < text->size(); i++)
// 				nt += text->at(i);

// 			*text = nt;

// 			(*loc)--;
// 			*locHigh = *loc;
// 		}
// 	}
// 	else if (ev.code == sf::Keyboard::Key::Delete)
// 	{
// 		if (*locHigh != *loc)
// 		{
// 			std::string nt;

// 			for (uint64_t i = 0; i < std::min(*locHigh, *loc); i++)
// 				nt += text->at(i);

// 			for (uint64_t i = std::max(*locHigh, *loc); i < text->size(); i++)
// 				nt += text->at(i);

// 			*text = nt;

// 			*loc = std::min(*locHigh, *loc);
// 			*locHigh = *loc;
// 		}
// 		else if (*loc < text->size())
// 		{
// 			std::string nt;

// 			for (uint64_t i = 0; i < *loc; i++)
// 				nt += text->at(i);

// 			for (uint64_t i = *loc + 1; i < text->size(); i++)
// 				nt += text->at(i);

// 			*text = nt;
// 		}
// 	}
// 	else if (ev.code == sf::Keyboard::Key::Escape)
// 	{
// 		if (escape != nullptr)
// 			(*escape)();
// 	}
// 	else if (ev.code == sf::Keyboard::Key::Enter)
// 	{
// 		if (enter != nullptr)
// 			(*enter)();
// 	}
// 	else if (ev.code >= 26 && ev.code <= 35)
// 	{
// 		if (ev.shift)
// 		{
// 			if (ev.code == sf::Keyboard::Key::Num0)
// 				insertChar(')');
// 			else if (ev.code == sf::Keyboard::Key::Num1)
// 				insertChar('!');
// 			else if (ev.code == sf::Keyboard::Key::Num2)
// 				insertChar('@');
// 			else if (ev.code == sf::Keyboard::Key::Num3)
// 				insertChar('#');
// 			else if (ev.code == sf::Keyboard::Key::Num4)
// 				insertChar('$');
// 			else if (ev.code == sf::Keyboard::Key::Num5)
// 				insertChar('%');
// 			else if (ev.code == sf::Keyboard::Key::Num6)
// 				insertChar('^');
// 			else if (ev.code == sf::Keyboard::Key::Num7)
// 				insertChar('&');
// 			else if (ev.code == sf::Keyboard::Key::Num8)
// 				insertChar('*');
// 			else if (ev.code == sf::Keyboard::Key::Num9)
// 				insertChar('(');
// 		}
// 		else
// 			insertChar(ev.code + 22);
// 	}
// 	else if (ev.code == sf::Keyboard::Key::Space)
// 		insertChar(' ');
// 	else if (ev.code == sf::Keyboard::Key::Tab)
// 		insertChar('	');
// 	else if (ev.code == sf::Keyboard::Key::Tilde)
// 	{
// 		if (ev.shift)
// 			insertChar('~');
// 		else
// 			insertChar('`');
// 	}
// 	else if (ev.code == sf::Keyboard::Key::Hyphen)
// 	{
// 		if (ev.shift)
// 			insertChar('_');
// 		else
// 			insertChar('-');
// 	}
// 	else if (ev.code == sf::Keyboard::Key::Equal)
// 	{
// 		if (ev.shift)
// 			insertChar('+');
// 		else
// 			insertChar('=');
// 	}
// 	else if (ev.code == sf::Keyboard::Key::LBracket)
// 	{
// 		if (ev.shift)
// 			insertChar('{');
// 		else
// 			insertChar('[');
// 	}
// 	else if (ev.code == sf::Keyboard::Key::RBracket)
// 	{
// 		if (ev.shift)
// 			insertChar(']');
// 		else
// 			insertChar('}');
// 	}
// 	else if (ev.code == sf::Keyboard::Key::BackSlash)
// 	{
// 		if (ev.shift)
// 			insertChar('|');
// 		else
// 			insertChar('\\');
// 	}
// 	else if (ev.code == sf::Keyboard::Key::SemiColon)
// 	{
// 		if (ev.shift)
// 			insertChar(':');
// 		else
// 			insertChar(';');
// 	}
// 	else if (ev.code == sf::Keyboard::Key::Quote)
// 	{
// 		if (ev.shift)
// 			insertChar('"');
// 		else
// 			insertChar('\'');
// 	}
// 	else if (ev.code == sf::Keyboard::Key::Comma)
// 	{
// 		if (ev.shift)
// 			insertChar('<');
// 		else
// 			insertChar(',');
// 	}
// 	else if (ev.code == sf::Keyboard::Key::Period)
// 	{
// 		if (ev.shift)
// 			insertChar('>');
// 		else
// 			insertChar('.');
// 	}
// 	else if (ev.code == sf::Keyboard::Key::Slash)
// 	{
// 		if (ev.shift)
// 			insertChar('?');
// 		else
// 			insertChar('/');
// 	}
// 	else if (ev.code >= 75 && ev.code <= 84)
// 		insertChar(ev.code - 27);
// 	else if (ev.code == 67)
// 		insertChar('+');
// 	else if (ev.code == 68)
// 		insertChar('-');
// 	else if (ev.code == 69)
// 		insertChar('*');
// 	else if (ev.code == 70)
// 		insertChar('/');
// }

// void InputHandler::RunNumberProccess(std::string* text, uint64_t* locHigh, uint64_t* loc, std::function<void()>* enter, std::function<void()>* escape, const sf::Event::KeyEvent& ev)
// {
// 	std::function<void(const char& ch)> insertChar = [&](const char& ch)
// 	{
// 		std::string nt;

// 		for (uint64_t i = 0; i < std::min(*locHigh, *loc); i++)
// 			nt += text->at(i);

// 		nt += ch;

// 		for (uint64_t i = std::max(*locHigh, *loc); i < text->size(); i++)
// 			nt += text->at(i);

// 		*text = nt;

// 		*loc = std::min(*locHigh, *loc) + 1;
// 		*locHigh = *loc;
// 	};

// 	std::function<void(const std::string& ch)> insertString = [&](const std::string& ch)
// 	{
// 		// whitelist chars

// 		std::string nCh;
// 		nCh.reserve(ch.length());
// 		bool hasDecimal = false;

// 		for (uint64_t i = 0; i < ch.length(); i++)
// 		{
// 			if (i == 0 && ch[i] == '-')
// 				nCh += ch[i];
// 			else if (ch[i] >= 48 && ch[i] <= 57)
// 				nCh += ch[i];
// 			else if (ch[i] == '.')
// 			{
// 				if (hasDecimal)
// 					continue;

// 				hasDecimal = true;
// 				nCh += ch[i];
// 			}
// 		}

// 		std::string nt;

// 		for (uint64_t i = 0; i < std::min(*locHigh, *loc); i++)
// 			nt += text->at(i);

// 		nt += ch;

// 		for (uint64_t i = std::max(*locHigh, *loc); i < text->size(); i++)
// 			nt += text->at(i);

// 		*text = nt;

// 		*loc = std::min(*locHigh, *loc) + ch.length();
// 		*locHigh = *loc;
// 	};

// 	if (ev.code == sf::Keyboard::Key::Left)
// 	{
// 		if (*loc > 0)
// 		{
// 			if (ev.control)
// 			{
// 				if (text->at(*loc - 1) == ' ')
// 				{
// 					for (int64_t i = *loc - 1; i >= 0; i--)
// 					{
// 						if (text->at(i) != ' ')
// 							break;

// 						(*loc)--;
// 					}
// 				}
// 				else if (text->at(*loc - 1) == '	')
// 				{
// 					for (int64_t i = *loc - 1; i >= 0; i--)
// 					{
// 						if (text->at(i) != '	')
// 							break;

// 						(*loc)--;
// 					}
// 				}
// 				else
// 				{
// 					uint64_t oLoc = *loc;
// 					for (int64_t i = *loc - 1; i >= 0; i--)
// 					{
// 						char re = text->at(i);
// 						if (i != static_cast<int64_t>(oLoc) - 1 && (
// 							text->at(i) == ' ' || text->at(i) == '`' || text->at(i) == '~' || text->at(i) == '!' || text->at(i) == '@' || text->at(i) == '#' || text->at(i) == '$' ||
// 							text->at(i) == '%' || text->at(i) == '^' || text->at(i) == '&' || text->at(i) == '*' || text->at(i) == '(' || text->at(i) == ')' || text->at(i) == '-' ||
// 							text->at(i) == '=' || text->at(i) == '+' || text->at(i) == '[' || text->at(i) == ']' || text->at(i) == '{' || text->at(i) == '}' || text->at(i) == '\\' ||
// 							text->at(i) == '|' || text->at(i) == ';' || text->at(i) == ':' || text->at(i) == '\'' || text->at(i) == '"' || text->at(i) == ',' || text->at(i) == '.' ||
// 							text->at(i) == '<' || text->at(i) == '>' || text->at(i) == '/' || text->at(i) == '?' || text->at(i) == '	'))
// 							break;

// 						(*loc)--;
// 					}
// 				}
// 			}
// 			else
// 				(*loc)--;
// 		}

// 		if (!ev.shift)
// 			*locHigh = *loc;
// 	}
// 	else if (ev.code == sf::Keyboard::Key::Right)
// 	{
// 		if (*loc < text->size())
// 		{
// 			if (ev.control)
// 			{
// 				if (text->at(*loc) == ' ')
// 				{
// 					for (uint64_t i = *loc; i < text->size(); i++)
// 					{
// 						if (text->at(i) != ' ')
// 							break;

// 						(*loc)++;
// 					}
// 				}
// 				else if (text->at(*loc) == '	')
// 				{
// 					for (uint64_t i = *loc; i < text->size(); i++)
// 					{
// 						if (text->at(i) != '	')
// 							break;

// 						(*loc)++;
// 					}
// 				}
// 				else
// 				{
// 					uint64_t oLoc = *loc;
// 					for (uint64_t i = *loc; i < text->size(); i++)
// 					{
// 						if (i != oLoc && (
// 							text->at(i) == ' ' || text->at(i) == '`' || text->at(i) == '~' || text->at(i) == '!' || text->at(i) == '@' || text->at(i) == '#' || text->at(i) == '$' ||
// 							text->at(i) == '%' || text->at(i) == '^' || text->at(i) == '&' || text->at(i) == '*' || text->at(i) == '(' || text->at(i) == ')' || text->at(i) == '-' ||
// 							text->at(i) == '=' || text->at(i) == '+' || text->at(i) == '[' || text->at(i) == ']' || text->at(i) == '{' || text->at(i) == '}' || text->at(i) == '\\' ||
// 							text->at(i) == '|' || text->at(i) == ';' || text->at(i) == ':' || text->at(i) == '\'' || text->at(i) == '"' || text->at(i) == ',' || text->at(i) == '.' ||
// 							text->at(i) == '<' || text->at(i) == '>' || text->at(i) == '/' || text->at(i) == '?' || text->at(i) == '	'))
// 							break;

// 						(*loc)++;
// 					}
// 				}
// 			}
// 			else
// 				(*loc)++;
// 		}

// 		if (!ev.shift)
// 			*locHigh = *loc;
// 	}
// 	else if (ev.control && ev.code == sf::Keyboard::Key::A)
// 	{
// 		*locHigh = 0;
// 		*loc = text->size();
// 	}
// 	else if (ev.code == sf::Keyboard::Key::Backspace)
// 	{
// 		if (*locHigh != *loc)
// 		{
// 			std::string nt;

// 			for (uint64_t i = 0; i < std::min(*locHigh, *loc); i++)
// 				nt += text->at(i);

// 			for (uint64_t i = std::max(*locHigh, *loc); i < text->size(); i++)
// 				nt += text->at(i);

// 			*text = nt;

// 			*loc = std::min(*locHigh, *loc);
// 			*locHigh = *loc;
// 		}
// 		else if (*loc > 0)
// 		{
// 			std::string nt;

// 			for (uint64_t i = 0; i < *loc - 1; i++)
// 				nt += text->at(i);

// 			for (uint64_t i = *loc; i < text->size(); i++)
// 				nt += text->at(i);

// 			*text = nt;

// 			(*loc)--;
// 			*locHigh = *loc;
// 		}
// 	}
// 	else if (ev.code == sf::Keyboard::Key::Delete)
// 	{
// 		if (*locHigh != *loc)
// 		{
// 			std::string nt;

// 			for (uint64_t i = 0; i < std::min(*locHigh, *loc); i++)
// 				nt += text->at(i);

// 			for (uint64_t i = std::max(*locHigh, *loc); i < text->size(); i++)
// 				nt += text->at(i);

// 			*text = nt;

// 			*loc = std::min(*locHigh, *loc);
// 			*locHigh = *loc;
// 		}
// 		else if (*loc < text->size())
// 		{
// 			std::string nt;

// 			for (uint64_t i = 0; i < *loc; i++)
// 				nt += text->at(i);

// 			for (uint64_t i = *loc + 1; i < text->size(); i++)
// 				nt += text->at(i);

// 			*text = nt;
// 		}
// 	}
// 	else if (ev.code == sf::Keyboard::Key::Escape)
// 	{
// 		if (escape != nullptr)
// 			(*escape)();
// 	}
// 	else if (ev.code == sf::Keyboard::Key::Enter)
// 	{
// 		if (enter != nullptr)
// 			(*enter)();
// 	}
// 	else if (ev.code >= 26 && ev.code <= 35 && !ev.shift)
// 		insertChar(ev.code + 22);
// 	else if (ev.code == sf::Keyboard::Key::Period && !ev.shift)
// 	{
// 		bool found = false;

// 		for (uint64_t i = 0; i < text->length(); i++)
// 		{
// 			if (text->at(i) == '.')
// 			{
// 				found = true;
// 				break;
// 			}
// 		}

// 		if (!found)
// 			insertChar('.');
// 	}
// 	else if (ev.code == sf::Keyboard::Key::Hyphen && !ev.shift && (*loc == 0 || *locHigh == 0))
// 	{
// 		if (text->at(0) != '-')
// 			insertChar('-');
// 	}
// 	else if (ev.code >= 75 && ev.code <= 84)
// 		insertChar(ev.code - 27);
// 	else if (ev.control && ev.code == sf::Keyboard::Key::C)
// 		sf::Clipboard::setString(text->substr(std::min(*locHigh, *loc), std::max(*locHigh, *loc) - std::min(*locHigh, *loc)));
// 	else if (ev.control && ev.code == sf::Keyboard::Key::V)
// 		insertString(sf::Clipboard::getString());
// }

// bool InputHandler::RunMouseProccess(sf::Text* text, uint64_t* locHigh, uint64_t* loc, bool* isDown, bool down, const sf::Vector2i& pos, uint64_t yOverride)
// {
// 	uint64_t yHeight = yOverride;

// 	if (yHeight == 0)
// 		yHeight = text->getLocalBounds().height;

// 	if (pos.x >= text->getPosition().x && pos.x <= text->getPosition().x + text->getLocalBounds().width)
// 	{
// 		if (down && pos.y >= text->getPosition().y && pos.y <= text->getPosition().y + yHeight)
// 		{
// 			*isDown = true;

// 			for (uint64_t i = 0; i < text->getString().getSize() + 1; i++)
// 			{
// 				double ix = text->getPosition().x;

// 				if (i == text->getString().getSize())
// 				{
// 					*loc = i;
// 					*locHigh = i;
// 					break;
// 				}
// 				else
// 				{
// 					ix += sf::Text(text->getString().substring(0, i), Global::Font, 16).getLocalBounds().width;
// 					ix += sf::Text(text->getString().substring(i, 1), Global::Font, 16).getLocalBounds().width / 2.0;
// 				}

// 				if (pos.x <= ix)
// 				{
// 					*loc = i;
// 					*locHigh = i;
// 					break;
// 				}
// 			}
// 		}
// 		else
// 			*isDown = false;

// 		return pos.y >= text->getPosition().y && pos.y <= text->getPosition().y + yHeight;
// 	}

// 	return false;
// }

// bool InputHandler::RunMouseProccess(TextSystem& system, const sf::Vector2i& tPos, const sf::Vector2u& tSize, bool down, const sf::Vector2i& pos, uint16_t fontSize, int16_t vanityOffset)
// {
// 	if (pos.x >= tPos.x && pos.x <= tPos.x + static_cast<int64_t>(tSize.x))
// 	{
// 		if (!down)
// 			*system.IsDown = false;

// 		if (down && pos.y >= tPos.y && pos.y <= tPos.y + static_cast<int64_t>(tSize.y))
// 		{
// 			*system.IsDown = true;

// 			for (uint64_t i = 0; i < system.Text->length() + 1; i++)
// 			{
// 				int64_t ix = tPos.x + vanityOffset + Global::BlockBorder;

// 				if (i == system.Text->length())
// 				{
// 					*system.Loc = i;
// 					*system.LocHigh = i;
// 					break;
// 				}
// 				else
// 				{
// 					ix += sf::Text(system.Text->substr(0, i), Global::Font, fontSize).getLocalBounds().width;
// 					ix += sf::Text(system.Text->substr(i, 1), Global::Font, fontSize).getLocalBounds().width / 2.0;
// 				}

// 				if (pos.x <= ix)
// 				{
// 					*system.Loc = i;
// 					*system.LocHigh = i;
// 					break;
// 				}
// 			}
// 		}

// 		return pos.y >= tPos.y && pos.y <= tPos.y + static_cast<int64_t>(tSize.y);
// 	}

// 	return false;
// }

// void InputHandler::RunMouseProccessFrame(sf::Text* text, uint64_t* loc, bool* isDown, const sf::Vector2i& pos, uint64_t yOverride)
// {
// 	if (*isDown)
// 	{
// 		uint64_t yHeight = yOverride;

// 		if (yHeight == 0)
// 			yHeight = text->getLocalBounds().height;

// 		if (pos.x >= text->getPosition().x && pos.x <= text->getPosition().x + text->getLocalBounds().width)
// 		{
// 			for (uint64_t i = 0; i < text->getString().getSize() + 1; i++)
// 			{
// 				float ix = text->getPosition().x;

// 				if (i == text->getString().getSize())
// 				{
// 					*loc = i;
// 					break;
// 				}
// 				else
// 				{
// 					ix += sf::Text(text->getString().substring(0, i), Global::Font, 16).getLocalBounds().width;
// 					ix += sf::Text(text->getString().substring(i, 1), Global::Font, 16).getLocalBounds().width / 2.0;
// 				}

// 				if (pos.x <= ix)
// 				{
// 					*loc = i;
// 					break;
// 				}
// 			}
// 		}
// 	}
// }

// void InputHandler::RunMouseProccessFrame(std::string* text, const sf::Vector2i& tPos, const sf::Vector2u& tSize, uint64_t* loc, bool* isDown, const sf::Vector2i& pos, uint16_t fontSize, int16_t vanityOffset)
// {
// 	if (*isDown)
// 	{
// 		if (pos.x >= tPos.x && pos.x <= tPos.x + static_cast<int64_t>(tSize.x))
// 		{
// 			for (uint64_t i = 0; i < text->length() + 1; i++)
// 			{
// 				int64_t ix = tPos.x + vanityOffset + Global::BlockBorder;

// 				if (i == text->length())
// 				{
// 					*loc = i;
// 					break;
// 				}
// 				else
// 				{
// 					ix += sf::Text(text->substr(0, i), Global::Font, fontSize).getLocalBounds().width;
// 					ix += sf::Text(text->substr(i, 1), Global::Font, fontSize).getLocalBounds().width / 2.0;
// 				}

// 				if (pos.x <= ix)
// 				{
// 					*loc = i;
// 					break;
// 				}
// 			}
// 		}
// 	}
// }

void InputHandler::RegisterKeyCallback(void(*callback)(int16_t key, bool down, bool shift))
{
	std::unique_lock lock(m_KeyTextMutex);
	m_KeyCallbacks.push_back(callback);
}

void InputHandler::UnregisterKeyCallback(void(*callback)(int16_t key, bool down, bool shift))
{
	std::unique_lock lock(m_KeyTextMutex);

	for (size_t i = 0; i < m_KeyCallbacks.size(); i++)
	{
		if (m_KeyCallbacks[i] == callback)
		{
			m_KeyCallbacks.erase(m_KeyCallbacks.begin() + i);
			return;
		}
	}

	Logger::Error("failed to unregister key callback");
}

void InputHandler::RegisterTextCallback(void(*callback)(int16_t key, bool shift))
{
	std::unique_lock lock(m_KeyTextMutex);
	m_TextCallbacks.push_back(callback);
}

void InputHandler::UnregisterTextCallback(void(*callback)(int16_t key, bool shift))
{
	std::unique_lock lock(m_KeyTextMutex);

	for (size_t i = 0; i < m_TextCallbacks.size(); i++)
	{
		if (m_TextCallbacks[i] == callback)
		{
			m_TextCallbacks.erase(m_TextCallbacks.begin() + i);
			return;
		}
	}

	Logger::Error("failed to unregister text callback");
}

void InputHandler::RegisterMouseCallback(void(*callback)(glm::vec<2, int32_t> position, float scroll, MouseOperation operation))
{
	std::unique_lock lock(m_MouseMutex);
	m_MouseCallbacks.push_back(callback);
}

void InputHandler::UnregisterMouseCallback(void(*callback)(glm::vec<2, int32_t> position, float scroll, MouseOperation operation))
{
	std::unique_lock lock(m_MouseMutex);

	for (size_t i = 0; i < m_MouseCallbacks.size(); i++)
	{
		if (m_MouseCallbacks[i] == callback)
		{
			m_MouseCallbacks.erase(m_MouseCallbacks.begin() + i);
			return;
		}
	}

	Logger::Error("failed to unregister mouse callback");
}

void InputHandler::FireKey(int16_t key, uint8_t operation)
{
	std::unique_lock lock(m_KeyTextMutex);

	if (key == GLFW_KEY_LEFT_SHIFT)
	{
		if (operation == GLFW_PRESS)
			m_ShiftDown = true;
		else if (operation == GLFW_RELEASE)
			m_ShiftDown = false;
	}

	if (operation == GLFW_PRESS)
	{
		for (auto callback : m_KeyCallbacks)
			callback(key, true, m_ShiftDown);
	}
	else if (operation == GLFW_RELEASE)
	{
		for (auto callback : m_KeyCallbacks)
			callback(key, false, m_ShiftDown);
	}
	else if (operation == GLFW_REPEAT)
	{
		if (key >= 32 && key <= 96)
		{
			for (auto callback : m_TextCallbacks)
				callback(key, m_ShiftDown);
		}
	}
}

void InputHandler::FireMouseButton(uint8_t button, uint8_t operation)
{
	if (button != GLFW_MOUSE_BUTTON_LEFT)
		return;

	std::unique_lock lock(m_MouseMutex);

	if (operation == GLFW_PRESS)
	{
		for (auto callback : m_MouseCallbacks)
			callback(m_MousePosition, 0.0f, MouseOperation::Press);
	}
	else if (operation == GLFW_RELEASE)
	{
		for (auto callback : m_MouseCallbacks)
			callback(m_MousePosition, 0.0f, MouseOperation::Release);

		for (auto callback : m_MouseCallbacks)
			callback(m_MousePosition, 0.0f, MouseOperation::Click);
	}
}

void InputHandler::FireMouseMove(glm::vec<2, int32_t> position)
{
	std::unique_lock lock(m_MouseMutex);
	m_MousePosition = position;

	for (auto callback : m_MouseCallbacks)
		callback(position, 0.0f, MouseOperation::Move);
}

void InputHandler::FireMouseScroll(float scroll)
{
	std::unique_lock lock(m_MouseMutex);

	for (auto callback : m_MouseCallbacks)
		callback(m_MousePosition, scroll, MouseOperation::Scroll);
}

bool InputHandler::m_MouseWasDown;
bool InputHandler::m_ShiftDown;
glm::vec<2, int32_t> InputHandler::m_MousePosition;
std::shared_mutex InputHandler::m_KeyTextMutex;
std::vector<void(*)(int16_t key, bool down, bool shift)> InputHandler::m_KeyCallbacks;
std::vector<void(*)(int16_t key, bool shift)> InputHandler::m_TextCallbacks;
std::shared_mutex InputHandler::m_MouseMutex;
std::vector<void(*)(glm::vec<2, int32_t> position, float scroll, MouseOperation operation)> InputHandler::m_MouseCallbacks;