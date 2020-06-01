#include "MessageHandler.h"
#include "InputHandler.h"
#include "RHR/registries/UIRegistry.h"

#include <Espresso/Global.h>

void ThreadMessage()
{
	std::vector<Message*>& messages = MessageHandler::GetMessages();
	std::mutex& messageMutex = MessageHandler::GetMessageMutex();
	std::atomic<bool>& finish = MessageHandler::GetFinished();

	while (true)
	{
		messageMutex.lock();

		while (messages.size() == 0)
		{
			messageMutex.unlock();
			std::this_thread::sleep_for(std::chrono::milliseconds(100));

			if (finish)
				return;

			messageMutex.lock();
		}

	skipMutex:

		sf::RenderWindow window;
		window.create(sf::VideoMode(800, 100), "", sf::Style::Titlebar);
		window.setVerticalSyncEnabled(false);
		window.setFramerateLimit(60);
		window.clear(MOD_BACKGROUND_LOW);
		window.display();

		std::function<void()> ubShutdown = []()
		{
			abort();
		};

		std::function<void()> windowClose = [&window]()
		{
			window.close();
		};

		ButtonText* button = new ButtonText(&ubShutdown, "abort", 16, sf::Vector2f(100, 20), MOD_VAR, sf::Color::Black);
		button->setPosition(10, 10);

		messages.back()->SetClose(&windowClose);

		while (window.isOpen())
		{
			sf::Event ev;

			while (window.pollEvent(ev))
			{
				if (ev.type == sf::Event::MouseButtonPressed)
				{
					button->mouseButton(true, sf::Mouse::getPosition(window), sf::Mouse::Left);
					messages.back()->mouseButton(true, sf::Mouse::getPosition(window), sf::Mouse::Left);
				}
				else if (ev.type == sf::Event::MouseButtonReleased)
				{
					button->mouseButton(false, sf::Mouse::getPosition(window), sf::Mouse::Left);
					messages.back()->mouseButton(false, sf::Mouse::getPosition(window), sf::Mouse::Left);
				}
				else if (ev.type == sf::Event::EventType::KeyPressed) {
					InputHandler::FireKeyEvent(ev.key);
				}
				else if (ev.type == sf::Event::EventType::TextEntered) {
					InputHandler::FireTextEvent(ev.text);
				}
			}

			if (window.hasFocus())
			{
				window.clear(MOD_BACKGROUND_LOW);

				messages.back()->frameUpdate(0.0);
				window.draw(*messages.back());

				button->frameUpdate(0.0);
				window.draw(*button);

				window.display();
			}

			if (finish)
			{
				messageMutex.unlock();
				return;
			}
		}

		delete button;
		delete messages.back();

		messages.pop_back();
		
		if (messages.size() > 0)
			goto skipMutex;

		messageMutex.unlock();
	}
}

void MessageHandler::Initialize()
{
	m_messageThread = std::thread(ThreadMessage);
}

void MessageHandler::Finish()
{
	m_finish = true;
	m_messageThread.join();
}

void MessageHandler::RegisterMessage(Message* message, bool sync)
{
	if (sync)
	{
		m_messageMutex.lock();
		m_messages.push_back(message);

		while (m_messages.size() > 0)
		{
			m_messageMutex.unlock();
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			m_messageMutex.lock();
		}

		m_messageMutex.unlock();
	}
	else
	{
		std::unique_lock<std::mutex> lock(m_messageMutex);
		m_messages.push_back(message);
	}
}

std::mutex& MessageHandler::GetMessageMutex()
{
	return m_messageMutex;
}

std::vector<Message*>& MessageHandler::GetMessages()
{
	return m_messages;
}

std::atomic<bool>& MessageHandler::GetFinished()
{
	return m_finish;
}

std::atomic<bool> MessageHandler::m_finish;
std::mutex MessageHandler::m_messageMutex;
std::vector<Message*> MessageHandler::m_messages;
std::thread MessageHandler::m_messageThread;

void Message::SetClose(std::function<void()>* stop)
{
	m_stop = stop;
}

MessageInfo::MessageInfo(const std::string& message, std::function<void()>* cb)
	:Message()
{
	m_message = sf::Text("[Info] " + message, Global::Font, 16);
	m_message.setPosition(sf::Vector2f(10, 35));
	m_message.setFillColor(MOD_BUTTON_TEXT_FG);

	m_callback = cb;

	m_buttonCallback = [this]()
	{
		(*m_callback)();
		(*m_stop)();
	};

	m_buttonOk = new ButtonText(&m_buttonCallback, "ok", 16, sf::Vector2f(100, 20), MOD_BUTTON_TEXT_BG, MOD_BUTTON_TEXT_FG);
	m_buttonOk->setPosition(120, 10);

	UIRegistry::GetRegistry().AddComponent(m_buttonOk);
}

MessageInfo::~MessageInfo()
{
	UIRegistry::GetRegistry().RemoveComponent(m_buttonOk);
	delete m_buttonOk;
}

void MessageInfo::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_message);
}

