#include "Engine/Networking/TCPSession.hpp"
#include "Engine/Networking/LoopBackConnection.hpp"
#include "Engine/Networking/NetMessage.hpp"
#include "Engine/Networking/NetMessageDefinition.hpp"
#include "Engine/Networking/TCPSocket.hpp"
#include "Engine/Networking/TCPConnection.hpp"
#include "Engine/Networking/RemoteCommandService.hpp"
#include "Engine/Networking/net_address_t.hpp"
#include "Game/Game.hpp"
#include <queue>

void TCPSession::host(uint16_t hostPort)
{
	my_loopBack = new LoopBackConnection();
	host_connection = new LoopBackConnection();
	my_connection = new LoopBackConnection();

	host_connection->owner = this;
	//host_connection->address = new net_address_t();
	host_connection->address = RemoteCommandService::GetInstance()->GetMyAddress();
	host_connection->address.port = hostPort;

	join_connection(0, host_connection); // 0 for this class; 
	my_connection = host_connection;
	//connections.push_back(host_connection);
	
//	connections[my_connection->connection_index]->address = &my_connection->address;
	listen_socket = nullptr;
}

bool TCPSession::join(net_address_t &addr)
{
	TCPConnection *host = new TCPConnection();
	host->address = addr;

	join_connection(0, host); // 0 for this class; 
	host_connection = host;

	// Try to connect to host
	if (!host_connection->connect()) {
		destroy_connection(host);
		leave();
		return false;
	}

	(*((TCPConnection*)host_connection)).socket->SetBlocking(false);
	my_connection = new LoopBackConnection();
	my_connection->address = RemoteCommandService::GetInstance()->GetMyAddress();

	my_connection->connection_index = 0;
	connections[my_connection->connection_index]->address = my_connection->address;
	send_join_info(host_connection);

	m_sessionState = SESSION_CONNECTING;
	return true;

}

void TCPSession::leave()
{
	my_connection = nullptr;
}

void TCPSession::update()
{
	if (is_listening()) {
		TCPSocket *socket = listen_socket->Accept();
		if (socket != nullptr) {
			TCPConnection *new_guy = new TCPConnection();
			new_guy->socket = socket;
			uint8_t conn_idx = get_free_connection_index();
			if (conn_idx == INVALID_CONNECTION_INDEX)
				{
					delete new_guy;
				}
			else 
				{
					join_connection(conn_idx, new_guy);
					send_join_info(new_guy);// more to do; 
				}
			}
		}


	// connections.size() - 1
	for (unsigned int i = 0; i < connections.size(); ++i) {
		NetConnection *cp = connections[i];
		if ((cp != nullptr) && (cp != my_connection)) 
		{
			TCPConnection *tcp_connection = (TCPConnection*)cp;
			NetMessage* message = nullptr;

			bool receiveMessage = true;
			while (receiveMessage == true)
			{
				receiveMessage = tcp_connection->receive(&message);
				if (receiveMessage == true && message != nullptr)
				{
					message->sender = tcp_connection;
					RunReceivedCommand(message);
				}
			}

			if (tcp_connection->is_disconnected()) {
				destroy_connection(tcp_connection);
			}
		}
	}

	if (host_connection == nullptr) {
		leave();
	}
}

bool TCPSession::start_listening()
{
	if (!am_i_host()) {
		return false;
	}

	if (is_listening()) {
		return true;
	}

	listen_socket = new TCPSocket();
	if (listen_socket->Listen(my_connection->address.port)) {
		listen_socket->SetBlocking(false);
		return true;
	}
	else {
		delete listen_socket;
		listen_socket = nullptr;
		return false;
	}
}

void TCPSession::stop_listening()
{
	if (is_listening()) {
		delete listen_socket;
		listen_socket = nullptr;
	}
}

bool TCPSession::is_listening()
{
	return (nullptr != listen_socket);
}

void TCPSession::sendToEveryoneElse(NetMessage* message)
{
	for (unsigned int i = 0; i < connections.size(); ++i)
	{
		NetConnection *cp = connections[i];
		if ((cp != nullptr) && (cp != my_connection))
		{
			TCPConnection *tcp_connection = (TCPConnection*)cp;
			connections[i]->send(message);
		}
	}
}

void TCPSession::sendToEveryone(NetMessage* message)
{
	for (unsigned int i = 0; i < connections.size(); ++i)
	{
		NetConnection *cp = connections[i];
		if ((cp != nullptr))
		{
			//TCPConnection *tcp_connection = (TCPConnection*)cp;
			connections[i]->send(message);
		}
	}
}


void TCPSession::SendToOnlyHost(NetMessage* message)
{
		NetConnection *cp = host_connection;
		if ((cp != nullptr) && (cp != my_connection))
		{
			TCPConnection *tcp_connection = (TCPConnection*)cp;
			tcp_connection->send(message);
		}
}


