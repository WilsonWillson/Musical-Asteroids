#pragma once
#include "Engine/Networking/net_address_t.hpp"

class NetSession;
class NetMessage;
class TCPSocket;

class NetConnection
{
public:
	//virtual ~NetConnection();

	virtual void send(NetMessage *msg) = 0;
	virtual bool receive(NetMessage **msg) = 0;
	virtual bool connect() = 0;

public:
	NetSession *owner;
	TCPSocket* socket;
	//net_address_t* address;
	net_address_t address;
	uint8_t connection_index; // LUID 
};