MessageWarning::MessageWarning(const std::string& message, std::function<void()>* cb)
	:Message()
{
	m_message = sf::Text("[Warn] " + message, Global::Font, 16);
	m_message.setPosition(sf::Vector2f(10, 35));
	m_message.setFillColor(MOD_BUTTON_TEXT_FG);

	m_callback = cb;

	m_buttonCallback = [this]()
	{
		(*m_callback)();
		(*m_stop)();
	};

	m_buttonOk = new ButtonText(&m_buttonCallback, "ok", 16, sf::Vector2f(100, 20), MOD_BUTTON_TEXT_BG, MOD_BUTTON_TEXT_FG);
	m_buttonOk->setPosition(120, 10);

	UIRegistry::GetRegistry().AddComponent(m_buttonOk);
}

MessageWarning::~MessageWarning()
{
	UIRegistry::GetRegistry().RemoveComponent(m_buttonOk);
	delete m_buttonOk;
}

void MessageWarning::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_message);
}

MessageError::MessageError(const std::string& message, std::function<void()>* cb)
	:Message()
{
	m_message = sf::Text("[Error] " + message, Global::Font, 16);
	m_message.setPosition(sf::Vector2f(10, 35));
	m_message.setFillColor(MOD_BUTTON_TEXT_FG);

	m_callback = cb;

	m_buttonCallback = [this]()
	{
		(*m_callback)();
		(*m_stop)();
	};

	m_buttonOk = new ButtonText(&m_buttonCallback, "ok", 16, sf::Vector2f(100, 20), MOD_BUTTON_TEXT_BG, MOD_BUTTON_TEXT_FG);
	m_buttonOk->setPosition(120, 10);

	UIRegistry::GetRegistry().AddComponent(m_buttonOk);
}

MessageError::~MessageError()
{
	UIRegistry::GetRegistry().RemoveComponent(m_buttonOk);
	delete m_buttonOk;
}

void MessageError::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_message);
}

MessageConfirm::MessageConfirm(const std::string& message, std::function<void(bool)>* cb)
	:Message()
{
	m_message = sf::Text(message, Global::Font, 16);
	m_message.setPosition(sf::Vector2f(10, 35));
	m_message.setFillColor(MOD_BUTTON_TEXT_FG);

	m_callback = cb;

	m_buttonCallbackContinue = [this]()
	{
		(*m_callback)(true);
		(*m_stop)();
	};

	m_buttonCallbackCancel = [this]()
	{
		(*m_callback)(false);
		(*m_stop)();
	};

	m_buttonContinue = new ButtonText(&m_buttonCallbackContinue, "continue", 16, sf::Vector2f(100, 20), MOD_BUTTON_TEXT_BG, MOD_BUTTON_TEXT_FG);
	m_buttonContinue->setPosition(120, 10);

	m_buttonCancel = new ButtonText(&m_buttonCallbackCancel, "cancel", 16, sf::Vector2f(100, 20), MOD_BUTTON_TEXT_BG, MOD_BUTTON_TEXT_FG);
	m_buttonCancel->setPosition(230, 10);

	UIRegistry::GetRegistry().AddComponent(m_buttonContinue);
	UIRegistry::GetRegistry().AddComponent(m_buttonCancel);
}

MessageConfirm::~MessageConfirm()
{
	UIRegistry::GetRegistry().RemoveComponent(m_buttonContinue);
	UIRegistry::GetRegistry().RemoveComponent(m_buttonCancel);

	delete m_buttonContinue;
	delete m_buttonCancel;
}

void MessageConfirm::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_message);
}

MessageInput::MessageInput(const std::string& message, std::function<void(const std::string&)>* cb)
	:Message(), m_field(message, false, FieldType::TEXT)
{
	m_field.setPosition(10, 35);

	m_message = sf::Text(message, Global::Font, 16);
	m_message.setPosition(sf::Vector2f(10, 35));
	m_message.setFillColor(MOD_BUTTON_TEXT_FG);

	m_callback = cb;

	m_enter = [this]()
	{
		(*m_callback)(m_field.GetText());
		(*m_stop)();
	};

	m_escape = [this]()
	{
		(*m_callback)("");
		(*m_stop)();
	};

	m_textCallback = [this](const sf::Event::KeyEvent& ev)
	{
		if (ev.code == sf::Keyboard::Key::Enter)
			m_enter();
		else if (ev.code == sf::Keyboard::Key::Escape)
			m_escape();
	};

	InputHandler::RegisterKeyCallback(&m_textCallback);

	m_buttonEnter = new ButtonText(&m_enter, "enter", 16, sf::Vector2f(100, 20), MOD_BUTTON_TEXT_BG, MOD_BUTTON_TEXT_FG);
	m_buttonEnter->setPosition(120, 10);

	m_buttonCancel = new ButtonText(&m_escape, "cancel", 16, sf::Vector2f(100, 20), MOD_BUTTON_TEXT_BG, MOD_BUTTON_TEXT_FG);
	m_buttonCancel->setPosition(230, 10);

	UIRegistry::GetRegistry().AddComponent(m_buttonEnter);
	UIRegistry::GetRegistry().AddComponent(m_buttonCancel);
	UIRegistry::GetRegistry().AddComponent(&m_field);
}

MessageInput::~MessageInput()
{
	UIRegistry::GetRegistry().RemoveComponent(m_buttonEnter);
	UIRegistry::GetRegistry().RemoveComponent(m_buttonCancel);
	UIRegistry::GetRegistry().RemoveComponent(&m_field);

	delete m_buttonEnter;
	delete m_buttonCancel;
}

void MessageInput::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_message);
}
