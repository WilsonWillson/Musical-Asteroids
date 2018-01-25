#pragma once
#include "Engine/Networking/net_address_t.hpp"



class TCPSocket
{
public:
	TCPSocket();

	~TCPSocket();


	bool join(net_address_t address);
	bool is_valid() const;
	unsigned int Send(void const *payload, unsigned int payloadSizeInBytes);
	unsigned int receive(void* payload, int max_size);
	bool Listen(uint16_t port);
	bool IsListening() const;
	TCPSocket* Accept();
	bool CheckForDisconnect();
	void SetBlocking(bool blocking);
	void close();


public:

	SOCKET my_socket;
	net_address_t address;
	bool m_isListenSocket;
};
