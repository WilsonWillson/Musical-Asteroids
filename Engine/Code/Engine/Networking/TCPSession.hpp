#pragma once
#include "Engine/Networking/NetSession.hpp"
#include "Engine/Networking/TCPSocket.hpp"
#include "Engine/Networking/LoopBackConnection.hpp"
#pragma warning(disable:4996)

class TCPSocket;
class TCPConnection;
class LoopBackConnection;
class SimpleRenderer;
class DX11TextRenderer;

// net/tcp/session.h
class TCPSession : public NetSession
{
public:
	TCPSession()
	{
		connection_list = nullptr;
		my_connection = nullptr;	// helpers
		host_connection = nullptr; 	// 
		my_loopBack = new LoopBackConnection();
		listen_socket = new TCPSocket();
		m_networkTime = 0;
		m_AssumedNetworkTime = 0;
	};
	virtual ~TCPSession() {};

public:
	// create a loopback - add it to connections, set my and host connection to it;
	virtual void host(uint16_t port) override;

	// unimplemented so far;
	virtual bool join(net_address_t  &addr) override; // production version: session_join_info_t instead

	// if I have a connection - destroy it.					  
	virtual void leave() override;

	// Process all connections; 
	virtual void update() override;

	void send_join_info(NetConnection * cp); 



	virtual bool start_listening() override;
	virtual void stop_listening() override;
	virtual bool is_listening() override;

public:
	TCPSocket *listen_socket;
	void sendToEveryoneElse(NetMessage* message);
	void sendToEveryone(NetMessage* message);
	void SendToOnlyHost(NetMessage* message);
	void RenderSessionInfo(SimpleRenderer* renderer, DX11TextRenderer* m_loadedTextFile, uint16_t gamePort);
private:
	void RunReceivedCommand(NetMessage* message);

};

void on_join_response(void* user_arg, void* msg_arg);
