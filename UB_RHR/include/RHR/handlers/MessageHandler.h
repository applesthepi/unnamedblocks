#pragma once
#include "RHR/ui/ButtonText.h"
#include "RHR/ui/Field.h"
#include "RHR/ui/IRenderable.h"
#include "RHR/ui/IMouseUpdatable.h"

#include <vector>
#include <thread>
#include <mutex>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <functional>
#include <atomic>

class Message : public IRenderable, public IMouseUpdatable
{
public:
	void SetClose(std::function<void()>* stop);
protected:
	std::function<void()>* m_stop;
};

class MessageInfo : public Message
{
public:
	MessageInfo(const std::string& message, std::function<void()>* cb);
	~MessageInfo();
protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	std::function<void()>* m_callback;
	std::function<void()> m_buttonCallback;
	ButtonText* m_buttonOk;
	sf::Text m_message;
};

class MessageWarning : public Message
{
public:
	MessageWarning(const std::string& message, std::function<void()>* cb);
	~MessageWarning();
protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	std::function<void()>* m_callback;
	std::function<void()> m_buttonCallback;
	ButtonText* m_buttonOk;
	sf::Text m_message;
};

class MessageError : public Message
{
public:
	MessageError(const std::string& message, std::function<void()>* cb);
	~MessageError();
protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	std::function<void()>* m_callback;
	std::function<void()> m_buttonCallback;
	ButtonText* m_buttonOk;
	sf::Text m_message;
};

class MessageConfirm : public Message
{
public:
	MessageConfirm(const std::string& message, std::function<void(bool)>* cb);
	~MessageConfirm();
protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	std::function<void(bool)>* m_callback;
	std::function<void()> m_buttonCallbackContinue;
	std::function<void()> m_buttonCallbackCancel;

	ButtonText* m_buttonContinue;
	ButtonText* m_buttonCancel;

	sf::Text m_message;
};

class MessageInput : public Message
{
public:
	MessageInput(const std::string& message, std::function<void(const std::string&)>* cb);
	~MessageInput();
protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	std::function<void(const std::string&)>* m_callback;
	std::function<void(const sf::Event::KeyEvent&)> m_textCallback;
	
	std::function<void()> m_escape;
	std::function<void()> m_enter;

	ButtonText* m_buttonEnter;
	ButtonText* m_buttonCancel;

	Field m_field;
	sf::Text m_message;
};

class MessageHandler
{
public:
	static void Initialize();
	static void Finish();
	static void RegisterMessage(Message* message, bool sync = false);

	static std::mutex& GetMessageMutex();
	static std::vector<Message*>& GetMessages();
	static std::atomic<bool>& GetFinished();
private:
	static std::atomic<bool> m_finish;
	static std::mutex m_messageMutex;
	static std::vector<Message*> m_messages;
	static std::thread m_messageThread;
};