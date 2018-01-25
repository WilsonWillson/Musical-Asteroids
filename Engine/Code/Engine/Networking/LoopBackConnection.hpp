#pragma once
#include "Engine/Networking/NetConnection.hpp"
#include <queue>

//All loopback connection is, is a queue that inherits from netConnection because it needs that data in NetConnection

class LoopBackConnection : public NetConnection
{
public:
	LoopBackConnection();
	virtual ~LoopBackConnection();


	virtual void send(NetMessage *msg) override; //enquqe
	virtual bool receive(NetMessage **msg) override; //deququq

	virtual bool connect() override;

public:
	std::queue<NetMessage*> myqueue;
};