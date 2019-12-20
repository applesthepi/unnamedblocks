#pragma once

#include <vector>
#include <thread>
#include <mutex>

enum MessageType
{
	INFO, WARNING, ERROR, INPUT, CONFIRM
};

class Message
{
public:
	Message(MessageType type, std::string string);
	Message(std::string question, std::string* result);
	Message(std::string question, bool* result);

	MessageType Type;
	std::string String;
	std::string* Result;
	bool* ResultBool;
};

class MessageHandler
{
public:
	static void Initialize();
	static void RunSyncMessages();
	static void RegisterMessageSync(Message* message);
	static void RegisterMessageAsync(Message* message);
private:
	static std::thread* m_asyncThread;
	static std::mutex* m_asyncMessagesMutex;
	static std::vector<Message*>* m_asyncMessages;
	static std::vector<Message*>* m_messages;
};