void TCPSession::RunReceivedCommand(NetMessage* message)
{
	//my_loopBack->send(message);

	//NetMessage* mesg = nullptr;

	NetMessageDefinition* msg = RemoteCommandService::GetInstance()->session->get_message_definition(message->message_type_index);

	if (msg != nullptr)
	{
		msg->eventHandler->trigger(message);
		//my_loopBack->receive(&message);
	}

	NetMessageDefinition* gamemsg = get_message_definition(message->message_type_index);

	if (gamemsg != nullptr && gamemsg->type_index == 9)
	{
		gamemsg->eventHandler->trigger(message);
	}

	if (gamemsg != nullptr && gamemsg->type_index != 0)
	{
		gamemsg->eventHandler->trigger(message);
		//my_loopBack->receive(&message);
	}
}
#if defined(TOOLS_BUILD) 
void TCPSession::RenderSessionInfo(SimpleRenderer* renderer, DX11TextRenderer* m_loadedTextFile, uint16_t gamePort)
{
	renderer->DrawDX11Text2D(Vector2(-.95f, .85f), 1, Rgba(255, 0, 0, 255), *m_loadedTextFile, "TCPSession Ready");
	renderer->DrawDX11Text2D(Vector2(-.95f, .80f), 1, Rgba(255, 0, 0, 255), *m_loadedTextFile, "[host] join address: ");
	net_address_t address = RemoteCommandService::GetInstance()->GetMyAddress();
	std::string name = NetAddressToString(address);

	char *text = new char[name.length() + 1];
	strcpy(text, name.c_str());
	renderer->DrawDX11Text2D(Vector2(-.80f, .80f), 10, Rgba(255, 0, 0, 255), *m_loadedTextFile, text);

	std::string addr = std::to_string(gamePort);
	char * port = new char[addr.length() + 1];
	strcpy(port, addr.c_str());
	renderer->DrawDX11Text2D(Vector2(-.69f, .80f), 1, Rgba(255, 0, 0, 255), *m_loadedTextFile, port);



	int connected = connections.size();
	std::string word = std::to_string(connected);
	char *text2 = new char[word.length() + 1];
	strcpy(text2, word.c_str());
	renderer->DrawDX11Text2D(Vector2(-.95f, .70f), 1, Rgba(255, 0, 0, 255), *m_loadedTextFile, "Client: ");
	renderer->DrawDX11Text2D(Vector2(-.89f, .70f), 1, Rgba(255, 0, 0, 255), *m_loadedTextFile, text2);
	renderer->DrawDX11Text2D(Vector2(-.88f, .70f), 1, Rgba(255, 0, 0, 255), *m_loadedTextFile, "/ 8");

	//Others!

	for (unsigned int i = 0; i < connections.size(); i++)
	{
		if (connections[i] != nullptr)
		{
			if (connections[i]->connection_index == 0 || connections[i]->connection_index == 0 && connections[i] != my_connection)
				continue;

			float y = .60f - (i* .05f);
			std::string connectionNumber = std::to_string(connections[i]->connection_index);
			char *text3 = new char[connectionNumber.length() + 1];
			strcpy(text3, connectionNumber.c_str());
			renderer->DrawDX11Text2D(Vector2(-.95f, y), 1, Rgba(255, 0, 0, 255), *m_loadedTextFile, text3);
			renderer->DrawDX11Text2D(Vector2(-.94f, y), 1, Rgba(255, 0, 0, 255), *m_loadedTextFile, ":");

			std::string addrname = NetAddressToString((*((TCPConnection*)connections[i])).socket->address);
			char* addrText = new char[addrname.length() + 1];
			strcpy(addrText, addrname.c_str());
			renderer->DrawDX11Text2D(Vector2(-.92f, y), 1, Rgba(255, 0, 0, 255), *m_loadedTextFile, addrText);


			std::string portname = std::to_string((*((TCPConnection*)connections[i])).socket->address.port);
			char * text4 = new char[portname.length() + 1];
			strcpy(text4, portname.c_str());
			renderer->DrawDX11Text2D(Vector2(-.75f, y), 1, Rgba(255, 0, 0, 255), *m_loadedTextFile, text4);

		}
	}

}
#else
#endif
void TCPSession::send_join_info(NetConnection * cp)
{
	NetMessage *msg = new NetMessage(JOIN_RESPONSE);
	std::vector<std::string> connection;
	std::string connectionIndex = std::to_string(cp->connection_index);
	connection.push_back(connectionIndex);
	msg->write_string(connection, 0);

	msg->payload[3] = cp->connection_index;
	cp->send(msg);
}


void on_join_response(void* user_arg, void* msg_arg)
{
	user_arg;
	NetMessage* msg = (NetMessage*)msg_arg;

	RemoteCommandService::GetInstance()->session->join_connection(msg->payload[0], RemoteCommandService::GetInstance()->session->my_connection);
	RemoteCommandService::GetInstance()->session->m_sessionState =  SESSION_CONNECTED;
}