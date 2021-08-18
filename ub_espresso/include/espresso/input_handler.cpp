#include "input_handler.hpp"

void InputHandler::Initialization()
{
	m_MouseWasDown = false;
	m_ShiftDown = false;

	m_MousePosition = { 0, 0 };
}

// void InputHandler::RunTextProccess(std::string* text, u64* locHigh, u64* loc, std::function<void()>* enter, std::function<void()>* escape, const sf::Event::KeyEvent& ev)
// {
// 	std::function<void(const char& ch)> insertChar = [&](const char& ch)
// 	{
// 		std::string nt;

// 		for (u64 i = 0; i < std::min(*locHigh, *loc); i++)
// 			nt += text->at(i);

// 		nt += ch;

// 		for (u64 i = std::max(*locHigh, *loc); i < text->size(); i++)
// 			nt += text->at(i);

// 		*text = nt;

// 		*loc = std::min(*locHigh, *loc) + 1;
// 		*locHigh = *loc;
// 	};

// 	std::function<void(const std::string& ch)> insertString = [&](const std::string& ch)
// 	{
// 		std::string nt;

// 		for (u64 i = 0; i < std::min(*locHigh, *loc); i++)
// 			nt += text->at(i);

// 		nt += ch;

// 		for (u64 i = std::max(*locHigh, *loc); i < text->size(); i++)
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
// 					for (i64 i = *loc - 1; i >= 0; i--)
// 					{
// 						if (text->at(i) != ' ')
// 							break;

// 						(*loc)--;
// 					}
// 				}
// 				else if (text->at(*loc - 1) == '	')
// 				{
// 					for (i64 i = *loc - 1; i >= 0; i--)
// 					{
// 						if (text->at(i) != '	')
// 							break;

// 						(*loc)--;
// 					}
// 				}
// 				else
// 				{
// 					u64 oLoc = *loc;
// 					for (i64 i = *loc - 1; i >= 0; i--)
// 					{
// 						char re = text->at(i);
// 						if (i != static_cast<i64>(oLoc) - 1 && (
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
// 					for (u64 i = *loc; i < text->size(); i++)
// 					{
// 						if (text->at(i) != ' ')
// 							break;

// 						(*loc)++;
// 					}
// 				}
// 				else if (text->at(*loc) == '	')
// 				{
// 					for (u64 i = *loc; i < text->size(); i++)
// 					{
// 						if (text->at(i) != '	')
// 							break;

// 						(*loc)++;
// 					}
// 				}
// 				else
// 				{
// 					u64 oLoc = *loc;
// 					for (u64 i = *loc; i < text->size(); i++)
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

// 			for (u64 i = 0; i < std::min(*locHigh, *loc); i++)
// 				nt += text->at(i);

// 			for (u64 i = std::max(*locHigh, *loc); i < text->size(); i++)
// 				nt += text->at(i);

// 			*text = nt;

// 			*loc = std::min(*locHigh, *loc);
// 			*locHigh = *loc;
// 		}
// 		else if (*loc > 0)
// 		{
// 			std::string nt;

// 			for (u64 i = 0; i < *loc - 1; i++)
// 				nt += text->at(i);

// 			for (u64 i = *loc; i < text->size(); i++)
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

// 			for (u64 i = 0; i < std::min(*locHigh, *loc); i++)
// 				nt += text->at(i);

// 			for (u64 i = std::max(*locHigh, *loc); i < text->size(); i++)
// 				nt += text->at(i);

// 			*text = nt;

// 			*loc = std::min(*locHigh, *loc);
// 			*locHigh = *loc;
// 		}
// 		else if (*loc < text->size())
// 		{
// 			std::string nt;

// 			for (u64 i = 0; i < *loc; i++)
// 				nt += text->at(i);

// 			for (u64 i = *loc + 1; i < text->size(); i++)
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

// void InputHandler::RunNumberProccess(std::string* text, u64* locHigh, u64* loc, std::function<void()>* enter, std::function<void()>* escape, const sf::Event::KeyEvent& ev)
// {
// 	std::function<void(const char& ch)> insertChar = [&](const char& ch)
// 	{
// 		std::string nt;

