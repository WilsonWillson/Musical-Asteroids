#include "Engine/Networking/RemoteCommandService.hpp"
#include "Engine/Networking/NetMessage.hpp"
#include "Engine/Networking/NetMessageDefinition.hpp"
#include "Engine/Networking/NetConnection.hpp"
#include "Engine/Core/Console.hpp"
#include "Engine/Core/Event.hpp"
#include "Engine/Core/Console.hpp"
#include "Engine/RHI/SimpleRenderer.hpp"
#include "Game/App.hpp"
#include "Engine/DX11/DX11TextRenderer.hpp"

RemoteCommandService* RemoteCommandService::s_instance = nullptr;


void RemoteCommandService::Render(SimpleRenderer* renderer, DX11TextRenderer* m_loadedTextFile)
{
	renderer->DrawDX11Text2D(Vector2(-.95f, .85f), 1, Rgba(255, 0, 0, 255), *m_loadedTextFile, "Remote Client Service [Ready]");
	renderer->DrawDX11Text2D(Vector2(-.95f, .80f), 1, Rgba(255, 0, 0, 255), *m_loadedTextFile, "[host] join address: ");
	net_address_t address = GetMyAddress();
	std::string name = NetAddressToString(address);

	char *text = new char[name.length() + 1];
	strcpy(text,name.c_str());
	renderer->DrawDX11Text2D(Vector2(-.80f, .80f), 10, Rgba(255, 0, 0, 255), *m_loadedTextFile, text);
	
	std::string addr = std::to_string(address.port);
	char * port = new char[addr.length() + 1];
	strcpy(port, addr.c_str());
	renderer->DrawDX11Text2D(Vector2(-.69f, .80f), 1, Rgba(255, 0, 0, 255), *m_loadedTextFile, port);
	

	
	int connected = session->connections.size();
	std::string word = std::to_string(connected);
	char *text2 = new char[word.length() + 1];
	strcpy(text2, word.c_str());
	renderer->DrawDX11Text2D(Vector2(-.95f, .70f), 1, Rgba(255, 0, 0, 255), *m_loadedTextFile, "Client: ");
	renderer->DrawDX11Text2D(Vector2(-.89f, .70f), 1, Rgba(255, 0, 0, 255), *m_loadedTextFile, text2);
	renderer->DrawDX11Text2D(Vector2(-.88f, .70f), 1, Rgba(255, 0, 0, 255), *m_loadedTextFile, "/ 8");

	//Others!

	for (unsigned int i = 0; i < session->connections.size(); i++)
	{
		if (session->connections[i] != nullptr)
		{
			float y = .60f - (i* .05f);
			std::string connectionNumber = std::to_string(RemoteCommandService::GetInstance()->session->connections[i]->connection_index);
			char *text3 = new char[connectionNumber.length() + 1];
			strcpy(text3, connectionNumber.c_str());
			renderer->DrawDX11Text2D(Vector2(-.95f, y), 1, Rgba(255, 0, 0, 255), *m_loadedTextFile, text3);
			renderer->DrawDX11Text2D(Vector2(-.94f, y), 1, Rgba(255, 0, 0, 255), *m_loadedTextFile, ":");

			std::string addrname = NetAddressToString(session->connections[i]->address);
			char* addrText = new char[addrname.length() + 1];
			strcpy(addrText, addrname.c_str());
			renderer->DrawDX11Text2D(Vector2(-.92f, y), 1, Rgba(255, 0, 0, 255), *m_loadedTextFile, addrText);

		
			std::string portname = std::to_string(session->connections[i]->address.port);
			char * text4 = new char[portname.length() + 1];
			strcpy(text4, portname.c_str());
			renderer->DrawDX11Text2D(Vector2(-.75f, y), 1, Rgba(255, 0, 0, 255), *m_loadedTextFile, text4);

		}
	}

}

RemoteCommandService::RemoteCommandService()
{
	WORD version = MAKEWORD(2, 2);

	WSADATA data;
	int error = ::WSAStartup(version, &data);
	error;
	current_sender = nullptr;
	session = new TCPSession();
	disableEcho = false;

}

RemoteCommandService::~RemoteCommandService()
{
	s_instance = nullptr;
}

net_address_t RemoteCommandService::GetMyAddress()
{
	std::vector<net_address_t> addresses = GetAddressFromHostName("", RCS_PORT, true);
	return addresses[0];
}

void RemoteCommandService::Update()
{
	if (session->is_running())
	{
		session->update();
	}
	else
	{
		net_address_t* net = new net_address_t();
		//Allows you to host a service and then automatically join it
		if (!session->join(*net))
		{
			session->host(RCS_PORT);
			session->start_listening();
		}
	}
}

void RemoteCommandService::Join(net_address_t &addr)
{
	session->leave();
	session->join(addr);
}

void RemoteCommandService::setupMessageDefinition()
{
	NetMessageDefinition* onCommand = new NetMessageDefinition();
	onCommand->type_index = SEND_COMMAND;
	EventV0* newEvent = new EventV0;
	newEvent->subscribe(nullptr, on_command);
	onCommand->eventHandler = newEvent;

	NetMessageDefinition* onMessage = new NetMessageDefinition();
	onMessage->type_index = SEND_MESSAGE;
	EventV0* newEvent2 = new EventV0;
	newEvent2->subscribe(nullptr, on_message);
	onMessage->eventHandler = newEvent2;

	NetMessageDefinition* onJoin = new NetMessageDefinition();
	onJoin->type_index = JOIN_RESPONSE;
	EventV0* newEvent3 = new EventV0;
	newEvent3->subscribe(nullptr, on_join_response);
	onJoin->eventHandler = newEvent3;


	session->register_message_definition(SEND_COMMAND, *onCommand);
	session->register_message_definition(SEND_MESSAGE, *onMessage);
	session->register_message_definition(JOIN_RESPONSE, *onJoin);
}

void on_command(void* user_arg, void* msg_arg)
{
	user_arg;
	NetMessage* msg = (NetMessage*)msg_arg;

	RemoteCommandService::GetInstance()->current_sender = msg->sender;
	std::string commandAndArgs = msg->readString();
	//console->m_currentConsoleCommand = commandAndArgs;
	//console->RunConsoleCommands();


	if (RemoteCommandService::GetInstance()->disableEcho == false)
	{
		std::vector<std::string> lastMessage;
		//lastMessage.push_back(console->m_CommandHistory.back());
		NetMessage *msgg = new NetMessage(SEND_MESSAGE);
		msgg->write_string(lastMessage, 0);
		msg->sender->send(msgg);
	}

	RemoteCommandService::GetInstance()->current_sender = nullptr;
}

void on_message(void* user_arg, void* msg_arg)
{
	user_arg;
	NetMessage* msg = (NetMessage*)msg_arg;

	std::string response = msg->readString();
	//console->PrintOutGivenMessage(response);

	//ConsolePrintf("[%s], %s", NetAddressToString(msg->sender->address), response.c_str());
}




void RemoteCommandService::send_command_to_others(std::vector<std::string> parsedConsoleCommands)
{
	//Make a Message
	NetMessage *message = new NetMessage(SEND_COMMAND);

	//Fill message
	message->write_string(parsedConsoleCommands, 0);

	//Send message to everyone but myself
	session->sendToEveryoneElse(message);
}

void RemoteCommandService::send_message(std::vector<std::string> parsedConsoleCommands)
{
	//Make a Message
	NetMessage *message = new NetMessage(SEND_MESSAGE);
	//Fill message
	message->write_string(parsedConsoleCommands, 0);
	//Send message to everyone but myself
	session->sendToEveryoneElse(message);
}