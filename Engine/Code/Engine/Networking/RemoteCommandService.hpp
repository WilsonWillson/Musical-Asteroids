#pragma once
#include "Engine/Networking/net_address_t.hpp"
#include "Engine/Networking/TCPSession.hpp"
#pragma warning(disable:4996)

class NetConnection;
class SimpleRenderer;
class DX11TextRenderer;

const uint16_t RCS_PORT = 2345;
const uint8_t JOIN_RESPONSE = 0;
const uint8_t SEND_COMMAND = 32;
const uint8_t SEND_MESSAGE = 33;

class RemoteCommandService
{
public:

	void Update();

	void Join(net_address_t  &addr);
	void setupMessageDefinition();
	void send_command_to_others(std::vector<std::string> parsedConsoleCommands);

	void send_message(std::vector<std::string> str);

	net_address_t GetMyAddress();

	void Render(SimpleRenderer* renderer, DX11TextRenderer* m_loadedTextFile);

private:
	RemoteCommandService();
	~RemoteCommandService();

public:

	TCPSession* session;
	NetConnection* current_sender;
	bool		disableEcho;

	static RemoteCommandService* GetInstance() { if (s_instance == nullptr) { s_instance = new RemoteCommandService(); } return s_instance; }

private:
	static RemoteCommandService* s_instance;

};

void on_command(void* user_arg, void* msg_arg);
void on_message(void* user_arg, void* msg_arg);