// 		for (u64 i = 0; i < std::min(*locHigh, *loc); i++)
// 			nt += text->at(i);

// 		nt += ch;

// 		for (u64 i = std::max(*locHigh, *loc); i < text->size(); i++)
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

// 		for (u64 i = 0; i < ch.length(); i++)
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

// 		for (u64 i = 0; i < std::min(*locHigh, *loc); i++)
// 			nt += text->at(i);

// 		nt += ch;

// 		for (u64 i = std::max(*locHigh, *loc); i < text->size(); i++)
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
// 					for (i64 i = *loc - 1; i >= 0; i--)
// 					{
// 						if (text->at(i) != ' ')
// 							break;

// 						(*loc)--;
// 					}
// 				}
// 				else if (text->at(*loc - 1) == '	')
// 				{
// 					for (i64 i = *loc - 1; i >= 0; i--)
// 					{
// 						if (text->at(i) != '	')
// 							break;

// 						(*loc)--;
// 					}
// 				}
// 				else
// 				{
// 					u64 oLoc = *loc;
// 					for (i64 i = *loc - 1; i >= 0; i--)
// 					{
// 						char re = text->at(i);
// 						if (i != static_cast<i64>(oLoc) - 1 && (
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
// 					for (u64 i = *loc; i < text->size(); i++)
// 					{
// 						if (text->at(i) != ' ')
// 							break;

// 						(*loc)++;
// 					}
// 				}
// 				else if (text->at(*loc) == '	')
// 				{
// 					for (u64 i = *loc; i < text->size(); i++)
// 					{
// 						if (text->at(i) != '	')
// 							break;

// 						(*loc)++;
// 					}
// 				}
// 				else
// 				{
// 					u64 oLoc = *loc;
// 					for (u64 i = *loc; i < text->size(); i++)
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

// 			for (u64 i = 0; i < std::min(*locHigh, *loc); i++)
// 				nt += text->at(i);

// 			for (u64 i = std::max(*locHigh, *loc); i < text->size(); i++)
// 				nt += text->at(i);

// 			*text = nt;

// 			*loc = std::min(*locHigh, *loc);
// 			*locHigh = *loc;
// 		}
// 		else if (*loc > 0)
// 		{
// 			std::string nt;

// 			for (u64 i = 0; i < *loc - 1; i++)
// 				nt += text->at(i);

// 			for (u64 i = *loc; i < text->size(); i++)
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

// 			for (u64 i = 0; i < std::min(*locHigh, *loc); i++)
// 				nt += text->at(i);

// 			for (u64 i = std::max(*locHigh, *loc); i < text->size(); i++)
// 				nt += text->at(i);

// 			*text = nt;

// 			*loc = std::min(*locHigh, *loc);
// 			*locHigh = *loc;
// 		}
// 		else if (*loc < text->size())
// 		{
// 			std::string nt;

// 			for (u64 i = 0; i < *loc; i++)
// 				nt += text->at(i);

// 			for (u64 i = *loc + 1; i < text->size(); i++)
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

// 		for (u64 i = 0; i < text->length(); i++)
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

// bool InputHandler::RunMouseProccess(sf::Text* text, u64* locHigh, u64* loc, bool* isDown, bool down, const sf::Vector2i& pos, u64 yOverride)
// {
// 	u64 yHeight = yOverride;

// 	if (yHeight == 0)
// 		yHeight = text->getLocalBounds().height;

// 	if (pos.x >= text->getPosition().x && pos.x <= text->getPosition().x + text->getLocalBounds().width)
// 	{
// 		if (down && pos.y >= text->getPosition().y && pos.y <= text->getPosition().y + yHeight)
// 		{
// 			*isDown = true;

// 			for (u64 i = 0; i < text->getString().getSize() + 1; i++)
// 			{
// 				f64 ix = text->getPosition().x;

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

// bool InputHandler::RunMouseProccess(TextSystem& system, const sf::Vector2i& tPos, const sf::Vector2u& tSize, bool down, const sf::Vector2i& pos, u16 fontSize, i16 vanityOffset)
// {
// 	if (pos.x >= tPos.x && pos.x <= tPos.x + static_cast<i64>(tSize.x))
// 	{
// 		if (!down)
// 			*system.IsDown = false;

