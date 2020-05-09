#pragma once
#include "RHR/ui/Button.h"

#include <vector>
#include <thread>
#include <mutex>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <functional>
#include <atomic>

class Message
{
public:
	Message();
	~Message();

	void SetClose(std::function<void()>* stop);

	virtual void FrameUpdate(sf::RenderWindow& window);
	virtual void Render(sf::RenderWindow& window);
	virtual void MouseUpdate(const bool& down, const sf::Vector2i& pos, const sf::Mouse::Button& button);

	std::function<void()>* Stop;
};

class MessageInfo : public Message
{
public:
	MessageInfo(const std::string& message, std::function<void()>* cb);
	~MessageInfo();

	void FrameUpdate(sf::RenderWindow& window) override;
	void Render(sf::RenderWindow& window) override;
	void MouseUpdate(const bool& down, const sf::Vector2i& pos, const sf::Mouse::Button& button) override;

	std::function<void()>* Callback;
private:
	std::function<void()> m_buttonCallback;
	Button* m_buttonOk;
	sf::Text m_message;
};

class MessageWarning : public Message
{
public:
	MessageWarning(const std::string& message, std::function<void()>* cb);
	~MessageWarning();

	void FrameUpdate(sf::RenderWindow& window) override;
	void Render(sf::RenderWindow& window) override;
	void MouseUpdate(const bool& down, const sf::Vector2i& pos, const sf::Mouse::Button& button) override;

	std::function<void()>* Callback;
private:
	std::function<void()> m_buttonCallback;
	Button* m_buttonOk;
	sf::Text m_message;
};

class MessageError : public Message
{
public:
	MessageError(const std::string& message, std::function<void()>* cb);
	~MessageError();

	void FrameUpdate(sf::RenderWindow& window) override;
	void Render(sf::RenderWindow& window) override;
	void MouseUpdate(const bool& down, const sf::Vector2i& pos, const sf::Mouse::Button& button) override;

	std::function<void()>* Callback;
private:
	std::function<void()> m_buttonCallback;
	Button* m_buttonOk;
	sf::Text m_message;
};

class MessageConfirm : public Message
{
public:
	MessageConfirm(const std::string& message, std::function<void(const bool&)>* cb);
	~MessageConfirm();

	void FrameUpdate(sf::RenderWindow& window) override;
	void Render(sf::RenderWindow& window) override;
	void MouseUpdate(const bool& down, const sf::Vector2i& pos, const sf::Mouse::Button& button) override;

	std::function<void(const bool&)>* Callback;
private:
	std::function<void()> m_buttonCallbackContinue;
	std::function<void()> m_buttonCallbackCancel;
	Button* m_buttonContinue;
	Button* m_buttonCancel;

	sf::Text m_message;
};

class MessageInput : public Message
{
public:
	MessageInput(const std::string& message, std::function<void(const std::string&)>* cb);
	~MessageInput();

	void FrameUpdate(sf::RenderWindow& window) override;
	void Render(sf::RenderWindow& window) override;
	void MouseUpdate(const bool& down, const sf::Vector2i& pos, const sf::Mouse::Button& button) override;

	std::function<void(const std::string&)>* Callback;
private:
	std::function<void(const sf::Event::KeyEvent&)> m_textCallback;
	std::function<void()> m_inputEscape;
	std::function<void()> m_buttonCallbackEnter;
	Button* m_buttonEnter;
	Button* m_buttonCancel;

	sf::Text m_message;

	uint64_t m_textLocHigh;
	uint64_t m_textLoc;
	std::string m_text;
	sf::Text m_input;
	sf::RectangleShape m_inputBackground;
	sf::RectangleShape m_inputLoc;
	sf::RectangleShape m_inputLocHigh;
};

class MessageHandler
{
public:
	static void Initialize();
	static void Finish();
	static void RegisterMessage(Message* message, const bool& sync = false);

	static std::mutex& GetMessageMutex();
	static std::vector<Message*>& GetMessages();
	static std::atomic<bool>& GetFinished();
private:
	static std::atomic<bool> m_finish;
	static std::mutex m_messageMutex;
	static std::vector<Message*> m_messages;
	static std::thread m_messageThread;
};