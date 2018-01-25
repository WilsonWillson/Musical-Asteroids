#pragma once
#include "Engine/Networking/Netconnection.hpp"

class NetMessage;
class TCPSocket;
typedef unsigned char byte_t;

// net/tcp/connection.h
class TCPConnection : public NetConnection
{
public:
	TCPConnection();
	virtual ~TCPConnection();
	virtual void send(NetMessage *msg) override; //enquqe
	virtual bool receive(NetMessage **msg) override; //deququq

	virtual bool connect() override;
	bool is_disconnected();

public:
	TCPSocket *socket;
	int BytesReadSoFar;
	//uint16_t bytesToRead;

	byte_t bufferMessage[1000];
};