// 		if (down && pos.y >= tPos.y && pos.y <= tPos.y + static_cast<i64>(tSize.y))
// 		{
// 			*system.IsDown = true;

// 			for (u64 i = 0; i < system.Text->length() + 1; i++)
// 			{
// 				i64 ix = tPos.x + vanityOffset + Global::BlockBorder;

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

// 		return pos.y >= tPos.y && pos.y <= tPos.y + static_cast<i64>(tSize.y);
// 	}

// 	return false;
// }

// void InputHandler::RunMouseProccessFrame(sf::Text* text, u64* loc, bool* isDown, const sf::Vector2i& pos, u64 yOverride)
// {
// 	if (*isDown)
// 	{
// 		u64 yHeight = yOverride;

// 		if (yHeight == 0)
// 			yHeight = text->getLocalBounds().height;

// 		if (pos.x >= text->getPosition().x && pos.x <= text->getPosition().x + text->getLocalBounds().width)
// 		{
// 			for (u64 i = 0; i < text->getString().getSize() + 1; i++)
// 			{
// 				f32 ix = text->getPosition().x;

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

// void InputHandler::RunMouseProccessFrame(std::string* text, const sf::Vector2i& tPos, const sf::Vector2u& tSize, u64* loc, bool* isDown, const sf::Vector2i& pos, u16 fontSize, i16 vanityOffset)
// {
// 	if (*isDown)
// 	{
// 		if (pos.x >= tPos.x && pos.x <= tPos.x + static_cast<i64>(tSize.x))
// 		{
// 			for (u64 i = 0; i < text->length() + 1; i++)
// 			{
// 				i64 ix = tPos.x + vanityOffset + Global::BlockBorder;

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

void InputHandler::RegisterKeyCallback(void(*callback)(key_state state, void* data), void* data)
{
	std::unique_lock lock(m_KeyTextMutex);

	m_KeyCallbacks.push_back(callback);
	m_KeyDatas.push_back(data);
}

void InputHandler::UnregisterKeyCallback(void(*callback)(key_state state, void* data))
{
	std::unique_lock lock(m_KeyTextMutex);

	for (usize i = 0; i < m_KeyCallbacks.size(); i++)
	{
		if (m_KeyCallbacks[i] == callback)
		{
			m_KeyCallbacks.erase(m_KeyCallbacks.begin() + i);
			m_KeyDatas.erase(m_KeyDatas.begin() + i);
			return;
		}
	}

	Logger::Error("failed to unregister key callback");
}

void InputHandler::RegisterTextCallback(void(*callback)(key_state state, void* data), void* data)
{
	std::unique_lock lock(m_KeyTextMutex);

	m_TextCallbacks.push_back(callback);
	m_TextDatas.push_back(data);
}

void InputHandler::UnregisterTextCallback(void(*callback)(key_state state, void* data))
{
	std::unique_lock lock(m_KeyTextMutex);

	for (usize i = 0; i < m_TextCallbacks.size(); i++)
	{
		if (m_TextCallbacks[i] == callback)
		{
			m_TextCallbacks.erase(m_TextCallbacks.begin() + i);
			m_TextDatas.erase(m_TextDatas.begin() + i);
			return;
		}
	}

	Logger::Error("failed to unregister text callback");
}

void InputHandler::RegisterMouseCallback(void(*callback)(glm::vec<2, i32> position, f32 scroll, MouseOperation operation, void* data), void* data)
{
	std::unique_lock lock(m_MouseMutex);

	m_MouseCallbacks.push_back(callback);
	m_MouseDatas.push_back(data);
}

void InputHandler::UnregisterMouseCallback(void(*callback)(glm::vec<2, i32> position, f32 scroll, MouseOperation operation, void* data))
{
	std::unique_lock lock(m_MouseMutex);

	for (usize i = 0; i < m_MouseCallbacks.size(); i++)
	{
		if (m_MouseCallbacks[i] == callback)
		{
			m_MouseCallbacks.erase(m_MouseCallbacks.begin() + i);
			m_MouseDatas.erase(m_MouseDatas.begin() + i);
			return;
		}
	}

	Logger::Error("failed to unregister mouse callback");
}

