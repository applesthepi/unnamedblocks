#include "MessageHandler.h"
#include "Global.h"
#include "ui/TypingSystem.h"
#include "ui/Button.h"

#include <SFML/Graphics.hpp>
#include <functional>
void ThreadAsyncMessage(std::mutex* mutex, std::vector<Message*>* messages)
{
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
	for (unsigned int i = 0; i < m_messages->size(); i++)
	{
		std::function<void(int key)>* callback = new std::function<void(int key)>();
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

			*callback = [&answer, &textAgent, &finish](int key)
			{
				if (key == 8)
				{
					if (answer.length() > 0)
					{
						std::string newText = std::string();

						for (unsigned int i = 0; i < answer.length() - 1; i++)
							newText += answer[i];

						answer = newText;
					}
				}
				else if (key == 13)
				{
					finish = true;
				}
				else if (key != 129 && key != 130 && key != 127 && key != -2 && key != -4 && key != 10)
				{
					answer += (char)key;
				}

				textAgent.setString(answer);
			};

			TypingSystem::AddKeypressRegister(callback);
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

			buttonContinue->SetButtonModeText("continue", sf::Color(140, 212, 140), 16);
			buttonCancel->SetButtonModeText("cancel", sf::Color(140, 212, 212), 16);
		}

		sf::RenderWindow window;
		if ((*m_messages)[i]->Type == MessageType::INPUT)
			window.create(sf::VideoMode(500, 50), title, sf::Style::Close | sf::Style::Titlebar);
		else if ((*m_messages)[i]->Type == MessageType::CONFIRM)
			window.create(sf::VideoMode(500, 60), title, sf::Style::Titlebar);
		else
			window.create(sf::VideoMode(500, 24), title, sf::Style::Close | sf::Style::Titlebar);

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

			TypingSystem::Update();

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
			else if ((*m_messages)[i]->Type == MessageType::CONFIRM)
			{
				buttonContinue->FrameUpdate(&window);
				buttonContinue->Render(&window);

				buttonCancel->FrameUpdate(&window);
				buttonCancel->Render(&window);
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

			TypingSystem::RemoveKeypressRegister(callback);
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
