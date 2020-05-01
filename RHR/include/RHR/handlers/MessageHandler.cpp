#include "MessageHandler.h"
#include "RHR/Global.h"
#include "InputHandler.h"
#include "RHR/ui/Button.h"
#include "RHR/registries/ButtonRegistry.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <functional>
void ThreadAsyncMessage(std::mutex* mutex, std::vector<Message*>* messages)
{
	//TODO this function is supost to be the same as the sync function
	while (Global::ApplicationRunning)
	{
		mutex->lock();
		while (messages->size() == 0)
		{
			mutex->unlock();
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			mutex->lock();
		}
		
		for (unsigned int i = 0; i < messages->size(); i++)
		{
			std::string title;
			if ((*messages)[i]->Type == MessageType::INFO)
				title = "Info";
			else if ((*messages)[i]->Type == MessageType::WARNING)
				title = "Warning";
			else if ((*messages)[i]->Type == MessageType::ERROR)
				title = "Error";

			sf::RenderWindow window;
			window.create(sf::VideoMode(500, 24), title, sf::Style::Close | sf::Style::Titlebar);

			while (window.isOpen())
			{
				sf::Event ev;
				while (window.pollEvent(ev))
				{
					if (ev.type == sf::Event::Closed)
					{
						window.close();
					}
					else if (ev.type == sf::Event::LostFocus)
					{
						window.requestFocus();
					}
				}

				window.clear(sf::Color::White);

				sf::Text txt = sf::Text((*messages)[i]->String, *Global::Font, 18);
				txt.setFillColor(sf::Color::Black);
				window.draw(txt);

				window.display();

				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}

			delete (*messages)[i];
		}

		messages->clear();

		mutex->unlock();
	}
}

void MessageHandler::Initialize()
{
	m_asyncMessages = new std::vector<Message*>();
	m_messages = new std::vector<Message*>();
	m_asyncMessagesMutex = new std::mutex();

	m_asyncThread = new std::thread(ThreadAsyncMessage, m_asyncMessagesMutex, m_asyncMessages);
	m_asyncThread->detach();
}