void InputHandler::RegisterBullishMouseCallback(bool(*callback)(glm::vec<2, i32> position, f32 scroll, MouseOperation operation, void* data), u8 layer, void* data)
{
	std::unique_lock lock(m_MouseMutex);

	if (layer >= m_BullishMouseCallbacks.size())
	{
		for (u8 i = 0; i < layer - m_BullishMouseCallbacks.size() + 1; i++)
		{
			m_BullishMouseCallbacks.push_back(std::vector<bool(*)(glm::vec<2, i32> position, f32 scroll, MouseOperation operation, void* data)>());
			m_BullishMouseDatas.push_back(std::vector<void*>());
		}
	}

	m_BullishMouseCallbacks[layer].push_back(callback);
	m_BullishMouseDatas[layer].push_back(data);
}

void InputHandler::UnregisterBullishMouseCallback(bool(*callback)(glm::vec<2, i32> position, f32 scroll, MouseOperation operation, void* data), u8 layer)
{
	std::unique_lock lock(m_MouseMutex);

	for (usize i = 0; i < m_BullishMouseCallbacks[layer].size(); i++)
	{
		if (m_BullishMouseCallbacks[layer][i] == callback)
		{
			m_BullishMouseCallbacks[layer].erase(m_BullishMouseCallbacks[layer].begin() + i);
			m_BullishMouseDatas[layer].erase(m_BullishMouseDatas[layer].begin() + i);
			return;
		}
	}

	Logger::Error("failed to unregister bullish mouse callback");
}

void InputHandler::FireKey(i16 key, u8 operation)
{
	std::unique_lock lock(m_KeyTextMutex);

	if (key == GLFW_KEY_LEFT_SHIFT)
	{
		if (operation == GLFW_PRESS)
			m_ShiftDown = true;
		else if (operation == GLFW_RELEASE)
			m_ShiftDown = false;
	}

	if (key == GLFW_KEY_LEFT_CONTROL)
	{
		if (operation == GLFW_PRESS)
			m_ControlDown = true;
		else if (operation == GLFW_RELEASE)
			m_ControlDown = false;
	}

	if (key == GLFW_KEY_LEFT_ALT)
	{
		if (operation == GLFW_PRESS)
			m_AltDown = true;
		else if (operation == GLFW_RELEASE)
			m_AltDown = false;
	}

	if (operation == GLFW_PRESS)
	{
		for (usize i = 0; i < m_KeyCallbacks.size(); i++)
			m_KeyCallbacks[i]({ key, true, m_ShiftDown, m_ControlDown, m_AltDown }, m_KeyDatas[i]);

		for (usize i = 0; i < m_TextCallbacks.size(); i++)
			m_TextCallbacks[i]({ key, true, m_ShiftDown, m_ControlDown, m_AltDown }, m_TextDatas[i]);
	}
	else if (operation == GLFW_RELEASE)
	{
		for (usize i = 0; i < m_KeyCallbacks.size(); i++)
			m_KeyCallbacks[i]({ key, false, m_ShiftDown, m_ControlDown, m_AltDown }, m_KeyDatas[i]);
	}
	else if (operation == GLFW_REPEAT)
	{
		for (usize i = 0; i < m_TextCallbacks.size(); i++)
			m_TextCallbacks[i]({ key, true, m_ShiftDown, m_ControlDown, m_AltDown }, m_TextDatas[i]);
	}
}

