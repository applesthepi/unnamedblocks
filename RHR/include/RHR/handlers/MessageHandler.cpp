#include "MessageHandler.h"
#include "RHR/Global.h"
#include "InputHandler.h"
#include "RHR/registries/ButtonRegistry.h"

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

		std::function<void()> ubShutdown = []()
		{
			abort();
		};

		std::function<void()> windowClose = [&window]()
		{
			window.close();
		};

		Button* button = new Button(sf::Vector2i(10, 10), sf::Vector2u(100, 20), &ubShutdown);
		button->SetButtonModeText("abort", MOD_BUTTON_TEXT_BG, MOD_BUTTON_TEXT_FG, 16);

		messages.back()->SetClose(&windowClose);

		while (window.isOpen())
		{
			sf::Event ev;

			while (window.pollEvent(ev))
			{
				if (ev.type == sf::Event::MouseButtonPressed)
				{
					button->MouseButton(true, sf::Mouse::getPosition(window), sf::Mouse::Left);
					messages.back()->MouseUpdate(true, sf::Mouse::getPosition(window), sf::Mouse::Left);
				}
				else if (ev.type == sf::Event::MouseButtonReleased)
				{
					button->MouseButton(false, sf::Mouse::getPosition(window), sf::Mouse::Left);
					messages.back()->MouseUpdate(false, sf::Mouse::getPosition(window), sf::Mouse::Left);
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

				messages.back()->FrameUpdate(window);
				messages.back()->Render(window);

				button->FrameUpdate(&window);
				button->Render(&window);

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

void MessageHandler::RegisterMessage(Message* message, const bool& sync)
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

Message::Message()
{
	Stop = new std::function<void()>();
}

Message::~Message()
{
	delete Stop;
}

void Message::SetClose(std::function<void()>* stop)
{
	*Stop = *stop;
}

void Message::FrameUpdate(sf::RenderWindow& window)
{
	
}

void Message::Render(sf::RenderWindow& window)
{

}

void Message::MouseUpdate(const bool& down, const sf::Vector2i& pos, const sf::Mouse::Button& button)
{

}

MessageInfo::MessageInfo(const std::string& message, std::function<void()>* cb)
	:Message()
{
	m_message = sf::Text("[Info] " + message, *Global::Font, 16);
	m_message.setPosition(sf::Vector2f(10, 35));
	m_message.setFillColor(MOD_BUTTON_TEXT_FG);

	Callback = cb;

	m_buttonCallback = [this]()
	{
		(*Callback)();
		(*Stop)();
	};

	m_buttonOk = new Button(sf::Vector2i(120, 10), sf::Vector2u(100, 20), &m_buttonCallback);
	m_buttonOk->SetButtonModeText("ok", MOD_BUTTON_TEXT_BG, MOD_BUTTON_TEXT_FG, 16);
}

MessageInfo::~MessageInfo()
{
	delete m_buttonOk;
}

void MessageInfo::FrameUpdate(sf::RenderWindow& window)
{
	m_buttonOk->FrameUpdate(&window);
	window.setTitle("Info");
}

void MessageInfo::Render(sf::RenderWindow& window)
{
	window.draw(m_message);
	m_buttonOk->Render(&window);
}

void MessageInfo::MouseUpdate(const bool& down, const sf::Vector2i& pos, const sf::Mouse::Button& button)
{
	m_buttonOk->MouseButton(down, pos, button);
}

MessageWarning::MessageWarning(const std::string& message, std::function<void()>* cb)
	:Message()
{
	m_message = sf::Text("[Warn] " + message, *Global::Font, 16);
	m_message.setPosition(sf::Vector2f(10, 35));
	m_message.setFillColor(MOD_BUTTON_TEXT_FG);

	Callback = cb;

	m_buttonCallback = [this]()
	{
		(*Callback)();
		(*Stop)();
	};

	m_buttonOk = new Button(sf::Vector2i(120, 10), sf::Vector2u(100, 20), &m_buttonCallback);
	m_buttonOk->SetButtonModeText("ok", MOD_BUTTON_TEXT_BG, MOD_BUTTON_TEXT_FG, 16);
}

MessageWarning::~MessageWarning()
{
	delete m_buttonOk;
}

void MessageWarning::FrameUpdate(sf::RenderWindow& window)
{
	m_buttonOk->FrameUpdate(&window);
	window.setTitle("Warning");
}

void MessageWarning::Render(sf::RenderWindow& window)
{
	window.draw(m_message);
	m_buttonOk->Render(&window);
}

void MessageWarning::MouseUpdate(const bool& down, const sf::Vector2i& pos, const sf::Mouse::Button& button)
{
	m_buttonOk->MouseButton(down, pos, button);
}

MessageError::MessageError(const std::string& message, std::function<void()>* cb)
	:Message()
{
	m_message = sf::Text("[ERROR] " + message, *Global::Font, 16);
	m_message.setPosition(sf::Vector2f(10, 35));
	m_message.setFillColor(MOD_BUTTON_TEXT_FG);

	Callback = cb;

	m_buttonCallback = [this]()
	{
		(*Callback)();
		(*Stop)();
	};

	m_buttonOk = new Button(sf::Vector2i(120, 10), sf::Vector2u(100, 20), &m_buttonCallback);
	m_buttonOk->SetButtonModeText("ok", MOD_BUTTON_TEXT_BG, MOD_BUTTON_TEXT_FG, 16);
}

MessageError::~MessageError()
{
	delete m_buttonOk;
}

void MessageError::FrameUpdate(sf::RenderWindow& window)
{
	m_buttonOk->FrameUpdate(&window);
	window.setTitle("ERROR");
}

void MessageError::Render(sf::RenderWindow& window)
{
	window.draw(m_message);
	m_buttonOk->Render(&window);
}

void MessageError::MouseUpdate(const bool& down, const sf::Vector2i& pos, const sf::Mouse::Button& button)
{
	m_buttonOk->MouseButton(down, pos, button);
}

MessageConfirm::MessageConfirm(const std::string& message, std::function<void(const bool&)>* cb)
	:Message()
{
	m_message = sf::Text(message, *Global::Font, 16);
	m_message.setPosition(sf::Vector2f(10, 35));
	m_message.setFillColor(MOD_BUTTON_TEXT_FG);

	Callback = cb;

	m_buttonCallbackContinue = [this]()
	{
		(*Callback)(true);
		(*Stop)();
	};

	m_buttonCallbackCancel = [this]()
	{
		(*Callback)(false);
		(*Stop)();
	};

	m_buttonContinue = new Button(sf::Vector2i(120, 10), sf::Vector2u(100, 20), &m_buttonCallbackContinue);
	m_buttonContinue->SetButtonModeText("continue", MOD_BUTTON_TEXT_BG, MOD_BUTTON_TEXT_FG, 16);

	m_buttonCancel = new Button(sf::Vector2i(230, 10), sf::Vector2u(100, 20), &m_buttonCallbackCancel);
	m_buttonCancel->SetButtonModeText("cancel", MOD_BUTTON_TEXT_BG, MOD_BUTTON_TEXT_FG, 16);
}

MessageConfirm::~MessageConfirm()
{
	delete m_buttonContinue;
	delete m_buttonCancel;
}

void MessageConfirm::FrameUpdate(sf::RenderWindow& window)
{
	m_buttonContinue->FrameUpdate(&window);
	m_buttonCancel->FrameUpdate(&window);
}

void MessageConfirm::Render(sf::RenderWindow& window)
{
	window.draw(m_message);
	m_buttonContinue->Render(&window);
	m_buttonCancel->Render(&window);
}

void MessageConfirm::MouseUpdate(const bool& down, const sf::Vector2i& pos, const sf::Mouse::Button& button)
{
	m_buttonContinue->MouseButton(down, pos, button);
	m_buttonCancel->MouseButton(down, pos, button);
}

MessageInput::MessageInput(const std::string& message, std::function<void(const std::string&)>* cb)
	:Message()
{
	m_message = sf::Text(message, *Global::Font, 16);
	m_message.setPosition(sf::Vector2f(10, 35));
	m_message.setFillColor(MOD_BUTTON_TEXT_FG);

	Callback = cb;
	m_textLoc = 0;
	m_textLocHigh = 0;

	m_buttonCallbackEnter = [this]()
	{
		(*Callback)(m_text);
		(*Stop)();
	};

	m_inputEscape = [this]()
	{
		m_text = "";
		m_buttonCallbackEnter();
	};

	m_textCallback = [this](const sf::Event::KeyEvent& ev)
	{
		InputHandler::RunTextProccess(&m_text, &m_textLocHigh, &m_textLoc, &m_buttonCallbackEnter, &m_inputEscape, ev);
	};

	m_buttonEnter = new Button(sf::Vector2i(120, 10), sf::Vector2u(100, 20), &m_buttonCallbackEnter);
	m_buttonEnter->SetButtonModeText("enter", MOD_BUTTON_TEXT_BG, MOD_BUTTON_TEXT_FG, 16);

	m_buttonCancel = new Button(sf::Vector2i(230, 10), sf::Vector2u(100, 20), &m_inputEscape);
	m_buttonCancel->SetButtonModeText("cancel", MOD_BUTTON_TEXT_BG, MOD_BUTTON_TEXT_FG, 16);

	m_inputBackground = sf::RectangleShape(sf::Vector2f(780, 24));
	m_inputBackground.setPosition(sf::Vector2f(10, 58));
	m_inputBackground.setFillColor(MOD_BUTTON_TEXT_BG);

	m_inputLoc = sf::RectangleShape(sf::Vector2f(1, 16));
	m_inputLoc.setFillColor(MOD_BUTTON_TEXT_FG);

	m_inputLocHigh = sf::RectangleShape(sf::Vector2f(0, 16));
	m_inputLocHigh.setFillColor(MOD_HIGHLIGHT_COLOR);

	m_input = sf::Text("", *Global::Font, 16);
	m_input.setPosition(10, 61);
	m_input.setFillColor(MOD_BUTTON_TEXT_FG);

	InputHandler::RegisterKeyCallback(&m_textCallback);
}

MessageInput::~MessageInput()
{
	delete m_buttonEnter;
	InputHandler::UnregisterKeyCallback(&m_textCallback);
}

void MessageInput::FrameUpdate(sf::RenderWindow& window)
{
	m_buttonEnter->FrameUpdate(&window);
	m_buttonCancel->FrameUpdate(&window);

	m_input.setString(m_text);

	m_inputLoc.setPosition(sf::Text(m_text.substr(0, m_textLoc), *Global::Font, 16).getLocalBounds().width + m_input.getPosition().x, m_input.getPosition().y + 2);

	m_inputLocHigh.setPosition(sf::Text(m_text.substr(0, std::min(m_textLocHigh, m_textLoc)), *Global::Font, 16).getLocalBounds().width + m_input.getPosition().x, m_input.getPosition().y + 2);
	m_inputLocHigh.setSize(sf::Vector2f(sf::Text(m_text.substr(std::min(m_textLocHigh, m_textLoc), std::max(m_textLocHigh, m_textLoc) - std::min(m_textLocHigh, m_textLoc)), *Global::Font, 16).getLocalBounds().width, 16));
}

void MessageInput::Render(sf::RenderWindow& window)
{
	window.draw(m_message);
	window.draw(m_inputBackground);
	window.draw(m_input);
	window.draw(m_inputLocHigh);
	window.draw(m_inputLoc);

	m_buttonEnter->Render(&window);
	m_buttonCancel->Render(&window);
}

void MessageInput::MouseUpdate(const bool& down, const sf::Vector2i& pos, const sf::Mouse::Button& button)
{
	m_buttonEnter->MouseButton(down, pos, button);
	m_buttonCancel->MouseButton(down, pos, button);
}