#include "Engine/Networking/NetSystem.hpp"
#include "Engine/Networking/net_address_t.hpp"
#include "Engine/Networking/TCPSocket.hpp"
#include "Engine/Networking/NetSystem.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/Win32Includes.hpp"

#include "Engine/Networking/NetSession.hpp"
#include "Engine/Networking/NetMessage.hpp"
#include "Game/Game.hpp"
#include "Engine/Networking/NetConnection.hpp"

TCPSocket* hostSocket = nullptr;
std::vector<TCPSocket*> listenedSockets;
bool startedNetSystem = false;

void SendToAllListnedSockets(void const* payload, unsigned int payloadSizeInBytes)
{
	for (unsigned int i = 0; i < listenedSockets.size(); i++)
	{
		listenedSockets[i]->Send(payload, payloadSizeInBytes);
	}
}

void CheckReceiveFromAllListenSockets()
{
	for (unsigned int i = 0; i < listenedSockets.size(); i++)
	{
		char buffer[128];
		unsigned int recev2 = listenedSockets[i]->receive(buffer, 128);

		if (recev2 > 0)
		{
			buffer[recev2] = NULL;
			DebuggerPrintf(buffer);
			DebuggerPrintf("\n");
		}

		if (listenedSockets[i]->CheckForDisconnect())
		{
			DebuggerPrintf("Disconnect Happened");
			SendToAllListnedSockets("Computer Disconnected", 21);
		}
	}
}

void UpdateNetSystem()
{
	if (startedNetSystem == true)
	{
		TCPSocket* new_connection = listenedSockets[0]->Accept();
		if (new_connection)
		{
			listenedSockets.push_back(new_connection);
			DebuggerPrintf("Got New Connection: \n");
			//char buffer[128];
			//unsigned int recev2 = new_connection->receive(buffer, 128);
			SendToAllListnedSockets("Wilson", 6);
		}
		CheckReceiveFromAllListenSockets();

		if (hostSocket != nullptr)
		{
			char buffer[128];
			unsigned int recev2 = hostSocket->receive(buffer, 128);

			if (recev2 > 0)
			{
				buffer[recev2] = NULL;
				DebuggerPrintf(buffer);
				DebuggerPrintf("\n");
			}
		}
	}
}

bool NetSystemHostConnection()
{
	startedNetSystem = true;
	hostSocket = new TCPSocket();
	WORD version = MAKEWORD(2, 2);

	WSADATA data;
	int error = ::WSAStartup(version, &data);

	TCPSocket *listner = new TCPSocket();
	listner->Listen(54321);
	listner->SetBlocking(false);
	listenedSockets.push_back(listner);

	return (error = 0);
}


bool NetSystemConnectToHost(char const *hostname, uint16_t port)
{
	startedNetSystem = true;
	hostSocket = new TCPSocket();
	WORD version = MAKEWORD(2, 2);

	WSADATA data;
	int error = ::WSAStartup(version, &data);

	TCPSocket *listner = new TCPSocket();
	listner->Listen(54321);
	listner->SetBlocking(false);
	listenedSockets.push_back(listner);

	std::vector<net_address_t> add = GetAddressFromHostName(hostname, port, false);
	//TCPSocket testSocket;
	hostSocket->join(add[0]);
	hostSocket->SetBlocking(false);
	hostSocket->Send("Wilson", 6);

	return (error = 0);
}

void NetSystemShutdown()
{
	::WSACleanup();
}