void MessageHandler::RunSyncMessages()
{
	//TODO this function has lots of memory leaks
	for (unsigned int i = 0; i < m_messages->size(); i++)
	{
		std::function<void(const sf::Event::TextEvent& event)>* text_callback = new std::function<void(const sf::Event::TextEvent& event)>();
		std::function<void(const sf::Event::KeyEvent& event)>* key_callback = new std::function<void(const sf::Event::KeyEvent& event)>();
		std::function<void()>* callbackButtonContinue = new std::function<void()>();
		std::function<void()>* callbackButtonCancel = new std::function<void()>();
		Button* buttonContinue = new Button(sf::Vector2i(10, 30), sf::Vector2u(100, 24), callbackButtonContinue);
		Button* buttonCancel = new Button(sf::Vector2i(390, 30), sf::Vector2u(100, 24), callbackButtonCancel);
		std::string title;
		std::string answer = std::string();
		bool finish = false;
		bool answerBool = false;
		
		sf::Text textAgent = sf::Text("", *Global::Font, 16);
		sf::RectangleShape rect = sf::RectangleShape(sf::Vector2f(500, 20));
		sf::RectangleShape marker = sf::RectangleShape(sf::Vector2f(2, 20));
		marker.setFillColor(sf::Color::Black);
		rect.setPosition(0, 20);
		rect.setFillColor(sf::Color(220, 220, 220, 255));

		textAgent.setFillColor(sf::Color::Black);
		textAgent.setPosition(0, 20);

		if ((*m_messages)[i]->Type == MessageType::INFO)
			title = "Info";
		else if ((*m_messages)[i]->Type == MessageType::WARNING)
			title = "Warning";
		else if ((*m_messages)[i]->Type == MessageType::ERROR)
			title = "Error";
		else if ((*m_messages)[i]->Type == MessageType::INPUT)
		{
			title = "Input";

			*key_callback = [&answer, &textAgent, &finish](const sf::Event::KeyEvent& event)
			{
				const bool whitespace_is_barrier = true;
				if (event.code == sf::Keyboard::BackSpace)
				{
					if (event.control)
					{
						for (auto iter = answer.end(); iter != answer.begin(); --iter)
						{
							if ( whitespace_is_barrier && isspace(*iter) ) {
								answer.erase(iter, answer.end());
							}
						}
						
					}
					else
					{
						if (answer.length() > 0) answer.erase(answer.end());
					}
					textAgent.setString(answer);
				}
				else if (event.code == sf::Keyboard::Key::Enter)
				{
					finish = true;
				}
				
			};
			*text_callback = [&answer, &textAgent](const sf::Event::TextEvent& event)
			{
				answer += static_cast<char>(event.unicode);
				textAgent.setString(answer);
			};

			InputHandler::RegisterTextCallback(text_callback);
			InputHandler::RegisterKeyCallback(key_callback);
		}
		else if ((*m_messages)[i]->Type == MessageType::CONFIRM)
		{
			title = "Confirm";

			*callbackButtonContinue = [&answerBool, &finish]()
			{
				answerBool = true;
				finish = true;
			};

			*callbackButtonCancel = [&answerBool, &finish]()
			{
				answerBool = false;
				finish = true;
			};

			buttonContinue->SetButtonModeText("continue", MOD_BUTTON_TEXT_BG, MOD_BUTTON_TEXT_FG, 16);
			buttonCancel->SetButtonModeText("cancel", MOD_BUTTON_TEXT_BG, MOD_BUTTON_TEXT_FG, 16);

			ButtonRegistry::Push();
			ButtonRegistry::AddButton(buttonContinue);
			ButtonRegistry::AddButton(buttonCancel);
		}

		sf::RenderWindow window;
		if ((*m_messages)[i]->Type == MessageType::INPUT)
			window.create(sf::VideoMode(500, 50), title, sf::Style::Close | sf::Style::Titlebar);
		else if ((*m_messages)[i]->Type == MessageType::CONFIRM)
			window.create(sf::VideoMode(500, 60), title, sf::Style::Titlebar);
		else
			window.create(sf::VideoMode(500, 24), title, sf::Style::Close | sf::Style::Titlebar);

		bool wasDownLeft = false;

		while (window.isOpen())
		{
			sf::Event ev;
			while (window.pollEvent(ev))
			{
				if (ev.type == sf::Event::Closed)
				{
					finish = false;
					window.close();
				}
				else if (ev.type == sf::Event::LostFocus)
				{
					window.requestFocus();
				}
			}

			window.clear(sf::Color::White);

			Global::MousePosition = sf::Mouse::getPosition(window);

			if ((*m_messages)[i]->Type == MessageType::CONFIRM)
			{
				ButtonRegistry::FrameUpdateUI(&window);

				if (!wasDownLeft && sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					wasDownLeft = true;
					ButtonRegistry::MouseUpdateButtons(sf::Mouse::Left, true);
				}
				else if (wasDownLeft && !sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					wasDownLeft = false;
					ButtonRegistry::MouseUpdateButtons(sf::Mouse::Left, false);
				}

				ButtonRegistry::RenderUI(&window);
			}

			if (finish)
			{
				window.close();
				continue;
			}

			sf::Text txt = sf::Text((*m_messages)[i]->String, *Global::Font, 16);
			txt.setFillColor(sf::Color::Black);
			window.draw(txt);

			if ((*m_messages)[i]->Type == MessageType::INPUT)
			{
				window.draw(rect);
				window.draw(textAgent);

				marker.setPosition(textAgent.getLocalBounds().width, 20);

				window.draw(marker);
			}

			window.display();

			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		}

		if ((*m_messages)[i]->Type == MessageType::INPUT)
		{
			if (finish)
			{
				if ((*m_messages)[i]->Result != nullptr)
					*((*m_messages)[i]->Result) = answer;
			}
			else
			{
				if ((*m_messages)[i]->Result != nullptr)
					*((*m_messages)[i]->Result) = "";
			}

			InputHandler::UnregisterTextCallback(text_callback);
			InputHandler::UnregisterKeyCallback(key_callback);
		}
		else if ((*m_messages)[i]->Type == MessageType::CONFIRM)
		{
			if (finish)
			{
				if ((*m_messages)[i]->ResultBool != nullptr)
					*((*m_messages)[i]->ResultBool) = answerBool;
			}
			else
			{
				if ((*m_messages)[i]->ResultBool != nullptr)
					*((*m_messages)[i]->ResultBool) = false;
			}

			ButtonRegistry::RemoveButton(buttonContinue);
			ButtonRegistry::RemoveButton(buttonCancel);
			ButtonRegistry::Pop();
		}

		delete (*m_messages)[i];
	}

	m_messages->clear();
}

void MessageHandler::RegisterMessageSync(Message* message)
{
	m_messages->push_back(message);
	RunSyncMessages();
}

void MessageHandler::RegisterMessageAsync(Message* message)
{
	m_asyncMessagesMutex->lock();
	m_asyncMessages->push_back(message);
	m_asyncMessagesMutex->unlock();
}

std::thread* MessageHandler::m_asyncThread;

std::mutex* MessageHandler::m_asyncMessagesMutex;

std::vector<Message*>* MessageHandler::m_asyncMessages;

std::vector<Message*>* MessageHandler::m_messages;

Message::Message(MessageType type, std::string string)
{
	Type = type;
	String = string;
	Result = nullptr;
	ResultBool = nullptr;
}

Message::Message(std::string question, std::string* result)
{
	Type = MessageType::INPUT;
	String = question;
	Result = result;
	ResultBool = nullptr;
}

Message::Message(std::string question, bool* result)
{
	Type = MessageType::CONFIRM;
	String = question;
	Result = nullptr;
	ResultBool = result;
}