void InputHandler::FireMouseButton(u8 button, u8 operation)
{
	if (button != GLFW_MOUSE_BUTTON_LEFT)
		return;

	std::unique_lock lock(m_MouseMutex);

	if (operation == GLFW_PRESS)
	{
		TIME_POINT now = std::chrono::high_resolution_clock::now();

		if ((m_PressLog.size() > 0 && std::chrono::duration_cast<std::chrono::milliseconds>(now - m_PressLog.back()).count() > 300) || m_PressLog.size() >= 3)
			m_PressLog.clear();

		m_PressLog.push_back(now);

		if (m_PressLog.size() == 1)
		{
			for (usize i = 0; i < m_MouseCallbacks.size(); i++)
				m_MouseCallbacks[i](m_MousePosition, 0.0f, MouseOperation::Press, m_MouseDatas[i]);
		}
		else if (m_PressLog.size() == 2)
		{
			for (usize i = 0; i < m_MouseCallbacks.size(); i++)
				m_MouseCallbacks[i](m_MousePosition, 0.0f, MouseOperation::DoublePress, m_MouseDatas[i]);
		}
		else if (m_PressLog.size() == 3)
		{
			for (usize i = 0; i < m_MouseCallbacks.size(); i++)
				m_MouseCallbacks[i](m_MousePosition, 0.0f, MouseOperation::TripplePress, m_MouseDatas[i]);
		}
	}
	else if (operation == GLFW_RELEASE)
	{
		for (usize i = 0; i < m_MouseCallbacks.size(); i++)
			m_MouseCallbacks[i](m_MousePosition, 0.0f, MouseOperation::Release, m_MouseDatas[i]);

		for (usize i = 0; i < m_MouseCallbacks.size(); i++)
			m_MouseCallbacks[i](m_MousePosition, 0.0f, MouseOperation::Click, m_MouseDatas[i]);
	}
}

void InputHandler::FireMouseMove(glm::vec<2, i32> position)
{
	std::unique_lock lock(m_MouseMutex);
	m_MousePosition = position;

	for (usize i = 0; i < m_MouseCallbacks.size(); i++)
		m_MouseCallbacks[i](position, 0.0f, MouseOperation::Move, m_MouseDatas[i]);

	for (u8 i = 0; i < m_BullishMouseCallbacks.size(); i++)
	{
		for (usize a = 0; a < m_BullishMouseCallbacks[static_cast<usize>(i)].size(); a++)
		{
			if (m_BullishMouseCallbacks[i][a](position, 0.0f, MouseOperation::Move, m_BullishMouseDatas[i][a]))
				return;
		}
	}
}

void InputHandler::FireMouseScroll(f32 scroll)
{
	std::unique_lock lock(m_MouseMutex);

	for (usize i = 0; i < m_MouseCallbacks.size(); i++)
		m_MouseCallbacks[i](m_MousePosition, scroll, MouseOperation::Scroll, m_MouseDatas[i]);
}

glm::vec<2, i32> InputHandler::GetMousePosition()
{
	return m_MousePosition;
}

u8 InputHandler::BullishLayerPopups = 10;
u8 InputHandler::BullishLayerUI = 15;
u8 InputHandler::BullishLayerArguments = 20;
u8 InputHandler::BullishLayerBackground = 25;

bool InputHandler::m_MouseWasDown;
bool InputHandler::m_ShiftDown;
bool InputHandler::m_ControlDown;
bool InputHandler::m_AltDown;

glm::vec<2, i32> InputHandler::m_MousePosition;
std::shared_mutex InputHandler::m_KeyTextMutex;
std::shared_mutex InputHandler::m_MouseMutex;

std::vector<void(*)(InputHandler::key_state state, void* data)> InputHandler::m_KeyCallbacks;
std::vector<void*> InputHandler::m_KeyDatas;
std::vector<void(*)(InputHandler::key_state state, void* data)> InputHandler::m_TextCallbacks;
std::vector<void*> InputHandler::m_TextDatas;
std::vector<void(*)(glm::vec<2, i32> position, f32 scroll, MouseOperation operation, void* data)> InputHandler::m_MouseCallbacks;
std::vector<void*> InputHandler::m_MouseDatas;
std::vector<std::vector<bool(*)(glm::vec<2, i32> position, f32 scroll, MouseOperation operation, void* data)>> InputHandler::m_BullishMouseCallbacks;
std::vector<std::vector<void*>> InputHandler::m_BullishMouseDatas;
std::vector<TIME_POINT> InputHandler::m_PressLog;
