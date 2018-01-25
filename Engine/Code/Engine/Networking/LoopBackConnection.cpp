#include "Engine/Networking/LoopBackConnection.hpp"

LoopBackConnection::LoopBackConnection()
{

}

LoopBackConnection::~LoopBackConnection()
{

}

void LoopBackConnection::send(NetMessage *msg)
{
	myqueue.push(msg);
}

bool LoopBackConnection::receive(NetMessage **msg)
{
	msg;
	 myqueue.pop();
	return true;
}

bool LoopBackConnection::connect()
{

	return false;
}
